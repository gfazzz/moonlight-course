/**
 * Episode 05: Memory Map
 * Memory dump parser and mapper
 * 
 * TODO: Implement the memory mapper functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MAX_DUMP_SIZE 4096

// Data structure types
#define TYPE_STRING     0x01
#define TYPE_COORDS     0x02
#define TYPE_TIMESTAMP  0x03
#define TYPE_GARBAGE    0xFF

/**
 * Calculate XOR checksum for data
 * 
 * @param data Pointer to data array
 * @param size Size of data in bytes
 * @return XOR checksum value
 */
uint8_t calculate_checksum(uint8_t *data, int size) {
    // TODO: Implement XOR checksum calculation
    // Hint: XOR all bytes together
    
    return 0;  // placeholder
}

/**
 * Parse and print STRING structure
 * 
 * @param data Pointer to string data
 * @param size Size of string
 */
void parse_string(uint8_t *data, int size) {
    // TODO: Print the string
    // Format: "content"
    // Handle non-printable characters if needed
    
}

/**
 * Parse and print COORDINATES structure
 * 
 * @param data Pointer to coordinate data (2 floats)
 * @param size Size of data (should be 8)
 */
void parse_coordinates(uint8_t *data, int size) {
    // TODO: Extract two floats (latitude, longitude)
    // Print in format: LAT° N/S, LON° E/W
    // Hint: Use memcpy to convert bytes to float
    
}

/**
 * Parse and print TIMESTAMP structure
 * 
 * @param data Pointer to timestamp data (uint32_t)
 * @param size Size of data (should be 4)
 */
void parse_timestamp(uint8_t *data, int size) {
    // TODO: Extract uint32_t timestamp
    // Convert to human-readable date/time
    // Format: YYYY-MM-DD HH:MM:SS UTC
    
}

/**
 * Convert type code to string name
 * 
 * @param type Type code
 * @return String name of type
 */
const char* type_to_string(uint8_t type) {
    switch (type) {
        case TYPE_STRING:    return "STRING";
        case TYPE_COORDS:    return "COORDINATES";
        case TYPE_TIMESTAMP: return "TIMESTAMP";
        case TYPE_GARBAGE:   return "GARBAGE";
        default:             return "UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <memory_dump.dat>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    // TODO: Implement the main logic:
    // 1. Open the file in binary mode
    // 2. Read file content into buffer
    // 3. Parse the buffer:
    //    - Loop through bytes
    //    - Read TYPE byte
    //    - If TYPE is GARBAGE (0xFF), skip and continue
    //    - Otherwise: read SIZE, DATA, CHECKSUM
    //    - Verify checksum
    //    - Call appropriate parse_* function
    //    - Print structure info with offset
    // 4. Print summary statistics
    // 5. Clean up and close file

    printf("=== MEMORY MAP ===\n");
    
    // Your code here
    
    printf("\nTotal structures found: 0\n");
    printf("Total garbage bytes: 0\n");

    return 0;
}
