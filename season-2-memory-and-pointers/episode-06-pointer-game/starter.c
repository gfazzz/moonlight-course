/**
 * Episode 06: Pointer Game
 * Pointer chain navigator
 * 
 * TODO: Implement pointer navigation functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_ENTRIES 256

// Entry structure (12 bytes total)
typedef struct {
    uint32_t offset;   // This entry's offset
    uint32_t value;    // Value stored here
    uint32_t next;     // Offset of next entry (0 = end)
} Entry;

/**
 * Find entry by offset in the entries array
 * 
 * @param entries Array of Entry structures
 * @param count Number of entries in array
 * @param offset Target offset to find
 * @return Pointer to found entry, or NULL if not found
 */
Entry* find_entry(Entry *entries, int count, uint32_t offset) {
    // TODO: Search for entry with matching offset
    // Return pointer to entry if found, NULL otherwise
    
    return NULL;  // placeholder
}

/**
 * Check if value is printable ASCII character
 * 
 * @param value Value to check
 * @return 1 if printable (32-126), 0 otherwise
 */
int is_printable(uint32_t value) {
    // TODO: Check if value is in printable ASCII range
    
    return 0;  // placeholder
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pointers.dat>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    // TODO: Implement the main logic:
    // 1. Open file in binary mode
    // 2. Read all Entry structures into array
    // 3. Find starting entry (offset = 0)
    // 4. Follow the chain:
    //    - Print current entry info
    //    - Collect value for message (if printable)
    //    - Move to next entry
    //    - Detect cycles
    // 5. Print decoded message
    // 6. Print statistics

    printf("=== POINTER NAVIGATOR ===\n");
    printf("Starting at offset: 0x0000\n\n");
    
    // Your code here
    
    printf("\nDecoded message: \"\"\n");
    printf("Total entries traversed: 0\n");

    return 0;
}
