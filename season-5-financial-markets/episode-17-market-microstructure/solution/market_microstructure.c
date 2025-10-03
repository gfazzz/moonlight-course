/*
 * Episode 17: Market Microstructure Analysis - SOLUTION
 * Operation MOONLIGHT - Season 5
 * 
 * Mission: Analyze OHLCV data to detect market manipulation
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ OHLCV CSV parser with error handling
 * ✅ Bid-ask spread calculation
 * ✅ Volume analysis and anomaly detection
 * ✅ Volatility calculation (standard deviation of returns)
 * ✅ Manipulation pattern detection
 * ✅ Comprehensive statistics and reporting
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_TICKERS 100
#define MAX_CANDLES 1000
#define VOLUME_SPIKE_THRESHOLD 3.0
#define SPREAD_ANOMALY_THRESHOLD 2.0
#define PRICE_JUMP_THRESHOLD 0.05  // 5%

// OHLCV candle structure
typedef struct {
    char timestamp[32];
    double open;
    double high;
    double low;
    double close;
    long volume;
} OHLCV;

// Ticker with all candles
typedef struct {
    char symbol[16];
    OHLCV *candles;
    int count;
    int capacity;
} Ticker;

// Market statistics for a ticker
typedef struct {
    double avg_volume;
    double avg_spread;
    double volatility;
    int volume_spikes;
    int price_jumps;
    int spread_anomalies;
    double total_anomaly_score;
} MarketStats;

/*
 * Helper: Trim whitespace from string
 */
char* trim(char *str) {
    char *end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0)  // All spaces?
        return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    end[1] = '\0';
    
    return str;
}

/*
 * Add candle to ticker (with dynamic reallocation)
 */
int add_candle(Ticker *ticker, OHLCV *candle) {
    if (ticker->count >= ticker->capacity) {
        ticker->capacity = ticker->capacity == 0 ? 10 : ticker->capacity * 2;
        OHLCV *new_candles = realloc(ticker->candles, ticker->capacity * sizeof(OHLCV));
        if (!new_candles) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            return -1;
        }
        ticker->candles = new_candles;
    }
    
    ticker->candles[ticker->count++] = *candle;
    return 0;
}

/*
 * Find or create ticker in array
 */
Ticker* find_or_create_ticker(const char *symbol, Ticker *tickers, int *ticker_count) {
    // Search for existing ticker
    for (int i = 0; i < *ticker_count; i++) {
        if (strcmp(tickers[i].symbol, symbol) == 0) {
            return &tickers[i];
        }
    }
    
    // Create new ticker
    if (*ticker_count >= MAX_TICKERS) {
        fprintf(stderr, "Error: Too many tickers (max %d)\n", MAX_TICKERS);
        return NULL;
    }
    
    Ticker *ticker = &tickers[*ticker_count];
    strncpy(ticker->symbol, symbol, sizeof(ticker->symbol) - 1);
    ticker->symbol[sizeof(ticker->symbol) - 1] = '\0';
    ticker->candles = NULL;
    ticker->count = 0;
    ticker->capacity = 0;
    
    (*ticker_count)++;
    return ticker;
}

/*
 * Parse CSV file with OHLCV data
 * Format: TICKER,TIMESTAMP,OPEN,HIGH,LOW,CLOSE,VOLUME
 */
