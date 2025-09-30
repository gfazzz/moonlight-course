/*
 * MOONLIGHT OPERATION - Episode 13: Crypto Basics
 * 
 * Mission: Decrypt intercepted messages using XOR and Caesar cipher
 * 
 * Your task:
 * 1. Implement XOR decryption with single-byte key
 * 2. Implement Caesar cipher breaker
 * 3. Crack multi-layer encryption (Caesar + XOR)
 * 
 * Compilation: make
 * Usage: ./crypto_tool <command> <file>
 * 
 * Commands:
 *   xor <file>       - Brute-force XOR encryption
 *   caesar <file>    - Break Caesar cipher
 *   multi <file>     - Decrypt multi-layer encryption
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_FILE_SIZE 1048576  // 1 MB

// ============================================================================
// HELPER FUNCTIONS (Already implemented)
// ============================================================================

/**
 * Read file into buffer
 * Returns: number of bytes read, or -1 on error
 */
int read_file(const char *filename, unsigned char *buffer, int max_size) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening file");
        return -1;
    }
    
    int bytes_read = fread(buffer, 1, max_size, f);
    fclose(f);
    
    return bytes_read;
}

/**
 * Check if data contains mostly printable ASCII characters
 * Returns: percentage of printable characters (0-100)
 */
int is_readable(const unsigned char *data, int len) {
    int printable_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (isprint(data[i]) || isspace(data[i])) {
            printable_count++;
        }
    }
    
    return (printable_count * 100) / len;
}

/**
 * Check if buffer starts with specific string
 */
bool starts_with(const unsigned char *data, int len, const char *prefix) {
    int prefix_len = strlen(prefix);
    if (len < prefix_len) return false;
    
    return memcmp(data, prefix, prefix_len) == 0;
}

// ============================================================================
// TODO: IMPLEMENT THESE FUNCTIONS
// ============================================================================

/**
 * XOR decrypt data with single-byte key
 * 
 * Hint: XOR is self-inverse, so encryption and decryption are the same!
 *       encrypted ^ key = decrypted
 *       decrypted ^ key = encrypted
 * 
 * @param data - buffer to decrypt (modified in-place)
 * @param len  - length of data
 * @param key  - XOR key (single byte)
 */
void xor_decrypt_single_byte(unsigned char *data, int len, unsigned char key) {
    // TODO: XOR each byte with the key
    // YOUR CODE HERE
    
}

/**
 * Brute-force XOR encryption with all possible single-byte keys
 * 
 * Strategy:
 * 1. Try all keys from 0x20 to 0x7E (printable ASCII)
 * 2. For each key, decrypt the data
 * 3. Check if result is readable (>80% printable chars)
 * 4. Check if it starts with "URGENT:" signature
 * 5. Print successful decryptions
 * 
 * @param data - encrypted buffer
 * @param len  - length of data
 */
void brute_force_xor(const unsigned char *data, int len) {
    printf("Brute-forcing XOR encryption...\n\n");
    
    // TODO: Implement brute-force attack
    // Allocate temporary buffer for decryption attempts
    unsigned char *temp = malloc(len);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // TODO: Try all possible keys (0x20 - 0x7E)
    for (unsigned char key = 0x20; key <= 0x7E; key++) {
        // YOUR CODE HERE
        // 1. Copy original data to temp
        // 2. Decrypt with current key
        // 3. Check if readable
        // 4. Check signature "URGENT:"
        // 5. Print if successful
    }
    
    free(temp);
}

/**
 * Decrypt text using Caesar cipher with given shift
 * 
 * Caesar cipher: shift each letter by N positions in alphabet
 * Example: "HELLO" with shift 3 -> "KHOOR"
 * 
 * @param text  - buffer to decrypt (modified in-place)
 * @param shift - number of positions to shift back
 */
void caesar_decrypt(char *text, int shift) {
    // TODO: Implement Caesar cipher decryption
    // Normalize shift to 0-25 range
    shift = shift % 26;
    
    // TODO: For each character:
    // - If uppercase A-Z: shift back
    // - If lowercase a-z: shift back
    // - Otherwise: leave unchanged
    
    // YOUR CODE HERE
}

/**
 * Count letter frequencies in text
 * 
 * @param text - input text
 * @param freq - output array[26] of frequencies (0-100%)
 */
