/*
 * OPERATION MOONLIGHT - Episode 03
 * Solution: pattern_finder.c
 */

#include <stdio.h>

#define MAX_RECORDS 1000

int main() {
    printf("=== PATTERN FINDER v1.0 ===\n");
    printf("Analyzing temporal patterns...\n\n");
    
    long timestamps[MAX_RECORDS];
    int count = 0;
    float lat, lon;
    
    // Read all timestamps
    while (scanf("%ld %f %f", &timestamps[count], &lat, &lon) == 3) {
        count++;
        if (count >= MAX_RECORDS) {
            printf("Warning: Maximum records reached\n");
            break;
        }
    }
    
    if (count < 2) {
        printf("Error: Need at least 2 records\n");
        return 1;
    }
    
    printf("Read %d records\n\n", count);
    
    // Calculate intervals
    int intervals[MAX_RECORDS - 1];
    for (int i = 0; i < count - 1; i++) {
        intervals[i] = timestamps[i+1] - timestamps[i];
    }
    
    // Find most frequent interval
    int freq[1000] = {0};
    for (int i = 0; i < count - 1; i++) {
        int interval = intervals[i];
        if (interval >= 0 && interval < 1000) {
            freq[interval]++;
        }
    }
    
    // Find maximum frequency
    int max_freq = 0;
    int period = 0;
    for (int i = 0; i < 1000; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            period = i;
        }
    }
    
    // Print results
    printf("=== PATTERN ANALYSIS ===\n");
    printf("Detected period: %d seconds\n", period);
    printf("Occurrences: %d out of %d intervals (%.1f%%)\n", 
           max_freq, count - 1, 
           (max_freq * 100.0) / (count - 1));
    
    // Show first few intervals
    printf("\nFirst 10 intervals:\n");
    for (int i = 0; i < 10 && i < count - 1; i++) {
        printf("  %d: %d seconds\n", i+1, intervals[i]);
    }
    
    printf("\n=== PATTERN DETECTED ===\n");
    printf("Signal transmitted every %d seconds\n", period);
    
    return 0;
}
