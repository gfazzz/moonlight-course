/*
 * OPERATION MOONLIGHT — s10e01 "Архитектура moonlight_core"
 * Эталонное решение: core.c
 *
 * Концепт серии: за девять сезонов накопились разрозненные артефакты. Финал
 *                требует собрать их в СИСТЕМУ, а система — это не «весь код в
 *                одном файле», а слои с явными границами:
 *                  - модуль объявляет ИНТЕРФЕЙС (что умеет), скрывая реализацию;
 *                  - зависимости направлены В ОДНУ СТОРОНУ (нижний слой не знает
 *                    о верхнем) — иначе получается клубок, который не собрать;
 *                  - подключение модулей идёт через РЕЕСТР, а не через прямые
 *                    вызовы: так систему можно расширять, не трогая ядро.
 * Задача: ядро с реестром модулей, разрешением зависимостей и проверкой циклов.
 *
 * Топологическая сортировка здесь — не украшение: именно она отвечает на вопрос
 * «в каком порядке инициализировать подсистемы», и она же ловит циклы.
 */
#include <stdio.h>
#include <string.h>

#define MAXMOD 16
#define MAXDEP 4

typedef enum { LAYER_PLATFORM, LAYER_DATA, LAYER_LOGIC, LAYER_UI } Layer;
static const char *LAYER_NAME[] = {"platform", "data", "logic", "ui"};

typedef struct {
    const char *name;
    Layer layer;
    const char *deps[MAXDEP];
    int  (*init)(void);          /* интерфейс модуля: только то, что нужно ядру */
    int   ready;
} Module;

static Module mods[MAXMOD];
static int nmods = 0;

/* Реализации модулей условны: важна структура, а не их содержимое. */
static int init_ok(void)   { return 0; }
static int init_fail(void) { return -1; }

static int reg(const char *name, Layer layer, int (*init)(void),
               const char *d0, const char *d1) {
    if (nmods >= MAXMOD) return -1;
    Module *m = &mods[nmods++];
    memset(m, 0, sizeof *m);
    m->name = name; m->layer = layer; m->init = init;
    m->deps[0] = d0; m->deps[1] = d1;
    return 0;
}

static int find(const char *name) {
    for (int i = 0; i < nmods; i++) if (strcmp(mods[i].name, name) == 0) return i;
    return -1;
}

/* --- Правило слоёв: зависеть можно только ВНИЗ или на свой уровень --- */
static int check_layers(void) {
    int bad = 0;
    for (int i = 0; i < nmods; i++)
        for (int d = 0; d < MAXDEP && mods[i].deps[d]; d++) {
            int j = find(mods[i].deps[d]);
            if (j < 0) {
                printf("  ОШИБКА: %s зависит от неизвестного модуля %s\n",
                       mods[i].name, mods[i].deps[d]);
                bad++;
                continue;
            }
            if (mods[j].layer > mods[i].layer) {
                printf("  НАРУШЕНИЕ СЛОЁВ: %s (%s) -> %s (%s)\n",
                       mods[i].name, LAYER_NAME[mods[i].layer],
                       mods[j].name, LAYER_NAME[mods[j].layer]);
                bad++;
            }
        }
    return bad;
}

/* --- Топологическая сортировка (обход в глубину с тремя цветами) --- */
static int color[MAXMOD];        /* 0 = не посещён, 1 = в обработке, 2 = готов */
static int order[MAXMOD];
static int norder = 0;
static int cycle_found = 0;

static void visit(int i) {
    if (color[i] == 2) return;
    if (color[i] == 1) {                     /* вернулись в узел, который ещё в стеке */
        printf("  ЦИКЛ обнаружен на модуле %s\n", mods[i].name);
        cycle_found = 1;
        return;
    }
    color[i] = 1;
    for (int d = 0; d < MAXDEP && mods[i].deps[d]; d++) {
        int j = find(mods[i].deps[d]);
        if (j >= 0) visit(j);
    }
    color[i] = 2;
    order[norder++] = i;                     /* зависимости уже добавлены — значит раньше */
}

