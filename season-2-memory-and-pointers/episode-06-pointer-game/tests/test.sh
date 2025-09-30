#!/bin/bash

# Episode 06: Pointer Game - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 06: Pointer Game - Running Tests"
echo "==========================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./pointer_navigator" ]; then
    echo -e "${RED}❌ Error: pointer_navigator not found. Run 'make' first.${NC}"
    exit 1
fi

# Generate test data if it doesn't exist
if [ ! -f "./pointers.dat" ]; then
    echo -e "${YELLOW}⚠️  pointers.dat not found. Generating...${NC}"
    make generate
fi

# Test 1: Run the program
echo ""
echo "Test 1: Running pointer_navigator..."
if ./pointer_navigator pointers.dat > /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ Program executed successfully${NC}"
else
    echo -e "${RED}❌ Program failed to execute${NC}"
    exit 1
fi

# Test 2: Check output format
echo ""
echo "Test 2: Checking output format..."

if grep -q "=== POINTER NAVIGATOR ===" /tmp/episode06_output.txt && \
   grep -q "Starting at offset:" /tmp/episode06_output.txt && \
   grep -q "Decoded message:" /tmp/episode06_output.txt && \
   grep -q "Total entries traversed:" /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ Output format is correct${NC}"
else
    echo -e "${RED}❌ Output format is incorrect${NC}"
    cat /tmp/episode06_output.txt
    exit 1
fi

# Test 3: Check for entry format [0xXXXX] Value: 0xXX → Next: 0xXXXX
echo ""
echo "Test 3: Verifying entry format..."

if grep -E "\[0x[0-9A-F]{4}\] Value: 0x[0-9A-F]{2}" /tmp/episode06_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Entry format is correct${NC}"
else
    echo -e "${RED}❌ Entry format is incorrect${NC}"
    echo "Expected format: [0xXXXX] Value: 0xXX → Next: 0xXXXX"
    exit 1
fi

# Test 4: Check for decoded message (should contain "MOONLIGHT")
echo ""
echo "Test 4: Checking decoded message..."

if grep -q "MOONLIGHT" /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ Message decoded correctly${NC}"
else
    echo -e "${RED}❌ Message not decoded correctly${NC}"
    echo "Expected 'MOONLIGHT' in decoded message"
    grep "Decoded message:" /tmp/episode06_output.txt
    exit 1
fi

# Test 5: Verify character display (' ')
echo ""
echo "Test 5: Checking character display..."

if grep -q "('M')" /tmp/episode06_output.txt && \
   grep -q "('O')" /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ Characters displayed correctly${NC}"
else
    echo -e "${RED}❌ Characters not displayed correctly${NC}"
    exit 1
fi

# Test 6: Check for END marker
echo ""
echo "Test 6: Checking END marker..."

if grep -q "(END)" /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ END marker found${NC}"
else
    echo -e "${RED}❌ END marker not found${NC}"
    exit 1
fi

# Test 7: Verify path taken is displayed
echo ""
echo "Test 7: Checking path display..."

if grep -q "Path taken:" /tmp/episode06_output.txt; then
    echo -e "${GREEN}✅ Path display found${NC}"
else
    echo -e "${YELLOW}⚠️  Path display not found (optional feature)${NC}"
fi

# Test 8: Count traversed entries (should be 16 for "MOONLIGHT.ACTIVE")
echo ""
echo "Test 8: Verifying entry count..."

TRAVERSED=$(grep "Total entries traversed:" /tmp/episode06_output.txt | grep -oE "[0-9]+")
if [ "$TRAVERSED" -eq 16 ]; then
    echo -e "${GREEN}✅ Correct number of entries traversed: $TRAVERSED${NC}"
elif [ "$TRAVERSED" -gt 0 ]; then
    echo -e "${YELLOW}⚠️  Traversed $TRAVERSED entries (expected 16)${NC}"
else
    echo -e "${RED}❌ No entries traversed${NC}"
    exit 1
fi

# Display actual output
echo ""
echo "==========================================="
echo "Actual output:"
echo "==========================================="
cat /tmp/episode06_output.txt
echo "==========================================="

echo ""
echo -e "${GREEN}🎉 All tests passed!${NC}"
echo ""
echo "Mission accomplished! You've mastered pointer navigation."
echo "Next: Episode 07 - Deep Dive (Dynamic Memory)"

exit 0
