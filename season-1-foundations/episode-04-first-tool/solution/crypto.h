/*
 * OPERATION MOONLIGHT - Episode 04
 * Solution: crypto.h - Cryptography Module Header
 */

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

// XOR encryption/decryption (symmetric)
unsigned char xor_byte(unsigned char byte, unsigned char key);
void xor_decrypt(unsigned char *data, size_t length, unsigned char key);
void xor_encrypt(unsigned char *data, size_t length, unsigned char key);

// Validation
int validate_message(const unsigned char *data, size_t length);

// Checksum calculation
void calculate_checksum(const unsigned char *data, size_t length, unsigned char *result);

#endif // CRYPTO_H
