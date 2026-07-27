/*
 * OPERATION MOONLIGHT — s10e06 "Дашборд операции"
 * Эталонное решение: dashboard.c
 *
 * Концепт серии: свести всё в один экран — задача не рисования, а АРХИТЕКТУРЫ.
 *                Данные приходят из разных подсистем с разной частотой и разной
 *                достоверностью. Дашборд обязан:
 *                  - отделять МОДЕЛЬ (состояние операции) от ПРЕДСТАВЛЕНИЯ (кадр);
 *                  - показывать ВОЗРАСТ данных: телеметрия трёхминутной давности
 *                    выглядит так же, как свежая, — и это опаснее её отсутствия;
 *                  - деградировать явно: подсистема упала -> на экране «нет данных»,
 *                    а не последнее известное значение, притворяющееся текущим.
 * Задача: собрать экран из телеметрии (s10e05), орбиты (s10e02/03) и статуса
 *         модулей (s10e01), с индикацией устаревших и отсутствующих данных.
 *
 * Рендер в буфер (приём из s09e06): тест не требует терминала.
 */
#include <stdio.h>
#include <string.h>

#define W 62
#define H 18
#define STALE_SEC 30        /* старше — данные считаются устаревшими */

/* ---------- модель: состояние операции ---------- */
typedef enum { SRC_OK, SRC_STALE, SRC_DOWN } SrcState;

typedef struct {
    const char *name;
    int   value;
    int   age_sec;          /* сколько секунд назад обновлялось */
    int   alive;            /* подсистема жива? */
} Source;

static SrcState state_of(const Source *s) {
    if (!s->alive) return SRC_DOWN;
    return (s->age_sec > STALE_SEC) ? SRC_STALE : SRC_OK;
}

/* ---------- представление: буфер экрана ----------
   Одна ячейка = один байт (как в s09e06), поэтому ВСЕ подписи внутри экрана —
   только ASCII: кириллица в UTF-8 заняла бы по два байта и разорвала вёрстку. */
static char scr[H][W];

static void clear(void) { memset(scr, ' ', sizeof scr); }

static void put(int x, int y, char c) {
    if (x < 0 || x >= W || y < 0 || y >= H) return;
    scr[y][x] = c;
}
static void text(int x, int y, const char *s) {
    for (int i = 0; s[i]; i++) put(x + i, y, s[i]);
}
static void box(int x, int y, int w, int h, const char *title) {
    for (int i = 1; i < w - 1; i++) { put(x + i, y, '-'); put(x + i, y + h - 1, '-'); }
    for (int j = 1; j < h - 1; j++) { put(x, y + j, '|'); put(x + w - 1, y + j, '|'); }
    put(x, y, '+'); put(x + w - 1, y, '+');
    put(x, y + h - 1, '+'); put(x + w - 1, y + h - 1, '+');
    if (title) text(x + 2, y, title);
}
static void gauge(int x, int y, int w, int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int filled = percent * w / 100;
    for (int i = 0; i < w; i++) put(x + i, y, i < filled ? '#' : '.');
}

static void dump(void) {
    for (int y = 0; y < H; y++) {
        putchar('|');
        for (int x = 0; x < W; x++) putchar(scr[y][x]);
        printf("|\n");
    }
}

/* Пометка состояния: свежие данные / устаревшие / нет связи. */
static const char *mark(SrcState st) {
    switch (st) {
        case SRC_OK:    return "[ok]  ";
        case SRC_STALE: return "[OLD] ";
        default:        return "[DOWN]";
    }
}

static void render(Source *src, int nsrc, double window_min, double elev_deg,
                   int modules_up, int modules_total) {
    clear();
    box(0, 0, W, H, " OPERATION MOONLIGHT / core ");

    char line[128];

    /* --- блок источников --- */
    text(2, 2, "TELEMETRY SOURCES");
    for (int i = 0; i < nsrc; i++) {
        SrcState st = state_of(&src[i]);
        if (st == SRC_DOWN)
            snprintf(line, sizeof line, "%s %-9s   ---   no data", mark(st), src[i].name);
        else
            snprintf(line, sizeof line, "%s %-9s %5d   age %2d s",
                     mark(st), src[i].name, src[i].value, src[i].age_sec);
        text(3, 3 + i, line);
    }

    /* --- блок связи --- */
    int row = 3 + nsrc + 1;
    text(2, row, "COMM WINDOW");
    snprintf(line, sizeof line, "window %.2f min    elevation %+.1f deg", window_min, elev_deg);
    text(3, row + 1, line);
    text(3, row + 2, "signal:");
    gauge(12, row + 2, 30, (int)(elev_deg / 90.0 * 100.0));

    /* --- блок ядра --- */
    row += 4;
    text(2, row, "SUBSYSTEMS");
    snprintf(line, sizeof line, "up %d/%d", modules_up, modules_total);
    text(3, row + 1, line);
    gauge(12, row + 1, 30, modules_up * 100 / modules_total);

    /* --- сводный вердикт --- */
    int down = 0, stale = 0;
    for (int i = 0; i < nsrc; i++) {
        SrcState st = state_of(&src[i]);
        if (st == SRC_DOWN) down++;
        else if (st == SRC_STALE) stale++;
    }
    const char *verdict;
    if (down > 0 || modules_up < modules_total) verdict = "STATUS: DEGRADED";
    else if (stale > 0)                          verdict = "STATUS: STALE DATA";
    else                                          verdict = "STATUS: NOMINAL";
    text(2, H - 3, verdict);
}

static void frame(const char *title, Source *src, int nsrc,
                  double win, double elev, int up, int total) {
    printf("--- %s ---\n", title);
    render(src, nsrc, win, elev, up, total);
    dump();

    int down = 0, stale = 0;
    for (int i = 0; i < nsrc; i++) {
        SrcState st = state_of(&src[i]);
        if (st == SRC_DOWN) down++;
        else if (st == SRC_STALE) stale++;
    }
    printf("источников: %d, устаревших: %d, недоступных: %d, подсистем: %d/%d\n\n",
           nsrc, stale, down, up, total);
}

int main(void) {
    printf("=== дашборд операции ===\n");
    printf("экран %dx%d, порог устаревания %d с\n\n", W, H, STALE_SEC);

    Source src[4] = {
        {"radar",     742, 2,  1},
        {"gps",        18, 5,  1},
        {"thermal",    64, 9,  1},
        {"uplink",    301, 3,  1},
    };

    /* 1. Всё в норме. */
    frame("штатный режим", src, 4, 8.74, 86.4, 7, 7);

    /* 2. Часть данных устарела — это должно быть видно. */
    src[1].age_sec = 95;
    src[2].age_sec = 41;
    frame("данные устаревают", src, 4, 3.10, 22.7, 7, 7);

    /* 3. Подсистема отвалилась + деградация ядра. */
    src[3].alive = 0;
    frame("потеря источника и деградация ядра", src, 4, 0.00, -12.5, 5, 7);

    printf("вывод: экран обязан показывать не только значения, но и их ВОЗРАСТ.\n");
    printf("       Устаревшие данные, выглядящие свежими, опаснее их отсутствия.\n");
    return 0;
}
