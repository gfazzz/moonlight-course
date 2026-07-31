/*
 * OPERATION MOONLIGHT — s08e02 "Статистика и выбросы"
 * Каркас. Скопируй в artifacts/stats.c и закрой TODO.
 *
 *   cp starter.c artifacts/stats.c
 *   make test
 *
 * Идея: среднее и sigma неустойчивы (выброс тянет их за собой),
 * медиана и IQR — устойчивы. Правило Тьюки: [Q1 - 1.5*IQR, Q3 + 1.5*IQR].
 */
#include <stdio.h>
#include <stdlib.h>

#define N 20

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);
    return g;
}

static int cmp(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* TODO 1: quantile(s, n, q) по отсортированному массиву:
     pos = q*(n-1); lo = (int)pos; hi = min(lo+1, n-1); frac = pos - lo;
     вернуть s[lo]*(1-frac) + s[hi]*frac. */
static double quantile(const double *s, int n, double q);

int main(void) {
    double data[N] = {
        42, 45, 43, 47, 44, 46, 41, 48, 43, 45,
        44, 46, 42, 47, 45, 43, 46, 44, 980, 1250
    };

    printf("=== устойчивая статистика и выбросы ===\n");
    printf("наблюдений: %d (среди них две явные аномалии)\n\n", N);

    /* TODO 2: mean по data; sd = my_sqrt(Σ(x-mean)² / (N-1)). */

    /* TODO 3: скопировать data в s[], qsort(s, N, sizeof(double), cmp);
       q1 = quantile(s,N,0.25); med = quantile(s,N,0.50); q3 = quantile(s,N,0.75);
       iqr = q3 - q1. */

    /* TODO 4: печать (формат — см. tests/expected.txt):
         "--- неустойчивые (чувствительны к выбросам) ---"
         "среднее:            %8.2f"   "станд. отклонение:  %8.2f\n"
         "--- устойчивые ---"
         "медиана:            %8.2f"   "Q1 / Q3:            %8.2f / %.2f"
         "IQR:                %8.2f\n"
         "среднее выше медианы на %.2f мс — выбросы утащили его вверх\n" */

    /* TODO 5: правило Тьюки: lo_f = q1 - 1.5*iqr; hi_f = q3 + 1.5*iqr;
         "--- правило Тьюки: [%.2f, %.2f] ---"
         для каждого выброса "  выброс: индекс %2d, значение %.0f"
         "найдено выбросов (Тьюки): %d\n" */

    /* TODO 6: правило z-score: z = (x-mean)/sd, |z| > 3:
         "--- правило |z| > 3 (на среднем и sigma) ---"
         "  выброс: индекс %2d"
         "найдено выбросов (z-score): %d\n"
       затем вывод про маскировку (две строки, см. expected.txt). */

    (void)data; (void)quantile;
    return 0;
}

static double quantile(const double *s, int n, double q) {
    (void)s; (void)n; (void)q; return 0.0; /* TODO 1 */
}
