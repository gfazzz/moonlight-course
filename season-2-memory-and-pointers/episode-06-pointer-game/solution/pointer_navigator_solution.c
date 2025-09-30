/**
 * Episode 06: Pointer Game - SOLUTION
 * Pointer chain navigator
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_ENTRIES 256

// Entry structure (12 bytes total)
typedef struct {
    uint32_t offset;
    uint32_t value;
    uint32_t next;
} Entry;

/**
 * Find entry by offset
 */
Entry* find_entry(Entry *entries, int count, uint32_t offset) {
    for (int i = 0; i < count; i++) {
        if (entries[i].offset == offset) {
            return &entries[i];
        }
    }
    return NULL;
}

/**
 * Check if value is printable ASCII
 */
int is_printable(uint32_t value) {
    return (value >= 32 && value < 127);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pointers.dat>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    // Open file
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    // Read all entries
    Entry entries[MAX_ENTRIES];
    size_t count = fread(entries, sizeof(Entry), MAX_ENTRIES, file);
    fclose(file);
    
    if (count == 0) {
        fprintf(stderr, "Error: No entries read\n");
        return 1;
    }
    
    // Find starting entry (offset = 0)
    Entry *start = find_entry(entries, count, 0);
    if (!start) {
        fprintf(stderr, "Error: No starting entry found (offset=0)\n");
        return 1;
    }
    
    printf("=== POINTER NAVIGATOR ===\n");
    printf("Starting at offset: 0x%04X\n\n", start->offset);
    
    // Navigate the chain
    char message[256] = {0};
    int msg_len = 0;
    int traversed = 0;
    uint32_t visited[MAX_ENTRIES] = {0};
    int visit_count = 0;
    
    Entry *current = start;
    char path[1024] = "0x0000";
    
    while (current != NULL) {
        // Check for cycle
        for (int i = 0; i < visit_count; i++) {
            if (visited[i] == current->offset) {
                printf("\n⚠️  CYCLE DETECTED at offset 0x%04X!\n", current->offset);
                return 1;
            }
        }
        visited[visit_count++] = current->offset;
        
        // Print entry info
        printf("[0x%04X] Value: 0x%02X", current->offset, current->value);
        
        if (is_printable(current->value)) {
            printf(" ('%c')", (char)current->value);
            message[msg_len++] = (char)current->value;
        }
        
        if (current->next == 0) {
            printf(" → Next: 0x0000 (END)\n");
        } else {
            printf(" → Next: 0x%04X\n", current->next);
        }
        
        traversed++;
        
        // Move to next
        if (current->next == 0) {
            strcat(path, " → END");
            break;
        }
        
        char next_str[16];
        snprintf(next_str, sizeof(next_str), " → 0x%04X", current->next);
        strcat(path, next_str);
        
        current = find_entry(entries, count, current->next);
        
        if (current == NULL) {
            printf("\n⚠️  ERROR: Invalid next offset, entry not found!\n");
            break;
        }
    }
    
    message[msg_len] = '\0';
    
    printf("\nDecoded message: \"%s\"\n", message);
    printf("Total entries traversed: %d\n", traversed);
    printf("\nPath taken: %s\n", path);

    return 0;
}
