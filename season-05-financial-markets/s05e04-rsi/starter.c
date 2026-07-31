/*
 * OPERATION MOONLIGHT — s05e04 "RSI"
 * Каркас. Скопируй в artifacts/rsi.c и закрой TODO.
 *
 *   cp starter.c artifacts/rsi.c
 *   make test
 *
 * Концепт: RSI = 100 - 100/(1+RS), RS = avg_gain/avg_loss за period.
 *          >70 — перекуплен, <30 — перепродан. Сглаживание — по Уайлдеру.
 */
#include <stdio.h>

#define N 22
#define PERIOD 14

int main(void) {
    double price[N] = {
        100, 101.5, 103, 104.2, 106, 108, 107.5, 110, 112, 113.5,
        115, 116.2, 118, 119, 120,
        117, 114, 110, 106, 103, 101, 100
    };

    printf("=== RSI (period %d) ===\n", PERIOD);

    /* TODO 1: первые PERIOD изменений -> суммы gain и loss;
       avg_gain = gain/PERIOD; avg_loss = loss/PERIOD. */
    double avg_gain = 0.0, avg_loss = 0.0;

    printf("%3s %8s %8s %-14s\n", "i", "price", "rsi", "zone");

    for (int i = PERIOD; i < N; i++) {
        /* TODO 2: при i > PERIOD обнови avg по Уайлдеру:
             ch = price[i]-price[i-1]; g = max(ch,0); l = max(-ch,0);
             avg_gain = (avg_gain*(PERIOD-1) + g)/PERIOD; аналогично avg_loss. */

        /* TODO 3: rsi: если avg_loss==0 -> 100; иначе rs=avg_gain/avg_loss,
             rsi = 100 - 100/(1+rs).
           zone: >70 "OVERBOUGHT", <30 "OVERSOLD", иначе "neutral".
           Печать "%3d %8.2f %8.2f %-14s\n". */
    }

    printf("\nRSI>70 warns of a pullback; RSI<30 of a bounce. Extremes hint reversals.\n");
    (void)avg_gain; (void)avg_loss;
    return 0;
}
