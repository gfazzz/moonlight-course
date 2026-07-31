/*
 * OPERATION MOONLIGHT — библиотека Season 6: СКУД
 * Извлечено из s06e04 (Wiegand-26).
 */
#ifndef ML_WIEGAND_H
#define ML_WIEGAND_H

#include <stdint.h>

/* Собрать корректный 26-битный кадр из кода объекта и номера карты. */
uint32_t ml_wiegand_encode(uint8_t facility, uint16_t card);

/* Разобрать кадр. Возврат: 1 — чётности сошлись, 0 — кадр повреждён. */
int ml_wiegand_decode(uint32_t code, uint8_t *facility, uint16_t *card);

#endif /* ML_WIEGAND_H */
