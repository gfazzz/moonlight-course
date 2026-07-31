/* Реализация индикаторов Season 5. */
#include "ml_indicators.h"

double ml_sma(const double *v, size_t end, size_t period) {
    if (period == 0 || end + 1 < period) return 0.0;
    double s = 0.0;
    for (size_t i = end + 1 - period; i <= end; i++) s += v[i];
    return s / (double)period;
}

double ml_ema(const double *v, size_t n, size_t period) {
    if (n == 0 || period == 0) return 0.0;
    double alpha = 2.0 / ((double)period + 1.0);
    double ema = v[0];
    for (size_t i = 1; i < n; i++) ema = alpha * v[i] + (1.0 - alpha) * ema;
    return ema;
}

double ml_rsi(const double *v, size_t n, size_t period) {
    if (n <= period || period == 0) return 0.0;
    double gain = 0.0, loss = 0.0;
    for (size_t i = 1; i <= period; i++) {
        double d = v[i] - v[i - 1];
        if (d >= 0) gain += d; else loss -= d;
    }
    gain /= (double)period;
    loss /= (double)period;
    for (size_t i = period + 1; i < n; i++) {          /* сглаживание Уайлдера */
        double d = v[i] - v[i - 1];
        double g = (d > 0) ? d : 0.0;
        double l = (d < 0) ? -d : 0.0;
        gain = (gain * (double)(period - 1) + g) / (double)period;
        loss = (loss * (double)(period - 1) + l) / (double)period;
    }
    if (loss == 0.0) return 100.0;
    double rs = gain / loss;
    return 100.0 - 100.0 / (1.0 + rs);
}
