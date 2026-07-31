/* Реализация модуля статистики Season 8. */
#include "ml_stats.h"

void ml_welford_init(MlWelford *w) {
    w->n = 0; w->mean = 0.0; w->m2 = 0.0; w->min = 0.0; w->max = 0.0;
}

void ml_welford_push(MlWelford *w, double x) {
    if (w->n == 0) { w->min = w->max = x; }
    else { if (x < w->min) w->min = x; if (x > w->max) w->max = x; }
    w->n++;
    double delta = x - w->mean;
    w->mean += delta / (double)w->n;
    w->m2   += delta * (x - w->mean);
}

double ml_welford_mean(const MlWelford *w) { return w->mean; }

double ml_welford_var(const MlWelford *w) {
    return (w->n > 1) ? w->m2 / (double)(w->n - 1) : 0.0;
}

double ml_quantile_sorted(const double *sorted, size_t n, double q) {
    if (n == 0) return 0.0;
    if (n == 1) return sorted[0];
    double pos = q * (double)(n - 1);
    size_t lo = (size_t)pos;
    size_t hi = (lo + 1 < n) ? lo + 1 : lo;
    double frac = pos - (double)lo;
    return sorted[lo] * (1.0 - frac) + sorted[hi] * frac;
}

void ml_tukey_bounds(const double *sorted, size_t n, double *lo, double *hi) {
    double q1 = ml_quantile_sorted(sorted, n, 0.25);
    double q3 = ml_quantile_sorted(sorted, n, 0.75);
    double iqr = q3 - q1;
    *lo = q1 - 1.5 * iqr;
    *hi = q3 + 1.5 * iqr;
}
