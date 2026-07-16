/*
 * OPERATION MOONLIGHT — s01e05
 * Модуль crypto (тот же, что собран в s01e04; здесь — для самодостаточности).
 * В реальном Season Project moonlight_decoder это один общий модуль.
 */
#ifndef CRYPTO_H
#define CRYPTO_H

void xor_crypt(unsigned char *buf, int len, unsigned char key);
unsigned char checksum(const unsigned char *buf, int len);

#endif /* CRYPTO_H */