int parse_csv(const char *filename, Ticker *tickers, int *ticker_count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    char line[MAX_LINE];
    int line_num = 0;
    int data_lines = 0;
    
    // Skip header line if present
    if (fgets(line, sizeof(line), fp)) {
        line_num++;
        // Check if first line is header (contains "TICKER" or "ticker")
        char *upper_line = strdup(line);
        for (int i = 0; upper_line[i]; i++) {
            upper_line[i] = toupper(upper_line[i]);
        }
        if (strstr(upper_line, "TICKER") == NULL) {
            // Not a header, rewind
            fseek(fp, 0, SEEK_SET);
            line_num = 0;
        }
        free(upper_line);
    }
    
    // Parse data lines
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        
        // Skip empty lines
        if (strlen(trim(line)) == 0) {
            continue;
        }
        
        // Parse line: TICKER,TIMESTAMP,OPEN,HIGH,LOW,CLOSE,VOLUME
        char symbol[16];
        char timestamp[32];
        double open, high, low, close;
        long volume;
        
        int parsed = sscanf(line, "%15[^,],%31[^,],%lf,%lf,%lf,%lf,%ld",
                           symbol, timestamp, &open, &high, &low, &close, &volume);
        
        if (parsed != 7) {
            fprintf(stderr, "Warning: Invalid line %d (parsed %d/7 fields), skipping\n", 
                    line_num, parsed);
            continue;
        }
        
        // Validate data
        if (high < low || high < open || high < close || low > open || low > close) {
            fprintf(stderr, "Warning: Invalid OHLC on line %d: H=%.2f L=%.2f O=%.2f C=%.2f\n",
                    line_num, high, low, open, close);
            continue;
        }
        
        if (volume < 0) {
            fprintf(stderr, "Warning: Negative volume on line %d: %ld\n", line_num, volume);
            continue;
        }
        
        // Find or create ticker
        Ticker *ticker = find_or_create_ticker(symbol, tickers, ticker_count);
        if (!ticker) {
            fclose(fp);
            return -1;
        }
        
        // Create candle
        OHLCV candle;
        strncpy(candle.timestamp, timestamp, sizeof(candle.timestamp) - 1);
        candle.timestamp[sizeof(candle.timestamp) - 1] = '\0';
        candle.open = open;
        candle.high = high;
        candle.low = low;
        candle.close = close;
        candle.volume = volume;
        
        // Add to ticker
        if (add_candle(ticker, &candle) != 0) {
            fclose(fp);
            return -1;
        }
        
        data_lines++;
    }
    
    fclose(fp);
    
    if (data_lines == 0) {
        fprintf(stderr, "Error: No valid data found in CSV\n");
        return -1;
    }
    
    return 0;
}

/*
 * Calculate bid-ask spread (simplified)
 * Spread = (High - Low) / Close * 100%
 */
double calculate_spread(OHLCV *candle) {
    if (candle->close == 0.0) {
        return 0.0;
    }
    return ((candle->high - candle->low) / candle->close) * 100.0;
}

/*
 * Calculate average volume
 */
double calculate_avg_volume(OHLCV *candles, int count) {
    if (count == 0) return 0.0;
    
    long total = 0;
    for (int i = 0; i < count; i++) {
        total += candles[i].volume;
    }
    
    return (double)total / count;
}

/*
 * Calculate average spread
 */
double calculate_avg_spread(OHLCV *candles, int count) {
    if (count == 0) return 0.0;
    
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += calculate_spread(&candles[i]);
    }
    
    return total / count;
}

/*
 * Detect volume spike (anomaly)
 */
int detect_volume_spike(OHLCV *candles, int count, int index, double threshold) {
    if (count == 0 || index >= count) return 0;
    
    double avg_volume = calculate_avg_volume(candles, count);
    if (avg_volume == 0.0) return 0;
    
    return (candles[index].volume > avg_volume * threshold) ? 1 : 0;
}

/*
 * Detect price jump (sudden change)
 */
int detect_price_jump(OHLCV *candle, double threshold) {
    if (candle->open == 0.0) return 0;
    
    double change = fabs((candle->close - candle->open) / candle->open);
    return (change > threshold) ? 1 : 0;
}

/*
 * Detect spread anomaly (unusually wide spread)
 */
int detect_spread_anomaly(OHLCV *candles, int count, int index, double threshold) {
    if (count == 0 || index >= count) return 0;
    
    double avg_spread = calculate_avg_spread(candles, count);
    if (avg_spread == 0.0) return 0;
    
    double spread = calculate_spread(&candles[index]);
    return (spread > avg_spread * threshold) ? 1 : 0;
}

/*
 * Calculate volatility (standard deviation of returns)
 */
double calculate_volatility(OHLCV *candles, int count) {
    if (count < 2) return 0.0;
    
    // Calculate returns
    double *returns = malloc((count - 1) * sizeof(double));
    if (!returns) return 0.0;
    
    for (int i = 1; i < count; i++) {
        if (candles[i-1].close != 0.0) {
            returns[i-1] = (candles[i].close - candles[i-1].close) / candles[i-1].close;
        } else {
            returns[i-1] = 0.0;
        }
    }
    
    // Calculate mean
    double mean = 0.0;
    for (int i = 0; i < count - 1; i++) {
        mean += returns[i];
    }
    mean /= (count - 1);
    
    // Calculate variance
    double variance = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double diff = returns[i] - mean;
        variance += diff * diff;
    }
    variance /= (count - 2); // Sample variance (n-1)
    
    free(returns);
    
    return sqrt(variance);
}

/*
 * Calculate comprehensive market statistics
 */
