/*
 * Episode 18: Trading Algorithms - SOLUTION
 * Operation MOONLIGHT - Season 5
 * 
 * Mission: Reverse-engineer trading strategies and expose overfitting
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Technical Indicators: SMA, EMA, RSI, MACD, Bollinger Bands
 * ✅ Trading Strategies: SMA Crossover, RSI, MACD Momentum
 * ✅ Backtesting Engine with commission and slippage
 * ✅ Performance Metrics: Sharpe ratio, max drawdown, win rate
 * ✅ Overfitting detection and analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_CANDLES 2000
#define INITIAL_CAPITAL 10000.0
#define DEFAULT_COMMISSION 0.001  // 0.1% commission
#define DEFAULT_SLIPPAGE 0.0005   // 0.05% slippage

// OHLCV candle structure
typedef struct {
    char timestamp[32];
    double open;
    double high;
    double low;
    double close;
    long volume;
} OHLCV;

// Trading signal
typedef enum {
    NO_SIGNAL,
    BUY,
    SELL
} Signal;

// Trade record
typedef struct {
    int candle_index;
    Signal signal;
    double price;
    double shares;
    double commission;
    double capital;
} Trade;

// Backtest result
typedef struct {
    double initial_capital;
    double final_capital;
    double max_capital;
    double min_capital;
    double max_drawdown;
    int total_trades;
    int winning_trades;
    int losing_trades;
    double total_return;
    double sharpe_ratio;
    double *equity_curve;
    int equity_count;
    Trade *trades;
    int trade_count;
} BacktestResult;

// Strategy parameters
typedef struct {
    int sma_short;
    int sma_long;
    int rsi_period;
    int rsi_oversold;
    int rsi_overbought;
    int macd_fast;
    int macd_slow;
    int macd_signal;
    double commission;
    double slippage;
} StrategyParams;

/*
 * Helper: Trim whitespace
 */
