/*
 * OPERATION MOONLIGHT - Episode 04
 * Starter: utils.c - Utilities Module Implementation
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: Implement file reading
unsigned char* read_file(const char *filename, size_t *length) {
    // FILE *fp = fopen(filename, "rb");
    // if (fp == NULL) {
    //     perror("Error opening file for reading");
    //     return NULL;
    // }
    // 
    // // Get file size
    // fseek(fp, 0, SEEK_END);
    // *length = ftell(fp);
    // fseek(fp, 0, SEEK_SET);
    // 
    // // Allocate memory
    // unsigned char *data = malloc(*length);
    // if (data == NULL) {
    //     fclose(fp);
    //     return NULL;
    // }
    // 
    // // Read data
    // fread(data, 1, *length, fp);
    // fclose(fp);
    // 
    // return data;
    
    *length = 0;
    return NULL;  // Placeholder
}

// TODO: Implement file writing
int write_file(const char *filename, const unsigned char *data, size_t length) {
    // FILE *fp = fopen(filename, "wb");
    // if (fp == NULL) {
    //     perror("Error opening file for writing");
    //     return -1;
    // }
    // 
    // size_t written = fwrite(data, 1, length, fp);
    // fclose(fp);
    // 
    // return (written == length) ? 0 : -1;
    
    return -1;  // Placeholder
}

// TODO: Implement statistics printing
void print_stats(const unsigned char *data, size_t length) {
    // int letters = 0, digits = 0, spaces = 0, others = 0;
    // 
    // for (size_t i = 0; i < length; i++) {
    //     if (isalpha(data[i])) {
    //         letters++;
    //     } else if (isdigit(data[i])) {
    //         digits++;
    //     } else if (isspace(data[i])) {
    //         spaces++;
    //     } else {
    //         others++;
    //     }
    // }
    // 
    // printf("=== STATISTICS ===\n");
    // printf("Total bytes: %zu\n", length);
    // printf("Letters: %d\n", letters);
    // printf("Digits: %d\n", digits);
    // printf("Spaces: %d\n", spaces);
    // printf("Others: %d\n", others);
}

// TODO: Implement hex dump
void print_hex(const unsigned char *data, size_t length) {
    // printf("Hex dump:\n");
    // for (size_t i = 0; i < length; i++) {
    //     printf("%02X ", data[i]);
    //     if ((i + 1) % 16 == 0) {
    //         printf("\n");
    //     }
    // }
    // if (length % 16 != 0) {
    //     printf("\n");
    // }
}
