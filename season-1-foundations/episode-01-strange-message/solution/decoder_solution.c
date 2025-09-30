/*
 * OPERATION MOONLIGHT - Episode 01
 * Solution: decoder.c
 * 
 * WARNING: Only look at this if you're really stuck!
 * Try to solve it yourself first - that's where the learning happens!
 */

#include <stdio.h>

#define XOR_KEY 0x42

int main() {
    printf("=== MOONLIGHT DECODER v1.0 ===\n");
    printf("Decrypting message from Victor...\n\n");
    
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
    
    // Calculate length of encrypted message
    int length = sizeof(encrypted) / sizeof(encrypted[0]);
    
    printf("Message length: %d bytes\n", length);
    printf("XOR Key: 0x%02X\n\n", XOR_KEY);
    
    // Create array for decrypted message (+1 for null terminator)
    unsigned char decrypted[length + 1];
    
    // Decrypt: XOR each byte with the key
    for (int i = 0; i < length; i++) {
        decrypted[i] = encrypted[i] ^ XOR_KEY;
    }
    
    // Add null terminator to make it a valid C string
    decrypted[length] = '\0';
    
    // Print decrypted message
    printf("--- DECRYPTED MESSAGE ---\n");
    printf("%s\n", decrypted);
    printf("--- END MESSAGE ---\n\n");
    
    // Validation: character statistics
    int letters = 0, digits = 0, spaces = 0, others = 0;
    
    for (int i = 0; i < length; i++) {
        char c = decrypted[i];
        
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            letters++;
        } else if (c >= '0' && c <= '9') {
            digits++;
        } else if (c == ' ') {
            spaces++;
        } else {
            others++;
        }
    }
    
    printf("=== STATISTICS ===\n");
    printf("Letters: %d\n", letters);
    printf("Digits:  %d\n", digits);
    printf("Spaces:  %d\n", spaces);
    printf("Others:  %d\n", others);
    
    // Calculate checksum (simple sum of all bytes)
    unsigned char checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum += decrypted[i];
    }
    
    printf("Checksum: 0x%02X\n", checksum);
    
    printf("\n=== DECRYPTION COMPLETE ===\n");
    
    return 0;
}

/*
 * Expected output:
 * 
 * The decrypted message reveals Victor's warning about MOONLIGHT.
 * It should be a readable text message.
 * 
 * This is just the beginning of your journey...
 */
