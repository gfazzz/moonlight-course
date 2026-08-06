/* OPERATION MOONLIGHT — s05e03 «Разбор настоящего CSV»
 * Эталонное решение: csv.c
 *
 * Нью-Йорк, 9 октября, 10:20.
 *
 * Сто восемьдесят миллионов строк журнала сделок. Наивный разбор даёт
 * записей больше, чем строк в файле, — потому что в названиях контрагентов
 * есть запятые, а в одном из них ещё и перевод строки.
 *
 * Концепт серии: конечный автомат разбора.
 *   - кавычки, экранированная кавычка внутри поля;
 *   - запятая и перевод строки внутри поля;
 *   - CRLF против LF;
 *   - потоковый разбор без загрузки файла в память;
 *   - восстановление после битой записи.
 *
 * Детерминизм: разбор целиком определяется байтами файла. Ни системных
 * вызовов времени, ни зависимости от локали. Собирается без -lm.
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define MAX_FIELDS   16
#define ROW_BYTES   512          /* весь буфер записи: поля лежат в нём подряд */
#define COLUMNS       5          /* сколько полей обязано быть в записи */

/* ---------- состояния автомата ---------- */

enum {
    S_START,        /* начало поля: ещё не знаем, в кавычках оно или нет */
    S_PLAIN,        /* поле без кавычек                                  */
    S_QUOTED,       /* внутри кавычек: запятая и перевод строки — данные  */
    S_AFTER_QUOTE   /* встретили " внутри кавычек: закрытие или удвоение  */
};

/* ---------- накопитель записи ---------- */

typedef struct {
    char   buf[ROW_BYTES];
    size_t len;                       /* занято в buf                     */
    char  *field[MAX_FIELDS];         /* указатели на начала полей в buf   */
    int    nfield;                    /* сколько полей начато             */
    int    overflow;                  /* запись не поместилась в буфер     */
    /* Что встретилось в записи — для отчёта, не для разбора. */
    int    had_comma, had_quote, had_newline;
} row_t;

static void row_reset(row_t *r) {
    r->len = 0; r->nfield = 0; r->overflow = 0;
    r->had_comma = r->had_quote = r->had_newline = 0;
    r->field[0] = r->buf;
}

/* Добавить байт в текущее поле. */
static void row_putc(row_t *r, char c) {
    if (r->len + 1 >= ROW_BYTES) { r->overflow = 1; return; }
    r->buf[r->len++] = c;
}

/* Завершить текущее поле: поставить ноль и учесть его в счётчике.
   Вызывается и на разделителе, и в конце записи — разница только в том,
   начинается ли после этого новое поле. Считать поля инкрементом на
   каждой запятой нельзя: у записи из пяти полей запятых четыре. */
static void row_end_field(row_t *r) {
    if (r->len + 1 >= ROW_BYTES) { r->overflow = 1; return; }
    r->buf[r->len++] = '\0';
    if (r->nfield + 1 < MAX_FIELDS) r->field[r->nfield + 1] = r->buf + r->len;
    r->nfield++;
}

/* ---------- отчёт ---------- */

/* Длина строки в символах, а не в байтах (см. s05e02). */
static size_t utf8_len(const char *s) {
    size_t n = 0;
    for (; *s; s++) if ((*s & 0xC0) != 0x80) n++;
    return n;
}
static void pad_left(const char *s, size_t width) {
    size_t l = utf8_len(s);
    fputs(s, stdout);
    for (; l < width; l++) putchar(' ');
}

static int rec_ok, rec_bad, rec_total;

/* Поля, которые наивный разбор разрезал бы: копим для отчёта. */
#define MAX_TRICKY 8
static char tricky[MAX_TRICKY][96];
static int  tricky_rec[MAX_TRICKY];
static int  ntricky;

/* Перевод строки внутри поля нельзя печатать как есть: он развалит
   таблицу. Показываем его видимым знаком — как это делает любой
   приличный просмотрщик данных. */
static void escape_into(char *dst, size_t n, const char *src) {
    size_t k = 0;
    for (; *src && k + 3 < n; src++) {
        if (*src == '\n')      { dst[k++] = '\\'; dst[k++] = 'n'; }
        else if (*src == '\r') { dst[k++] = '\\'; dst[k++] = 'r'; }
        else                   { dst[k++] = *src; }
    }
    dst[k] = '\0';
}

static void report_row(const row_t *r, long start_off) {
    rec_total++;

    char marks[64];
    marks[0] = '\0';
    if (r->had_comma)   strcat(marks, "запятая ");
    if (r->had_quote)   strcat(marks, "кавычки ");
    if (r->had_newline) strcat(marks, "перенос ");
    if (marks[0] == '\0') strcpy(marks, "—");

    const char *verdict;
    if (r->overflow)            { verdict = "БИТАЯ: не влезла в буфер"; rec_bad++; }
    else if (r->nfield != COLUMNS) { verdict = "БИТАЯ: не то число полей"; rec_bad++; }
    else                        { verdict = "ok"; rec_ok++; }

    printf("%7d %6ld %6d  ", rec_total, start_off, r->nfield);
    pad_left(marks, 26);
    printf("%s\n", verdict);

    /* Ищем поле, внутри которого оказался символ со специальным значением.
       Именно такие поля наивный разбор режет пополам или теряет. */
    for (int i = 0; i < r->nfield && i < MAX_FIELDS && ntricky < MAX_TRICKY; i++) {
        const char *v = r->field[i];
        if (strpbrk(v, ",\n\"") == NULL) continue;
        escape_into(tricky[ntricky], sizeof tricky[0], v);
        tricky_rec[ntricky] = rec_total;
        ntricky++;
        break;
    }
}

