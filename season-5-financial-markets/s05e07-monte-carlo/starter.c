/*
 * OPERATION MOONLIGHT — s05e07 "Monte Carlo"
 * Каркас. Скопируй в artifacts/montecarlo.c и закрой TODO.
 *
 *   cp starter.c artifacts/montecarlo.c
 *   make test
 *
 * Концепт: прогнать тысячи случайных сценариев портфеля и посмотреть распределение.
 * Секвенциально (один поток), на СВОЁМ seeded LCG (библиотечный rand() не воспроизводим).
 * Параллельный Monte Carlo (pthread) и Knuth shuffle — Advanced после S7/S8.
 */
#include <stdio.h>
#include <stdlib.h>

#define NSIM     10000
#define HORIZON  20
#define START    50000000.0
#define DAILY_SIG 0.013

static unsigned long rng = 12345;
static double uniform(void) {
    rng = rng * 1103515245UL + 12345UL;
    return (double)((rng >> 16) & 0x7FFF) / 32768.0;
}
/* TODO 1: normal() — приближённо N(0,1): сумма 12 uniform() минус 6 (ЦПТ). */

static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static double pnl[NSIM];

int main(void) {
    printf("=== MONTE CARLO (sequential, %d paths, %d days) ===\n", NSIM, HORIZON);
    printf("start portfolio: $%.0f\n\n", START);

    double sum_final = 0.0;
    /* TODO 2: для каждого сценария p:
         value = START; для каждого дня d: r = DAILY_SIG * normal(); value *= (1+r);
         pnl[p] = value - START; sum_final += value. */

    printf("mean ending value: $%.0f\n", sum_final / NSIM);

    /* TODO 3: qsort(pnl, NSIM, sizeof(double), cmp_double);
       var95 = -pnl[(int)(0.05*NSIM)]; var99 = -pnl[(int)(0.01*NSIM)].
       Печать:
         "--- %d-day P&L distribution ---"
         "worst case:  $%.0f"  (pnl[0])
         "95%% VaR:     $%.0f"  "99%% VaR:     $%.0f"
         "best case:   $%+.0f"  (pnl[NSIM-1]). */

    printf("\nMonte Carlo shows the whole distribution, not just one number.\n");
    (void)cmp_double;
    return 0;
}
