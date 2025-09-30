/*
 * Helper: Encode message for Episode 01
 * This generates the encrypted array for the lesson
 */

#include <stdio.h>
#include <string.h>

#define XOR_KEY 0x42

int main() {
    // The message we want students to decrypt
    char message[] = "MOONLIGHT activated.\nThey know.\nFiles at old place.\nTrust only code.\n— V.";
    
    int length = strlen(message);
    
    printf("// Message: \"%s\"\n\n", message);
    printf("unsigned char encrypted[] = {\n    ");
    
    for (int i = 0; i < length; i++) {
        unsigned char encrypted = message[i] ^ XOR_KEY;
        printf("0x%02X", encrypted);
        
        if (i < length - 1) {
            printf(", ");
            if ((i + 1) % 8 == 0) {
                printf("\n    ");
            }
        }
    }
    
    printf("\n};\n\n");
    printf("// Length: %d bytes\n", length);
    
    return 0;
}

// Compile and run:
// gcc -o encoder encoder_helper.c && ./encoder
