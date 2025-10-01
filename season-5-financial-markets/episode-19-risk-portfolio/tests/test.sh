#!/bin/bash

# Episode 19: Risk & Portfolio Management - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 19: Risk & Portfolio Management - Running Tests"
echo "==========================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if binary exists
if [ ! -f "./portfolio_manager" ]; then
    echo -e "${RED}❌ Error: portfolio_manager not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ portfolio_manager compiled successfully${NC}"

# Test 1: Run portfolio analyzer
echo ""
echo "Test 1: Running portfolio analyzer..."
./portfolio_manager artifacts/portfolio_holdings.csv > /tmp/episode19_output.txt 2>&1 || true

if [ -s /tmp/episode19_output.txt ]; then
    echo -e "${GREEN}✅ Program executed${NC}"
else
    echo -e "${YELLOW}⚠️  No output generated${NC}"
fi

# Test 2: Check for VaR calculation
echo ""
echo "Test 2: Checking VaR calculation..."
if [ -f "starter.c" ]; then
    VAR_COUNT=$(grep -c "var\|VaR\|value.*at.*risk" starter.c 2>/dev/null || echo "0")
    if [ "$VAR_COUNT" -gt 0 ]; then
        echo -e "${GREEN}✅ VaR code found ($VAR_COUNT references)${NC}"
    else
        echo -e "${YELLOW}⚠️  VaR needs implementation${NC}"
    fi
fi

# Test 3: Check for Sharpe Ratio
echo ""
echo "Test 3: Checking Sharpe Ratio calculation..."
if grep -qi "sharpe.*ratio" /tmp/episode19_output.txt; then
    echo -e "${GREEN}✅ Sharpe Ratio found in output${NC}"
else
    echo -e "${YELLOW}⚠️  Sharpe Ratio needs implementation${NC}"
fi

# Test 4: Check for correlation matrix
echo ""
echo "Test 4: Checking correlation matrix..."
if grep -qi "correlation\|correlation.*matrix" /tmp/episode19_output.txt; then
    echo -e "${GREEN}✅ Correlation analysis found${NC}"
else
    echo -e "${YELLOW}⚠️  Correlation matrix needs implementation${NC}"
fi

# Test 5: Check for Monte Carlo simulation
echo ""
echo "Test 5: Checking Monte Carlo simulation..."
if [ -f "starter.c" ]; then
    if grep -qi "monte.*carlo\|simulation\|random.*normal" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Monte Carlo code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Monte Carlo simulation needs implementation${NC}"
    fi
fi

# Test 6: Check for portfolio optimization
echo ""
echo "Test 6: Checking portfolio optimization..."
if grep -qi "optimization\|markowitz\|efficient.*frontier" /tmp/episode19_output.txt; then
    echo -e "${GREEN}✅ Portfolio optimization found${NC}"
else
    echo -e "${YELLOW}⚠️  Portfolio optimization needs implementation${NC}"
fi

# Test 7: Check for stress testing
echo ""
echo "Test 7: Checking stress testing..."
if grep -qi "stress.*test\|2008\|2020\|crash\|scenario" /tmp/episode19_output.txt; then
    echo -e "${GREEN}✅ Stress testing found${NC}"
else
    echo -e "${YELLOW}⚠️  Stress testing needs implementation${NC}"
fi

# Display output
echo ""
echo "==========================================================="
echo "Actual output:"
echo "==========================================================="
cat /tmp/episode19_output.txt 2>/dev/null | head -50 || echo "(no output)"
echo "==========================================================="

# Summary
echo ""
echo "==========================================================="
echo -e "${BLUE}📝 Implementation checklist:${NC}"
echo "==========================================================="
echo ""
echo "Value at Risk (VaR):"
echo "  ☐ Historical VaR: sort returns, find 5th percentile"
echo "  ☐ Parametric VaR: mean + z*std_dev (z=-1.645 for 95%)"
echo "  ☐ Monte Carlo VaR: simulate 10,000 scenarios"
echo "  ☐ Report VaR(95%, 1 day) in dollars"
echo ""
echo "Risk Metrics:"
echo "  ☐ Sharpe Ratio: (Return - RF) / Volatility"
echo "    - Annualize: return*252, vol*sqrt(252)"
echo "  ☐ Sortino Ratio: use downside deviation only"
echo "  ☐ Max Drawdown: largest peak-to-trough decline"
echo "  ☐ Beta: correlation * (vol_portfolio / vol_market)"
echo ""
echo "Correlation Analysis:"
echo "  ☐ Build correlation matrix (n x n)"
echo "  ☐ Pearson correlation: cov(X,Y) / (std(X)*std(Y))"
echo "  ☐ Identify high correlations (> 0.8)"
echo "  ☐ Check diversification quality"
echo ""
echo "Portfolio Optimization (Markowitz):"
echo "  ☐ Calculate expected returns per asset"
echo "  ☐ Build covariance matrix"
echo "  ☐ Minimize: w^T * Cov * w (portfolio variance)"
echo "  ☐ Constraint: sum(weights) = 1"
echo "  ☐ Build efficient frontier"
echo ""
echo "Stress Testing:"
echo "  ☐ 2008 scenario: apply historical crash returns"
echo "  ☐ 2020 COVID scenario: -34% in March"
echo "  ☐ Flash Crash 2010: -9% in minutes"
echo "  ☐ Calculate portfolio loss in each scenario"
echo ""
echo "Mission:"
echo "  ☐ Analyze $50M portfolio"
echo "  ☐ Detect critical risk ($15.6M VaR)"
echo "  ☐ Optimize to reduce VaR by 60%+"
echo "  ☐ Save portfolio from disaster"
echo ""
echo "==========================================================="
echo ""
echo -e "${GREEN}🎉 Tests completed!${NC}"
echo ""
echo "🏆 Achievement: Risk Master"
echo ""
echo "Next: Episode 20 - Crypto & DeFi (FINALE)"

exit 0
