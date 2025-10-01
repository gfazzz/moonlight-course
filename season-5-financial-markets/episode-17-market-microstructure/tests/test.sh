#!/bin/bash

# Episode 17: Market Microstructure - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 17: Market Microstructure - Running Tests"
echo "====================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if binary exists
if [ ! -f "./market_analyzer" ]; then
    echo -e "${RED}❌ Error: market_analyzer not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ market_analyzer compiled successfully${NC}"

# Generate test data if needed
if [ ! -f "./artifacts/market_data.csv" ]; then
    echo -e "${YELLOW}⚠️  market_data.csv not found. Generating...${NC}"
    make generate 2>/dev/null || echo -e "${YELLOW}? No generate target${NC}"
fi

# Test 1: Basic CSV parsing
echo ""
echo "Test 1: CSV Parsing..."
if ./market_analyzer artifacts/market_data.csv > /tmp/episode17_output.txt 2>&1; then
    echo -e "${GREEN}✅ CSV parsing works${NC}"
else
    echo -e "${RED}❌ CSV parsing failed${NC}"
    cat /tmp/episode17_output.txt
    exit 1
fi

# Test 2: Check output format
echo ""
echo "Test 2: Output Format..."
if grep -q "=== MARKET MICROSTRUCTURE ===\|Statistics\|OHLCV" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Output format correct${NC}"
else
    echo -e "${YELLOW}⚠️  Output format unclear${NC}"
fi

# Test 3: OHLCV parsing
echo ""
echo "Test 3: OHLCV Data Parsing..."
if grep -E "Open|High|Low|Close|Volume" /tmp/episode17_output.txt > /dev/null; then
    echo -e "${GREEN}✅ OHLCV fields found${NC}"
else
    echo -e "${YELLOW}⚠️  OHLCV parsing needs implementation${NC}"
fi

# Test 4: Bid-Ask spread calculation
echo ""
echo "Test 4: Bid-Ask Spread Calculation..."
if grep -qi "spread\|bid.*ask" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Spread calculation found${NC}"
else
    echo -e "${YELLOW}⚠️  Spread calculation needs implementation${NC}"
fi

# Test 5: Volume analysis
echo ""
echo "Test 5: Volume Analysis..."
if grep -qi "volume\|avg.*volume\|volume.*spike" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Volume analysis found${NC}"
else
    echo -e "${YELLOW}⚠️  Volume analysis needs implementation${NC}"
fi

# Test 6: Anomaly detection
echo ""
echo "Test 6: Anomaly Detection..."
if grep -qi "anomal\|spike\|suspicious\|manipulation" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Anomaly detection implemented${NC}"
else
    echo -e "${YELLOW}⚠️  Anomaly detection needs implementation${NC}"
fi

# Test 7: Ticker statistics
echo ""
echo "Test 7: Ticker Statistics..."
if grep -qi "ticker\|symbol.*:" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Ticker statistics found${NC}"
else
    echo -e "${YELLOW}⚠️  Ticker statistics need implementation${NC}"
fi

# Test 8: Volatility calculation
echo ""
echo "Test 8: Volatility Calculation..."
if grep -qi "volatility\|std.*dev" /tmp/episode17_output.txt; then
    echo -e "${GREEN}✅ Volatility calculation found${NC}"
else
    echo -e "${YELLOW}⚠️  Volatility needs implementation${NC}"
fi

# Display output
echo ""
echo "====================================================="
echo "Actual output:"
echo "====================================================="
cat /tmp/episode17_output.txt 2>/dev/null | head -40 || echo "(no output)"
echo "====================================================="

# Summary
echo ""
echo "====================================================="
echo -e "${BLUE}📝 Implementation checklist:${NC}"
echo "====================================================="
echo ""
echo "OHLCV Analysis:"
echo "  ☐ Parse CSV (TICKER,TIMESTAMP,OPEN,HIGH,LOW,CLOSE,VOLUME)"
echo "  ☐ Store in OHLCV struct array"
echo "  ☐ Calculate basic statistics per ticker"
echo ""
echo "Market Metrics:"
echo "  ☐ Bid-Ask Spread: (High - Low) / Close * 100%"
echo "  ☐ Average Volume per ticker"
echo "  ☐ Volatility: std deviation of returns"
echo "  ☐ Daily return: (Close - Open) / Open"
echo ""
echo "Anomaly Detection:"
echo "  ☐ Volume spikes (> 3x average)"
echo "  ☐ Price jumps (> 5% in single candle)"
echo "  ☐ Unusual spreads (> 2% average)"
echo "  ☐ Count anomalies per ticker"
echo ""
echo "Mission:"
echo "  ☐ Analyze 47 suspicious tickers"
echo "  ☐ Find manipulation patterns"
echo "  ☐ Estimate manipulation profit (~$2.7M)"
echo ""
echo "====================================================="
echo ""
echo -e "${GREEN}🎉 Tests completed!${NC}"
echo ""
echo "🏆 Achievement: Market Analyst"
echo ""
echo "Next: Episode 18 - Trading Algorithms"

exit 0
