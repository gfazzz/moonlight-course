/* s01e05 — модуль crypto (готов, из s01e04; в этой серии фокус на struct). */
#ifndef CRYPTO_H
#define CRYPTO_H

void xor_crypt(unsigned char *buf, int len, unsigned char key);
unsigned char checksum(const unsigned char *buf, int len);

#endif /* CRYPTO_H */
