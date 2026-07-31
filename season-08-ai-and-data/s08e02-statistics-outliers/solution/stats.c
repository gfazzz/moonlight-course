/*
 * OPERATION MOONLIGHT — s08e02 "Статистика и выбросы"
 * Эталонное решение: stats.c
 *
 * Концепт серии: среднее и стандартное отклонение НЕустойчивы — один выброс
 *                утаскивает их за собой. Медиана и межквартильный размах (IQR)
 *                устойчивы: их не сдвинуть меньшинством точек. Отсюда правило
 *                Тьюки: выброс — то, что вне [Q1 - 1.5*IQR, Q3 + 1.5*IQR].
 *                Ловушка: правило z-score само использует среднее и sigma,
 *                испорченные выбросами, — оно их и «маскирует».
 * Задача: посчитать обе группы метрик, найти выбросы двумя способами и показать,
 *         почему устойчивые оценки надёжнее.
 */
#include <stdio.h>
#include <stdlib.h>

#define N 20

static double my_sqrt(double x) {          /* без -lm: Ньютон */
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);
    return g;
}

static int cmp(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Квантиль по отсортированному массиву (линейная интерполяция). */
static double quantile(const double *s, int n, double q) {
    double pos = q * (n - 1);
    int lo = (int)pos;
    int hi = (lo + 1 < n) ? lo + 1 : lo;
    double frac = pos - lo;
    return s[lo] * (1.0 - frac) + s[hi] * frac;
}

int main(void) {
    /* Задержки отклика узлов, мс. Два последних значения — явные аномалии. */
    double data[N] = {
        42, 45, 43, 47, 44, 46, 41, 48, 43, 45,
        44, 46, 42, 47, 45, 43, 46, 44, 980, 1250
    };

    printf("=== устойчивая статистика и выбросы ===\n");
    printf("наблюдений: %d (среди них две явные аномалии)\n\n", N);

    /* --- Неустойчивые оценки --- */
    double sum = 0.0;
    for (int i = 0; i < N; i++) sum += data[i];
    double mean = sum / N;

    double m2 = 0.0;
    for (int i = 0; i < N; i++) { double d = data[i] - mean; m2 += d * d; }
    double sd = my_sqrt(m2 / (N - 1));

    /* --- Устойчивые оценки --- */
    double s[N];
    for (int i = 0; i < N; i++) s[i] = data[i];
    qsort(s, N, sizeof(double), cmp);

    double q1  = quantile(s, N, 0.25);
    double med = quantile(s, N, 0.50);
    double q3  = quantile(s, N, 0.75);
    double iqr = q3 - q1;

    printf("--- неустойчивые (чувствительны к выбросам) ---\n");
    printf("среднее:            %8.2f\n", mean);
    printf("станд. отклонение:  %8.2f\n\n", sd);

    printf("--- устойчивые ---\n");
    printf("медиана:            %8.2f\n", med);
    printf("Q1 / Q3:            %8.2f / %.2f\n", q1, q3);
    printf("IQR:                %8.2f\n\n", iqr);

    printf("среднее выше медианы на %.2f мс — выбросы утащили его вверх\n\n", mean - med);

    /* --- Правило Тьюки: границы по IQR --- */
    double lo_f = q1 - 1.5 * iqr;
    double hi_f = q3 + 1.5 * iqr;
    printf("--- правило Тьюки: [%.2f, %.2f] ---\n", lo_f, hi_f);

    int tukey = 0;
    for (int i = 0; i < N; i++)
        if (data[i] < lo_f || data[i] > hi_f) {
            printf("  выброс: индекс %2d, значение %.0f\n", i, data[i]);
            tukey++;
        }
    printf("найдено выбросов (Тьюки): %d\n\n", tukey);

    /* --- Правило z-score: считает по ИСПОРЧЕННЫМ среднему и sigma --- */
    printf("--- правило |z| > 3 (на среднем и sigma) ---\n");
    int zfound = 0;
    for (int i = 0; i < N; i++) {
        double z = (data[i] - mean) / sd;
        if (z > 3.0 || z < -3.0) { printf("  выброс: индекс %2d\n", i); zfound++; }
    }
    printf("найдено выбросов (z-score): %d\n\n", zfound);

    printf("вывод: z-score опирается на среднее и sigma, которые сами испорчены\n");
    printf("       выбросами, и потому маскирует их. IQR устойчив.\n");
    return 0;
}
