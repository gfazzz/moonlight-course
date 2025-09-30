/*
 * OPERATION MOONLIGHT - Episode 04
 * Solution: utils.h
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// File I/O
unsigned char* read_file(const char *filename, size_t *length);
int write_file(const char *filename, const unsigned char *data, size_t length);

// Statistics
void print_stats(const unsigned char *data, size_t length);

// Helper
void print_hex(const unsigned char *data, size_t length);

#endif // UTILS_H
