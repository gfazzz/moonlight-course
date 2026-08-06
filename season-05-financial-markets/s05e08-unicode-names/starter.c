/* OPERATION MOONLIGHT — s05e08 «Имя, которого нет в таблице»
 * Каркас: скопируй в artifacts/unicode.c и заполни TODO.
 *
 *     cp starter.c artifacts/unicode.c
 *     make test
 *
 * Заполнять нужно разбор UTF-8, три меры длины, добивку по знакоместам
 * и сведение строки к скелету. Данные и печать уже готовы.
 *
 * Нью-Йорк, 10 октября, 23:20.
 *
 * В списке контрагентов две компании с визуально неразличимыми названиями.
 * В отчёте регулятора это одна строка, в базе — две. Платежи идут двойнику.
 *
 * Концепт серии: байты, кодовые точки и ширина отображения — три разные
 * величины.
 *   - разбор UTF-8 и его правила;
 *   - почему printf("%-16s") врёт;
 *   - нормализация: одна и та же буква двумя способами;
 *   - сравнение строк, которые выглядят одинаково.
 *
 * Детерминизм: всё считается по строковым литералам. Ни локали, ни
 * системных таблиц. Собирается без -lm.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ---------- разбор UTF-8 ---------- */

/* Декодирует одну кодовую точку. Возвращает число съеденных байт,
   0 — если последовательность неверна. Длина определяется старшими
   битами первого байта, продолжающие обязаны иметь вид 10xxxxxx. */
/* TODO 1. Разобрать одну кодовую точку. Вернуть число съеденных байт
   или 0, если последовательность неверна.

   Длина читается по старшим битам первого байта:
     0xxxxxxx                             1 байт,  точка 0..7F
     110xxxxx 10xxxxxx                    2 байта, точка 80..7FF
     1110xxxx 10xxxxxx 10xxxxxx           3 байта, точка 800..FFFF
     11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  4 байта, точка 10000..10FFFF

   Каждый продолжающий байт обязан иметь вид 10xxxxxx — проверяй.
   И отдельно проверяй диапазон результата: одну и ту же точку можно
   записать несколькими способами, но законен только кратчайший.
   Переусложнённая запись (overlong) — классический способ протащить
   символ мимо проверки, поэтому она считается ошибкой, а не вариантом. */
static int utf8_decode(const char *s, uint32_t *cp) {
    (void)s; (void)cp;
    return 0;
}

/* ---------- три разные величины ---------- */

static size_t len_bytes(const char *s) { return strlen(s); }

/* TODO 2. Число кодовых точек. Не байт: строка идёт шагами по
   utf8_decode. При ошибке разбора счёт останавливается. */
static size_t len_points(const char *s) {
    (void)s;
    return 0;
}

/* Ширина кодовой точки в знакоместах терминала.
   Ноль — комбинирующие знаки: они пририсовываются к предыдущей букве.
   Два — иероглифы и знаки полной ширины. Единица — всё остальное.
   Таблица сокращённая: полная живёт в базе Unicode и занимает килобайты. */
static int cp_width(uint32_t cp) {
    if (cp == 0) return 0;
    if ((cp >= 0x0300 && cp <= 0x036F) ||     /* комбинирующие диакритики */
        (cp >= 0x0483 && cp <= 0x0489) ||
        (cp >= 0x20D0 && cp <= 0x20F0) ||
        (cp >= 0xFE00 && cp <= 0xFE0F))       /* селекторы начертания     */
        return 0;
    if ((cp >= 0x1100 && cp <= 0x115F) ||     /* хангыль                  */
        (cp >= 0x2E80 && cp <= 0xA4CF) ||     /* иероглифы и кана         */
        (cp >= 0xAC00 && cp <= 0xD7A3) ||
        (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFE30 && cp <= 0xFE6F) ||
        (cp >= 0xFF00 && cp <= 0xFF60) ||
        (cp >= 0xFFE0 && cp <= 0xFFE6))
        return 2;
    return 1;
}

/* TODO 3. Ширина строки в знакоместах: сумма cp_width по всем точкам.
   Не длина в байтах и не число точек — третья величина. */
static size_t len_columns(const char *s) {
    (void)s;
    (void)cp_width;   /* строка уйдёт с первым вызовом */
    return 0;
}

/* TODO 4. Напечатать строку и добить пробелами до width ЗНАКОМЕСТ.
   Именно этого не умеет "%-16s": он считает байты. Если строка шире
   width, добивать нечем — печатаем как есть. */
static void pad_cols(const char *s, size_t width) {
    (void)width;
    fputs(s, stdout);
}

/* ---------- алфавиты ---------- */

enum { SC_OTHER = 0, SC_LATIN = 1, SC_CYRILLIC = 2, SC_HAN = 4 };

