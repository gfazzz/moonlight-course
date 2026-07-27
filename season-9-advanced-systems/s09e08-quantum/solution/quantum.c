/*
 * OPERATION MOONLIGHT — s09e08 "Квантовые вычисления" (финал Season 9)
 * Эталонное решение: quantum.c
 *
 * Концепт серии: квантовый регистр из n кубитов — это ВЕКТОР из 2^n комплексных
 *                амплитуд. Отсюда сразу два вывода:
 *                  - симуляция на классической машине стоит экспоненциальной
 *                    памяти (30 кубитов ~ 16 ГБ) — поэтому квантовый компьютер
 *                    и интересен;
 *                  - «параллелизм» кубитов бесполезен сам по себе: измерение
 *                    даёт ОДИН исход. Выигрыш даёт ИНТЕРФЕРЕНЦИЯ — усиление
 *                    нужных амплитуд и гашение ненужных (алгоритм Гровера).
 * Задача: симулятор кубитов (H, X, Z, CNOT), запутанность и поиск Гровера.
 *
 * Комплексные числа — вручную (re, im): без -lm и без <complex.h>, чтобы вывод
 * был воспроизводим побитово, как во всех предыдущих сезонах.
 */
#include <stdio.h>

#define NQ   3                    /* кубитов */
#define DIM  (1 << NQ)            /* 2^NQ амплитуд */

typedef struct { double re, im; } Cx;

static Cx amp[DIM];               /* вектор состояния */

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 40; i++) g = 0.5 * (g + x / g);
    return g;
}

static void reset_zero(void) {    /* |00...0> */
    for (int i = 0; i < DIM; i++) { amp[i].re = 0.0; amp[i].im = 0.0; }
    amp[0].re = 1.0;
}

static double prob(int i) { return amp[i].re * amp[i].re + amp[i].im * amp[i].im; }

/* Однокубитный гейт применяется к ПАРАМ базисных состояний, различающимся битом q. */
static void apply_H(int q) {
    const double s = 1.0 / my_sqrt(2.0);
    int bit = 1 << q;
    for (int i = 0; i < DIM; i++) {
        if (i & bit) continue;                 /* обрабатываем пару один раз */
        int j = i | bit;
        Cx a = amp[i], b = amp[j];
        amp[i].re = s * (a.re + b.re); amp[i].im = s * (a.im + b.im);
        amp[j].re = s * (a.re - b.re); amp[j].im = s * (a.im - b.im);
    }
}

static void apply_X(int q) {                   /* NOT: меняет местами пару */
    int bit = 1 << q;
    for (int i = 0; i < DIM; i++) {
        if (i & bit) continue;
        int j = i | bit;
        Cx t = amp[i]; amp[i] = amp[j]; amp[j] = t;
    }
}

static void apply_Z(int q) {                   /* фазовый сдвиг для |1> */
    int bit = 1 << q;
    for (int i = 0; i < DIM; i++)
        if (i & bit) { amp[i].re = -amp[i].re; amp[i].im = -amp[i].im; }
}

static void apply_CNOT(int control, int target) {
    int cb = 1 << control, tb = 1 << target;
    for (int i = 0; i < DIM; i++) {
        if (!(i & cb)) continue;               /* управляющий должен быть 1 */
        if (i & tb) continue;                  /* пару обрабатываем один раз */
        int j = i | tb;
        Cx t = amp[i]; amp[i] = amp[j]; amp[j] = t;
    }
}

static void show(const char *title) {
    printf("%s\n", title);
    for (int i = 0; i < DIM; i++) {
        double p = prob(i);
        if (p < 1e-9) continue;                /* нулевые амплитуды не печатаем */
        printf("  |");
        for (int b = NQ - 1; b >= 0; b--) putchar((i >> b) & 1 ? '1' : '0');
        printf(">  амплитуда %+.3f  вероятность %.3f\n", amp[i].re, p);
    }
    double total = 0.0;
    for (int i = 0; i < DIM; i++) total += prob(i);
    printf("  сумма вероятностей: %.3f\n\n", total);
}

