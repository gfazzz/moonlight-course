/*
 * OPERATION MOONLIGHT — s05e07 "Monte Carlo"
 * Эталонное решение: montecarlo.c
 *
 * Концепт серии: вместо одной формулы риска — прогнать ТЫСЯЧИ случайных сценариев
 *                и посмотреть на распределение исходов. Это Monte Carlo. Версия здесь
 *                СЕКВЕНЦИАЛЬНАЯ (один поток) на простом детерминированном PRNG.
 * Задача: смоделировать P&L портфеля за горизонт и оценить VaR по распределению.
 *
 * Про rand() и параллельность:
 *   Библиотечный rand() даёт РАЗНЫЕ последовательности на разных системах, поэтому
 *   для воспроизводимого теста здесь свой seeded LCG (идея та же, что у rand()).
 *   Ускорение через несколько потоков (pthread) и «честное» перемешивание (Knuth)
 *   — это Advanced после Season 7 (threads) и Season 8 (statistics). См. README.
 */
#include <stdio.h>
#include <stdlib.h>

#define NSIM     10000
#define HORIZON  20
#define START    50000000.0
#define DAILY_SIG 0.013        /* ~1.3% дневная волатильность */

static unsigned long rng = 12345;      /* фиксированный сид -> воспроизводимо */
static double uniform(void) {          /* [0,1) */
    rng = rng * 1103515245UL + 12345UL;
    return (double)((rng >> 16) & 0x7FFF) / 32768.0;
}
/* Приближённо нормальное N(0,1): сумма 12 равномерных минус 6 (ЦПТ). Без -lm. */
static double normal(void) {
    double s = 0.0;
    for (int i = 0; i < 12; i++) s += uniform();
    return s - 6.0;
}

static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static double pnl[NSIM];

int main(void) {
    printf("=== MONTE CARLO (sequential, %d paths, %d days) ===\n", NSIM, HORIZON);
    printf("start portfolio: $%.0f\n\n", START);

    double sum_final = 0.0;
    for (int p = 0; p < NSIM; p++) {
        double value = START;
        for (int d = 0; d < HORIZON; d++) {
            double r = DAILY_SIG * normal();   /* дневная доходность, mu = 0 */
            value *= (1.0 + r);
        }
        pnl[p] = value - START;                /* прибыль/убыток сценария */
        sum_final += value;
    }

    printf("mean ending value: $%.0f\n", sum_final / NSIM);

    /* Распределение P&L -> перцентили -> VaR. */
    qsort(pnl, NSIM, sizeof(double), cmp_double);
    double var95 = -pnl[(int)(0.05 * NSIM)];
    double var99 = -pnl[(int)(0.01 * NSIM)];

    printf("\n--- %d-day P&L distribution ---\n", HORIZON);
    printf("worst case:  $%.0f\n", pnl[0]);
    printf("95%% VaR:     $%.0f\n", var95);
    printf("99%% VaR:     $%.0f\n", var99);
    printf("best case:   $%+.0f\n", pnl[NSIM - 1]);

    printf("\nMonte Carlo shows the whole distribution, not just one number.\n");
    return 0;
}
