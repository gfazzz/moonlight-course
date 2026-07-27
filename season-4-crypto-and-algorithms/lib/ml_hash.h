/*
 * OPERATION MOONLIGHT — библиотека Season 4: хеширование
 * Извлечено из s04e03 (хеш-функции) и s04e04 (хеш-таблица).
 *
 * Это ПЕРЕИСПОЛЬЗУЕМЫЙ модуль: его подключают проекты последующих сезонов
 * (trading_system в S5, moonlight_core в S10) через #include и линковку.
 */
#ifndef ML_HASH_H
#define ML_HASH_H

#include <stddef.h>

/* djb2 (Bernstein): h = h*33 + c, начальное значение 5381. */
unsigned long ml_hash_djb2(const char *s);

/* Индекс корзины для таблицы размера n (n > 0). */
size_t ml_hash_bucket(const char *s, size_t n);

#endif /* ML_HASH_H */
