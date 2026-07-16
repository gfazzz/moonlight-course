/*
 * OPERATION MOONLIGHT — s01e04
 * Модуль crypto: реализация (как модуль это делает).
 */
#include "crypto.h"

void xor_crypt(unsigned char *buf, int len, unsigned char key) {
    for (int i = 0; i < len; i++) {
        buf[i] ^= key;
    }
}

unsigned char checksum(const unsigned char *buf, int len) {
    unsigned char sum = 0;
    for (int i = 0; i < len; i++) {
        sum = (unsigned char)(sum + buf[i]);
    }
    return sum;
}
