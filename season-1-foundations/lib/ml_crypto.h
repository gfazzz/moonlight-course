/*
 * OPERATION MOONLIGHT — библиотека Season 1: простейшая криптография
 * Извлечено из s01e06 (модули и сборка) и s01e08 (сборка инструмента).
 */
#ifndef ML_CRYPTO_H
#define ML_CRYPTO_H

#include <stddef.h>

/* XOR-преобразование на месте. Самообратимо: два вызова с тем же ключом
   возвращают исходные байты. */
void ml_xor_crypt(unsigned char *buf, size_t len, unsigned char key);

/* Аддитивная контрольная сумма буфера. */
unsigned char ml_checksum(const unsigned char *buf, size_t len);

#endif /* ML_CRYPTO_H */
