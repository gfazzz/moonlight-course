#!/bin/bash

# Episode 18: Trading Algorithms - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 18: Trading Algorithms - Running Tests"
echo "=================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if binary exists
if [ ! -f "./strategy_tester" ]; then
    echo -e "${RED}❌ Error: strategy_tester not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ strategy_tester compiled successfully${NC}"

# Test 1: Run backtester
echo ""
echo "Test 1: Running strategy backtester..."
./strategy_tester artifacts/historical_prices.csv > /tmp/episode18_output.txt 2>&1 || true

if [ -s /tmp/episode18_output.txt ]; then
    echo -e "${GREEN}✅ Program executed${NC}"
else
    echo -e "${YELLOW}⚠️  No output generated${NC}"
fi

# Test 2: Check for SMA implementation
echo ""
echo "Test 2: Checking SMA implementation..."
if [ -f "starter.c" ]; then
    if grep -q "calculate.*sma\|simple.*moving.*average" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ SMA code found${NC}"
    else
        echo -e "${YELLOW}⚠️  SMA needs implementation${NC}"
    fi
fi

# Test 3: Check for RSI implementation
echo ""
echo "Test 3: Checking RSI implementation..."
if [ -f "starter.c" ]; then
    if grep -q "calculate.*rsi\|relative.*strength" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ RSI code found${NC}"
    else
        echo -e "${YELLOW}⚠️  RSI needs implementation${NC}"
    fi
fi

# Test 4: Check for MACD implementation
echo ""
echo "Test 4: Checking MACD implementation..."
if [ -f "starter.c" ]; then
    if grep -q "calculate.*macd\|moving.*average.*convergence" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ MACD code found${NC}"
    else
        echo -e "${YELLOW}⚠️  MACD needs implementation${NC}"
    fi
fi

# Test 5: Check for backtesting engine
echo ""
echo "Test 5: Checking backtesting engine..."
if grep -qi "backtest\|BacktestResult\|strategy.*test" /tmp/episode18_output.txt; then
    echo -e "${GREEN}✅ Backtesting found${NC}"
else
    echo -e "${YELLOW}⚠️  Backtesting needs implementation${NC}"
fi

# Test 6: Check for performance metrics
echo ""
echo "Test 6: Checking performance metrics..."
if grep -qi "sharpe.*ratio\|return\|drawdown\|win.*rate" /tmp/episode18_output.txt; then
    echo -e "${GREEN}✅ Performance metrics found${NC}"
else
    echo -e "${YELLOW}⚠️  Performance metrics need implementation${NC}"
fi

# Test 7: Check for overfitting detection
echo ""
echo "Test 7: Checking overfitting detection..."
if grep -E "127.*%|overfitting|in.*sample|out.*sample" /tmp/episode18_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Overfitting analysis found${NC}"
else
    echo -e "${YELLOW}⚠️  Overfitting detection needs implementation${NC}"
fi

# Display output
echo ""
echo "=================================================="
echo "Actual output:"
echo "=================================================="
cat /tmp/episode18_output.txt 2>/dev/null | head -40 || echo "(no output)"
echo "=================================================="

# Summary
echo ""
echo "=================================================="
echo -e "${BLUE}📝 Implementation checklist:${NC}"
echo "=================================================="
echo ""
echo "Technical Indicators:"
echo "  ☐ SMA (Simple Moving Average): sum/period"
echo "  ☐ EMA (Exponential MA): smoothing factor"
echo "  ☐ RSI (Relative Strength Index): 0-100 scale"
echo "  ☐ MACD: EMA(12) - EMA(26), Signal: EMA(MACD, 9)"
echo ""
echo "Trading Strategies:"
echo "  ☐ Golden Cross: SMA(20) crosses above SMA(50) → BUY"
echo "  ☐ Death Cross: SMA(20) crosses below SMA(50) → SELL"
echo "  ☐ RSI Strategy: RSI < 30 → BUY, RSI > 70 → SELL"
echo "  ☐ MACD Strategy: MACD crosses signal line"
echo ""
echo "Backtesting Engine:"
echo "  ☐ Process historical OHLCV data"
echo "  ☐ Generate BUY/SELL signals"
echo "  ☐ Simulate trades with commission"
echo "  ☐ Track equity curve"
echo "  ☐ Calculate max drawdown"
echo ""
echo "Performance Metrics:"
echo "  ☐ Total Return (%)"
echo "  ☐ Sharpe Ratio: (Return - RF) / StdDev"
echo "  ☐ Max Drawdown: largest peak-to-trough decline"
echo "  ☐ Win Rate: % of profitable trades"
echo ""
echo "Mission:"
echo "  ☐ Backtest 3 strategies of organization"
echo "  ☐ Expose overfitting (127% → 18%)"
echo "  ☐ Prove fraud with data"
echo ""
echo "=================================================="
echo ""
echo -e "${GREEN}🎉 Tests completed!${NC}"
echo ""
echo "🏆 Achievement: Algorithm Engineer"
echo ""
echo "Next: Episode 19 - Risk & Portfolio"

exit 0
