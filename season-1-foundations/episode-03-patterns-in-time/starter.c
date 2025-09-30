/*
 * OPERATION MOONLIGHT - Episode 03: Patterns in Time
 * Starter Code Templates
 */

#include <stdio.h>
#include <math.h>

#define MAX_RECORDS 1000

/* ========================================
 * PATTERN_FINDER.C - Find Periodic Pattern
 * ======================================== */

void pattern_finder_example() {
    long timestamps[MAX_RECORDS];
    int count = 0;
    
    float lat, lon;  // Not used here, just read
    
    // TODO: Read all timestamps
    // while (scanf("%ld %f %f", &timestamps[count], &lat, &lon) == 3) {
    //     count++;
    //     if (count >= MAX_RECORDS) break;
    // }
    
    // TODO: Calculate intervals
    // int intervals[MAX_RECORDS - 1];
    // for (int i = 0; i < count - 1; i++) {
    //     intervals[i] = timestamps[i+1] - timestamps[i];
    // }
    
    // TODO: Find most frequent interval (mode)
    // int freq[1000] = {0};  // Assume intervals < 1000
    // for (int i = 0; i < count - 1; i++) {
    //     int interval = intervals[i];
    //     if (interval < 1000 && interval >= 0) {
    //         freq[interval]++;
    //     }
    // }
    
    // Find maximum frequency
    // int max_freq = 0;
    // int period = 0;
    // for (int i = 0; i < 1000; i++) {
    //     if (freq[i] > max_freq) {
    //         max_freq = freq[i];
    //         period = i;
    //     }
    // }
    
    // TODO: Print results
    // printf("=== PATTERN ANALYSIS ===\n");
    // printf("Total records: %d\n", count);
    // printf("Detected period: %d seconds\n", period);
    // printf("Occurrences: %d\n", max_freq);
}


/* ========================================
 * SEQUENCE_ANALYZER.C - Analyze Coordinate Sequence
 * ======================================== */

float calculate_distance(float lat1, float lon1, float lat2, float lon2) {
    // Simplified distance formula for small distances
    float dlat = lat2 - lat1;
    float dlon = lon2 - lon1;
    return sqrt(dlat * dlat + dlon * dlon) * 111.0;  // km per degree
}

void sequence_analyzer_example() {
    long timestamps[MAX_RECORDS];
    float latitudes[MAX_RECORDS];
    float longitudes[MAX_RECORDS];
    int count = 0;
    
    // TODO: Read all data
    // while (scanf("%ld %f %f", 
    //              &timestamps[count], 
    //              &latitudes[count], 
    //              &longitudes[count]) == 3) {
    //     count++;
    //     if (count >= MAX_RECORDS) break;
    // }
    
    // TODO: Calculate distances between consecutive points
    // float distances[MAX_RECORDS - 1];
    // for (int i = 0; i < count - 1; i++) {
    //     distances[i] = calculate_distance(
    //         latitudes[i], longitudes[i],
    //         latitudes[i+1], longitudes[i+1]
    //     );
    // }
    
    // TODO: Calculate average distance
    // float sum = 0;
    // for (int i = 0; i < count - 1; i++) {
    //     sum += distances[i];
    // }
    // float average = sum / (count - 1);
    
    // TODO: Find anomalies (distances > 2 * average)
    // int anomaly_count = 0;
    // for (int i = 0; i < count - 1; i++) {
    //     if (distances[i] > 2 * average) {
    //         printf("Anomaly at record %d: %.2f km (avg: %.2f)\n", 
    //                i, distances[i], average);
    //         anomaly_count++;
    //     }
    // }
    
    // TODO: Print statistics
    // printf("\n=== SEQUENCE ANALYSIS ===\n");
    // printf("Total records: %d\n", count);
    // printf("Average distance: %.3f km\n", average);
    // printf("Anomalies found: %d\n", anomaly_count);
}


/* ========================================
 * BRUTEFORCE.C - Find PIN by Bruteforce
 * ======================================== */

unsigned int calculate_checksum(int pin) {
    // Simple checksum: sum of digits
    int sum = 0;
    int temp = pin;
    while (temp > 0) {
        sum += temp % 10;
        temp /= 10;
    }
    return sum;
}

void bruteforce_example() {
    // Target checksum (in real scenario, this would be from data)
    unsigned int target_checksum = 15;  // Example: PIN 6900 has checksum 15
    
    printf("=== BRUTEFORCE PIN FINDER ===\n");
    printf("Target checksum: %u\n", target_checksum);
    printf("Searching...\n\n");
    
    int found_count = 0;
    
    // TODO: Brute force all 4-digit PINs
    // for (int pin = 0; pin <= 9999; pin++) {
    //     unsigned int checksum = calculate_checksum(pin);
    //     
    //     if (checksum == target_checksum) {
    //         printf("Found: %04d (checksum: %u)\n", pin, checksum);
    //         found_count++;
    //     }
    // }
    
    // TODO: Print results
    // printf("\n=== BRUTEFORCE COMPLETE ===\n");
    // printf("Total PINs found: %d\n", found_count);
    
    // Note: In real scenario, you'd break after first match
    // or use more sophisticated validation
}


/* ========================================
 * DEMONSTRATION: Loop Types
 * ======================================== */

void loop_demonstrations() {
    printf("=== LOOP DEMONSTRATIONS ===\n\n");
    
    // For loop
    printf("For loop (0-4):\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n\n");
    
    // While loop
    printf("While loop (countdown from 5):\n");
    int count = 5;
    while (count > 0) {
        printf("%d ", count);
        count--;
    }
    printf("\n\n");
    
    // Do-while loop
    printf("Do-while loop (at least once):\n");
    int x = 0;
    do {
        printf("%d ", x);
        x++;
    } while (x < 3);
    printf("\n\n");
    
    // Break example
    printf("Break example (stop at 3):\n");
    for (int i = 0; i < 10; i++) {
        if (i == 3) break;
        printf("%d ", i);
    }
    printf("\n\n");
    
    // Continue example
    printf("Continue example (skip even):\n");
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;
        printf("%d ", i);
    }
    printf("\n\n");
    
    // Nested loops
    printf("Nested loops (3x3 grid):\n");
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            printf("(%d,%d) ", row, col);
        }
        printf("\n");
    }
}


/* ========================================
 * MAIN
 * ======================================== */

int main() {
    printf("OPERATION MOONLIGHT - Episode 03\n");
    printf("=================================\n\n");
    
    // Uncomment to test demonstrations
    // loop_demonstrations();
    
    printf("\nCopy code to artifacts/ and complete TODOs\n");
    printf("Compile: gcc -Wall -lm -o pattern artifacts/pattern_finder.c\n");
    printf("Run: ./pattern < data.txt\n");
    
    return 0;
}


/* ========================================
 * TIPS
 * ========================================
 * 
 * 1. Choosing the right loop:
 *    - for: known number of iterations
 *    - while: unknown iterations, condition-based
 *    - do-while: at least one iteration guaranteed
 * 
 * 2. Common patterns:
 *    - Counting: for (int i = 0; i < n; i++)
 *    - Reading until EOF: while (scanf(...) == n)
 *    - Menu loop: do { ... } while (choice != 0)
 * 
 * 3. Avoiding infinite loops:
 *    - Always modify loop variable
 *    - Check termination condition
 *    - Use break for safety exits
 * 
 * 4. Math functions:
 *    - sqrt(), pow() from math.h
 *    - Compile with -lm flag!
 * 
 * 5. Array bounds:
 *    - Always check: if (i < MAX)
 *    - Prevent overflow
 * 
 * ======================================== */