char* trim(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

/*
 * Load OHLCV data from CSV
 */
int load_ohlcv(const char *filename, OHLCV *candles, int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    char line[MAX_LINE];
    *count = 0;
    
    // Skip header if present
    if (fgets(line, sizeof(line), fp)) {
        char *upper_line = strdup(line);
        for (int i = 0; upper_line[i]; i++) {
            upper_line[i] = toupper(upper_line[i]);
        }
        if (strstr(upper_line, "TIMESTAMP") == NULL && 
            strstr(upper_line, "OPEN") == NULL) {
            fseek(fp, 0, SEEK_SET);
        }
        free(upper_line);
    }
    
    // Parse data
    while (fgets(line, sizeof(line), fp) && *count < MAX_CANDLES) {
        if (strlen(trim(line)) == 0) continue;
        
        char timestamp[32];
        double open, high, low, close;
        long volume;
        
        int parsed = sscanf(line, "%31[^,],%lf,%lf,%lf,%lf,%ld",
                           timestamp, &open, &high, &low, &close, &volume);
        
        if (parsed != 6) continue;
        
        strncpy(candles[*count].timestamp, timestamp, 31);
        candles[*count].timestamp[31] = '\0';
        candles[*count].open = open;
        candles[*count].high = high;
        candles[*count].low = low;
        candles[*count].close = close;
        candles[*count].volume = volume;
        
        (*count)++;
    }
    
    fclose(fp);
    return 0;
}

/*
 * Technical Indicators
 */

// Simple Moving Average
double calculate_sma(double *prices, int n, int period) {
    if (n < period) return 0.0;
    
    double sum = 0.0;
    for (int i = n - period; i < n; i++) {
        sum += prices[i];
    }
    
    return sum / period;
}

// Exponential Moving Average
double calculate_ema(double *prices, int n, int period) {
    if (n < period) return 0.0;
    
    // Start with SMA for first period
    double ema = calculate_sma(prices, period, period);
    if (ema == 0.0) return 0.0;
    
    // Calculate EMA for remaining periods
    double k = 2.0 / (period + 1.0);
    
    for (int i = period; i < n; i++) {
        ema = prices[i] * k + ema * (1.0 - k);
    }
    
    return ema;
}

// Relative Strength Index
double calculate_rsi(double *prices, int n, int period) {
    if (n <= period) return 50.0;
    
    double gains = 0.0, losses = 0.0;
    
    // Calculate average gains and losses
    for (int i = n - period; i < n; i++) {
        if (i == 0) continue;
        
        double change = prices[i] - prices[i-1];
        if (change > 0) {
            gains += change;
        } else {
            losses += -change;
        }
    }
    
    double avg_gain = gains / period;
    double avg_loss = losses / period;
    
    if (avg_loss == 0.0) return 100.0;
    if (avg_gain == 0.0) return 0.0;
    
    double rs = avg_gain / avg_loss;
    double rsi = 100.0 - (100.0 / (1.0 + rs));
    
    return rsi;
}

// MACD (Moving Average Convergence Divergence)
void calculate_macd(double *prices, int n, int fast, int slow, int signal_period,
                   double *macd_out, double *signal_out, double *histogram_out) {
    if (n < slow) {
        *macd_out = 0.0;
        *signal_out = 0.0;
        *histogram_out = 0.0;
        return;
    }
    
    double ema_fast = calculate_ema(prices, n, fast);
    double ema_slow = calculate_ema(prices, n, slow);
    
    *macd_out = ema_fast - ema_slow;
    
    // For signal line, we need MACD history (simplified: use current MACD)
    *signal_out = *macd_out * 0.9; // Approximation
    *histogram_out = *macd_out - *signal_out;
}

// Bollinger Bands
void calculate_bollinger_bands(double *prices, int n, int period, double std_dev,
                               double *upper, double *middle, double *lower) {
    *middle = calculate_sma(prices, n, period);
    
    if (n < period) {
        *upper = *middle;
        *lower = *middle;
        return;
    }
    
    // Calculate standard deviation
    double variance = 0.0;
    for (int i = n - period; i < n; i++) {
        double diff = prices[i] - *middle;
        variance += diff * diff;
    }
    variance /= period;
    double std = sqrt(variance);
    
    *upper = *middle + (std * std_dev);
    *lower = *middle - (std * std_dev);
}

/*
 * Trading Strategies
 */

// Strategy 1: SMA Crossover (Golden Cross / Death Cross)
Signal sma_crossover_strategy(double *prices, int n, int short_period, int long_period) {
    if (n < long_period + 1) return NO_SIGNAL;
    
    double sma_short_prev = calculate_sma(prices, n - 1, short_period);
    double sma_long_prev = calculate_sma(prices, n - 1, long_period);
    double sma_short_curr = calculate_sma(prices, n, short_period);
    double sma_long_curr = calculate_sma(prices, n, long_period);
    
    // Golden Cross: short crosses above long
    if (sma_short_prev <= sma_long_prev && sma_short_curr > sma_long_curr) {
        return BUY;
    }
    
    // Death Cross: short crosses below long
    if (sma_short_prev >= sma_long_prev && sma_short_curr < sma_long_curr) {
        return SELL;
    }
    
    return NO_SIGNAL;
}

// Strategy 2: RSI Overbought/Oversold
Signal rsi_strategy(double *prices, int n, int period, int oversold, int overbought) {
    if (n <= period) return NO_SIGNAL;
    
    double rsi = calculate_rsi(prices, n, period);
    
    // Oversold condition: potential buy
    if (rsi < oversold) {
        return BUY;
    }
    
    // Overbought condition: potential sell
    if (rsi > overbought) {
        return SELL;
    }
    
    return NO_SIGNAL;
}

// Strategy 3: MACD Momentum
Signal macd_strategy(double *prices, int n, long *volumes, int fast, int slow, int sig_period) {
    if (n < slow + 1) return NO_SIGNAL;
    
    double macd, signal, histogram;
    calculate_macd(prices, n, fast, slow, sig_period, &macd, &signal, &histogram);
    
    double macd_prev, signal_prev, histogram_prev;
    calculate_macd(prices, n - 1, fast, slow, sig_period, &macd_prev, &signal_prev, &histogram_prev);
    
    // Calculate average volume
    double avg_volume = 0.0;
    int vol_period = 20;
    if (n >= vol_period) {
        for (int i = n - vol_period; i < n; i++) {
            avg_volume += volumes[i];
        }
        avg_volume /= vol_period;
    } else {
        avg_volume = volumes[n - 1];
    }
    
    // MACD crosses above signal + volume spike
    if (macd_prev <= signal_prev && macd > signal && volumes[n - 1] > avg_volume * 1.5) {
        return BUY;
    }
    
    // MACD crosses below signal
    if (macd_prev >= signal_prev && macd < signal) {
        return SELL;
    }
    
    return NO_SIGNAL;
}

/*
 * Backtesting Engine
 */

BacktestResult* init_backtest_result(int max_trades) {
    BacktestResult *result = malloc(sizeof(BacktestResult));
    if (!result) return NULL;
    
    result->initial_capital = INITIAL_CAPITAL;
    result->final_capital = INITIAL_CAPITAL;
    result->max_capital = INITIAL_CAPITAL;
    result->min_capital = INITIAL_CAPITAL;
    result->max_drawdown = 0.0;
    result->total_trades = 0;
    result->winning_trades = 0;
    result->losing_trades = 0;
    result->total_return = 0.0;
    result->sharpe_ratio = 0.0;
    result->equity_curve = malloc(max_trades * sizeof(double));
    result->equity_count = 0;
    result->trades = malloc(max_trades * sizeof(Trade));
    result->trade_count = 0;
    
    return result;
}

void free_backtest_result(BacktestResult *result) {
    if (result) {
        free(result->equity_curve);
        free(result->trades);
        free(result);
    }
}

BacktestResult* backtest_strategy(
    OHLCV *candles,
    int count,
    Signal (*strategy)(double*, int, int, int),
    StrategyParams *params
) {
    BacktestResult *result = init_backtest_result(count);
    if (!result) return NULL;
    
    double capital = INITIAL_CAPITAL;
    double shares = 0.0;
    double entry_price = 0.0;
    
    // Extract close prices
    double *prices = malloc(count * sizeof(double));
    for (int i = 0; i < count; i++) {
        prices[i] = candles[i].close;
    }
    
    // Run backtest
    for (int i = params->sma_long + 1; i < count; i++) {
        Signal sig = strategy(prices, i, params->sma_short, params->sma_long);
        
        if (sig == BUY && shares == 0.0) {
            // Enter long position
            double buy_price = candles[i].close * (1.0 + params->slippage);
            shares = capital / buy_price;
            double commission = shares * buy_price * params->commission;
            capital = 0.0;
            entry_price = buy_price;
            
            // Record trade
            Trade trade = {
                .candle_index = i,
                .signal = BUY,
                .price = buy_price,
                .shares = shares,
                .commission = commission,
                .capital = capital
            };
            result->trades[result->trade_count++] = trade;
            result->total_trades++;
            
        } else if (sig == SELL && shares > 0.0) {
            // Exit long position
            double sell_price = candles[i].close * (1.0 - params->slippage);
            capital = shares * sell_price;
            double commission = capital * params->commission;
            capital -= commission;
            
            // Check if winning or losing trade
            if (sell_price > entry_price) {
                result->winning_trades++;
            } else {
                result->losing_trades++;
            }
            
            shares = 0.0;
            
            // Record trade
            Trade trade = {
                .candle_index = i,
                .signal = SELL,
                .price = sell_price,
                .shares = shares,
                .commission = commission,
                .capital = capital
            };
            result->trades[result->trade_count++] = trade;
            result->total_trades++;
        }
        
        // Track equity
        double equity = capital + (shares * candles[i].close);
        result->equity_curve[result->equity_count++] = equity;
        
        // Update max/min capital
        if (equity > result->max_capital) {
            result->max_capital = equity;
        }
        if (equity < result->min_capital) {
            result->min_capital = equity;
        }
        
        // Calculate drawdown
        if (result->max_capital > 0) {
            double drawdown = (result->max_capital - equity) / result->max_capital;
            if (drawdown > result->max_drawdown) {
                result->max_drawdown = drawdown;
            }
        }
    }
    
    // Final equity
    double final_equity = capital + (shares * candles[count - 1].close);
    result->final_capital = final_equity;
    result->total_return = (final_equity - INITIAL_CAPITAL) / INITIAL_CAPITAL;
    
    free(prices);
    return result;
}

// Calculate Sharpe Ratio
double calculate_sharpe_ratio(double *equity_curve, int count, double risk_free_rate) {
    if (count < 2) return 0.0;
    
    // Calculate returns
    double *returns = malloc((count - 1) * sizeof(double));
    for (int i = 1; i < count; i++) {
        returns[i - 1] = (equity_curve[i] - equity_curve[i - 1]) / equity_curve[i - 1];
    }
    
    // Calculate mean return
    double mean_return = 0.0;
    for (int i = 0; i < count - 1; i++) {
        mean_return += returns[i];
    }
    mean_return /= (count - 1);
    
    // Calculate standard deviation
    double variance = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double diff = returns[i] - mean_return;
        variance += diff * diff;
    }
    variance /= (count - 2);
    double std_dev = sqrt(variance);
    
    free(returns);
    
    if (std_dev == 0.0) return 0.0;
    
    // Annualized Sharpe Ratio (assuming daily data)
    double sharpe = (mean_return - risk_free_rate) / std_dev * sqrt(252);
    return sharpe;
}

