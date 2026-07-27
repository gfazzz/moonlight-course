/*
 * OPERATION MOONLIGHT — библиотека Season 8: устойчивая статистика
 * Извлечено из s08e01 (метод Уэлфорда) и s08e02 (медиана, IQR, Тьюки).
 */
#ifndef ML_STATS_H
#define ML_STATS_H

#include <stddef.h>

/* Однопроходный накопитель Уэлфорда: устойчив к катастрофическому вычитанию. */
typedef struct {
    long   n;
    double mean;
    double m2;
    double min, max;
} MlWelford;

void   ml_welford_init(MlWelford *w);
void   ml_welford_push(MlWelford *w, double x);
double ml_welford_mean(const MlWelford *w);
double ml_welford_var(const MlWelford *w);      /* выборочная, деление на n-1 */

/* Квантиль по УЖЕ отсортированному массиву (линейная интерполяция). */
double ml_quantile_sorted(const double *sorted, size_t n, double q);

/* Границы правила Тьюки: [Q1 - 1.5*IQR, Q3 + 1.5*IQR]. */
void   ml_tukey_bounds(const double *sorted, size_t n, double *lo, double *hi);

#endif /* ML_STATS_H */
