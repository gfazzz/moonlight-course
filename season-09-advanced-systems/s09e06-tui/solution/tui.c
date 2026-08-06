/*
 * OPERATION MOONLIGHT — s09e06 "TUI: терминал как экран"
 * Эталонное решение: tui.c
 *
 * Концепт серии: терминал — это не «поток строк», а устройство с адресуемыми
 *                ячейками. Управляют им ANSI-последовательностями. Правильная
 *                архитектура TUI: РЕНДЕР В БУФЕР отдельно, ВЫВОД в терминал
 *                отдельно. Тогда:
 *                  - рисование становится чистой функцией (легко тестировать);
 *                  - можно сравнить два кадра и вывести ТОЛЬКО изменения
 *                    (damage tracking) — терминал медленный, полная перерисовка
 *                    в цикле даёт мерцание;
 *                  - код не зависит от того, есть ли вообще TTY.
 * Задача: мини-фреймворк — буфер экрана, рамки, текст, диффер кадров.
 *
 * Сырой режим (termios) и реальный вывод разобраны в theory.md; здесь он не
 * нужен — и это осознанное решение, а не упрощение: тест не должен требовать
 * терминала.
 */
#include <stdio.h>
#include <string.h>

#define W 40
#define H 12

/* Одна ячейка = ОДИН СИМВОЛ, а не один байт. Разница существенная:
   кириллическая буква в UTF-8 занимает два байта, иероглиф — три и вдобавок
   два знакоместа. Ячейка хранит последовательность байт целиком, поэтому
   текст внутри экрана не обязан быть ASCII.
   Ширина считается так же, как в s05e08: комбинирующий знак — 0, иероглиф
   и знак полной ширины — 2, остальное — 1. Символ шириной 2 занимает две
   ячейки: во второй лежит пустая строка-продолжение. */
#define CELL 5                     /* до 4 байт UTF-8 плюс завершающий ноль */

typedef struct {
    char ch[H][W][CELL];
    unsigned char attr[H][W];      /* 0 = обычный, 1 = выделенный */
} Screen;

/* Разбор одной кодовой точки. Возвращает число байт или 0 при ошибке. */
static int u8_decode(const char *s, unsigned *cp) {
    const unsigned char *p = (const unsigned char *)s;
    if (p[0] < 0x80)           { *cp = p[0]; return 1; }
    if ((p[0] & 0xE0) == 0xC0) { if ((p[1] & 0xC0) != 0x80) return 0;
        *cp = ((unsigned)(p[0] & 0x1F) << 6) | (p[1] & 0x3F); return *cp < 0x80 ? 0 : 2; }
    if ((p[0] & 0xF0) == 0xE0) { if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80) return 0;
        *cp = ((unsigned)(p[0] & 0x0F) << 12) | ((unsigned)(p[1] & 0x3F) << 6) | (p[2] & 0x3F);
        return *cp < 0x800 ? 0 : 3; }
    if ((p[0] & 0xF8) == 0xF0) { if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80
                                  || (p[3] & 0xC0) != 0x80) return 0;
        *cp = ((unsigned)(p[0] & 0x07) << 18) | ((unsigned)(p[1] & 0x3F) << 12)
            | ((unsigned)(p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        return (*cp < 0x10000 || *cp > 0x10FFFF) ? 0 : 4; }
    return 0;
}

/* Ширина в знакоместах. Таблица сокращённая — как в s05e08. */
static int cp_width(unsigned cp) {
    if ((cp >= 0x0300 && cp <= 0x036F) || (cp >= 0xFE00 && cp <= 0xFE0F)) return 0;
    if ((cp >= 0x1100 && cp <= 0x115F) || (cp >= 0x2E80 && cp <= 0xA4CF)
     || (cp >= 0xAC00 && cp <= 0xD7A3) || (cp >= 0xFF00 && cp <= 0xFF60)) return 2;
    return 1;
}

static Screen front, back;         /* front — что «на экране», back — что рисуем */

static void clear(Screen *s) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) { s->ch[y][x][0] = ' '; s->ch[y][x][1] = '\0'; }
    memset(s->attr, 0, sizeof s->attr);
}

/* Положить в ячейку последовательность байт одного символа. */
static void put_cell(Screen *s, int x, int y, const char *g, int len, unsigned char a) {
    if (x < 0 || x >= W || y < 0 || y >= H) return;    /* отсечение по границам */
    if (len >= CELL) len = CELL - 1;
    for (int i = 0; i < len; i++) s->ch[y][x][i] = g[i];
    s->ch[y][x][len] = '\0';
    s->attr[y][x] = a;
}

static void put(Screen *s, int x, int y, char c, unsigned char a) {
    put_cell(s, x, y, &c, 1, a);
}

/* Текст кладётся посимвольно, а курсор двигается на ШИРИНУ символа.
   Широкий символ занимает две ячейки: вторая пустая, иначе она затрёт
   правую половину при выводе. */
static void text(Screen *s, int x, int y, const char *str, unsigned char a) {
    int col = x;
    for (const char *p = str; *p; ) {
        unsigned cp;
        int len = u8_decode(p, &cp);
        if (!len) { put(s, col++, y, '?', a); p++; continue; }
        int w = cp_width(cp);
        if (w == 0) { p += len; continue; }            /* комбинирующий знак */
        put_cell(s, col, y, p, len, a);
        if (w == 2) put_cell(s, col + 1, y, "", 0, a); /* продолжение */
        col += w;
        p += len;
    }
}

