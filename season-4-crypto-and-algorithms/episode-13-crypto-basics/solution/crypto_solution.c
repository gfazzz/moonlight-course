/*
 * MOONLIGHT OPERATION - Episode 13: Crypto Basics
 * SOLUTION - For reference only!
 * 
 * This is a complete implementation of the crypto tool.
 * Use this ONLY after you've attempted the challenges yourself.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_FILE_SIZE 1048576  // 1 MB

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

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

int write_file(const char *filename, const unsigned char *data, int len) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Error opening file for writing");
        return -1;
    }
    
    fwrite(data, 1, len, f);
    fclose(f);
    return 0;
}

int is_readable(const unsigned char *data, int len) {
    int printable_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (isprint(data[i]) || isspace(data[i])) {
            printable_count++;
        }
    }
    
    return (printable_count * 100) / len;
}

bool starts_with(const unsigned char *data, int len, const char *prefix) {
    int prefix_len = strlen(prefix);
    if (len < prefix_len) return false;
    
    return memcmp(data, prefix, prefix_len) == 0;
}

// ============================================================================
// XOR ENCRYPTION/DECRYPTION
// ============================================================================

void xor_decrypt_single_byte(unsigned char *data, int len, unsigned char key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

void brute_force_xor(const unsigned char *data, int len) {
    printf("Brute-forcing XOR encryption...\n\n");
    
    unsigned char *temp = malloc(len);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    for (unsigned char key = 0x20; key <= 0x7E; key++) {
        // Copy and decrypt
        memcpy(temp, data, len);
        xor_decrypt_single_byte(temp, len, key);
        
        // Check readability
        int readable = is_readable(temp, len);
        if (readable < 80) continue;
        
        // Check signature
        if (starts_with(temp, len, "URGENT:")) {
            printf("✓ [SUCCESS] Found key: 0x%02X ('%c')\n\n", key, key);
            printf("Decrypted message:\n");
            printf("─────────────────────────────────────────────\n");
            fwrite(temp, 1, len, stdout);
            printf("\n─────────────────────────────────────────────\n\n");
            
            // Save to artifacts
            write_file("artifacts/message1_decrypted.txt", temp, len);
            printf("✓ Saved to artifacts/message1_decrypted.txt\n");
            break;
        }
    }
    
    free(temp);
}

// ============================================================================
// CAESAR CIPHER
// ============================================================================

void caesar_decrypt(char *text, int shift) {
    shift = shift % 26;
    
    for (int i = 0; text[i]; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = 'A' + (text[i] - 'A' - shift + 26) % 26;
        }
        else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = 'a' + (text[i] - 'a' - shift + 26) % 26;
        }
    }
}

void frequency_analysis(const char *text, float *freq) {
    int counts[26] = {0};
    int total = 0;
    
    for (int i = 0; text[i]; i++) {
        char c = toupper(text[i]);
        if (c >= 'A' && c <= 'Z') {
            counts[c - 'A']++;
            total++;
        }
    }
    
    if (total > 0) {
        for (int i = 0; i < 26; i++) {
            freq[i] = (float)counts[i] * 100.0f / total;
        }
    }
}

int detect_caesar_shift(const char *ciphertext) {
    float freq[26];
    frequency_analysis(ciphertext, freq);
    
    // Find most frequent letter
    int max_index = 0;
    for (int i = 1; i < 26; i++) {
        if (freq[i] > freq[max_index]) {
            max_index = i;
        }
    }
    
    // Assume most frequent is 'E' (index 4)
    int shift = (max_index - 4 + 26) % 26;
    
    return shift;
}

void brute_force_caesar(const char *ciphertext) {
    printf("Brute-forcing Caesar cipher...\n\n");
    
    int len = strlen(ciphertext);
    char *temp = malloc(len + 1);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    printf("Trying all 26 shifts:\n");
    printf("═══════════════════════════════════════════════\n");
    
    for (int shift = 0; shift < 26; shift++) {
        strcpy(temp, ciphertext);
        caesar_decrypt(temp, shift);
        
        printf("Shift %2d: %.60s", shift, temp);
        if (strlen(temp) > 60) printf("...");
        printf("\n");
        
        // Auto-detect reasonable English text
        if (strstr(temp, "the ") || strstr(temp, "The ") || 
            strstr(temp, "and ") || strstr(temp, "is ")) {
            printf("         ^ Looks like English!\n");
        }
    }
    
    printf("═══════════════════════════════════════════════\n\n");
    
    // Try automatic detection
    int detected_shift = detect_caesar_shift(ciphertext);
    printf("Frequency analysis suggests shift: %d\n\n", detected_shift);
    
    strcpy(temp, ciphertext);
    caesar_decrypt(temp, detected_shift);
    
    printf("Auto-decrypted message:\n");
    printf("─────────────────────────────────────────────\n");
    printf("%s\n", temp);
    printf("─────────────────────────────────────────────\n\n");
    
    write_file("artifacts/message2_decrypted.txt", (unsigned char*)temp, strlen(temp));
    printf("✓ Saved to artifacts/message2_decrypted.txt\n");
    
    free(temp);
}

// ============================================================================
// MULTI-LAYER DECRYPTION
// ============================================================================

void multi_layer_decrypt(unsigned char *data, int len, int caesar_shift, unsigned char xor_key) {
    // Step 1: XOR (last applied during encryption)
    xor_decrypt_single_byte(data, len, xor_key);
    
    // Step 2: Caesar (first applied during encryption)
    data[len] = '\0';  // Ensure null-termination
    caesar_decrypt((char*)data, caesar_shift);
}

void decrypt_multi_layer(const unsigned char *data, int len) {
    printf("Multi-layer decryption\n");
    printf("Hint: XOR key = 0x5A, trying all Caesar shifts...\n\n");
    
    unsigned char *temp = malloc(len + 1);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    unsigned char xor_key = 0x5A;
    
    printf("Trying different Caesar shifts:\n");
    printf("═══════════════════════════════════════════════\n");
    
    for (int shift = 0; shift < 26; shift++) {
        memcpy(temp, data, len);
        multi_layer_decrypt(temp, len, shift, xor_key);
        
        if (is_readable(temp, len) > 90) {
            printf("Shift %2d: %.60s", shift, temp);
            if (strlen((char*)temp) > 60) printf("...");
            printf("\n");
            
            if (strstr((char*)temp, "MOONLIGHT") || 
                strstr((char*)temp, "operation") ||
                strstr((char*)temp, "classified")) {
                printf("         ^ Contains operation keywords!\n");
                
                printf("\n✓ [SUCCESS] Found correct decryption!\n");
                printf("Caesar shift: %d, XOR key: 0x%02X\n\n", shift, xor_key);
                printf("Full message:\n");
                printf("─────────────────────────────────────────────\n");
                printf("%s\n", temp);
                printf("─────────────────────────────────────────────\n\n");
                
                write_file("artifacts/message3_decrypted.txt", temp, strlen((char*)temp));
                printf("✓ Saved to artifacts/message3_decrypted.txt\n");
                break;
            }
        }
    }
    
    free(temp);
}

// ============================================================================
// TEST FILE GENERATION
// ============================================================================

void generate_test_files() {
    printf("Generating test encrypted files...\n\n");
    
    // Message 1: XOR encrypted with key 0x42 ('B')
    const char *msg1 = "URGENT: Operation MOONLIGHT compromised. Extraction point changed to coordinates 52.3676,4.9041. Use backup protocol EXODUS. Trust no one. Destroy this message after reading. Time is critical. — V.";
    int len1 = strlen(msg1);
    unsigned char *enc1 = malloc(len1);
    memcpy(enc1, msg1, len1);
    xor_decrypt_single_byte(enc1, len1, 0x42);  // Encrypt (same as decrypt)
    write_file("message1.enc", enc1, len1);
    printf("✓ message1.enc (XOR key: 0x42)\n");
    free(enc1);
    
    // Message 2: Caesar cipher with shift 7
    const char *msg2 = "The package contains classified documents about GENESIS project. Dead drop location: old warehouse near the port. Password: MOONLIGHT2024. Proceed with caution. Surveillance detected in the area.";
    char *enc2 = strdup(msg2);
    // Encrypt: shift forward by 7
    for (int i = 0; enc2[i]; i++) {
        if (enc2[i] >= 'A' && enc2[i] <= 'Z') {
            enc2[i] = 'A' + (enc2[i] - 'A' + 7) % 26;
        } else if (enc2[i] >= 'a' && enc2[i] <= 'z') {
            enc2[i] = 'a' + (enc2[i] - 'a' + 7) % 26;
        }
    }
    write_file("message2.enc", (unsigned char*)enc2, strlen(enc2));
    printf("✓ message2.enc (Caesar shift: 7)\n");
    free(enc2);
    
    // Message 3: Caesar (shift 13) + XOR (0x5A)
    const char *msg3 = "MOONLIGHT database access granted. Blockchain verification successful. All artifacts secured. Financial transaction records intact. Next operation: Episode 14. Crypto systems fully operational. — Command";
    char *enc3 = strdup(msg3);
    // Step 1: Caesar shift 13 (ROT13)
    for (int i = 0; enc3[i]; i++) {
        if (enc3[i] >= 'A' && enc3[i] <= 'Z') {
            enc3[i] = 'A' + (enc3[i] - 'A' + 13) % 26;
        } else if (enc3[i] >= 'a' && enc3[i] <= 'z') {
            enc3[i] = 'a' + (enc3[i] - 'a' + 13) % 26;
        }
    }
    // Step 2: XOR with 0x5A
    xor_decrypt_single_byte((unsigned char*)enc3, strlen(enc3), 0x5A);
    write_file("message3.enc", (unsigned char*)enc3, strlen(enc3));
    printf("✓ message3.enc (Caesar shift: 13, XOR key: 0x5A)\n");
    free(enc3);
    
    printf("\n✓ All test files generated successfully!\n");
}

// ============================================================================
// MAIN
// ============================================================================

void print_usage(const char *program_name) {
    printf("MOONLIGHT Crypto Tool - Episode 13 (SOLUTION)\n\n");
    printf("Usage: %s <command> <file>\n\n", program_name);
    printf("Commands:\n");
    printf("  xor <file>       Brute-force XOR encryption\n");
    printf("  caesar <file>    Break Caesar cipher\n");
    printf("  multi <file>     Decrypt multi-layer (Caesar+XOR)\n");
    printf("  generate         Generate test encrypted files\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *command = argv[1];
    
    // Special command: generate test files
    if (strcmp(command, "generate") == 0) {
        generate_test_files();
        return 0;
    }
    
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
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
        buffer[file_size] = '\0';
        brute_force_caesar((char*)buffer);
    }
    else if (strcmp(command, "multi") == 0) {
        decrypt_multi_layer(buffer, file_size);
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
