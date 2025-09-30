/*
 * OPERATION MOONLIGHT - Episode 01: Strange Message
 * Starter Code Templates
 * 
 * Instructions:
 * 1. Copy the relevant section to artifacts/ folder
 * 2. Complete the TODOs
 * 3. Compile and test
 * 4. Run automated tests
 */

#include <stdio.h>

/* ========================================
 * WARMUP.C - XOR Calculator
 * ======================================== */

// TODO: Complete this warmup program
void warmup_example() {
    // 1. Declare variables for input byte and key
    // unsigned char byte;
    // unsigned char key = 0x42;
    
    // 2. Ask user for input
    // printf("Enter byte (0-255): ");
    // scanf("%hhu", &byte);  // %hhu for unsigned char
    
    // 3. Apply XOR
    // unsigned char result = byte ^ key;
    
    // 4. Print result in hex
    // printf("Encrypted: 0x%02X\n", result);
}


/* ========================================
 * DECODER.C - Main Decoder
 * ======================================== */

void decoder_example() {
    // Encrypted message from Victor
    unsigned char encrypted[] = {
        0x0F, 0x0D, 0x0D, 0x0C, 0x0E, 0x0B, 0x05, 0x0A, 
        0x16, 0x62, 0x23, 0x21, 0x36, 0x2B, 0x34, 0x23, 
        0x36, 0x27, 0x26, 0x6C, 0x48, 0x16, 0x2A, 0x27, 
        0x3B, 0x62, 0x29, 0x2C, 0x2D, 0x35, 0x6C, 0x48, 
        0x04, 0x2B, 0x2E, 0x27, 0x31, 0x62, 0x23, 0x36, 
        0x62, 0x2D, 0x2E, 0x26, 0x62, 0x32, 0x2E, 0x23, 
        0x21, 0x27, 0x6C, 0x48, 0x16, 0x30, 0x37, 0x31, 
        0x36, 0x62, 0x2D, 0x2C, 0x2E, 0x3B, 0x62, 0x21, 
        0x2D, 0x26, 0x27, 0x6C, 0x48, 0xA0, 0xC2, 0xD6, 
        0x62, 0x14, 0x6C
    };
    
    // TODO: 1. Calculate array length
    // int length = sizeof(encrypted) / sizeof(encrypted[0]);
    
    // TODO: 2. Create array for decrypted message
    // Important: +1 for null terminator!
    // unsigned char decrypted[length + 1];
    
    // TODO: 3. XOR key
    // unsigned char key = 0x42;
    
    // TODO: 4. Decrypt in a loop
    // for (int i = 0; i < length; i++) {
    //     decrypted[i] = encrypted[i] ^ key;
    // }
    
    // TODO: 5. Add null terminator (end of string)
    // decrypted[length] = '\0';
    
    // TODO: 6. Print decrypted message
    // printf("Decrypted message:\n%s\n", decrypted);
}


/* ========================================
 * VALIDATOR.C - Validate Decryption
 * ======================================== */

void validator_example() {
    // TODO: After decryption, validate the result
    
    // 1. Count character types
    // int letters = 0, digits = 0, spaces = 0, others = 0;
    
    // 2. Loop through decrypted message
    // for (each character) {
    //     if (character >= 'A' && character <= 'Z') letters++;
    //     else if (character >= 'a' && character <= 'z') letters++;
    //     else if (character >= '0' && character <= '9') digits++;
    //     else if (character == ' ') spaces++;
    //     else others++;
    // }
    
    // 3. Print statistics
    // printf("Statistics:\n");
    // printf("Letters: %d\n", letters);
    // printf("Digits: %d\n", digits);
    // printf("Spaces: %d\n", spaces);
    // printf("Others: %d\n", others);
    
    // 4. BONUS: Calculate checksum
    // unsigned char checksum = 0;
    // for (each byte in decrypted) {
    //     checksum += byte;
    // }
    // printf("Checksum: 0x%02X\n", checksum);
}


/* ========================================
 * HELPER: Full working example of XOR
 * ======================================== */

void xor_demonstration() {
    printf("=== XOR Demonstration ===\n\n");
    
    // Simple example
    char original = 'A';
    char key = 0x42;
    
    printf("Original character: %c (0x%02X)\n", original, (unsigned char)original);
    printf("Key: 0x%02X\n", (unsigned char)key);
    
    // Encrypt
    char encrypted = original ^ key;
    printf("After XOR (encrypted): 0x%02X\n", (unsigned char)encrypted);
    
    // Decrypt (XOR again with same key!)
    char decrypted = encrypted ^ key;
    printf("After XOR again (decrypted): %c (0x%02X)\n", decrypted, (unsigned char)decrypted);
    
    printf("\nMagic: (A ^ KEY) ^ KEY = A\n");
}


/* ========================================
 * MAIN - Uncomment to test demonstrations
 * ======================================== */

int main() {
    printf("OPERATION MOONLIGHT - Episode 01\n");
    printf("=================================\n\n");
    
    // Uncomment to see XOR demonstration:
    // xor_demonstration();
    
    // Your code goes in artifacts/ folder!
    printf("\nCopy code templates to artifacts/ and complete TODOs\n");
    printf("Then compile: gcc -Wall -o decoder artifacts/decoder.c\n");
    printf("And run: ./decoder\n");
    
    return 0;
}


/* ========================================
 * TIPS AND TRICKS
 * ========================================
 * 
 * 1. Compile with warnings:
 *    gcc -Wall -Wextra -std=c11 -o program file.c
 * 
 * 2. Format specifiers:
 *    %d   - int
 *    %c   - char
 *    %s   - string (char array)
 *    %x   - hex (lowercase)
 *    %X   - hex (uppercase)
 *    %02X - hex with leading zeros (2 digits)
 *    %hhu - unsigned char (for scanf)
 * 
 * 3. Array length:
 *    int len = sizeof(array) / sizeof(array[0]);
 * 
 * 4. XOR properties:
 *    A ^ 0 = A
 *    A ^ A = 0
 *    A ^ B = B ^ A (commutative)
 *    (A ^ B) ^ C = A ^ (B ^ C) (associative)
 *    (A ^ B) ^ B = A (self-inverse!)
 * 
 * 5. String in C = array of char ending with '\0'
 *    char str[] = "Hello";  // Actually: {'H','e','l','l','o','\0'}
 * 
 * ======================================== */
