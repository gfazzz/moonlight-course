/*
 * OPERATION MOONLIGHT — s01e04 "Первый инструмент"
 * Модуль crypto: интерфейс (что модуль умеет).
 *
 * Header guard #ifndef/#define/#endif защищает от двойного включения.
 */
#ifndef CRYPTO_H
#define CRYPTO_H

/* XOR-преобразование буфера на месте. Самообратимо: вызови дважды тем же
   ключом — вернёшь исходные байты. */
void xor_crypt(unsigned char *buf, int len, unsigned char key);

/* Аддитивная контрольная сумма буфера (для проверки целостности). */
unsigned char checksum(const unsigned char *buf, int len);

#endif /* CRYPTO_H */
