/**
 * Test data generator for Episode 05
 * Creates memory_dump.dat with various structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define TYPE_STRING     0x01
#define TYPE_COORDS     0x02
#define TYPE_TIMESTAMP  0x03
#define TYPE_GARBAGE    0xFF

// Calculate XOR checksum
uint8_t calculate_checksum(uint8_t *data, int size) {
    uint8_t checksum = 0;
    for (int i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Write a structure to file
void write_structure(FILE *f, uint8_t type, uint8_t *data, uint8_t size) {
    fwrite(&type, 1, 1, f);
    fwrite(&size, 1, 1, f);
    fwrite(data, 1, size, f);
    
    uint8_t checksum = calculate_checksum(data, size);
    fwrite(&checksum, 1, 1, f);
}

// Write garbage bytes
void write_garbage(FILE *f, int count) {
    for (int i = 0; i < count; i++) {
        uint8_t garbage = TYPE_GARBAGE;
        fwrite(&garbage, 1, 1, f);
    }
}

int main() {
    FILE *f = fopen("memory_dump.dat", "wb");
    if (!f) {
        perror("Failed to create dump file");
        return 1;
    }
    
    // Add some leading garbage
    write_garbage(f, 3);
    
    // String: "access_log.dat"
    {
        uint8_t data[] = "access_log.dat";
        write_structure(f, TYPE_STRING, data, sizeof(data) - 1);  // -1 to exclude null terminator
    }
    
    // Garbage
    write_garbage(f, 5);
    
    // Coordinates: Moscow (55.7558° N, 37.6173° E)
    {
        float lat = 55.7558f;
        float lon = 37.6173f;
        uint8_t data[8];
        memcpy(data, &lat, 4);
        memcpy(data + 4, &lon, 4);
        write_structure(f, TYPE_COORDS, data, 8);
    }
    
    // String: "encrypted.key"
    {
        uint8_t data[] = "encrypted.key";
        write_structure(f, TYPE_STRING, data, sizeof(data) - 1);
    }
    
    // Garbage
    write_garbage(f, 2);
    
    // Timestamp: 2024-03-15 18:30:00 UTC (example timestamp)
    {
        struct tm tm_info = {0};
        tm_info.tm_year = 2024 - 1900;
        tm_info.tm_mon = 2;  // March (0-based)
        tm_info.tm_mday = 15;
        tm_info.tm_hour = 18;
        tm_info.tm_min = 30;
        tm_info.tm_sec = 0;
        
        time_t t = timegm(&tm_info);  // Use timegm for UTC
        uint32_t timestamp = (uint32_t)t;
        
        uint8_t data[4];
        memcpy(data, &timestamp, 4);
        write_structure(f, TYPE_TIMESTAMP, data, 4);
    }
    
    // Garbage
    write_garbage(f, 4);
    
    // Coordinates: New York (40.7128° N, -74.0060° W)
    {
        float lat = 40.7128f;
        float lon = -74.0060f;
        uint8_t data[8];
        memcpy(data, &lat, 4);
        memcpy(data + 4, &lon, 4);
        write_structure(f, TYPE_COORDS, data, 8);
    }
    
    // String: "backup_2024.tar.gz"
    {
        uint8_t data[] = "backup_2024.tar.gz";
        write_structure(f, TYPE_STRING, data, sizeof(data) - 1);
    }
    
    // Timestamp: 2024-09-30 12:00:00 UTC
    {
        struct tm tm_info = {0};
        tm_info.tm_year = 2024 - 1900;
        tm_info.tm_mon = 8;  // September (0-based)
        tm_info.tm_mday = 30;
        tm_info.tm_hour = 12;
        tm_info.tm_min = 0;
        tm_info.tm_sec = 0;
        
        time_t t = timegm(&tm_info);
        uint32_t timestamp = (uint32_t)t;
        
        uint8_t data[4];
        memcpy(data, &timestamp, 4);
        write_structure(f, TYPE_TIMESTAMP, data, 4);
    }
    
    // Trailing garbage
    write_garbage(f, 6);
    
    fclose(f);
    
    printf("✅ Generated memory_dump.dat successfully!\n");
    printf("   Contains: 3 strings, 2 coordinates, 2 timestamps, garbage bytes\n");
    
    return 0;
}