void frequency_analysis(const char *text, float *freq) {
    int counts[26] = {0};
    int total = 0;
    
    // TODO: Count letter occurrences
    for (int i = 0; text[i]; i++) {
        char c = toupper(text[i]);
        if (c >= 'A' && c <= 'Z') {
            // YOUR CODE HERE
        }
    }
    
    // TODO: Convert counts to percentages
    if (total > 0) {
        // YOUR CODE HERE
    }
}

/**
 * Detect Caesar cipher shift using frequency analysis
 * 
 * Strategy:
 * 1. Find most frequent letter in ciphertext
 * 2. Assume it represents 'E' (most common in English)
 * 3. Calculate shift: (most_frequent - 'E' + 26) % 26
 * 
 * @param ciphertext - encrypted text
 * @return detected shift value (0-25)
 */
int detect_caesar_shift(const char *ciphertext) {
    float freq[26];
    frequency_analysis(ciphertext, freq);
    
    // TODO: Find letter with maximum frequency
    int max_index = 0;
    // YOUR CODE HERE
    
    // TODO: Calculate shift assuming max_index is 'E'
    // 'E' is at position 4 in alphabet (A=0, B=1, ..., E=4)
    int shift = 0;  // YOUR CODE HERE
    
    return shift;
}

/**
 * Break Caesar cipher using brute-force
 * Tries all 26 possible shifts and prints results
 * 
 * @param ciphertext - encrypted text
 */
void brute_force_caesar(const char *ciphertext) {
    printf("Brute-forcing Caesar cipher...\n\n");
    
    int len = strlen(ciphertext);
    char *temp = malloc(len + 1);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // TODO: Try all 26 shifts
    for (int shift = 0; shift < 26; shift++) {
        // YOUR CODE HERE
        // 1. Copy ciphertext to temp
        // 2. Decrypt with current shift
        // 3. Print result
    }
    
    free(temp);
}

/**
 * Decrypt multi-layer encryption: Caesar then XOR
 * 
 * IMPORTANT: Decryption order is REVERSE of encryption!
 * If encrypted as: plaintext -> Caesar -> XOR -> ciphertext
 * Then decrypt as:  ciphertext -> XOR -> Caesar -> plaintext
 * 
 * @param data         - encrypted buffer (modified in-place)
 * @param len          - length of data
 * @param caesar_shift - Caesar cipher shift
 * @param xor_key      - XOR key
 */
void multi_layer_decrypt(unsigned char *data, int len, int caesar_shift, unsigned char xor_key) {
    // TODO: Decrypt in reverse order
    // 1. First XOR (last applied)
    // YOUR CODE HERE
    
    // 2. Then Caesar (first applied)
    // YOUR CODE HERE
}

// ============================================================================
// COMMAND-LINE INTERFACE
// ============================================================================

void print_usage(const char *program_name) {
    printf("MOONLIGHT Crypto Tool - Episode 13\n\n");
    printf("Usage: %s <command> <file>\n\n", program_name);
    printf("Commands:\n");
    printf("  xor <file>       Brute-force XOR encryption\n");
    printf("  caesar <file>    Break Caesar cipher\n");
    printf("  multi <file>     Decrypt multi-layer (Caesar+XOR)\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s xor message1.enc\n", program_name);
    printf("  %s caesar message2.enc\n", program_name);
    printf("  %s multi message3.enc\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *command = argv[1];
    const char *filename = argv[2];
    
    // Read input file
    unsigned char *buffer = malloc(MAX_FILE_SIZE);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int file_size = read_file(filename, buffer, MAX_FILE_SIZE);
    if (file_size < 0) {
        free(buffer);
        return 1;
    }
    
    printf("File: %s (%d bytes)\n\n", filename, file_size);
    
    // Execute command
    if (strcmp(command, "xor") == 0) {
        brute_force_xor(buffer, file_size);
    }
    else if (strcmp(command, "caesar") == 0) {
        buffer[file_size] = '\0';  // Null-terminate
        brute_force_caesar((char*)buffer);
    }
    else if (strcmp(command, "multi") == 0) {
        // Hint given: "First rotate, then XOR with 0x5A"
        // This means: plaintext -> Caesar(?) -> XOR(0x5A) -> ciphertext
        // So we decrypt: ciphertext -> XOR(0x5A) -> Caesar(?) -> plaintext
        
        printf("Multi-layer decryption (hint: XOR key = 0x5A)\n\n");
        
        // TODO: Try different Caesar shifts with XOR 0x5A
        // YOUR CODE HERE
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", command);
        print_usage(argv[0]);
        free(buffer);
        return 1;
    }
    
    free(buffer);
    return 0;
}
