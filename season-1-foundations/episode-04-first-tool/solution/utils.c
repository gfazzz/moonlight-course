/*
 * OPERATION MOONLIGHT - Episode 04
 * Solution: utils.c
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

unsigned char* read_file(const char *filename, size_t *length) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error opening file for reading");
        return NULL;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Allocate memory
    unsigned char *data = malloc(*length + 1);  // +1 for safety null terminator
    if (data == NULL) {
        fclose(fp);
        perror("Error allocating memory");
        return NULL;
    }
    
    // Read data
    size_t read = fread(data, 1, *length, fp);
    fclose(fp);
    
    if (read != *length) {
        fprintf(stderr, "Warning: Read %zu bytes, expected %zu\n", read, *length);
    }
    
    data[*length] = '\0';  // Null terminate for safety
    return data;
}

int write_file(const char *filename, const unsigned char *data, size_t length) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return -1;
    }
    
    size_t written = fwrite(data, 1, length, fp);
    fclose(fp);
    
    if (written != length) {
        fprintf(stderr, "Warning: Wrote %zu bytes, expected %zu\n", written, length);
        return -1;
    }
    
    return 0;
}

void print_stats(const unsigned char *data, size_t length) {
    int letters = 0, digits = 0, spaces = 0, others = 0;
    
    for (size_t i = 0; i < length; i++) {
        if (isalpha(data[i])) {
            letters++;
        } else if (isdigit(data[i])) {
            digits++;
        } else if (isspace(data[i])) {
            spaces++;
        } else {
            others++;
        }
    }
    
    printf("\n=== STATISTICS ===\n");
    printf("Total bytes: %zu\n", length);
    printf("Letters: %d (%.1f%%)\n", letters, (letters * 100.0) / length);
    printf("Digits: %d (%.1f%%)\n", digits, (digits * 100.0) / length);
    printf("Spaces: %d (%.1f%%)\n", spaces, (spaces * 100.0) / length);
    printf("Others: %d (%.1f%%)\n", others, (others * 100.0) / length);
}

void print_hex(const unsigned char *data, size_t length) {
    printf("Hex dump:\n");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (length % 16 != 0) {
        printf("\n");
    }
}