/* ---------- автомат ---------- */

/* Возвращает 0, если файл кончился корректно, и 1, если кавычка осталась
   незакрытой. Разбор потоковый: файл читается по байту, в памяти живёт
   ровно одна запись. Сто восемьдесят миллионов строк так же, как десять. */
static int csv_parse(FILE *f) {
    row_t r;
    row_reset(&r);

    int  state    = S_START;
    long off      = 0;          /* смещение текущего байта      */
    long row_off  = 0;          /* смещение начала записи       */
    int  pending  = 0;          /* есть незавершённая запись    */
    int  c;

    while ((c = fgetc(f)) != EOF) {
        off++;
        switch (state) {

        case S_START:
            pending = 1;
            if (c == '"')       { r.had_quote = 1; state = S_QUOTED; }
            else if (c == ',')  { row_end_field(&r); }
            else if (c == '\n') { row_end_field(&r); report_row(&r, row_off);
                                  row_reset(&r); row_off = off; pending = 0; }
            else if (c == '\r') { /* CR перед LF — разделитель, а не данные */ }
            else                { row_putc(&r, (char)c); state = S_PLAIN; }
            break;

        case S_PLAIN:
            if (c == ',')       { row_end_field(&r); state = S_START; }
            else if (c == '\n') { row_end_field(&r); report_row(&r, row_off);
                                  row_reset(&r); row_off = off; pending = 0;
                                  state = S_START; }
            else if (c == '\r') { /* хвост CRLF */ }
            else                { row_putc(&r, (char)c); }
            break;

        case S_QUOTED:
            /* Внутри кавычек особых символов нет: всё это данные. */
            if (c == '"')       { state = S_AFTER_QUOTE; }
            else {
                if (c == ',')  r.had_comma = 1;
                if (c == '\n') r.had_newline = 1;
                row_putc(&r, (char)c);
            }
            break;

        case S_AFTER_QUOTE:
            if (c == '"')       { row_putc(&r, '"'); state = S_QUOTED; }  /* "" -> " */
            else if (c == ',')  { row_end_field(&r); state = S_START; }
            else if (c == '\n') { row_end_field(&r); report_row(&r, row_off);
                                  row_reset(&r); row_off = off; pending = 0;
                                  state = S_START; }
            else if (c == '\r') { /* хвост CRLF */ }
            else                { row_putc(&r, (char)c); state = S_PLAIN; } /* мусор за кавычкой */
            break;
        }
    }

    /* Файл кончился. Если запись начата — закрыть её. */
    if (state == S_QUOTED) {
        /* Кавычка не закрыта: всё до конца файла втянулось в одно поле.
           Восстановиться нельзя — неизвестно, где должна была быть граница. */
        row_end_field(&r);
        report_row(&r, row_off);
        return 1;
    }
    if (pending || r.len > 0 || r.nfield > 0) {
        row_end_field(&r);
        report_row(&r, row_off);
    }
    return 0;
}

/* ---------- наивный разбор для сравнения ---------- */

static int naive_parse(FILE *f) {
    char line[ROW_BYTES];
    int lines = 0, five = 0, other = 0;

    while (fgets(line, sizeof line, f)) {
        lines++;
        int fields = 1;
        for (const char *p = line; *p; p++) if (*p == ',') fields++;
        if (fields == COLUMNS) five++; else other++;
    }

    printf("строк в файле:            %d\n", lines);
    printf("из них с %d полями:        %d\n", COLUMNS, five);
    printf("из них с другим числом:   %d\n", other);
    printf("наивный вывод: записей —  %d\n", lines);
    return lines;
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "../data/journal.csv";

    FILE *f = fopen(path, "rb");     /* rb: перевод строки нам нужен как есть */
    if (!f) { printf("FAIL: не открыть %s\n", path); return 1; }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    printf("=== журнал сделок: разбор ===\n");
    printf("размер файла: %ld байт\n\n", size);

    /* Файл открыт в двоичном режиме, поэтому CR виден и его можно посчитать.
       Если бы файл прошёл через нормализацию переводов строк, размер
       изменился бы — и тест поймал бы это раньше, чем разбор. */

    printf("--- наивный разбор: строка = запись, запятая = разделитель ---\n");
    int naive_records = naive_parse(f);
    rewind(f);

    printf("\n--- автомат ---\n");
    printf("запись  смещ.  полей  особенности               вердикт\n");
    int unterminated = csv_parse(f);
    fclose(f);

    printf("\nразобрано записей:   %d\n", rec_total);
    printf("целых:               %d\n", rec_ok);
    printf("битых:               %d\n", rec_bad);
    printf("кавычка не закрыта:  %s\n", unterminated ? "да" : "нет");

    printf("\n--- поля, которые наивный разбор разрезал бы ---\n");
    for (int i = 0; i < ntricky; i++)
        printf("запись %2d: «%s»\n", tricky_rec[i], tricky[i]);

    printf("\n--- итог ---\n");
    printf("наивный насчитал записей:  %d\n", naive_records);
    printf("автомат насчитал записей:  %d\n", rec_total);
    printf("наивный ошибся:            %s\n", naive_records != rec_total ? "да" : "нет");
    printf("разбор продолжился после битой записи: %s\n",
           rec_bad > 0 && rec_ok > rec_bad ? "да" : "нет");

    return 0;
}
