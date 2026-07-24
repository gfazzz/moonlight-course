/*
 * OPERATION MOONLIGHT — s05e05 "Бэктест и overfitting"
 * Эталонное решение: backtest.c
 *
 * Концепт серии: стратегию нельзя оценивать на тех же данных, где её настраивали.
 *                Grid search подберёт «идеальные» параметры под прошлое (in-sample),
 *                но на новых данных (out-of-sample) они разваливаются. Это overfitting.
 * Задача: бэктест SMA-crossover, перебор параметров, честная проверка на OOS.
 */
#include <stdio.h>

#define TOTAL 240
#define SPLIT 120        /* [0,SPLIT) — in-sample, [SPLIT,TOTAL) — out-of-sample */

static double price[TOTAL];

/* Детерминированный «случайный» ряд цен (random walk на LCG). */
static void gen_prices(void) {
    unsigned long s = 777;
    double p = 100.0;
    for (int i = 0; i < TOTAL; i++) {
        s = s * 1103515245UL + 12345UL;
        double r = ((double)((s >> 16) & 0x7FFF) / 32768.0) - 0.5;  /* [-0.5, 0.5) */
        p += r * 2.0;                     /* шаг ±1 */
        if (p < 1.0) p = 1.0;
        price[i] = p;
    }
}

static double sma(int end, int period) {   /* среднее price[end-period+1 .. end] */
    if (end + 1 < period) return 0.0;
    double sum = 0.0;
    for (int k = end - period + 1; k <= end; k++) sum += price[k];
    return sum / period;
}

/* Бэктест long-only SMA-crossover на отрезке [lo, hi). Возврат — доходность в %. */
static double backtest(int lo, int hi, int fast, int slow) {
    double equity = 1.0;
    int pos = 0;
    for (int i = lo + slow; i < hi; i++) {
        if (pos == 1) equity *= price[i] / price[i - 1];   /* держим позицию */
        double f = sma(i, fast), s = sma(i, slow);
        pos = (f > s) ? 1 : 0;                              /* сигнал на след. шаг */
    }
    return (equity - 1.0) * 100.0;
}

int main(void) {
    gen_prices();

    printf("=== BACKTEST & OVERFITTING ===\n");
    printf("in-sample [0,%d), out-of-sample [%d,%d)\n\n", SPLIT, SPLIT, TOTAL);

    int fasts[] = {3, 5, 8};
    int slows[] = {13, 21, 34};

    /* Grid search: ищем ЛУЧШИЕ параметры на in-sample. */
    printf("--- grid search on in-sample ---\n");
    double best_is = -1e9; int bf = 0, bs = 0;
    for (int a = 0; a < 3; a++) for (int b = 0; b < 3; b++) {
        double r = backtest(0, SPLIT, fasts[a], slows[b]);
        printf("fast %2d slow %2d: in-sample %+7.2f%%\n", fasts[a], slows[b], r);
        if (r > best_is) { best_is = r; bf = fasts[a]; bs = slows[b]; }
    }

    printf("\nbest in-sample: fast %d slow %d -> %+.2f%%\n", bf, bs, best_is);

    /* Честная проверка: те же параметры на НОВЫХ данных. */
    double oos = backtest(SPLIT, TOTAL, bf, bs);
    printf("same params out-of-sample -> %+.2f%%\n", oos);

    /* Ориентир: просто «купил и держи» на OOS. */
    double bh = (price[TOTAL - 1] / price[SPLIT] - 1.0) * 100.0;
    printf("buy & hold out-of-sample -> %+.2f%%\n", bh);

    printf("\nlesson: the in-sample winner does not carry over. that gap is overfitting.\n");
    return 0;
}
