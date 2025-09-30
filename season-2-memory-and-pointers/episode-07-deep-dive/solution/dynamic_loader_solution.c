/**
 * Episode 07: Deep Dive - SOLUTION
 * Dynamic memory archive loader
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define TYPE_TEXT       0x01
#define TYPE_BINARY     0x02
#define TYPE_COMPRESSED 0x03

typedef struct {
    char magic[4];
    uint16_t version;
    uint16_t block_count;
} Header;

typedef struct {
    uint8_t type;
    uint32_t size;
    uint8_t *data;
} Block;

/**
 * Read and validate header
 */
Header* read_header(FILE *f) {
    Header *h = malloc(sizeof(Header));
    if (!h) {
        perror("malloc header failed");
        return NULL;
    }
    
    if (fread(h, sizeof(Header), 1, f) != 1) {
        fprintf(stderr, "Error reading header\n");
        free(h);
        return NULL;
    }
    
    if (memcmp(h->magic, "MOON", 4) != 0) {
        fprintf(stderr, "Invalid magic number (expected 'MOON')\n");
        free(h);
        return NULL;
    }
    
    return h;
}

/**
 * Read one block
 */
Block* read_block(FILE *f) {
    Block *b = malloc(sizeof(Block));
    if (!b) {
        perror("malloc block failed");
        return NULL;
    }
    
    if (fread(&b->type, 1, 1, f) != 1 ||
        fread(&b->size, sizeof(uint32_t), 1, f) != 1) {
        fprintf(stderr, "Error reading block header\n");
        free(b);
        return NULL;
    }
    
    b->data = malloc(b->size);
    if (!b->data) {
        perror("malloc block data failed");
        free(b);
        return NULL;
    }
    
    if (fread(b->data, 1, b->size, f) != b->size) {
        fprintf(stderr, "Error reading block data\n");
        free(b->data);
        free(b);
        return NULL;
    }
    
    return b;
}

/**
 * Print block
 */
void print_block(Block *block, int index) {
    printf("\nBlock #%d (%s, %u bytes):\n", index + 1, 
           block->type == TYPE_TEXT ? "TEXT" :
           block->type == TYPE_BINARY ? "BINARY" :
           block->type == TYPE_COMPRESSED ? "COMPRESSED" : "UNKNOWN",
           block->size);
    
    if (block->type == TYPE_TEXT) {
        printf("\"");
        for (uint32_t i = 0; i < block->size; i++) {
            if (isprint(block->data[i]) || block->data[i] == '\n') {
                printf("%c", block->data[i]);
            } else {
                printf("\\x%02X", block->data[i]);
            }
        }
        printf("\"\n");
    } else if (block->type == TYPE_BINARY) {
        printf("[Binary data, %u bytes]\n", block->size);
        // Optional: hex dump first 64 bytes
        int limit = block->size < 64 ? block->size : 64;
        for (int i = 0; i < limit; i++) {
            if (i % 16 == 0) printf("%08X: ", i);
            printf("%02X ", block->data[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        if (limit % 16 != 0) printf("\n");
        if (block->size > 64) printf("... (truncated)\n");
    } else if (block->type == TYPE_COMPRESSED) {
        printf("[Compressed data, %u bytes]\n", block->size);
    }
}

/**
 * Free block
 */
void free_block(Block *block) {
    if (block && block->data) {
        free(block->data);
        block->data = NULL;
    }
}

/**
 * Calculate total memory
 */
size_t calculate_total_memory(Block *blocks, int count, Header *header) {
    size_t total = sizeof(Header);
    total += count * sizeof(Block);
    
    for (int i = 0; i < count; i++) {
        total += blocks[i].size;
    }
    
    return total;
}

const char* type_to_string(uint8_t type) {
    switch (type) {
        case TYPE_TEXT:       return "TEXT";
        case TYPE_BINARY:     return "BINARY";
        case TYPE_COMPRESSED: return "COMPRESSED";
        default:              return "UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <archive.dat>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    // Read header
    Header *header = read_header(file);
    if (!header) {
        fclose(file);
        return 1;
    }
    
    printf("=== DYNAMIC LOADER ===\n");
    printf("Archive: %s\n", filename);
    printf("Magic: %.4s\n", header->magic);
    printf("Version: %u\n", header->version);
    printf("Blocks: %u\n", header->block_count);
    
    // Allocate array of blocks
    Block *blocks = malloc(header->block_count * sizeof(Block));
    if (!blocks) {
        perror("malloc blocks array failed");
        free(header);
        fclose(file);
        return 1;
    }
    
    printf("\nLoading blocks...\n");
    
    // Read all blocks
    for (int i = 0; i < header->block_count; i++) {
        Block *b = read_block(file);
        if (!b) {
            // Clean up what we've loaded so far
            for (int j = 0; j < i; j++) {
                free_block(&blocks[j]);
            }
            free(blocks);
            free(header);
            fclose(file);
            return 1;
        }
        blocks[i] = *b;  // Copy structure
        free(b);         // Free the temporary structure (data is preserved in blocks[i])
        
        printf("[%d/%u] %s (%u bytes) loaded\n", 
               i + 1, header->block_count, 
               type_to_string(blocks[i].type),
               blocks[i].size);
    }
    
    fclose(file);
    
    // Print all blocks
    for (int i = 0; i < header->block_count; i++) {
        print_block(&blocks[i], i);
    }
    
    // Calculate memory
    size_t total_memory = calculate_total_memory(blocks, header->block_count, header);
    printf("\nTotal memory allocated: %zu bytes\n", total_memory);
    
    // Free everything
    printf("Freeing memory...\n");
    for (int i = 0; i < header->block_count; i++) {
        free_block(&blocks[i]);
    }
    free(blocks);
    free(header);
    
    printf("Memory freed successfully. No leaks!\n");

    return 0;
}
