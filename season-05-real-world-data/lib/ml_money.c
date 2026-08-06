/*
 * OPERATION MOONLIGHT — библиотека Season 5: деньги в фиксированной точке
 * Реализация ml_money.h. Извлечено из s05e02.
 */
#include "ml_money.h"
#include <stdio.h>

int ml_money_parse(const char *s, int scale, ml_money_t *out) {
    if (!s || !out || scale <= 0) return 0;

    int64_t whole = 0, frac = 0;
    int digits = 0, neg = 0, maxdig = 0;
    for (int k = scale; k > 1; k /= 10) maxdig++;   /* 100 -> 2 знака */

    const char *p = s;
    if (*p == '-') { neg = 1; p++; }
    else if (*p == '+') p++;
    if (*p < '0' || *p > '9') return 0;

    while (*p >= '0' && *p <= '9') {
        if (whole > (INT64_MAX - 9) / 10) return 0;  /* переполнение целой части */
        whole = whole * 10 + (*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;
        if (*p < '0' || *p > '9') return 0;          /* точка без цифр */
        while (*p >= '0' && *p <= '9') {
            if (digits == maxdig) return 0;          /* лишний знак — это не деньги */
            frac = frac * 10 + (*p - '0');
            digits++;
            p++;
        }
    }
    if (*p != '\0') return 0;

    while (digits < maxdig) { frac *= 10; digits++; }

    if (whole > (INT64_MAX - frac) / scale) return 0;
    ml_money_t v = whole * scale + frac;
    *out = neg ? -v : v;
    return 1;
}

int ml_money_str(ml_money_t m, int scale, char *buf, size_t n) {
    if (!buf || n == 0 || scale <= 0) return 0;

    const char *sign = "";
    /* Модуль берём в беззнаковом виде: -INT64_MIN не представим. */
    uint64_t um;
    if (m < 0) { sign = "-"; um = (uint64_t)(-(m + 1)) + 1u; }
    else       { um = (uint64_t)m; }

    int width = 0;
    for (int k = scale; k > 1; k /= 10) width++;

    int need = snprintf(buf, n, "%s%llu.%0*llu", sign,
                        (unsigned long long)(um / (uint64_t)scale),
                        width,
                        (unsigned long long)(um % (uint64_t)scale));
    return (need > 0 && (size_t)need < n) ? 1 : 0;
}

ml_money_t ml_money_mul_ppm(ml_money_t amount, int64_t ppm, int mode) {
    int64_t num = amount * ppm;
    int64_t q   = num / 1000000;
    int64_t r   = num % 1000000;
    if (r == 0) return q;

    int64_t ar = r < 0 ? -r : r;
    int step = 0;

    if (mode == ML_ROUND_HALF_UP) {
        step = (ar * 2 >= 1000000);
    } else if (mode == ML_ROUND_HALF_EVEN) {
        if      (ar * 2 > 1000000) step = 1;
        else if (ar * 2 < 1000000) step = 0;
        else                       step = (q % 2 != 0);
    }
    /* ML_ROUND_TRUNC оставляет step нулевым: целочисленное деление
       уже отбросило дробную часть в сторону нуля. */
    if (!step) return q;
    return num < 0 ? q - 1 : q + 1;    /* шаг от нуля, симметрично для знаков */
}

int ml_money_add(ml_money_t a, ml_money_t b, ml_money_t *out) {
    if (!out) return 0;
    if (b > 0 && a > INT64_MAX - b) return 0;
    if (b < 0 && a < INT64_MIN - b) return 0;
    *out = a + b;
    return 1;
}

int ml_money_split(ml_money_t amount, int n, ml_money_t *parts) {
    if (n <= 0 || !parts) return 0;

    ml_money_t base = amount / n;
    ml_money_t rem  = amount % n;      /* знак остатка совпадает со знаком amount */
    ml_money_t step = rem < 0 ? -1 : 1;
    ml_money_t k    = rem < 0 ? -rem : rem;

    for (int i = 0; i < n; i++)
        parts[i] = base + (i < k ? step : 0);
    return 1;
}