MarketStats calculate_market_stats(Ticker *ticker) {
    MarketStats stats = {0};
    
    if (ticker->count == 0) return stats;
    
    // Basic metrics
    stats.avg_volume = calculate_avg_volume(ticker->candles, ticker->count);
    stats.avg_spread = calculate_avg_spread(ticker->candles, ticker->count);
    stats.volatility = calculate_volatility(ticker->candles, ticker->count);
    
    // Detect anomalies
    for (int i = 0; i < ticker->count; i++) {
        if (detect_volume_spike(ticker->candles, ticker->count, i, VOLUME_SPIKE_THRESHOLD)) {
            stats.volume_spikes++;
        }
        if (detect_price_jump(&ticker->candles[i], PRICE_JUMP_THRESHOLD)) {
            stats.price_jumps++;
        }
        if (detect_spread_anomaly(ticker->candles, ticker->count, i, SPREAD_ANOMALY_THRESHOLD)) {
            stats.spread_anomalies++;
        }
    }
    
    // Calculate total anomaly score
    stats.total_anomaly_score = stats.volume_spikes * 1.0 + 
                                 stats.price_jumps * 1.5 + 
                                 stats.spread_anomalies * 2.0;
    
    return stats;
}

/*
 * Print ticker statistics
 */
void print_ticker_stats(Ticker *ticker, MarketStats *stats) {
    printf("\n=== %s Statistics ===\n", ticker->symbol);
    printf("Candles: %d\n", ticker->count);
    printf("Avg Volume: %.0f\n", stats->avg_volume);
    printf("Avg Spread: %.4f%%\n", stats->avg_spread);
    printf("Volatility: %.4f (%.2f%%)\n", stats->volatility, stats->volatility * 100);
    
    // Anomalies
    printf("\n🚨 Anomalies:\n");
    printf("  Volume Spikes (>%.1fx avg): %d\n", VOLUME_SPIKE_THRESHOLD, stats->volume_spikes);
    printf("  Price Jumps (>%.0f%%): %d\n", PRICE_JUMP_THRESHOLD * 100, stats->price_jumps);
    printf("  Spread Anomalies (>%.1fx avg): %d\n", SPREAD_ANOMALY_THRESHOLD, stats->spread_anomalies);
    printf("  Total Anomaly Score: %.1f\n", stats->total_anomaly_score);
    
    // Interpretation
    if (stats->total_anomaly_score > 10.0) {
        printf("  ⚠️  HIGH SUSPICION - Likely manipulation\n");
    } else if (stats->total_anomaly_score > 5.0) {
        printf("  ⚠️  MODERATE SUSPICION - Further investigation needed\n");
    } else {
        printf("  ✅ NORMAL - No significant anomalies\n");
    }
    
    // Price range
    if (ticker->count > 0) {
        double min_price = ticker->candles[0].low;
        double max_price = ticker->candles[0].high;
        
        for (int i = 1; i < ticker->count; i++) {
            if (ticker->candles[i].low < min_price) min_price = ticker->candles[i].low;
            if (ticker->candles[i].high > max_price) max_price = ticker->candles[i].high;
        }
        
        printf("\n📊 Price Range: $%.2f - $%.2f (%.2f%% range)\n", 
               min_price, max_price, ((max_price - min_price) / min_price) * 100);
    }
}

/*
 * Print detailed manipulation analysis
 */
