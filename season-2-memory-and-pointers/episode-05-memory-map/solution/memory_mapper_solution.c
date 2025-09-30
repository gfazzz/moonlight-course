/**
 * Episode 05: Memory Map - SOLUTION
 * Memory dump parser and mapper
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
 */
uint8_t calculate_checksum(uint8_t *data, int size) {
    uint8_t checksum = 0;
    for (int i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

/**
 * Parse and print STRING structure
 */
void parse_string(uint8_t *data, int size) {
    printf("\"");
    for (int i = 0; i < size; i++) {
        if (data[i] >= 32 && data[i] < 127) {
            printf("%c", data[i]);
        } else {
            printf("\\x%02X", data[i]);
        }
    }
    printf("\"");
}

/**
 * Parse and print COORDINATES structure
 */
void parse_coordinates(uint8_t *data, int size) {
    if (size != 8) {
        printf("[INVALID SIZE]");
        return;
    }
    
    float latitude, longitude;
    memcpy(&latitude, data, sizeof(float));
    memcpy(&longitude, data + sizeof(float), sizeof(float));
    
    char lat_dir = (latitude >= 0) ? 'N' : 'S';
    char lon_dir = (longitude >= 0) ? 'E' : 'W';
    
    printf("%.4f° %c, %.4f° %c", 
           latitude >= 0 ? latitude : -latitude,
           lat_dir,
           longitude >= 0 ? longitude : -longitude,
           lon_dir);
}

/**
 * Parse and print TIMESTAMP structure
 */
void parse_timestamp(uint8_t *data, int size) {
    if (size != 4) {
        printf("[INVALID SIZE]");
        return;
    }
    
    uint32_t timestamp;
    memcpy(&timestamp, data, sizeof(uint32_t));
    
    time_t t = (time_t)timestamp;
    struct tm *tm_info = gmtime(&t);
    
    if (tm_info) {
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", tm_info);
        printf("%s", buffer);
    } else {
        printf("[INVALID TIMESTAMP]");
    }
}

/**
 * Convert type code to string name
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <memory_dump.dat>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    // Open file
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    // Read file into buffer
    uint8_t buffer[MAX_DUMP_SIZE];
    size_t bytes_read = fread(buffer, 1, MAX_DUMP_SIZE, file);
    fclose(file);
    
    if (bytes_read == 0) {
        fprintf(stderr, "Error: Empty file\n");
        return 1;
    }
    
    printf("=== MEMORY MAP ===\n");
    
    size_t offset = 0;
    int structures_found = 0;
    int garbage_bytes = 0;
    
    // Parse buffer
    while (offset < bytes_read) {
        uint8_t type = buffer[offset];
        
        // Handle garbage
        if (type == TYPE_GARBAGE) {
            size_t garbage_start = offset;
            while (offset < bytes_read && buffer[offset] == TYPE_GARBAGE) {
                offset++;
                garbage_bytes++;
            }
            size_t garbage_len = offset - garbage_start;
            printf("[0x%04zX] GARBAGE (%zu bytes)\n", garbage_start, garbage_len);
            continue;
        }
        
        // Check if we have enough bytes for header
        if (offset + 2 >= bytes_read) {
            break;  // Not enough data for TYPE + SIZE
        }
        
        size_t struct_offset = offset;
        uint8_t size = buffer[offset + 1];
        
        // Check if we have enough bytes for complete structure
        if (offset + 2 + size + 1 > bytes_read) {
            printf("[0x%04zX] [TRUNCATED STRUCTURE]\n", struct_offset);
            break;
        }
        
        // Extract data and checksum
        uint8_t *data = &buffer[offset + 2];
        uint8_t checksum_file = buffer[offset + 2 + size];
        uint8_t checksum_calc = calculate_checksum(data, size);
        
        // Verify checksum
        if (checksum_file != checksum_calc) {
            printf("[0x%04zX] [CORRUPTED] (checksum mismatch: expected 0x%02X, got 0x%02X)\n",
                   struct_offset, checksum_calc, checksum_file);
            offset += 2 + size + 1;
            continue;
        }
        
        // Print structure info
        printf("[0x%04zX] %s (%d bytes): ", struct_offset, type_to_string(type), size);
        
        switch (type) {
            case TYPE_STRING:
                parse_string(data, size);
                structures_found++;
                break;
            case TYPE_COORDS:
                parse_coordinates(data, size);
                structures_found++;
                break;
            case TYPE_TIMESTAMP:
                parse_timestamp(data, size);
                structures_found++;
                break;
            default:
                printf("[UNKNOWN TYPE 0x%02X]", type);
                // Treat unknown types as garbage
                garbage_bytes += 2 + size + 1;
        }
        
        printf("\n");
        
        // Move to next structure
        offset += 2 + size + 1;  // TYPE + SIZE + DATA + CHECKSUM
    }
    
    printf("\nTotal structures found: %d\n", structures_found);
    printf("Total garbage bytes: %d\n", garbage_bytes);

    return 0;
}
