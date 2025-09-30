/*
 * OPERATION MOONLIGHT - Episode 03
 * Solution: sequence_analyzer.c
 */

#include <stdio.h>
#include <math.h>

#define MAX_RECORDS 1000

float calculate_distance(float lat1, float lon1, float lat2, float lon2) {
    float dlat = lat2 - lat1;
    float dlon = lon2 - lon1;
    return sqrt(dlat * dlat + dlon * dlon) * 111.0;  // ~111 km per degree
}

int main() {
    printf("=== SEQUENCE ANALYZER v1.0 ===\n");
    printf("Analyzing coordinate sequence...\n\n");
    
    long timestamps[MAX_RECORDS];
    float latitudes[MAX_RECORDS];
    float longitudes[MAX_RECORDS];
    int count = 0;
    
    // Read all data
    while (scanf("%ld %f %f", 
                 &timestamps[count], 
                 &latitudes[count], 
                 &longitudes[count]) == 3) {
        count++;
        if (count >= MAX_RECORDS) break;
    }
    
    if (count < 2) {
        printf("Error: Need at least 2 records\n");
        return 1;
    }
    
    printf("Read %d records\n\n", count);
    
    // Calculate distances
    float distances[MAX_RECORDS - 1];
    float sum = 0;
    
    for (int i = 0; i < count - 1; i++) {
        distances[i] = calculate_distance(
            latitudes[i], longitudes[i],
            latitudes[i+1], longitudes[i+1]
        );
        sum += distances[i];
    }
    
    float average = sum / (count - 1);
    
    // Find min and max
    float min_dist = distances[0];
    float max_dist = distances[0];
    
    for (int i = 1; i < count - 1; i++) {
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
    }
    
    // Find anomalies (distance > 2 * average)
    int anomaly_count = 0;
    float anomaly_threshold = 2 * average;
    
    printf("=== ANOMALY DETECTION ===\n");
    printf("Threshold: %.3f km (2× average)\n\n", anomaly_threshold);
    
    for (int i = 0; i < count - 1; i++) {
        if (distances[i] > anomaly_threshold) {
            printf("⚠️  Anomaly at step %d:\n", i+1);
            printf("   From: (%.4f, %.4f)\n", latitudes[i], longitudes[i]);
            printf("   To:   (%.4f, %.4f)\n", latitudes[i+1], longitudes[i+1]);
            printf("   Distance: %.3f km (%.1f× average)\n\n", 
                   distances[i], distances[i] / average);
            anomaly_count++;
        }
    }
    
    if (anomaly_count == 0) {
        printf("No anomalies detected\n\n");
    }
    
    // Print statistics
    printf("=== SEQUENCE STATISTICS ===\n");
    printf("Total steps: %d\n", count - 1);
    printf("Average distance: %.3f km\n", average);
    printf("Min distance: %.3f km\n", min_dist);
    printf("Max distance: %.3f km\n", max_dist);
    printf("Anomalies: %d\n", anomaly_count);
    
    // Calculate total path length
    float total_distance = sum;
    printf("Total path length: %.2f km\n", total_distance);
    
    printf("\n=== ANALYSIS COMPLETE ===\n");
    
    return 0;
}