void print_manipulation_analysis(Ticker *tickers, int ticker_count, MarketStats *all_stats) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║           MARKET MANIPULATION ANALYSIS - FINAL REPORT             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    
    // Calculate totals
    int total_candles = 0;
    int total_volume_spikes = 0;
    int total_price_jumps = 0;
    int total_spread_anomalies = 0;
    double total_anomaly_score = 0.0;
    int high_suspicion_count = 0;
    
    for (int i = 0; i < ticker_count; i++) {
        total_candles += tickers[i].count;
        total_volume_spikes += all_stats[i].volume_spikes;
        total_price_jumps += all_stats[i].price_jumps;
        total_spread_anomalies += all_stats[i].spread_anomalies;
        total_anomaly_score += all_stats[i].total_anomaly_score;
        
        if (all_stats[i].total_anomaly_score > 10.0) {
            high_suspicion_count++;
        }
    }
    
    printf("\n📊 Dataset Overview:\n");
    printf("  Total Tickers: %d\n", ticker_count);
    printf("  Total Candles: %d\n", total_candles);
    printf("  Average Candles per Ticker: %.1f\n", (float)total_candles / ticker_count);
    
    printf("\n🚨 Anomaly Summary:\n");
    printf("  Total Volume Spikes: %d\n", total_volume_spikes);
    printf("  Total Price Jumps: %d\n", total_price_jumps);
    printf("  Total Spread Anomalies: %d\n", total_spread_anomalies);
    printf("  Total Anomaly Score: %.1f\n", total_anomaly_score);
    
    printf("\n⚠️  Suspicion Levels:\n");
    printf("  HIGH Suspicion (score > 10): %d tickers\n", high_suspicion_count);
    printf("  Percentage: %.1f%%\n", ((float)high_suspicion_count / ticker_count) * 100);
    
    // Estimate manipulation profit (simplified)
    double estimated_profit = total_anomaly_score * 100000.0; // $100k per anomaly point
    printf("\n💰 Estimated Manipulation Profit:\n");
    printf("  Conservative estimate: $%.1fM\n", estimated_profit / 1000000.0);
    
    // Top suspicious tickers
    printf("\n🎯 Top 5 Most Suspicious Tickers:\n");
    
    // Create sorted list
    typedef struct {
        int index;
        double score;
    } SuspiciousRank;
    
    SuspiciousRank *ranks = malloc(ticker_count * sizeof(SuspiciousRank));
    for (int i = 0; i < ticker_count; i++) {
        ranks[i].index = i;
        ranks[i].score = all_stats[i].total_anomaly_score;
    }
    
    // Simple bubble sort (good enough for small datasets)
    for (int i = 0; i < ticker_count - 1; i++) {
        for (int j = 0; j < ticker_count - i - 1; j++) {
            if (ranks[j].score < ranks[j + 1].score) {
                SuspiciousRank temp = ranks[j];
                ranks[j] = ranks[j + 1];
                ranks[j + 1] = temp;
            }
        }
    }
    
    // Print top 5
    int top_count = (ticker_count < 5) ? ticker_count : 5;
    for (int i = 0; i < top_count; i++) {
        int idx = ranks[i].index;
        printf("  %d. %-10s - Score: %.1f (V:%d, P:%d, S:%d)\n",
               i + 1,
               tickers[idx].symbol,
               all_stats[idx].total_anomaly_score,
               all_stats[idx].volume_spikes,
               all_stats[idx].price_jumps,
               all_stats[idx].spread_anomalies);
    }
    
    free(ranks);
    
    printf("\n═══════════════════════════════════════════════════════════════════\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <market_data.csv>\n", argv[0]);
        return 1;
    }
    
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║         Episode 17: Market Microstructure Analysis               ║\n");
    printf("║                   Operation MOONLIGHT - Season 5                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("📂 Loading market data from: %s\n", argv[1]);
    
    Ticker tickers[MAX_TICKERS];
    int ticker_count = 0;
    
    // Initialize tickers
    for (int i = 0; i < MAX_TICKERS; i++) {
        tickers[i].candles = NULL;
        tickers[i].count = 0;
        tickers[i].capacity = 0;
    }
    
    // Parse CSV data
    if (parse_csv(argv[1], tickers, &ticker_count) != 0) {
        fprintf(stderr, "❌ Error: Failed to parse CSV\n");
        return 1;
    }
    
    printf("✅ Loaded %d tickers\n", ticker_count);
    
    // Calculate statistics for all tickers
    MarketStats *all_stats = malloc(ticker_count * sizeof(MarketStats));
    if (!all_stats) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < ticker_count; i++) {
        all_stats[i] = calculate_market_stats(&tickers[i]);
    }
    
    // Print individual ticker statistics
    for (int i = 0; i < ticker_count; i++) {
        print_ticker_stats(&tickers[i], &all_stats[i]);
    }
    
    // Print comprehensive manipulation analysis
    print_manipulation_analysis(tickers, ticker_count, all_stats);
    
    printf("\n✅ Analysis Complete!\n");
    printf("\n🏆 Achievement Unlocked: \"Market Cartographer\"\n");
    printf("    You've mastered market microstructure analysis.\n\n");
    
    printf("📝 Mission Report:\n");
    printf("   Financial manipulation patterns detected.\n");
    printf("   Evidence gathered. Ready for next phase.\n\n");
    
    printf("➡️  Next: Episode 18 - Trading Algorithms\n");
    printf("    Reverse engineer the organization's trading strategies.\n\n");
    
    // Cleanup
    free(all_stats);
    for (int i = 0; i < ticker_count; i++) {
        free(tickers[i].candles);
    }
    
    return 0;
}

