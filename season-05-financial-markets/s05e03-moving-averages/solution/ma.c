/*
 * OPERATION MOONLIGHT — s05e03 "Скользящие средние"
 * Эталонное решение: ma.c
 *
 * Концепт серии: сглаживание цены от шума. SMA — среднее последних N значений
 *                (равные веса); EMA — экспоненциальное (свежие важнее). Пересечение
 *                быстрой и медленной средней — классический сигнал (crossover).
 * Задача: посчитать SMA и EMA по ряду цен и найти точки пересечения.
 */
#include <stdio.h>

#define N 20

/* SMA в точке i по окну period: среднее price[i-period+1 .. i]. */
static double sma(const double *price, int i, int period) {
    if (i + 1 < period) return 0.0;         /* окно ещё не набралось */
    double sum = 0.0;
    for (int k = i - period + 1; k <= i; k++) sum += price[k];
    return sum / period;
}

int main(void) {
    /* Цена: рост, шумная полка, разворот вниз. */
    double price[N] = {
        100, 101, 103, 102, 105, 107, 106, 109, 111, 110,
        112, 111, 109, 108, 106, 107, 104, 102, 103, 100
    };

    printf("=== MOVING AVERAGES ===\n");
    printf("%3s %8s %8s %8s\n", "i", "price", "sma5", "ema5");

    /* EMA: рекуррентно. alpha = 2/(period+1); старт — первая цена. */
    int period = 5;
    double alpha = 2.0 / (period + 1);
    double ema = price[0];
    double prev_fast = 0.0, prev_slow = 0.0;   /* для поиска пересечений SMA5 vs SMA10 */

    for (int i = 0; i < N; i++) {
        ema = alpha * price[i] + (1.0 - alpha) * ema;   /* EMA(i) */
        double s = sma(price, i, period);
        if (s > 0.0) printf("%3d %8.2f %8.2f %8.2f\n", i, price[i], s, ema);
        else         printf("%3d %8.2f %8s %8.2f\n", i, price[i], "-", ema);
    }

    /* Crossover: быстрая SMA(5) пересекает медленную SMA(10). */
    printf("\n--- SMA(5) x SMA(10) crossovers ---\n");
    for (int i = 0; i < N; i++) {
        double fast = sma(price, i, 5);
        double slow = sma(price, i, 10);
        if (fast > 0.0 && slow > 0.0) {
            if (prev_fast != 0.0 && prev_slow != 0.0) {
                if (prev_fast <= prev_slow && fast > slow)
                    printf("i=%d BULLISH (fast %.2f crosses above slow %.2f)\n", i, fast, slow);
                else if (prev_fast >= prev_slow && fast < slow)
                    printf("i=%d BEARISH (fast %.2f crosses below slow %.2f)\n", i, fast, slow);
            }
            prev_fast = fast; prev_slow = slow;
        }
    }
    return 0;
}