/* Рамка ASCII: она не обязана быть такой, но остаётся ей ради читаемости
   diff'ов теста. Ячейка теперь умеет хранить и псевдографику Unicode. */
static void box(Screen *s, int x, int y, int w, int h, const char *title) {
    for (int i = 1; i < w - 1; i++) { put(s, x + i, y, '-', 0); put(s, x + i, y + h - 1, '-', 0); }
    for (int j = 1; j < h - 1; j++) { put(s, x, y + j, '|', 0); put(s, x + w - 1, y + j, '|', 0); }
    put(s, x, y, '+', 0);
    put(s, x + w - 1, y, '+', 0);
    put(s, x, y + h - 1, '+', 0);
    put(s, x + w - 1, y + h - 1, '+', 0);
    if (title) text(s, x + 2, y, title, 1);
}

/* Горизонтальный индикатор заполнения. */
static void gauge(Screen *s, int x, int y, int w, int percent) {
    int filled = percent * w / 100;
    for (int i = 0; i < w; i++) put(s, x + i, y, i < filled ? '#' : '.', i < filled ? 1 : 0);
}

/* Сколько ячеек изменилось между кадрами — столько и надо отправить в терминал. */
static int diff_cells(const Screen *a, const Screen *b) {
    int n = 0;
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            if (strcmp(a->ch[y][x], b->ch[y][x]) != 0 || a->attr[y][x] != b->attr[y][x]) n++;
    return n;
}

/* Печать буфера как текста: детерминированно и не требует терминала. */
static void dump(const Screen *s) {
    for (int y = 0; y < H; y++) {
        printf("|");
        for (int x = 0; x < W; x++) fputs(s->ch[y][x], stdout);
        printf("|\n");
    }
}

/* Во сколько байт обошёлся бы вывод: полная перерисовка против только изменений. */
static long cost_full(void)          { return (long)W * H + H * 8; }      /* + переводы строк/позиционирование */
static long cost_diff(int changed)   { return (long)changed * 9; }        /* ESC[y;xH + символ */

static void frame(const char *title, int threats, int load, int alert) {
    clear(&back);
    box(&back, 0, 0, W, H, " MOONLIGHT MONITOR ");

    char line[64];
    snprintf(line, sizeof line, "threats detected : %d", threats);
    text(&back, 2, 2, line, 0);

    snprintf(line, sizeof line, "channel load     : %d%%", load);
    text(&back, 2, 4, line, 0);
    gauge(&back, 2, 5, 30, load);

    text(&back, 2, 7, alert ? "[ !! ] ALERT: anomalous traffic" : "[ ok ] nominal", alert ? 1 : 0);
    text(&back, 2, 9, "q quit   r refresh", 0);

    /* Кириллица и иероглифы внутри экрана — ровно та проверка, которую
       раньше приходилось заменять оговоркой «только ASCII». */
    text(&back, 2, 10, "\xD0\xBA\xD0\xB0\xD0\xBD\xD0\xB0\xD0\xBB: "
                       "\xE6\x9D\xB1\xE4\xBA\xAC", 0);

    int changed = diff_cells(&front, &back);

    printf("--- кадр: %s ---\n", title);
    dump(&back);
    long full = cost_full(), part = cost_diff(changed);
    printf("изменено ячеек: %d из %d\n", changed, W * H);
    printf("стоимость вывода: целиком %ld байт, только изменения %ld байт -> ", full, part);
    if (changed == 0)     printf("выводить нечего, кадр не изменился\n\n");
    else if (part < full) printf("шлём разницу (в %.1f раза дешевле)\n\n", (double)full / (double)part);
    else                  printf("дешевле перерисовать целиком\n\n");

    front = back;                    /* кадр стал текущим */
}

int main(void) {
    printf("=== TUI: экран как буфер ячеек ===\n");
    printf("размер: %dx%d, рендер в память, вывод — только изменения\n\n", W, H);

    clear(&front);

    frame("первый показ", 0, 12, 0);
    frame("рост нагрузки", 0, 47, 0);
    frame("обнаружены угрозы", 3, 61, 0);
    frame("тревога", 9, 88, 1);
    frame("повтор без изменений", 9, 88, 1);

    /* Проверка ширины: строка из двух иероглифов занимает четыре ячейки,
       а не две и не шесть. */
    Screen probe;
    clear(&probe);
    text(&probe, 0, 0, "\xE6\x9D\xB1\xE4\xBA\xAC", 0);
    int used = 0;
    for (int x = 0; x < W; x++) if (probe.ch[0][x][0] != ' ') used++;
    printf("--- ширина в ячейках ---\n");
    printf("два иероглифа занимают ячеек: %d (байт в строке: %zu)\n",
           used, strlen("\xE6\x9D\xB1\xE4\xBA\xAC"));
    printf("ячейка хранит символ, а не байт: %s\n\n",
           strlen(probe.ch[0][0]) == 3 ? "да" : "нет");

    printf("вывод: рисуем в буфер, отправляем в терминал только разницу.\n");
    printf("       так исчезает мерцание и не тратится пропускная способность.\n");
    return 0;
}
