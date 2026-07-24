/*
 * OPERATION MOONLIGHT — s05e04 "RSI"
 * Эталонное решение: rsi.c
 *
 * Концепт серии: RSI (Relative Strength Index) — осциллятор 0..100, меряет силу
 *                движения. RSI = 100 - 100/(1+RS), RS = среднийприрост/среднеепадение
 *                за период. >70 — перекуплен (риск отката), <30 — перепродан.
 * Задача: посчитать RSI по ряду цен (метод Уайлдера) и отметить экстремумы.
 */
#include <stdio.h>

#define N 22
#define PERIOD 14

int main(void) {
    /* Ряд: сильный рост (загонит RSI вверх), затем обвал (RSI вниз). */
    double price[N] = {
        100, 101.5, 103, 104.2, 106, 108, 107.5, 110, 112, 113.5,
        115, 116.2, 118, 119, 120,           /* рост -> перекупленность */
        117, 114, 110, 106, 103, 101, 100    /* обвал -> перепроданность */
    };

    printf("=== RSI (period %d) ===\n", PERIOD);

    /* Шаг 1: средний прирост и среднее падение за первые PERIOD изменений. */
    double gain = 0.0, loss = 0.0;
    for (int i = 1; i <= PERIOD; i++) {
        double ch = price[i] - price[i - 1];
        if (ch > 0) gain += ch; else loss += -ch;
    }
    double avg_gain = gain / PERIOD;
    double avg_loss = loss / PERIOD;

    printf("%3s %8s %8s %-14s\n", "i", "price", "rsi", "zone");

    /* Шаг 2: сглаживание Уайлдера для последующих точек. */
    for (int i = PERIOD; i < N; i++) {
        if (i > PERIOD) {
            double ch = price[i] - price[i - 1];
            double g = ch > 0 ? ch : 0.0;
            double l = ch < 0 ? -ch : 0.0;
            avg_gain = (avg_gain * (PERIOD - 1) + g) / PERIOD;
            avg_loss = (avg_loss * (PERIOD - 1) + l) / PERIOD;
        }
        double rsi;
        if (avg_loss == 0.0) rsi = 100.0;          /* только рост */
        else {
            double rs = avg_gain / avg_loss;
            rsi = 100.0 - 100.0 / (1.0 + rs);
        }
        const char *zone = rsi > 70 ? "OVERBOUGHT" : (rsi < 30 ? "OVERSOLD" : "neutral");
        printf("%3d %8.2f %8.2f %-14s\n", i, price[i], rsi, zone);
    }

    printf("\nRSI>70 warns of a pullback; RSI<30 of a bounce. Extremes hint reversals.\n");
    return 0;
}
