/*
 * OPERATION MOONLIGHT - Episode 04
 * Starter: decoder.c - Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"
#include "utils.h"

void print_help(const char *program_name) {
    printf("MOONLIGHT DECODER v1.0\n");
    printf("======================\n\n");
    printf("Usage: %s -i <input> -o <output> -k <key> [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -i <file>    Input file (encrypted)\n");
    printf("  -o <file>    Output file (decrypted)\n");
    printf("  -k <key>     XOR key (hex, e.g., 0x42)\n");
    printf("  -v           Verbose mode (show statistics)\n");
    printf("  -h           Show this help\n");
    printf("\nExample:\n");
    printf("  %s -i encrypted.dat -o decrypted.txt -k 0x42 -v\n", program_name);
}

int main(int argc, char *argv[]) {
    // TODO: Parse command line arguments
    char *input_file = NULL;
    char *output_file = NULL;
    unsigned char key = 0;
    int verbose = 0;
    
    // if (argc < 2) {
    //     print_help(argv[0]);
    //     return 1;
    // }
    
    // for (int i = 1; i < argc; i++) {
    //     if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
    //         input_file = argv[++i];
    //     } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
    //         output_file = argv[++i];
    //     } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
    //         key = (unsigned char)strtol(argv[++i], NULL, 0);
    //     } else if (strcmp(argv[i], "-v") == 0) {
    //         verbose = 1;
    //     } else if (strcmp(argv[i], "-h") == 0) {
    //         print_help(argv[0]);
    //         return 0;
    //     }
    // }
    
    // TODO: Validate arguments
    // if (!input_file || !output_file || key == 0) {
    //     fprintf(stderr, "Error: Missing required arguments\n");
    //     print_help(argv[0]);
    //     return 1;
    // }
    
    // TODO: Read input file
    // size_t length;
    // unsigned char *data = read_file(input_file, &length);
    // if (!data) {
    //     fprintf(stderr, "Error: Failed to read input file\n");
    //     return 1;
    // }
    
    // if (verbose) {
    //     printf("Read %zu bytes from %s\n", length, input_file);
    // }
    
    // TODO: Decrypt data
    // xor_decrypt(data, length, key);
    
    // TODO: Validate decrypted message
    // if (!validate_message(data, length)) {
    //     fprintf(stderr, "Warning: Decrypted message may be invalid\n");
    // }
    
    // TODO: Write output file
    // if (write_file(output_file, data, length) != 0) {
    //     fprintf(stderr, "Error: Failed to write output file\n");
    //     free(data);
    //     return 1;
    // }
    
    // if (verbose) {
    //     printf("Wrote %zu bytes to %s\n", length, output_file);
    //     print_stats(data, length);
    // }
    
    // TODO: Cleanup
    // free(data);
    
    printf("Decryption complete!\n");
    return 0;
}
