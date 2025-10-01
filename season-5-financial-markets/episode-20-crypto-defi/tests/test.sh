#!/bin/bash

# Episode 20: Crypto & DeFi - Test Script
# SEASON 5 FINALE

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 20: Crypto & DeFi - Running Tests"
echo "============================================="
echo "🎯 SEASON 5 FINALE"
echo "============================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
NC='\033[0m'

# Check if binary exists
if [ ! -f "./crypto_analyzer" ]; then
    echo -e "${RED}❌ Error: crypto_analyzer not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ crypto_analyzer compiled successfully${NC}"

# Test 1: Run crypto analyzer
echo ""
echo "Test 1: Running crypto analyzer..."
./crypto_analyzer artifacts/pump_dump_history.csv artifacts/arbitrage_logs.csv > /tmp/episode20_output.txt 2>&1 || true

if [ -s /tmp/episode20_output.txt ]; then
    echo -e "${GREEN}✅ Program executed${NC}"
else
    echo -e "${YELLOW}⚠️  No output generated${NC}"
fi

# Test 2: Check for pump & dump detection
echo ""
echo "Test 2: Checking pump & dump detection..."
if [ -f "starter.c" ]; then
    if grep -q "pump.*dump\|detect.*pump\|volume.*spike" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Pump & dump detection code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Pump & dump detection needs implementation${NC}"
    fi
fi

# Test 3: Check for arbitrage analysis
echo ""
echo "Test 3: Checking arbitrage analysis..."
if grep -qi "arbitrage\|spread\|exchange" /tmp/episode20_output.txt; then
    echo -e "${GREEN}✅ Arbitrage analysis found${NC}"
else
    echo -e "${YELLOW}⚠️  Arbitrage analysis needs implementation${NC}"
fi

# Test 4: Check for DeFi transaction tracing
echo ""
echo "Test 4: Checking DeFi transaction tracing..."
if grep -qi "defi\|tornado.*cash\|uniswap\|mixer\|liquidity.*pool" /tmp/episode20_output.txt; then
    echo -e "${GREEN}✅ DeFi tracing found${NC}"
else
    echo -e "${YELLOW}⚠️  DeFi tracing needs implementation${NC}"
fi

# Test 5: Check for total scheme calculation
echo ""
echo "Test 5: Checking total scheme calculation..."
if grep -E "\$[0-9,]+.*million|\$10.*[Mm]|TOTAL.*\$" /tmp/episode20_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Total profit calculation found${NC}"
else
    echo -e "${YELLOW}⚠️  Total calculation needs implementation${NC}"
fi

# Test 6: Check for wallet address tracking
echo ""
echo "Test 6: Checking wallet address tracking..."
if grep -E "0x[0-9A-Fa-f]{4}|wallet.*address|address.*identified" /tmp/episode20_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Wallet tracking found${NC}"
else
    echo -e "${YELLOW}⚠️  Wallet tracking needs implementation${NC}"
fi

# Test 7: Check for evidence report
echo ""
echo "Test 7: Checking evidence report generation..."
if grep -qi "evidence\|europol\|scheme.*exposed\|complete" /tmp/episode20_output.txt; then
    echo -e "${GREEN}✅ Evidence report found${NC}"
else
    echo -e "${YELLOW}⚠️  Evidence report needs implementation${NC}"
fi

# Display output
echo ""
echo "============================================="
echo "Actual output:"
echo "============================================="
cat /tmp/episode20_output.txt 2>/dev/null || echo "(no output)"
echo "============================================="

# Final summary
echo ""
echo "============================================="
echo -e "${MAGENTA}🎯 SEASON 5 FINALE CHECKLIST:${NC}"
echo "============================================="
echo ""
echo "Episode 17 Skills: Market Microstructure"
echo "  ☐ OHLCV data analysis"
echo ""
echo "Episode 18 Skills: Trading Algorithms"
echo "  ☐ Technical indicators"
echo "  ☐ Backtesting framework"
echo ""
echo "Episode 19 Skills: Risk Management"
echo "  ☐ Statistical analysis"
echo "  ☐ Portfolio math"
echo ""
echo "Episode 20 Skills: Crypto Analysis"
echo ""
echo "1. Pump & Dump Detection:"
echo "  ☐ Parse pump_dump_history.csv (15 altcoins)"
echo "  ☐ Detect: volume spike (>5x), price jump (>50%), crash (>80%)"
echo "  ☐ Calculate profit per scheme"
echo "  ☐ Total pump & dump profit: $4.2M"
echo ""
echo "2. Arbitrage Analysis:"
echo "  ☐ Parse arbitrage_logs.csv (847 trades)"
echo "  ☐ Calculate spread: (price2-price1)/price1 * 100"
echo "  ☐ Find profitable opportunities"
echo "  ☐ Total arbitrage profit: $2.8M"
echo ""
echo "3. DeFi Transaction Tracing:"
echo "  ☐ Parse defi_transactions.json"
echo "  ☐ Track Tornado Cash deposits (47 total)"
echo "  ☐ Analyze Uniswap swaps (234 total)"
echo "  ☐ Trace liquidity pool manipulation"
echo "  ☐ Total DeFi profit: $3.1M"
echo ""
echo "4. Evidence Package:"
echo "  ☐ GRAND TOTAL: $10,247,832"
echo "  ☐ Identify 23 wallet addresses"
echo "  ☐ Document blockchain trail"
echo "  ☐ Generate report for Europol"
echo ""
echo "============================================="
echo ""
if grep -q "10,247,832\|SEASON 5 COMPLETE" /tmp/episode20_output.txt 2>/dev/null; then
    echo -e "${MAGENTA}╔═══════════════════════════════════════════╗${NC}"
    echo -e "${MAGENTA}║  🎉 SEASON 5 COMPLETE! 🎉                 ║${NC}"
    echo -e "${MAGENTA}║  $10M+ crypto scheme EXPOSED               ║${NC}"
    echo -e "${MAGENTA}║  Evidence package ready                   ║${NC}"
    echo -e "${MAGENTA}╚═══════════════════════════════════════════╝${NC}"
else
    echo -e "${GREEN}🎉 Compilation successful!${NC}"
fi
echo ""
echo "🏆 Achievement: Crypto Detective"
echo "🏆 Achievement: Season 5 Master"
echo ""
echo -e "${BLUE}⚠️  WARNING: Organization now knows about you...${NC}"
echo ""
echo "Next: Season 6 - Embedded & IoT (They're watching)"

exit 0
