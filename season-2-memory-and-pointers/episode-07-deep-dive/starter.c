/**
 * Episode 07: Deep Dive
 * Dynamic memory archive loader
 * 
 * TODO: Implement dynamic memory management
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TYPE_TEXT       0x01
#define TYPE_BINARY     0x02
#define TYPE_COMPRESSED 0x03

// Archive header structure
typedef struct {
    char magic[4];        // Should be "MOON"
    uint16_t version;
    uint16_t block_count;
} Header;

// Data block structure
typedef struct {
    uint8_t type;
    uint32_t size;
    uint8_t *data;        // Dynamically allocated!
} Block;

/**
 * Read and validate archive header
 * 
 * @param f File pointer
 * @return Pointer to Header (allocated with malloc), or NULL on error
 */
Header* read_header(FILE *f) {
    // TODO:
    // 1. Allocate Header with malloc
    // 2. Read from file
    // 3. Validate magic ("MOON")
    // 4. Return header (or NULL if invalid)
    
    return NULL;  // placeholder
}

/**
 * Read one block from file
 * Allocates memory for Block structure AND for data!
 * 
 * @param f File pointer
 * @return Pointer to Block, or NULL on error
 */
Block* read_block(FILE *f) {
    // TODO:
    // 1. Allocate Block structure
    // 2. Read type and size
    // 3. Allocate memory for data (b->data = malloc(b->size))
    // 4. Read data from file
    // 5. Check for errors at each step
    
    return NULL;  // placeholder
}

/**
 * Print block contents based on type
 * 
 * @param block Pointer to block
 * @param index Block number (for display)
 */
void print_block(Block *block, int index) {
    // TODO:
    // - TEXT: print as string
    // - BINARY: print hex dump or just size
    // - COMPRESSED: print size and type
    
}

/**
 * Free block memory (including data!)
 * 
 * @param block Pointer to block
 */
void free_block(Block *block) {
    // TODO:
    // 1. Free block->data
    // 2. Set data to NULL (good practice)
    // Note: Block structure itself is freed separately
    
}

/**
 * Calculate total allocated memory
 * 
 * @param blocks Array of blocks
 * @param count Number of blocks
 * @param header Pointer to header
 * @return Total bytes allocated
 */
size_t calculate_total_memory(Block *blocks, int count, Header *header) {
    // TODO:
    // Count: header + array of blocks + all data
    
    return 0;  // placeholder
}

/**
 * Get type name as string
 */
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
    
    // TODO: Implement the main logic:
    // 1. Open file in binary mode
    // 2. Read and validate header
    // 3. Allocate array of Block structures
    // 4. Load each block (allocating memory for data)
    // 5. Print all blocks
    // 6. Calculate total memory usage
    // 7. Free all memory in correct order
    // 8. Print "No leaks!" message

    printf("=== DYNAMIC LOADER ===\n");
    printf("Archive: %s\n", filename);
    
    // Your code here
    
    printf("\nTotal memory allocated: 0 bytes\n");
    printf("Freeing memory...\n");
    printf("Memory freed successfully. No leaks!\n");

    return 0;
}
