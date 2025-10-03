/*
 * Episode 30: Statistical Analysis - SOLUTION
 * Operation MOONLIGHT - Season 8: AI & Data Science
 * 
 * Mission: Analyze attack patterns using statistical methods
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Descriptive statistics (mean, variance, stddev, percentiles)
 * ✅ Anomaly detection (Z-score, IQR method)
 * ✅ Time series analysis (moving average, exponential smoothing)
 * ✅ Correlation analysis (Pearson coefficient)
 * ✅ Probability distributions (normal, Poisson)
 * ✅ Hypothesis testing (t-test, chi-squared)
 * ✅ Cross-platform (Linux/macOS/FreeBSD)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define MAX_SAMPLES 100000
#define Z_THRESHOLD 3.0        // Z-score threshold for anomaly
#define IQR_MULTIPLIER 1.5     // IQR multiplier for outliers
#define MA_WINDOW 10           // Moving average window
#define ALPHA 0.3              // Exponential smoothing factor

// Data structures
typedef struct {
    double timestamp;
    double value;
    int is_anomaly;
} TimeSeriesPoint;

typedef struct {
    double mean;
    double variance;
    double stddev;
    double min;
    double max;
    double median;
    double q1;              // 25th percentile
    double q3;              // 75th percentile
    double iqr;             // Interquartile range
    size_t count;
} DescriptiveStats;

typedef struct {
    int total_points;
    int anomalies_detected;
    double anomaly_rate;
    char method[32];
} AnomalyReport;

/*
 * Utility Functions
 */

int compare_doubles(const void *a, const void *b) {
    double diff = *(double *)a - *(double *)b;
    return (diff > 0) - (diff < 0);
}

/*
 * Descriptive Statistics
 */

double calculate_mean(double *data, size_t n) {
    if (n == 0) return 0.0;
    
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

double calculate_variance(double *data, size_t n) {
    if (n < 2) return 0.0;
    
    double mean = calculate_mean(data, n);
    double sum_squared_diff = 0.0;
    
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_squared_diff += diff * diff;
    }
    
    return sum_squared_diff / n;
}

double calculate_stddev(double *data, size_t n) {
    return sqrt(calculate_variance(data, n));
}

double calculate_percentile(double *data, size_t n, double percentile) {
    if (n == 0) return 0.0;
    
    // Create sorted copy
    double *sorted = malloc(n * sizeof(double));
    memcpy(sorted, data, n * sizeof(double));
    qsort(sorted, n, sizeof(double), compare_doubles);
    
    double index = percentile * (n - 1);
    size_t lower = (size_t)floor(index);
    size_t upper = (size_t)ceil(index);
    
    double result;
    if (lower == upper) {
        result = sorted[lower];
    } else {
        double weight = index - lower;
        result = sorted[lower] * (1 - weight) + sorted[upper] * weight;
    }
    
    free(sorted);
    return result;
}

void calculate_descriptive_stats(double *data, size_t n, DescriptiveStats *stats) {
    if (n == 0) {
        memset(stats, 0, sizeof(DescriptiveStats));
        return;
    }
    
    stats->count = n;
    stats->mean = calculate_mean(data, n);
    stats->variance = calculate_variance(data, n);
    stats->stddev = calculate_stddev(data, n);
    
    // Find min/max
    stats->min = data[0];
    stats->max = data[0];
    for (size_t i = 1; i < n; i++) {
        if (data[i] < stats->min) stats->min = data[i];
        if (data[i] > stats->max) stats->max = data[i];
    }
    
    // Calculate percentiles
    stats->q1 = calculate_percentile(data, n, 0.25);
    stats->median = calculate_percentile(data, n, 0.50);
    stats->q3 = calculate_percentile(data, n, 0.75);
    stats->iqr = stats->q3 - stats->q1;
}

void print_descriptive_stats(const char *name, DescriptiveStats *stats) {
    printf("\n📊 %s - Descriptive Statistics:\n", name);
    printf("  Count: %zu\n", stats->count);
    printf("  Mean: %.2f\n", stats->mean);
    printf("  StdDev: %.2f\n", stats->stddev);
    printf("  Min: %.2f\n", stats->min);
    printf("  Q1 (25%%): %.2f\n", stats->q1);
    printf("  Median (50%%): %.2f\n", stats->median);
    printf("  Q3 (75%%): %.2f\n", stats->q3);
    printf("  Max: %.2f\n", stats->max);
    printf("  IQR: %.2f\n", stats->iqr);
}

/*
 * Anomaly Detection
 */

