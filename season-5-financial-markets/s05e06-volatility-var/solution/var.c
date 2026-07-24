/*
 * OPERATION MOONLIGHT — s05e06 "Волатильность и VaR"
 * Эталонное решение: var.c
 *
 * Концепт серии: риск измерим. Волатильность = стандартное отклонение доходностей.
 *                VaR (Value at Risk) — «сколько можно потерять за день с вероятностью 95%».
 *                Два способа: исторический (перцентиль) и параметрический (нормальное).
 * Задача: посчитать волатильность и VaR портфеля $50M двумя методами.
 *
 * sqrt реализован сами (Newton) — чтобы не тянуть -lm.
 */
#include <stdio.h>

#define N 40
#define PORTFOLIO 50000000.0    /* $50M */

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);   /* Ньютон */
    return g;
}

/* Дневные доходности портфеля (в %), 40 наблюдений; в левом хвосте — крупные потери. */
static double ret[N] = {
     0.4, -0.3,  0.8,  1.1, -0.6,  0.2, -1.2,  0.5,  0.9, -0.4,
    -2.8,  0.7,  0.3, -0.5,  1.4, -0.9,  0.6, -3.5,  0.1,  0.8,
    -0.2,  1.0, -0.7,  0.4, -5.2,  0.3,  0.9, -1.1,  0.5, -0.8,
     1.2, -0.6,  0.2, -2.1,  0.7,  0.4, -0.3,  1.5, -1.8,  0.6
};

int main(void) {
    printf("=== VOLATILITY & VaR ===\n");
    printf("portfolio: $%.0f, %d daily returns\n\n", PORTFOLIO, N);

    /* Среднее и стандартное отклонение (выборочное, деление на n-1). */
    double mean = 0.0;
    for (int i = 0; i < N; i++) mean += ret[i];
    mean /= N;

    double var = 0.0;
    for (int i = 0; i < N; i++) { double d = ret[i] - mean; var += d * d; }
    var /= (N - 1);
    double vol = my_sqrt(var);                 /* дневная волатильность, % */
    double vol_ann = vol * my_sqrt(252.0);     /* годовая (252 торговых дня) */

    printf("mean daily return: %+.3f%%\n", mean);
    printf("daily volatility:  %.3f%%\n", vol);
    printf("annualized vol:    %.2f%%\n\n", vol_ann);

    /* Исторический VaR 95%: сортируем и берём 5-й перцентиль. */
    double s[N];
    for (int i = 0; i < N; i++) s[i] = ret[i];
    for (int i = 0; i < N - 1; i++)               /* простая сортировка (n мало) */
        for (int j = 0; j < N - 1 - i; j++)
            if (s[j] > s[j + 1]) { double t = s[j]; s[j] = s[j + 1]; s[j + 1] = t; }

    int k = (int)(0.05 * N);                       /* индекс 5-го перцентиля */
    double var_hist = -s[k];                       /* потеря -> положительное число */

    /* Параметрический VaR 95%: нормальное, z = 1.645. */
    double z = 1.645;
    double var_param = -(mean - z * vol);

    printf("--- 95%% Value at Risk (1 day) ---\n");
    printf("historical VaR:   %.3f%%  =  $%.0f\n", var_hist, var_hist / 100.0 * PORTFOLIO);
    printf("parametric VaR:   %.3f%%  =  $%.0f\n", var_param, var_param / 100.0 * PORTFOLIO);

    printf("\nmeaning: on 95%% of days the 1-day loss stays below this. the other 5%% can be worse.\n");
    return 0;
}
