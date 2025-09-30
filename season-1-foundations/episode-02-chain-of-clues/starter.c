/*
 * OPERATION MOONLIGHT - Episode 02: Chain of Clues
 * Starter Code Templates
 */

#include <stdio.h>

/* ========================================
 * CLASSIFIER.C - Main Task
 * ======================================== */

void classifier_example() {
    // TODO: Read and classify records
    
    char type;
    int count_coords = 0, count_time = 0, count_id = 0;
    int valid = 0, invalid = 0;
    
    // Read until EOF
    while (scanf(" %c", &type) == 1) {
        switch (type) {
            case 'C': {
                // TODO: Read coordinates
                // float lat, lon;
                // long timestamp;
                // scanf("%f %f %ld", &lat, &lon, &timestamp);
                
                // TODO: Validate
                // if (lat >= -90.0 && lat <= 90.0 && 
                //     lon >= -180.0 && lon <= 180.0) {
                //     valid++;
                //     count_coords++;
                // } else {
                //     invalid++;
                // }
                break;
            }
            
            case 'T': {
                // TODO: Read time
                // int hours, minutes;
                // long timestamp;
                // scanf("%d %d %ld", &hours, &minutes, &timestamp);
                
                // TODO: Validate
                // if (hours >= 0 && hours <= 23 && 
                //     minutes >= 0 && minutes <= 59) {
                //     valid++;
                //     count_time++;
                // } else {
                //     invalid++;
                // }
                break;
            }
            
            case 'I': {
                // TODO: Read ID
                // int id, dummy;
                // long timestamp;
                // scanf("%d %d %ld", &id, &dummy, &timestamp);
                
                // TODO: Validate
                // if (id > 0) {
                //     valid++;
                //     count_id++;
                // } else {
                //     invalid++;
                // }
                break;
            }
            
            default:
                printf("Unknown type: %c\n", type);
                // Skip rest of line
                while (getchar() != '\n');
                invalid++;
        }
    }
    
    // TODO: Print statistics
    // printf("=== STATISTICS ===\n");
    // printf("Coordinates: %d\n", count_coords);
    // printf("Time records: %d\n", count_time);
    // printf("IDs: %d\n", count_id);
    // printf("Valid: %d\n", valid);
    // printf("Invalid: %d\n", invalid);
}


/* ========================================
 * ROUTER.C - Routing by Categories
 * ======================================== */

void router_example() {
    // TODO: Route data to categories
    
    // Moscow coordinates: ~55.0-56.0 N, 36.0-38.0 E
    // Day time: 06:00-18:00
    
    char type;
    while (scanf(" %c", &type) == 1) {
        if (type == 'C') {
            float lat, lon;
            long ts;
            scanf("%f %f %ld", &lat, &lon, &ts);
            
            // TODO: Check if in Moscow
            // if (lat >= 55.0 && lat <= 56.0 && 
            //     lon >= 36.0 && lon <= 38.0) {
            //     printf("MOSCOW: %.4f, %.4f\n", lat, lon);
            // } else {
            //     printf("OTHER: %.4f, %.4f\n", lat, lon);
            // }
            
        } else if (type == 'T') {
            int h, m;
            long ts;
            scanf("%d %d %ld", &h, &m, &ts);
            
            // TODO: Check day/night
            // if (h >= 6 && h < 18) {
            //     printf("DAY: %02d:%02d\n", h, m);
            // } else {
            //     printf("NIGHT: %02d:%02d\n", h, m);
            // }
            
        } else if (type == 'I') {
            int id, dummy;
            long ts;
            scanf("%d %d %ld", &id, &dummy, &ts);
            
            // TODO: Check even/odd
            // if (id % 2 == 0) {
            //     printf("EVEN ID: %d\n", id);
            // } else {
            //     printf("ODD ID: %d\n", id);
            // }
        }
    }
}


/* ========================================
 * PATTERN_FINDER.C - Find Patterns
 * ======================================== */

void pattern_finder_example() {
    // TODO: Find patterns in data
    
    // This is more advanced - involves arrays to store data
    // Then analyze for duplicates
    
    // Simple approach: count occurrences
    int id_counts[100] = {0};  // Assume IDs 0-99
    
    char type;
    while (scanf(" %c", &type) == 1) {
        if (type == 'I') {
            int id, dummy;
            long ts;
            scanf("%d %d %ld", &id, &dummy, &ts);
            
            if (id >= 0 && id < 100) {
                id_counts[id]++;
            }
        } else {
            // Skip other types
            // Read and discard
        }
    }
    
    // TODO: Find most frequent ID
    // int max_count = 0;
    // int most_frequent_id = -1;
    // for (int i = 0; i < 100; i++) {
    //     if (id_counts[i] > max_count) {
    //         max_count = id_counts[i];
    //         most_frequent_id = i;
    //     }
    // }
    
    // printf("Most frequent ID: %d (appears %d times)\n", 
    //        most_frequent_id, max_count);
}


/* ========================================
 * MAIN - Uncomment to test
 * ======================================== */

int main() {
    printf("OPERATION MOONLIGHT - Episode 02\n");
    printf("=================================\n\n");
    
    // Uncomment one to test:
    // classifier_example();
    // router_example();
    // pattern_finder_example();
    
    printf("\nCopy code to artifacts/ and complete TODOs\n");
    printf("Compile: gcc -Wall -o classifier artifacts/classifier.c\n");
    printf("Run: ./classifier < data.dat\n");
    
    return 0;
}


/* ========================================
 * TIPS
 * ========================================
 * 
 * 1. Reading with scanf:
 *    - " %c" - space before %c skips whitespace
 *    - Check return value: if (scanf(...) == 1)
 * 
 * 2. Switch statement:
 *    - Don't forget break!
 *    - Use {} for case blocks with variables
 * 
 * 3. Validation:
 *    - Use && for "both conditions"
 *    - Use || for "either condition"
 *    - Use ! for negation
 * 
 * 4. Ternary operator:
 *    - condition ? value_if_true : value_if_false
 *    - char *status = (valid) ? "OK" : "FAIL";
 * 
 * ======================================== */
