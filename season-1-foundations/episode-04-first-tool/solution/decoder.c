/*
 * OPERATION MOONLIGHT - Episode 04
 * Solution: decoder.c - Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"
#include "utils.h"

void print_help(const char *program_name) {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║   MOONLIGHT DECODER v1.0              ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
    printf("Usage: %s -i <input> -o <output> -k <key> [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -i <file>    Input file (encrypted)\n");
    printf("  -o <file>    Output file (decrypted)\n");
    printf("  -k <key>     XOR key (hex, e.g., 0x42)\n");
    printf("  -v           Verbose mode (statistics)\n");
    printf("  -x           Show hex dump\n");
    printf("  -h           Show this help\n");
    printf("\nExample:\n");
    printf("  %s -i encrypted.dat -o decrypted.txt -k 0x42 -v\n", program_name);
}

int main(int argc, char *argv[]) {
    char *input_file = NULL;
    char *output_file = NULL;
    unsigned char key = 0;
    int verbose = 0;
    int show_hex = 0;
    
    // Parse command line arguments
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            key = (unsigned char)strtol(argv[++i], NULL, 0);
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            show_hex = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }
    
    // Validate arguments
    if (!input_file || !output_file || key == 0) {
        fprintf(stderr, "Error: Missing required arguments\n\n");
        print_help(argv[0]);
        return 1;
    }
    
    printf("=== MOONLIGHT DECODER ===\n");
    printf("Input:  %s\n", input_file);
    printf("Output: %s\n", output_file);
    printf("Key:    0x%02X\n\n", key);
    
    // Read input file
    size_t length;
    unsigned char *data = read_file(input_file, &length);
    if (!data) {
        fprintf(stderr, "Error: Failed to read input file\n");
        return 1;
    }
    
    if (verbose) {
        printf("Read %zu bytes\n", length);
    }
    
    if (show_hex) {
        printf("\nBefore decryption:\n");
        print_hex(data, length > 64 ? 64 : length);
        if (length > 64) printf("... (showing first 64 bytes)\n");
    }
    
    // Decrypt data
    xor_decrypt(data, length, key);
    
    // Validate decrypted message
    if (!validate_message(data, length)) {
        fprintf(stderr, "⚠️  Warning: Decrypted message may be invalid\n");
        fprintf(stderr, "   (Key might be wrong)\n\n");
    } else {
        printf("✓ Message validation passed\n\n");
    }
    
    // Write output file
    if (write_file(output_file, data, length) != 0) {
        fprintf(stderr, "Error: Failed to write output file\n");
        free(data);
        return 1;
    }
    
    printf("✓ Decrypted %zu bytes\n", length);
    printf("✓ Saved to %s\n", output_file);
    
    // Print statistics if verbose
    if (verbose) {
        print_stats(data, length);
    }
    
    if (show_hex) {
        printf("\nAfter decryption:\n");
        print_hex(data, length > 64 ? 64 : length);
        if (length > 64) printf("... (showing first 64 bytes)\n");
    }
    
    // Calculate and show checksum
    unsigned char checksum;
    calculate_checksum(data, length, &checksum);
    if (verbose) {
        printf("\nChecksum: 0x%02X\n", checksum);
    }
    
    // Cleanup
    free(data);
    
    printf("\n=== DECRYPTION COMPLETE ===\n");
    return 0;
}
