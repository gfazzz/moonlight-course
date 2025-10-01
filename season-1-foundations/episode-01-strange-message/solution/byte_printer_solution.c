/*
 * OPERATION MOONLIGHT - Episode 01
 * Solution: byte_printer.c
 * 
 * This is the complete solution for the byte array analysis task.
 * Try to solve it yourself before looking at this!
 */

#include <stdio.h>

int main() {
    // Declare byte array
    unsigned char bytes[] = {0x42, 0x1A, 0xFF, 0x00, 0x7E};
    
    // Calculate array length
    int length = sizeof(bytes) / sizeof(bytes[0]);
    
    // Print header
    printf("=== BYTE ARRAY ANALYSIS ===\n");
    
    // Loop through and print each byte
    for (int i = 0; i < length; i++) {
        printf("Byte %d: 0x%02X (%d decimal)\n", i, bytes[i], bytes[i]);
    }
    printf("\n");
    
    // Calculate sum of all bytes
    unsigned int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += bytes[i];
    }
    printf("Sum of all bytes: %u\n", sum);
    
    // Find maximum byte
    unsigned char max = bytes[0];
    for (int i = 1; i < length; i++) {
        if (bytes[i] > max) {
            max = bytes[i];
        }
    }
    printf("Max byte: 0x%02X (%u)\n", max, max);
    
    return 0;
}

/*
 * Expected output:
 * 
 * === BYTE ARRAY ANALYSIS ===
 * Byte 0: 0x42 (66 decimal)
 * Byte 1: 0x1A (26 decimal)
 * Byte 2: 0xFF (255 decimal)
 * Byte 3: 0x00 (0 decimal)
 * Byte 4: 0x7E (126 decimal)
 * 
 * Sum of all bytes: 473
 * Max byte: 0xFF (255)
 * 
 * Explanation:
 * - 0x42 = 66 in decimal
 * - 0x1A = 26 in decimal
 * - 0xFF = 255 in decimal (maximum for unsigned char)
 * - 0x00 = 0 in decimal
 * - 0x7E = 126 in decimal
 * 
 * Sum: 66 + 26 + 255 + 0 + 126 = 473
 * Max: 255 (0xFF is the largest)
 */

