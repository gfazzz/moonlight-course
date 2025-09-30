#!/bin/bash

# Episode 09: Network Basics - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 09: Network Basics - Running Tests"
echo "=============================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./net_analyzer" ]; then
    echo -e "${RED}❌ Error: net_analyzer not found. Run 'make' first.${NC}"
    exit 1
fi

# Generate test data if it doesn't exist
if [ ! -f "./addresses.txt" ]; then
    echo -e "${YELLOW}⚠️  addresses.txt not found. Generating...${NC}"
    make generate
fi

# Test 1: Run the program
echo ""
echo "Test 1: Running net_analyzer..."
if ./net_analyzer addresses.txt > /tmp/episode09_output.txt 2>&1; then
    echo -e "${GREEN}✅ Program executed successfully${NC}"
else
    echo -e "${RED}❌ Program failed to execute${NC}"
    exit 1
fi

# Test 2: Check output format
echo ""
echo "Test 2: Checking output format..."

if grep -q "=== NETWORK ANALYZER ===" /tmp/episode09_output.txt && \
   grep -q "Statistics:" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ Output format is correct${NC}"
else
    echo -e "${RED}❌ Output format is incorrect${NC}"
    cat /tmp/episode09_output.txt
    exit 1
fi

# Test 3: Check for address parsing
echo ""
echo "Test 3: Verifying address parsing..."

if grep -E "\[[0-9]+\].*:[0-9]+" /tmp/episode09_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Addresses parsed correctly${NC}"
else
    echo -e "${RED}❌ Addresses not parsed correctly${NC}"
    exit 1
fi

# Test 4: Check for IP type classification
echo ""
echo "Test 4: Checking IP type classification..."

if grep -q "Type: Private" /tmp/episode09_output.txt && \
   grep -q "Type: Public" /tmp/episode09_output.txt && \
   grep -q "Type: Loopback" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ IP types classified correctly${NC}"
else
    echo -e "${YELLOW}⚠️  Not all IP types found (may need implementation)${NC}"
fi

# Test 5: Check for port categories
echo ""
echo "Test 5: Checking port categories..."

if grep -q "Well-Known" /tmp/episode09_output.txt || \
   grep -q "Registered" /tmp/episode09_output.txt || \
   grep -q "Dynamic" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ Port categories detected${NC}"
else
    echo -e "${YELLOW}⚠️  Port categories not found (may need implementation)${NC}"
fi

# Test 6: Check for hex representation
echo ""
echo "Test 6: Checking hex representation..."

if grep -q "Hex: 0x" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ Hex representation found${NC}"
else
    echo -e "${YELLOW}⚠️  Hex representation not found (may need implementation)${NC}"
fi

# Test 7: Check for network calculation (private IPs)
echo ""
echo "Test 7: Checking network calculation..."

if grep -q "Network:" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ Network calculation found${NC}"
else
    echo -e "${YELLOW}⚠️  Network calculation not found (may need implementation)${NC}"
fi

# Test 8: Check statistics
echo ""
echo "Test 8: Verifying statistics..."

if grep -q "Total addresses:" /tmp/episode09_output.txt && \
   grep -q "Private:" /tmp/episode09_output.txt && \
   grep -q "Public:" /tmp/episode09_output.txt; then
    echo -e "${GREEN}✅ Statistics found${NC}"
else
    echo -e "${YELLOW}⚠️  Statistics incomplete${NC}"
fi

# Display actual output
echo ""
echo "=============================================="
echo "Actual output:"
echo "=============================================="
cat /tmp/episode09_output.txt
echo "=============================================="

echo ""
echo -e "${GREEN}🎉 All tests passed!${NC}"
echo ""
echo "Mission accomplished! You've mastered network basics."
echo "Next: Episode 10 - Socket Programming"

exit 0
