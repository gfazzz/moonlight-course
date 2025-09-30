/**
 * Test data generator for Episode 07
 * Creates archive.dat with various block types
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TYPE_TEXT       0x01
#define TYPE_BINARY     0x02
#define TYPE_COMPRESSED 0x03

typedef struct {
    char magic[4];
    uint16_t version;
    uint16_t block_count;
} Header;

void write_block(FILE *f, uint8_t type, const void *data, uint32_t size) {
    fwrite(&type, 1, 1, f);
    fwrite(&size, sizeof(uint32_t), 1, f);
    fwrite(data, 1, size, f);
}

int main() {
    FILE *f = fopen("archive.dat", "wb");
    if (!f) {
        perror("Failed to create archive.dat");
        return 1;
    }
    
    // Write header
    Header header = {
        .magic = {'M', 'O', 'O', 'N'},
        .version = 1,
        .block_count = 5
    };
    fwrite(&header, sizeof(Header), 1, f);
    
    // Block 1: TEXT
    const char *text1 = "Operation MOONLIGHT is active. All agents stand by.";
    write_block(f, TYPE_TEXT, text1, strlen(text1));
    
    // Block 2: BINARY (some random bytes)
    uint8_t binary[128];
    for (int i = 0; i < 128; i++) {
        binary[i] = i ^ 0xAA;
    }
    write_block(f, TYPE_BINARY, binary, 128);
    
    // Block 3: TEXT
    const char *text2 = "V. was here. Trust the process.";
    write_block(f, TYPE_TEXT, text2, strlen(text2));
    
    // Block 4: COMPRESSED (fake compressed data)
    uint8_t compressed[256];
    for (int i = 0; i < 256; i++) {
        compressed[i] = (i * 7) % 256;
    }
    write_block(f, TYPE_COMPRESSED, compressed, 256);
    
    // Block 5: TEXT
    const char *text3 = "END_OF_TRANSMISSION";
    write_block(f, TYPE_TEXT, text3, strlen(text3));
    
    fclose(f);
    
    printf("✅ Generated archive.dat successfully!\n");
    printf("   Magic: MOON\n");
    printf("   Version: %u\n", header.version);
    printf("   Blocks: %u\n", header.block_count);
    printf("   Block types: TEXT, BINARY, TEXT, COMPRESSED, TEXT\n");
    
    return 0;
}