static void topo_sort(void) {
    memset(color, 0, sizeof color);
    norder = 0;
    cycle_found = 0;
    for (int i = 0; i < nmods; i++) visit(i);
}

static int boot(void) {
    int started = 0;
    for (int k = 0; k < norder; k++) {
        Module *m = &mods[order[k]];

        /* Модуль стартует, только если ВСЕ его зависимости уже готовы. */
        int deps_ok = 1;
        for (int d = 0; d < MAXDEP && m->deps[d]; d++) {
            int j = find(m->deps[d]);
            if (j < 0 || !mods[j].ready) deps_ok = 0;
        }
        if (!deps_ok) {
            printf("  [SKIP] %-12s зависимости не готовы\n", m->name);
            continue;
        }
        if (m->init() != 0) {
            printf("  [FAIL] %-12s не инициализирован\n", m->name);
            continue;
        }
        m->ready = 1;
        started++;
        printf("  [ OK ] %-12s (%s)\n", m->name, LAYER_NAME[m->layer]);
    }
    return started;
}

static void scenario_reset(void) {
    nmods = 0;
    memset(mods, 0, sizeof mods);
}

int main(void) {
    printf("=== moonlight_core: архитектура системы ===\n\n");

    /* ---------- 1. Корректная конфигурация ---------- */
    printf("--- сборка 1: корректная система ---\n");
    scenario_reset();
    reg("storage",  LAYER_PLATFORM, init_ok, NULL, NULL);          /* S9: B-дерево + WAL */
    reg("crypto",   LAYER_PLATFORM, init_ok, NULL, NULL);          /* S4 */
    reg("net",      LAYER_PLATFORM, init_ok, NULL, NULL);          /* S3 */
    reg("telemetry",LAYER_DATA,     init_ok, "net", "storage");    /* S6 */
    reg("analytics",LAYER_DATA,     init_ok, "storage", NULL);     /* S8 */
    reg("tracker",  LAYER_LOGIC,    init_ok, "telemetry", "analytics");
    reg("dashboard",LAYER_UI,       init_ok, "tracker", NULL);     /* S9: TUI */

    printf("модулей зарегистрировано: %d\n", nmods);
    int bad = check_layers();
    printf("нарушений слоёв: %d\n", bad);

    topo_sort();
    printf("порядок инициализации:");
    for (int k = 0; k < norder; k++) printf(" %s", mods[order[k]].name);
    printf("\n");

    int started = boot();
    printf("запущено модулей: %d из %d\n\n", started, nmods);

    /* ---------- 2. Нарушение направления зависимостей ---------- */
    printf("--- сборка 2: нижний слой зависит от верхнего ---\n");
    scenario_reset();
    reg("storage",  LAYER_PLATFORM, init_ok, "dashboard", NULL);   /* так нельзя */
    reg("dashboard",LAYER_UI,       init_ok, "storage", NULL);
    bad = check_layers();
    printf("нарушений слоёв: %d\n", bad);
    topo_sort();
    printf("цикл обнаружен: %s\n\n", cycle_found ? "да" : "нет");

    /* ---------- 3. Отказ зависимости тянет за собой зависимых ---------- */
    printf("--- сборка 3: отказ нижнего модуля ---\n");
    scenario_reset();
    reg("storage",  LAYER_PLATFORM, init_fail, NULL, NULL);        /* диск недоступен */
    reg("analytics",LAYER_DATA,     init_ok,   "storage", NULL);
    reg("dashboard",LAYER_UI,       init_ok,   "analytics", NULL);
    topo_sort();
    started = boot();
    printf("запущено модулей: %d из %d\n", started, nmods);
    printf("система работоспособна: %s\n\n", started == nmods ? "да" : "нет (деградация)");

    printf("вывод: слои задают направление зависимостей, топологическая сортировка —\n");
    printf("       порядок запуска, а цикл в зависимостях делает систему несобираемой.\n");
    return 0;
}
