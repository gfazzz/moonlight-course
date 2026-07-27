/*
 * OPERATION MOONLIGHT — s09e06 "TUI: терминал как экран"
 * Каркас. Скопируй в artifacts/tui.c и реализуй.
 *
 *   cp starter.c artifacts/tui.c
 *   make test
 *
 * Требования — в mission.md, точный вид экрана — в tests/expected.txt.
 *
 * Архитектура (главное решение серии):
 *   рисуем в БУФЕР -> сравниваем с предыдущим кадром -> в терминал шлём разницу.
 * Благодаря этому код не требует TTY и легко тестируется.
 *
 * Одна ячейка = один байт, поэтому текст внутри экрана только ASCII.
 */
#include <stdio.h>
#include <string.h>

#define W 40
#define H 12

typedef struct {
    char ch[H][W];
    unsigned char attr[H][W];
} Screen;

static Screen front, back;

/* Реализуй: */
static void clear(Screen *s);
static void put(Screen *s, int x, int y, char c, unsigned char a);   /* с отсечением по границам */
static void text(Screen *s, int x, int y, const char *str, unsigned char a);
static void box(Screen *s, int x, int y, int w, int h, const char *title);
static void gauge(Screen *s, int x, int y, int w, int percent);
static int  diff_cells(const Screen *a, const Screen *b);
static void dump(const Screen *s);        /* печать строк вида |....| */

static long cost_full(void)        { return (long)W * H + H * 8; }
static long cost_diff(int changed) { return (long)changed * 9; }

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

    front = back;
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

    printf("вывод: рисуем в буфер, отправляем в терминал только разницу.\n");
    printf("       так исчезает мерцание и не тратится пропускная способность.\n");
    return 0;
}

/* --- Твоя реализация --- */
static void clear(Screen *s) { (void)s; }
static void put(Screen *s, int x, int y, char c, unsigned char a) {
    (void)s; (void)x; (void)y; (void)c; (void)a;
}
static void text(Screen *s, int x, int y, const char *str, unsigned char a) {
    (void)s; (void)x; (void)y; (void)str; (void)a;
}
static void box(Screen *s, int x, int y, int w, int h, const char *title) {
    (void)s; (void)x; (void)y; (void)w; (void)h; (void)title;
}
static void gauge(Screen *s, int x, int y, int w, int percent) {
    (void)s; (void)x; (void)y; (void)w; (void)percent;
}
static int  diff_cells(const Screen *a, const Screen *b) { (void)a; (void)b; return 0; }
static void dump(const Screen *s) { (void)s; }
