/**
 * Test data generator for Episode 06
 * Creates pointers.dat with linked entry structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef struct {
    uint32_t offset;
    uint32_t value;
    uint32_t next;
} Entry;

// Shuffle array (Fisher-Yates)
void shuffle(Entry *array, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Entry temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main() {
    // Message to encode: "MOONLIGHT.ACTIVE"
    const char *message = "MOONLIGHT.ACTIVE";
    int msg_len = strlen(message);
    
    // Create entries
    Entry entries[16];
    
    for (int i = 0; i < msg_len; i++) {
        entries[i].offset = i * 0x000C;  // Each entry is 12 bytes
        entries[i].value = (uint32_t)message[i];
        
        if (i < msg_len - 1) {
            entries[i].next = (i + 1) * 0x000C;
        } else {
            entries[i].next = 0;  // Last entry
        }
    }
    
    // Shuffle to make it non-sequential in file
    shuffle(entries, msg_len);
    
    // Write to file
    FILE *f = fopen("pointers.dat", "wb");
    if (!f) {
        perror("Failed to create pointers.dat");
        return 1;
    }
    
    fwrite(entries, sizeof(Entry), msg_len, f);
    fclose(f);
    
    printf("✅ Generated pointers.dat successfully!\n");
    printf("   Message: \"%s\"\n", message);
    printf("   Entries: %d (shuffled)\n", msg_len);
    printf("   File size: %zu bytes\n", msg_len * sizeof(Entry));
    
    return 0;
}