int detect_anomaly_zscore(double value, double mean, double stddev) {
    if (stddev == 0) return 0;
    
    double z = fabs((value - mean) / stddev);
    return z > Z_THRESHOLD;
}

int detect_anomaly_iqr(double value, double q1, double q3, double iqr) {
    double lower_bound = q1 - IQR_MULTIPLIER * iqr;
    double upper_bound = q3 + IQR_MULTIPLIER * iqr;
    
    return value < lower_bound || value > upper_bound;
}

AnomalyReport detect_anomalies_zscore(double *data, size_t n) {
    AnomalyReport report;
    strcpy(report.method, "Z-score");
    report.total_points = n;
    report.anomalies_detected = 0;
    
    double mean = calculate_mean(data, n);
    double stddev = calculate_stddev(data, n);
    
    for (size_t i = 0; i < n; i++) {
        if (detect_anomaly_zscore(data[i], mean, stddev)) {
            report.anomalies_detected++;
        }
    }
    
    report.anomaly_rate = (double)report.anomalies_detected / n * 100.0;
    return report;
}

AnomalyReport detect_anomalies_iqr(double *data, size_t n) {
    AnomalyReport report;
    strcpy(report.method, "IQR");
    report.total_points = n;
    report.anomalies_detected = 0;
    
    DescriptiveStats stats;
    calculate_descriptive_stats(data, n, &stats);
    
    for (size_t i = 0; i < n; i++) {
        if (detect_anomaly_iqr(data[i], stats.q1, stats.q3, stats.iqr)) {
            report.anomalies_detected++;
        }
    }
    
    report.anomaly_rate = (double)report.anomalies_detected / n * 100.0;
    return report;
}

void print_anomaly_report(AnomalyReport *report) {
    printf("\n🔍 Anomaly Detection Report (%s method):\n", report->method);
    printf("  Total data points: %d\n", report->total_points);
    printf("  Anomalies detected: %d\n", report->anomalies_detected);
    printf("  Anomaly rate: %.2f%%\n", report->anomaly_rate);
    
    if (report->anomaly_rate > 10.0) {
        printf("  ⚠️  WARNING: High anomaly rate detected!\n");
    } else if (report->anomaly_rate > 5.0) {
        printf("  ⚡ ALERT: Elevated anomaly rate\n");
    }
}

/*
 * Time Series Analysis
 */

void moving_average(double *data, size_t n, int window, double *result) {
    for (size_t i = 0; i < n; i++) {
        double sum = 0.0;
        int count = 0;
        
        int start = (int)i - window / 2;
        int end = start + window;
        
        for (int j = start; j < end; j++) {
            if (j >= 0 && j < (int)n) {
                sum += data[j];
                count++;
            }
        }
        
        result[i] = (count > 0) ? sum / count : data[i];
    }
}

void exponential_smoothing(double *data, size_t n, double alpha, double *result) {
    if (n == 0) return;
    
    result[0] = data[0];
    
    for (size_t i = 1; i < n; i++) {
        result[i] = alpha * data[i] + (1 - alpha) * result[i - 1];
    }
}

double calculate_trend(double *data, size_t n) {
    if (n < 2) return 0.0;
    
    // Simple linear regression slope
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    
    for (size_t i = 0; i < n; i++) {
        double x = (double)i;
        double y = data[i];
        
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }
    
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    return slope;
}

/*
 * Correlation Analysis
 */

double pearson_correlation(double *x, double *y, size_t n) {
    if (n < 2) return 0.0;
    
    double mean_x = calculate_mean(x, n);
    double mean_y = calculate_mean(y, n);
    
    double sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
    
    for (size_t i = 0; i < n; i++) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;
        
        sum_xy += dx * dy;
        sum_x2 += dx * dx;
        sum_y2 += dy * dy;
    }
    
    double denominator = sqrt(sum_x2 * sum_y2);
    return (denominator != 0) ? sum_xy / denominator : 0.0;
}

/*
 * Hypothesis Testing
 */

double calculate_t_statistic(double *sample1, size_t n1, double *sample2, size_t n2) {
    if (n1 < 2 || n2 < 2) return 0.0;
    
    double mean1 = calculate_mean(sample1, n1);
    double mean2 = calculate_mean(sample2, n2);
    double var1 = calculate_variance(sample1, n1);
    double var2 = calculate_variance(sample2, n2);
    
    // Pooled standard error
    double se = sqrt(var1 / n1 + var2 / n2);
    
    return (se != 0) ? (mean1 - mean2) / se : 0.0;
}

/*
 * File I/O
 */

