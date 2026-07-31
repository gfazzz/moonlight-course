/*
 * OPERATION MOONLIGHT — s09e08 "Квантовые вычисления" (финал Season 9)
 * Каркас. Скопируй в artifacts/quantum.c и реализуй.
 *
 *   cp starter.c artifacts/quantum.c
 *   make test
 *
 * Требования — в mission.md, ожидаемый вывод — в tests/expected.txt.
 *
 * Модель: регистр из NQ кубитов = вектор из 2^NQ комплексных амплитуд.
 * Базисное состояние |b2 b1 b0> соответствует индексу i, где бит q числа i —
 * это состояние кубита q.
 *
 * Однокубитный гейт работает с ПАРАМИ индексов, различающимися битом q:
 *   для каждого i без бита q:  j = i | (1<<q);  преобразуем (amp[i], amp[j]).
 *
 *   H: amp[i] = (a + b)/sqrt(2);  amp[j] = (a - b)/sqrt(2)
 *   X: поменять amp[i] и amp[j]
 *   Z: сменить знак у всех amp с установленным битом q
 *   CNOT(c, t): менять amp[i] и amp[i|tb] только там, где бит c равен 1
 *
 * Гровер: оракул меняет знак амплитуды цели, диффузия отражает все амплитуды
 * относительно среднего:  amp[i] = 2*среднее - amp[i].
 */
#include <stdio.h>

#define NQ   3
#define DIM  (1 << NQ)

typedef struct { double re, im; } Cx;
static Cx amp[DIM];

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 40; i++) g = 0.5 * (g + x / g);
    return g;
}

/* Реализуй: */
static void   reset_zero(void);
static double prob(int i);
static void   apply_H(int q);
static void   apply_X(int q);
static void   apply_Z(int q);
static void   apply_CNOT(int control, int target);
static void   show(const char *title);          /* формат — см. tests/expected.txt */
static void   grover_oracle(int marked);
static void   grover_diffusion(void);

int main(void) {
    printf("=== квантовый регистр: %d кубита, %d амплитуд ===\n\n", NQ, DIM);

    reset_zero();
    show("--- начальное состояние |000> ---");
    apply_X(0);
    show("--- после X на кубит 0: обычный NOT, состояние всё ещё определённое ---");

    reset_zero();
    apply_H(0);
    show("--- после H на кубит 0: суперпозиция ---");

    reset_zero();
    for (int q = 0; q < NQ; q++) apply_H(q);
    show("--- H на все кубиты: все 8 состояний равновероятны ---");

    reset_zero();
    apply_H(0);
    apply_CNOT(0, 1);
    show("--- H + CNOT: запутанная пара (состояние Белла) ---");
    printf("  состояний |01> и |10> нет вовсе: кубиты 0 и 1 всегда СОВПАДАЮТ.\n");
    printf("  это и есть запутанность — по одному кубиту известен другой.\n\n");

    reset_zero();
    apply_H(0);
    apply_Z(0);
    show("--- H, затем Z: вероятности те же, знак амплитуды другой ---");
    apply_H(0);
    show("--- ещё один H: интерференция сделала состояние |001> ---");

    const int marked = 6;
    printf("--- поиск Гровера: цель |110> среди %d вариантов ---\n", DIM);
    printf("классический перебор: в среднем %d проверок\n", DIM / 2);

    reset_zero();
    for (int q = 0; q < NQ; q++) apply_H(q);

    int iterations = 2;
    for (int it = 1; it <= iterations; it++) {
        grover_oracle(marked);
        grover_diffusion();
        printf("  итерация %d: вероятность цели %.3f\n", it, prob(marked));
    }

    printf("\nпосле %d итераций вероятность найти цель: %.1f%%\n",
           iterations, 100.0 * prob(marked));
    printf("квантовых запросов к оракулу: %d вместо ~%d классических\n\n",
           iterations, DIM / 2);

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

/* --- Твоя реализация --- */
static void   reset_zero(void) { }
static double prob(int i) { (void)i; return 0.0; }
static void   apply_H(int q) { (void)q; }
static void   apply_X(int q) { (void)q; }
static void   apply_Z(int q) { (void)q; }
static void   apply_CNOT(int c, int t) { (void)c; (void)t; }
static void   show(const char *title) { (void)title; }
static void   grover_oracle(int marked) { (void)marked; }
static void   grover_diffusion(void) { }