static int cp_script(uint32_t cp) {
    if ((cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z')) return SC_LATIN;
    if (cp >= 0x0400 && cp <= 0x04FF) return SC_CYRILLIC;
    if (cp >= 0x4E00 && cp <= 0x9FFF) return SC_HAN;
    return SC_OTHER;
}

/* Набор алфавитов, встретившихся в строке. Смешение латиницы с кириллицей
   внутри одного слова — то, чего не бывает у честного названия. */
static int scripts_of(const char *s) {
    int mask = 0; uint32_t cp;
    for (int k; *s; s += k) { k = utf8_decode(s, &cp); if (!k) break; mask |= cp_script(cp); }
    return mask;
}

/* ---------- скелет строки ---------- */

/* Кириллические буквы, неотличимые от латинских на экране. Таблица
   Unicode называет такие пары confusables; здесь взята её часть. */
static uint32_t deconfuse(uint32_t cp) {
    switch (cp) {
    case 0x0410: return 'A'; case 0x0430: return 'a';   /* А а */
    case 0x0412: return 'B';                            /* В   */
    case 0x0415: return 'E'; case 0x0435: return 'e';   /* Е е */
    case 0x041A: return 'K';                            /* К   */
    case 0x041C: return 'M';                            /* М   */
    case 0x041D: return 'H';                            /* Н   */
    case 0x041E: return 'O'; case 0x043E: return 'o';   /* О о */
    case 0x0420: return 'P'; case 0x0440: return 'p';   /* Р р */
    case 0x0421: return 'C'; case 0x0441: return 'c';   /* С с */
    case 0x0422: return 'T';                            /* Т   */
    case 0x0425: return 'X'; case 0x0445: return 'x';   /* Х х */
    case 0x0443: return 'y';                            /* у   */
    default: return cp;
    }
}

/* Скелет: строка приведена к виду, в котором сравнение отвечает на вопрос
   «выглядят ли они одинаково». Заменяем двойников, отбрасываем
   комбинирующие знаки, приводим к нижнему регистру.
   Это не нормализация в смысле Unicode, а её практический заменитель:
   настоящая NFC/NFD требует таблиц разложения на несколько килобайт. */
/* TODO 5. Свести строку к скелету — виду, в котором сравнение отвечает
   на вопрос «выглядят ли они одинаково».
   Порядок действий для каждой кодовой точки:
     1) точки нулевой ширины пропустить — это комбинирующие знаки;
     2) прогнать через deconfuse: кириллический двойник станет латиницей;
     3) привести к нижнему регистру, если это латинская заглавная;
     4) записать в out; всё, что не ASCII, заменить на '?'.
   Не забудь завершающий ноль и проверку, что в out ещё есть место. */
static void skeleton(const char *s, char *out, size_t n) {
    (void)s;
    (void)deconfuse;  /* строка уйдёт с первым вызовом */
    if (n) out[0] = '\0';
}

/* ---------- данные ---------- */

typedef struct { const char *id, *name; int payments; long amount; } party_t;

/* Второе имя отличается одним символом: латинская M заменена на
   кириллическую М. На экране разницы нет. */
static const party_t PARTIES[] = {
    { "CP-0041", "Meridian Capital, LLC",  412, 118443200L },
    { "CP-0088", "Мeridian Capital, LLC", 37,  91775000L },
    { "CP-0102", "Nordwall Trading",       260,  44120500L },
    { "CP-0117", "Sokolov, Ivanov & Partners", 88, 20310000L }
};
#define NPARTIES ((int)(sizeof PARTIES / sizeof PARTIES[0]))

/* Строки для таблицы трёх величин. */
static const char *SAMPLES[] = {
    "Meridian Capital, LLC",
    "Мeridian Capital, LLC",
    "Café",                  /* é одной кодовой точкой   */
    "Café",                 /* e + комбинирующий акут   */
    "東京取引所"   /* иероглифы: 2 колонки каждый */
};
#define NSAMPLES ((int)(sizeof SAMPLES / sizeof SAMPLES[0]))

static const char *SAMPLE_NOTE[] = {
    "латиница",
    "первая буква кириллическая",
    "e с акутом одной точкой",
    "e плюс комбинирующий акут",
    "иероглифы"
};

static void hexdump(const char *s, int n) {
    for (int i = 0; i < n && s[i]; i++)
        printf(i ? " %02X" : "%02X", (unsigned char)s[i]);
}

int main(void) {
    char skel[NPARTIES][128];

    printf("=== имена в таблице контрагентов ===\n\n");

    /* ---------- 1. Три разные величины ---------- */
    printf("--- байты, кодовые точки, знакоместа ---\n");
    printf("байт  точек  колонок  примечание\n");
    for (int i = 0; i < NSAMPLES; i++)
        printf("%4zu  %5zu  %7zu  %s\n",
               len_bytes(SAMPLES[i]), len_points(SAMPLES[i]),
               len_columns(SAMPLES[i]), SAMPLE_NOTE[i]);

    printf("\nодна буква — не один байт:      %s\n",
           len_bytes(SAMPLES[1]) != len_points(SAMPLES[1]) ? "да" : "нет");
    printf("одна точка — не одно знакоместо: %s\n",
           len_points(SAMPLES[4]) != len_columns(SAMPLES[4]) ? "да" : "нет");
    printf("два разных написания одной буквы дают одну ширину: %s\n",
           len_columns(SAMPLES[2]) == len_columns(SAMPLES[3]) ? "да" : "нет");

    /* ---------- 2. Почему %-16s врёт ---------- */
    printf("\n--- выравнивание колонки шириной 24 ---\n");
    printf("через \"%%-24s\" (байты):\n");
    for (int i = 0; i < NPARTIES; i++)
        printf("|%-24s| %s\n", PARTIES[i].name, PARTIES[i].id);

    printf("\nпо знакоместам:\n");
    for (int i = 0; i < NPARTIES; i++) {
        putchar('|'); pad_cols(PARTIES[i].name, 24);
        printf("| %s\n", PARTIES[i].id);
    }

    int skewed = 0;
    for (int i = 0; i < NPARTIES; i++)
        if (len_bytes(PARTIES[i].name) != len_columns(PARTIES[i].name)) skewed++;
    printf("\nзаписей, у которых байты не равны знакоместам: %d из %d\n",
           skewed, NPARTIES);

    /* ---------- 3. Двойник ---------- */
    printf("\n--- два контрагента, которых не различить глазом ---\n");
    printf("%s  %s\n", PARTIES[0].id, PARTIES[0].name);
    printf("%s  %s\n", PARTIES[1].id, PARTIES[1].name);

    printf("\nпервые байты %s: ", PARTIES[0].id); hexdump(PARTIES[0].name, 3); printf("\n");
    printf("первые байты %s: ", PARTIES[1].id); hexdump(PARTIES[1].name, 3); printf("\n");

    uint32_t cp0, cp1;
    utf8_decode(PARTIES[0].name, &cp0);
    utf8_decode(PARTIES[1].name, &cp1);
    printf("первая кодовая точка: U+%04X против U+%04X\n", cp0, cp1);
    printf("равны как строки: %s\n",
           strcmp(PARTIES[0].name, PARTIES[1].name) == 0 ? "да" : "нет");
    printf("равны по ширине:  %s\n",
           len_columns(PARTIES[0].name) == len_columns(PARTIES[1].name) ? "да" : "нет");

    /* ---------- 4. Смешанный алфавит ---------- */
    printf("\n--- алфавиты в названиях ---\n");
    printf("контрагент  алфавиты              вердикт\n");
    for (int i = 0; i < NPARTIES; i++) {
        int m = scripts_of(PARTIES[i].name);
        char buf[64]; buf[0] = '\0';
        if (m & SC_LATIN)    strcat(buf, "латиница ");
        if (m & SC_CYRILLIC) strcat(buf, "кириллица ");
        if (m & SC_HAN)      strcat(buf, "иероглифы ");
        if (!buf[0]) strcpy(buf, "—");

        int mixed = ((m & SC_LATIN) && (m & (SC_CYRILLIC | SC_HAN)));
        printf("%-10s  ", PARTIES[i].id);
        pad_cols(buf, 22);
        printf("%s\n", mixed ? "СМЕШАННЫЙ" : "однородный");
    }

    /* ---------- 5. Скелеты ---------- */
    printf("\n--- сведение к скелету ---\n");
    for (int i = 0; i < NPARTIES; i++) {
        skeleton(PARTIES[i].name, skel[i], sizeof skel[i]);
        printf("%s -> \"%s\"\n", PARTIES[i].id, skel[i]);
    }

    printf("\nсовпавшие скелеты при разных идентификаторах:\n");
    int collisions = 0;
    for (int i = 0; i < NPARTIES; i++)
        for (int j = i + 1; j < NPARTIES; j++)
            if (strcmp(skel[i], skel[j]) == 0) {
                printf("  %s и %s\n", PARTIES[i].id, PARTIES[j].id);
                collisions++;
            }
    printf("всего совпадений: %d\n", collisions);

    /* ---------- 6. Куда ушли деньги ---------- */
    printf("\n--- платежи по совпавшим скелетам ---\n");
    printf("контрагент  платежей       сумма\n");
    for (int i = 0; i < NPARTIES; i++)
        if (strcmp(skel[i], skel[0]) == 0)
            printf("%-10s  %8d  %10ld\n",
                   PARTIES[i].id, PARTIES[i].payments, PARTIES[i].amount);

    long total = 0, twin = 0;
    for (int i = 0; i < NPARTIES; i++)
        if (strcmp(skel[i], skel[0]) == 0) {
            total += PARTIES[i].amount;
            if (i != 0) twin += PARTIES[i].amount;
        }
    printf("\nвсего под этим именем: %ld\n", total);
    printf("ушло двойнику:         %ld\n", twin);
    printf("доля двойника:         %ld%%\n", twin * 100 / total);

    /* ---------- 7. Вывод ---------- */
    printf("\nв отчёте регулятора это одна строка.\n");
    printf("в базе платежей — две.\n");
    printf("разница между ними — один символ.\n");

    return 0;
}
