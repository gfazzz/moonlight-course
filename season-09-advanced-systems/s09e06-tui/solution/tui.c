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

/* Одна ячейка = ОДИН байт. Поэтому текст внутри экрана — только ASCII:
   кириллица в UTF-8 занимает 2 байта на символ и разъехалась бы по ячейкам.
   Как это решают настоящие TUI (wcwidth, кластеры графем) — в theory.md. */
typedef struct {
    char ch[H][W];
    unsigned char attr[H][W];      /* 0 = обычный, 1 = выделенный */
} Screen;

static Screen front, back;         /* front — что «на экране», back — что рисуем */

static void clear(Screen *s) {
    memset(s->ch, ' ', sizeof s->ch);
    memset(s->attr, 0, sizeof s->attr);
}

static void put(Screen *s, int x, int y, char c, unsigned char a) {
    if (x < 0 || x >= W || y < 0 || y >= H) return;    /* отсечение по границам */
    s->ch[y][x] = c;
    s->attr[y][x] = a;
}

static void text(Screen *s, int x, int y, const char *str, unsigned char a) {
    for (int i = 0; str[i]; i++) put(s, x + i, y, str[i], a);
}

/* Рамка псевдографикой ASCII (чтобы вывод оставался однобайтовым). */
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
            if (a->ch[y][x] != b->ch[y][x] || a->attr[y][x] != b->attr[y][x]) n++;
    return n;
}

/* Печать буфера как текста: детерминированно и не требует терминала. */
static void dump(const Screen *s) {
    for (int y = 0; y < H; y++) {
        printf("|");
        for (int x = 0; x < W; x++) putchar(s->ch[y][x]);
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
    memset(front.ch, '\0', 0);       /* первый кадр рисуется «с нуля» */

    frame("первый показ", 0, 12, 0);
    frame("рост нагрузки", 0, 47, 0);
    frame("обнаружены угрозы", 3, 61, 0);
    frame("тревога", 9, 88, 1);
    frame("повтор без изменений", 9, 88, 1);

    printf("вывод: рисуем в буфер, отправляем в терминал только разницу.\n");
    printf("       так исчезает мерцание и не тратится пропускная способность.\n");
    return 0;
}