/* --- Гровер: поиск отмеченного состояния среди DIM вариантов --- */
static void grover_oracle(int marked) {        /* помечаем цель сменой знака */
    amp[marked].re = -amp[marked].re;
    amp[marked].im = -amp[marked].im;
}

static void grover_diffusion(void) {           /* отражение относительно среднего */
    double mre = 0.0, mim = 0.0;
    for (int i = 0; i < DIM; i++) { mre += amp[i].re; mim += amp[i].im; }
    mre /= DIM; mim /= DIM;
    for (int i = 0; i < DIM; i++) {
        amp[i].re = 2.0 * mre - amp[i].re;
        amp[i].im = 2.0 * mim - amp[i].im;
    }
}

int main(void) {
    printf("=== квантовый регистр: %d кубита, %d амплитуд ===\n\n", NQ, DIM);

    /* 1. Классический гейт X: обычное отрицание, никакой квантовости. */
    reset_zero();
    show("--- начальное состояние |000> ---");
    apply_X(0);
    show("--- после X на кубит 0: обычный NOT, состояние всё ещё определённое ---");

    /* 2. Суперпозиция: один H превращает определённость в равновероятность. */
    reset_zero();
    apply_H(0);
    show("--- после H на кубит 0: суперпозиция ---");

    /* 2. Полная суперпозиция всех состояний. */
    reset_zero();
    for (int q = 0; q < NQ; q++) apply_H(q);
    show("--- H на все кубиты: все 8 состояний равновероятны ---");

    /* 3. Запутанность: состояние Белла. */
    reset_zero();
    apply_H(0);
    apply_CNOT(0, 1);
    show("--- H + CNOT: запутанная пара (состояние Белла) ---");
    printf("  состояний |01> и |10> нет вовсе: кубиты 0 и 1 всегда СОВПАДАЮТ.\n");
    printf("  это и есть запутанность — по одному кубиту известен другой.\n\n");

    /* 4. Фаза не видна в вероятностях, но управляет интерференцией. */
    reset_zero();
    apply_H(0);
    apply_Z(0);
    show("--- H, затем Z: вероятности те же, знак амплитуды другой ---");
    apply_H(0);
    show("--- ещё один H: интерференция сделала состояние |001> ---");

    /* 5. Алгоритм Гровера: ищем помеченное состояние. */
    const int marked = 6;                       /* |110> */
    printf("--- поиск Гровера: цель |110> среди %d вариантов ---\n", DIM);
    printf("классический перебор: в среднем %d проверок\n", DIM / 2);

    reset_zero();
    for (int q = 0; q < NQ; q++) apply_H(q);

    int iterations = 2;                         /* ~ (pi/4)*sqrt(8) ≈ 2 */
    for (int it = 1; it <= iterations; it++) {
        grover_oracle(marked);
        grover_diffusion();
        printf("  итерация %d: вероятность цели %.3f\n", it, prob(marked));
    }

    printf("\nпосле %d итераций вероятность найти цель: %.1f%%\n",
           iterations, 100.0 * prob(marked));
    printf("квантовых запросов к оракулу: %d вместо ~%d классических\n\n",
           iterations, DIM / 2);

    /* 6. Финал сезона. */
    printf("--- advanced_systems ---\n");
    const char *sub[] = {
        "лексер (e01)", "парсер и AST (e02)", "интерпретатор (e03)",
        "B-дерево (e04)", "журнал WAL (e05)", "TUI (e06)",
        "фильтр Калмана (e07)", "квантовый симулятор (e08)"
    };
    for (int i = 0; i < 8; i++) printf("  [OK] %s\n", sub[i]);
    printf("\nSeason 9 complete.\n");
    return 0;
}
