# Mission 17: Market Microstructure Analysis

## 🎯 Objective

Analyze OHLCV (Open, High, Low, Close, Volume) data from 47 suspicious tickers to detect market manipulation patterns.

## 📋 Mission Briefing

**Date:** December 18, 03:15  
**Location:** Your apartment, Arbat district, Moscow  
**Status:** URGENT

V. has provided you with encrypted market data on a USB drive. The data contains 6 months of trading history for 47 tickers that show suspicious patterns.

**Your mission:**
1. Parse OHLCV data from CSV files
2. Calculate bid-ask spreads and detect anomalies
3. Analyze market depth and liquidity
4. Identify manipulation patterns (volume spikes, price distortions)
5. Generate a report on suspicious activities

**Time limit:** 15 hours 30 minutes (until 18:45)

## 📂 Files Provided

- `artifacts/market_data.csv` — OHLCV data for 47 tickers
- `artifacts/suspicious_tickers.txt` — List of tickers to focus on

## ✅ Success Criteria

Your implementation must:
- Parse CSV with OHLCV data correctly
- Calculate bid-ask spread for each candle
- Detect volume spikes (>3x average)
- Calculate volatility (standard deviation of returns)
- Identify at least 5 manipulation patterns

## 🔍 Expected Output

```
=== Episode 17: Market Microstructure Analysis ===

Loaded 47 tickers

=== TICKER_A Statistics ===
Candles: 120
Avg Volume: 1,234,567
Volatility: 2.34%
Volume Spikes (>3x avg): 7
Suspicious patterns detected!

...

=== Analysis Complete ===
Total anomalies found: 23
Estimated manipulation profit: $2.7M
```

## 💡 Hints

- Use `fscanf()` or `fgets()` to parse CSV
- Bid-ask spread approximation: `(high - low) / close * 100`
- Volume spike: current volume > 3x average volume
- Volatility: standard deviation of daily returns

## 🏆 Achievement

**"Market Analyst"** — Successfully detect market manipulation patterns

---

**Next:** [Episode 18: Trading Algorithms](../episode-18-trading-algorithms/)