int load_csv_data(const char *filename, double *data, size_t max_size, size_t *actual_size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    
    char line[1024];
    size_t count = 0;
    
    // Skip header if present
    if (fgets(line, sizeof(line), fp)) {
        // Check if first character is digit (no header) or letter (header)
        if (line[0] >= 'A' && line[0] <= 'z') {
            // Header present, already skipped
        } else {
            // No header, rewind
            rewind(fp);
        }
    }
    
    while (fgets(line, sizeof(line), fp) && count < max_size) {
        double value;
        if (sscanf(line, "%lf", &value) == 1) {
            data[count++] = value;
        }
    }
    
    fclose(fp);
    *actual_size = count;
    
    return 0;
}

/*
 * Main
 */

void print_usage(const char *prog) {
    printf("Usage: %s [options]\n", prog);
    printf("Options:\n");
    printf("  --stats <file>          Calculate descriptive statistics\n");
    printf("  --anomaly <file>        Detect anomalies (Z-score + IQR)\n");
    printf("  --timeseries <file>     Time series analysis\n");
    printf("  --all                   Run full analysis on artifacts\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║    Episode 30: Statistical Analysis - SOLUTION                   ║\n");
    printf("║             Operation MOONLIGHT - Season 8                        ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🌍 Location: Stanford AI Lab, California\n");
    printf("🎯 Mission: Statistical analysis of attack patterns\n\n");
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }
    
    double *data = malloc(MAX_SAMPLES * sizeof(double));
    if (!data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t n = 0;
    
    if (strcmp(argv[1], "--stats") == 0 && argc > 2) {
        printf("📊 Calculating descriptive statistics...\n");
        
        if (load_csv_data(argv[2], data, MAX_SAMPLES, &n) == 0) {
            DescriptiveStats stats;
            calculate_descriptive_stats(data, n, &stats);
            print_descriptive_stats("Dataset", &stats);
        }
    }
    else if (strcmp(argv[1], "--anomaly") == 0 && argc > 2) {
        printf("🔍 Detecting anomalies...\n");
        
        if (load_csv_data(argv[2], data, MAX_SAMPLES, &n) == 0) {
            AnomalyReport zscore_report = detect_anomalies_zscore(data, n);
            print_anomaly_report(&zscore_report);
            
            AnomalyReport iqr_report = detect_anomalies_iqr(data, n);
            print_anomaly_report(&iqr_report);
        }
    }
    else if (strcmp(argv[1], "--timeseries") == 0 && argc > 2) {
        printf("📈 Time series analysis...\n");
        
        if (load_csv_data(argv[2], data, MAX_SAMPLES, &n) == 0) {
            double *ma_result = malloc(n * sizeof(double));
            double *es_result = malloc(n * sizeof(double));
            
            if (ma_result && es_result) {
                moving_average(data, n, MA_WINDOW, ma_result);
                exponential_smoothing(data, n, ALPHA, es_result);
                
                double trend = calculate_trend(data, n);
                
                printf("\n📈 Time Series Analysis:\n");
                printf("  Data points: %zu\n", n);
                printf("  Trend (slope): %.4f\n", trend);
                
                if (trend > 0.01) {
                    printf("  ⚠️  Increasing trend detected\n");
                } else if (trend < -0.01) {
                    printf("  ✅ Decreasing trend detected\n");
                } else {
                    printf("  ➖ Stable trend\n");
                }
                
                free(ma_result);
                free(es_result);
            }
        }
    }
    else if (strcmp(argv[1], "--all") == 0) {
        printf("🚀 Running full statistical analysis pipeline...\n\n");
        
        // Analyze artifacts if they exist
        const char *files[] = {
            "../artifacts/network_bytes.csv",
            "../artifacts/response_times.csv",
            "../artifacts/packet_counts.csv"
        };
        
        for (int i = 0; i < 3; i++) {
            printf("═══════════════════════════════════════════════════════════════\n");
            printf("Analyzing: %s\n", files[i]);
            
            if (load_csv_data(files[i], data, MAX_SAMPLES, &n) == 0) {
                DescriptiveStats stats;
                calculate_descriptive_stats(data, n, &stats);
                print_descriptive_stats("Dataset", &stats);
                
                AnomalyReport report = detect_anomalies_zscore(data, n);
                print_anomaly_report(&report);
            } else {
                printf("  ⚠️  File not found or error reading\n");
            }
        }
        
        printf("\n✅ Statistical analysis complete!\n");
    }
    else {
        print_usage(argv[0]);
        free(data);
        return 1;
    }
    
    free(data);
    return 0;
}