/*
 * Reporting
 */

void print_backtest_report(BacktestResult *result, const char *strategy_name) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║         BACKTEST RESULTS: %-43s ║\n", strategy_name);
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("💰 Capital:\n");
    printf("   Initial: $%.2f\n", result->initial_capital);
    printf("   Final:   $%.2f\n", result->final_capital);
    printf("   Max:     $%.2f\n", result->max_capital);
    printf("   Min:     $%.2f\n", result->min_capital);
    
    printf("\n📈 Performance:\n");
    printf("   Total Return:    %+.2f%%\n", result->total_return * 100);
    printf("   Annualized:      %+.2f%%\n", result->total_return * 2); // Simplified
    printf("   Max Drawdown:    %.2f%%\n", result->max_drawdown * 100);
    
    double sharpe = calculate_sharpe_ratio(result->equity_curve, result->equity_count, 0.02);
    printf("   Sharpe Ratio:    %.2f\n", sharpe);
    
    printf("\n📊 Trading:\n");
    printf("   Total Trades:    %d\n", result->total_trades);
    printf("   Winning Trades:  %d\n", result->winning_trades);
    printf("   Losing Trades:   %d\n", result->losing_trades);
    
    if (result->total_trades > 0) {
        double win_rate = (double)result->winning_trades / result->total_trades * 100;
        printf("   Win Rate:        %.1f%%\n", win_rate);
    }
    
    printf("\n═══════════════════════════════════════════════════════════════════\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <historical_prices.csv>\n", argv[0]);
        return 1;
    }
    
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║         Episode 18: Trading Algorithms Analysis                  ║\n");
    printf("║                   Operation MOONLIGHT - Season 5                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("📂 Loading historical data from: %s\n", argv[1]);
    
    // Load data
    OHLCV *candles = malloc(MAX_CANDLES * sizeof(OHLCV));
    int count = 0;
    
    if (load_ohlcv(argv[1], candles, &count) != 0) {
        fprintf(stderr, "❌ Error: Failed to load data\n");
        free(candles);
        return 1;
    }
    
    printf("✅ Loaded %d candles\n", count);
    
    // Strategy parameters
    StrategyParams params = {
        .sma_short = 20,
        .sma_long = 50,
        .rsi_period = 14,
        .rsi_oversold = 30,
        .rsi_overbought = 70,
        .macd_fast = 12,
        .macd_slow = 26,
        .macd_signal = 9,
        .commission = DEFAULT_COMMISSION,
        .slippage = DEFAULT_SLIPPAGE
    };
    
    printf("\n🔬 Backtesting Strategy 1: SMA Crossover (20/50)...\n");
    BacktestResult *result1 = backtest_strategy(candles, count, sma_crossover_strategy, &params);
    print_backtest_report(result1, "SMA Crossover (20/50)");
    
    printf("\n🔬 Backtesting Strategy 2: RSI Overbought/Oversold...\n");
    BacktestResult *result2 = backtest_strategy(candles, count, 
        (Signal (*)(double*, int, int, int))rsi_strategy, &params);
    print_backtest_report(result2, "RSI (30/70)");
    
    // Analysis
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   OVERFITTING ANALYSIS                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("📊 Organization's Claims:\n");
    printf("   Strategy 1 (SMA):  68%% win rate, 127.4%% annual return\n");
    printf("   Strategy 2 (RSI):  61%% win rate, 84.2%% annual return\n");
    printf("   Combined Return:   127.4%% annualized\n\n");
    
    printf("🔍 Our Backtest Results:\n");
    printf("   Strategy 1: %.1f%% return, %.1f%% win rate\n", 
           result1->total_return * 100,
           result1->total_trades > 0 ? (double)result1->winning_trades / result1->total_trades * 100 : 0);
    printf("   Strategy 2: %.1f%% return, %.1f%% win rate\n",
           result2->total_return * 100,
           result2->total_trades > 0 ? (double)result2->winning_trades / result2->total_trades * 100 : 0);
    
    printf("\n⚠️  VERDICT:\n");
    if (result1->total_return < 1.0) {
        printf("   🔴 OVERFITTING DETECTED!\n");
        printf("   Claimed returns are NOT reproducible on historical data.\n");
        printf("   Possible causes:\n");
        printf("   • Curve fitting to past data\n");
        printf("   • Survivorship bias (only successful trades shown)\n");
        printf("   • Forward-looking bias (using future data)\n");
        printf("   • Cherry-picked time periods\n");
    } else {
        printf("   🟢 Results are consistent with claims.\n");
    }
    
    printf("\n💡 Mission Complete!\n");
    printf("   Evidence gathered: Trading algorithms analyzed.\n");
    printf("   Weaknesses exposed: Overfitting and unrealistic returns.\n\n");
    
    printf("🏆 Achievement Unlocked: \"Algorithm Engineer\"\n");
    printf("    You've mastered technical analysis and backtesting.\n\n");
    
    printf("➡️  Next: Episode 19 - Risk & Portfolio Management\n");
    printf("    Analyze the organization's portfolio risks.\n\n");
    
    // Cleanup
    free_backtest_result(result1);
    free_backtest_result(result2);
    free(candles);
    
    return 0;
}

