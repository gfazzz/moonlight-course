/*
 * OPERATION MOONLIGHT - Episode 04
 * Solution: crypto.c
 */

#include "crypto.h"
#include <ctype.h>

unsigned char xor_byte(unsigned char byte, unsigned char key) {
    return byte ^ key;
}

void xor_decrypt(unsigned char *data, size_t length, unsigned char key) {
    for (size_t i = 0; i < length; i++) {
        data[i] = xor_byte(data[i], key);
    }
}

void xor_encrypt(unsigned char *data, size_t length, unsigned char key) {
    xor_decrypt(data, length, key);  // XOR is symmetric!
}

int validate_message(const unsigned char *data, size_t length) {
    int printable_count = 0;
    
    for (size_t i = 0; i < length; i++) {
        if (isprint(data[i]) || isspace(data[i])) {
            printable_count++;
        }
    }
    
    // Consider valid if > 90% printable
    return (printable_count * 100 / length) > 90;
}

void calculate_checksum(const unsigned char *data, size_t length, unsigned char *result) {
    unsigned char sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }
    *result = sum;
}
