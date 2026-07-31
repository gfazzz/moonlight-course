/*
 * OPERATION MOONLIGHT — s05e05 "Бэктест и overfitting"
 * Каркас. Скопируй в artifacts/backtest.c и закрой TODO.
 *
 *   cp starter.c artifacts/backtest.c
 *   make test
 *
 * Концепт: подобрать «идеальные» параметры на прошлом (in-sample) легко —
 *          но на новых данных (out-of-sample) они разваливаются. Это overfitting.
 */
#include <stdio.h>

#define TOTAL 240
#define SPLIT 120

static double price[TOTAL];

static void gen_prices(void) {
    unsigned long s = 777;
    double p = 100.0;
    for (int i = 0; i < TOTAL; i++) {
        s = s * 1103515245UL + 12345UL;
        double r = ((double)((s >> 16) & 0x7FFF) / 32768.0) - 0.5;
        p += r * 2.0;
        if (p < 1.0) p = 1.0;
        price[i] = p;
    }
}

/* TODO 1: sma(end, period) — среднее price[end-period+1 .. end], 0 если окно мало. */

/* TODO 2: backtest(lo, hi, fast, slow) — long-only crossover:
     equity = 1; pos = 0;
     for i от lo+slow до hi: если pos==1 equity *= price[i]/price[i-1];
       f = sma(i,fast), s = sma(i,slow); pos = (f > s) ? 1 : 0;
     вернуть (equity-1)*100. */

int main(void) {
    gen_prices();

    printf("=== BACKTEST & OVERFITTING ===\n");
    printf("in-sample [0,%d), out-of-sample [%d,%d)\n\n", SPLIT, SPLIT, TOTAL);

    int fasts[] = {3, 5, 8};
    int slows[] = {13, 21, 34};

    printf("--- grid search on in-sample ---\n");
    /* TODO 3: перебери все (fast, slow), напечатай in-sample доходность
         "fast %2d slow %2d: in-sample %+7.2f%%\n"; запомни лучший (bf, bs, best_is). */

    /* TODO 4: печать "\nbest in-sample: fast %d slow %d -> %+.2f%%\n";
       затем те же параметры на OOS: backtest(SPLIT, TOTAL, bf, bs) ->
         "same params out-of-sample -> %+.2f%%\n";
       buy&hold OOS: (price[TOTAL-1]/price[SPLIT]-1)*100 ->
         "buy & hold out-of-sample -> %+.2f%%\n". */

    printf("\nlesson: the in-sample winner does not carry over. that gap is overfitting.\n");
    return 0;
}
