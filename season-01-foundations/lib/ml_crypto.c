#include "ml_crypto.h"

void ml_xor_crypt(unsigned char *buf, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) buf[i] ^= key;
}

unsigned char ml_checksum(const unsigned char *buf, size_t len) {
    unsigned char s = 0;
    for (size_t i = 0; i < len; i++) s = (unsigned char)(s + buf[i]);
    return s;
}
