/*
 * OPERATION MOONLIGHT — библиотека Season 5: индикаторы
 * Извлечено из s05e03 (SMA/EMA) и s05e04 (RSI).
 */
#ifndef ML_INDICATORS_H
#define ML_INDICATORS_H

#include <stddef.h>

/* Простое скользящее среднее по последним period значениям, кончая индексом end. */
double ml_sma(const double *v, size_t end, size_t period);

/* Экспоненциальное скользящее среднее по всему массиву (alpha = 2/(period+1)). */
double ml_ema(const double *v, size_t n, size_t period);

/* RSI по Уайлдеру. Возвращает 0..100; при отсутствии потерь — 100. */
double ml_rsi(const double *v, size_t n, size_t period);

#endif /* ML_INDICATORS_H */
