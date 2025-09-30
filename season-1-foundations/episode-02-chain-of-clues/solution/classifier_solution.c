/*
 * OPERATION MOONLIGHT - Episode 02
 * Solution: classifier.c
 */

#include <stdio.h>

int main() {
    printf("=== DATA CLASSIFIER v1.0 ===\n");
    printf("Analyzing intercepted data...\n\n");
    
    char type;
    int count_coords = 0, count_time = 0, count_id = 0;
    int valid = 0, invalid = 0;
    int total = 0;
    
    while (scanf(" %c", &type) == 1) {
        total++;
        
        switch (type) {
            case 'C': {
                float lat, lon;
                long timestamp;
                
                if (scanf("%f %f %ld", &lat, &lon, &timestamp) == 3) {
                    // Validate coordinates
                    if (lat >= -90.0 && lat <= 90.0 && 
                        lon >= -180.0 && lon <= 180.0) {
                        printf("✓ COORD: %.4f, %.4f (valid)\n", lat, lon);
                        valid++;
                        count_coords++;
                    } else {
                        printf("✗ COORD: %.4f, %.4f (INVALID)\n", lat, lon);
                        invalid++;
                    }
                } else {
                    printf("✗ Failed to parse coordinates\n");
                    invalid++;
                }
                break;
            }
            
            case 'T': {
                int hours, minutes;
                long timestamp;
                
                if (scanf("%d %d %ld", &hours, &minutes, &timestamp) == 3) {
                    // Validate time
                    if (hours >= 0 && hours <= 23 && 
                        minutes >= 0 && minutes <= 59) {
                        printf("✓ TIME: %02d:%02d (valid)\n", hours, minutes);
                        valid++;
                        count_time++;
                    } else {
                        printf("✗ TIME: %02d:%02d (INVALID)\n", hours, minutes);
                        invalid++;
                    }
                } else {
                    printf("✗ Failed to parse time\n");
                    invalid++;
                }
                break;
            }
            
            case 'I': {
                int id, dummy;
                long timestamp;
                
                if (scanf("%d %d %ld", &id, &dummy, &timestamp) == 3) {
                    // Validate ID (must be positive)
                    if (id > 0) {
                        printf("✓ ID: %d (valid)\n", id);
                        valid++;
                        count_id++;
                    } else {
                        printf("✗ ID: %d (INVALID)\n", id);
                        invalid++;
                    }
                } else {
                    printf("✗ Failed to parse ID\n");
                    invalid++;
                }
                break;
            }
            
            default:
                printf("✗ Unknown type: %c\n", type);
                // Skip rest of line
                while (getchar() != '\n' && !feof(stdin));
                invalid++;
        }
    }
    
    printf("\n=== STATISTICS ===\n");
    printf("Total records: %d\n", total);
    printf("Coordinates: %d\n", count_coords);
    printf("Time records: %d\n", count_time);
    printf("IDs: %d\n", count_id);
    printf("\n");
    printf("Valid: %d (%.1f%%)\n", valid, 
           total > 0 ? (valid * 100.0 / total) : 0);
    printf("Invalid: %d (%.1f%%)\n", invalid,
           total > 0 ? (invalid * 100.0 / total) : 0);
    
    printf("\n=== CLASSIFICATION COMPLETE ===\n");
    
    return 0;
}
