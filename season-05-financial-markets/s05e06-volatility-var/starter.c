/*
 * OPERATION MOONLIGHT — s05e06 "Волатильность и VaR"
 * Каркас. Скопируй в artifacts/var.c и закрой TODO.
 *
 *   cp starter.c artifacts/var.c
 *   make test
 *
 * Концепт: волатильность = std доходностей; VaR = «потеря, которую не превысим
 *          с вероятностью 95%». Историческая (перцентиль) и параметрическая (нормаль).
 * sqrt реализуй сам (Newton) — не тяни -lm.
 */
#include <stdio.h>

#define N 40
#define PORTFOLIO 50000000.0

/* TODO 1: my_sqrt(x) — Ньютон: g = x; повтори g = 0.5*(g + x/g) ~60 раз. */

static double ret[N] = {
     0.4, -0.3,  0.8,  1.1, -0.6,  0.2, -1.2,  0.5,  0.9, -0.4,
    -2.8,  0.7,  0.3, -0.5,  1.4, -0.9,  0.6, -3.5,  0.1,  0.8,
    -0.2,  1.0, -0.7,  0.4, -5.2,  0.3,  0.9, -1.1,  0.5, -0.8,
     1.2, -0.6,  0.2, -2.1,  0.7,  0.4, -0.3,  1.5, -1.8,  0.6
};

int main(void) {
    printf("=== VOLATILITY & VaR ===\n");
    printf("portfolio: $%.0f, %d daily returns\n\n", PORTFOLIO, N);

    /* TODO 2: mean = среднее ret[]; var = Σ(ret-mean)^2 / (N-1); vol = sqrt(var);
       vol_ann = vol * sqrt(252). Печать:
         "mean daily return: %+.3f%%", "daily volatility:  %.3f%%", "annualized vol:    %.2f%%". */

    /* TODO 3: исторический VaR 95%: скопируй ret в s[], отсортируй по возрастанию,
       k = (int)(0.05*N); var_hist = -s[k]. */

    /* TODO 4: параметрический VaR 95%: z = 1.645; var_param = -(mean - z*vol).
       Печать:
         "--- 95%% Value at Risk (1 day) ---"
         "historical VaR:   %.3f%%  =  $%.0f"  (var_hist, var_hist/100*PORTFOLIO)
         "parametric VaR:   %.3f%%  =  $%.0f". */

    printf("\nmeaning: on 95%% of days the 1-day loss stays below this. the other 5%% can be worse.\n");
    return 0;
}
