/*
 * OPERATION MOONLIGHT — s05e03 "Скользящие средние"
 * Каркас. Скопируй в artifacts/ma.c и закрой TODO.
 *
 *   cp starter.c artifacts/ma.c
 *   make test
 *
 * Концепт: SMA — среднее последних N (равные веса); EMA — экспоненциальное
 *          (свежие важнее). Пересечение быстрой и медленной SMA — сигнал.
 */
#include <stdio.h>

#define N 20

/* TODO 1: sma(price, i, period) — если окно не набралось (i+1 < period) верни 0;
   иначе среднее price[i-period+1 .. i]. */

int main(void) {
    double price[N] = {
        100, 101, 103, 102, 105, 107, 106, 109, 111, 110,
        112, 111, 109, 108, 106, 107, 104, 102, 103, 100
    };

    printf("=== MOVING AVERAGES ===\n");
    printf("%3s %8s %8s %8s\n", "i", "price", "sma5", "ema5");

    int period = 5;
    double alpha = 2.0 / (period + 1);   /* сглаживающий коэффициент EMA */
    double ema = price[0];
    double prev_fast = 0.0, prev_slow = 0.0;

    for (int i = 0; i < N; i++) {
        /* TODO 2: ema = alpha*price[i] + (1-alpha)*ema;  s = sma(price,i,period);
           если s>0: "%3d %8.2f %8.2f %8.2f\n"; иначе "%3d %8.2f %8s %8.2f\n" (прочерк). */
    }

    printf("\n--- SMA(5) x SMA(10) crossovers ---\n");
    /* TODO 3: для каждого i считай fast=sma(...,5), slow=sma(...,10);
       если оба > 0 и есть предыдущие:
         prev_fast<=prev_slow && fast>slow  -> "i=%d BULLISH (fast %.2f crosses above slow %.2f)\n"
         prev_fast>=prev_slow && fast<slow  -> "i=%d BEARISH (fast %.2f crosses below slow %.2f)\n"
       обновляй prev_fast/prev_slow. */
    (void)prev_fast; (void)prev_slow; (void)ema;
    return 0;
}
