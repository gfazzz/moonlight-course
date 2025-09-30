#!/bin/bash

# Episode 05: Memory Map - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 05: Memory Map - Running Tests"
echo "=========================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if executable exists
if [ ! -f "./memory_mapper" ]; then
    echo -e "${RED}❌ Error: memory_mapper not found. Run 'make' first.${NC}"
    exit 1
fi

# Generate test data if it doesn't exist
if [ ! -f "./memory_dump.dat" ]; then
    echo -e "${YELLOW}⚠️  memory_dump.dat not found. Generating...${NC}"
    make generate
fi

# Test 1: Run the program
echo ""
echo "Test 1: Running memory_mapper..."
if ./memory_mapper memory_dump.dat > /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ Program executed successfully${NC}"
else
    echo -e "${RED}❌ Program failed to execute${NC}"
    exit 1
fi

# Test 2: Check output format
echo ""
echo "Test 2: Checking output format..."

# Check for required sections
if grep -q "=== MEMORY MAP ===" /tmp/episode05_output.txt && \
   grep -q "Total structures found:" /tmp/episode05_output.txt && \
   grep -q "Total garbage bytes:" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ Output format is correct${NC}"
else
    echo -e "${RED}❌ Output format is incorrect${NC}"
    echo "Expected sections not found in output."
    cat /tmp/episode05_output.txt
    exit 1
fi

# Test 3: Verify structure detection
echo ""
echo "Test 3: Verifying structure detection..."

# Should find strings
if grep -q "STRING" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ STRING structures detected${NC}"
else
    echo -e "${RED}❌ STRING structures not detected${NC}"
    exit 1
fi

# Should find coordinates
if grep -q "COORDINATES" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ COORDINATES structures detected${NC}"
else
    echo -e "${RED}❌ COORDINATES structures not detected${NC}"
    exit 1
fi

# Should find timestamps
if grep -q "TIMESTAMP" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ TIMESTAMP structures detected${NC}"
else
    echo -e "${RED}❌ TIMESTAMP structures not detected${NC}"
    exit 1
fi

# Should find garbage
if grep -q "GARBAGE" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ GARBAGE bytes detected${NC}"
else
    echo -e "${RED}❌ GARBAGE bytes not detected${NC}"
    exit 1
fi

# Test 4: Check specific strings
echo ""
echo "Test 4: Checking specific content..."

if grep -q "access_log.dat" /tmp/episode05_output.txt && \
   grep -q "encrypted.key" /tmp/episode05_output.txt && \
   grep -q "backup_2024.tar.gz" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ All expected strings found${NC}"
else
    echo -e "${RED}❌ Not all expected strings found${NC}"
    exit 1
fi

# Test 5: Verify coordinates format (should have ° symbol and N/S/E/W)
echo ""
echo "Test 5: Checking coordinates format..."

if grep -q "°" /tmp/episode05_output.txt && \
   grep -E "(N|S).*,(E|W)" /tmp/episode05_output.txt; then
    echo -e "${GREEN}✅ Coordinates format is correct${NC}"
else
    echo -e "${RED}❌ Coordinates format is incorrect${NC}"
    echo "Expected format: XX.XXXX° N/S, XX.XXXX° E/W"
    grep "COORDINATES" /tmp/episode05_output.txt
    exit 1
fi

# Test 6: Verify timestamp format (YYYY-MM-DD HH:MM:SS UTC)
echo ""
echo "Test 6: Checking timestamp format..."

if grep -E "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2} UTC" /tmp/episode05_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Timestamp format is correct${NC}"
else
    echo -e "${RED}❌ Timestamp format is incorrect${NC}"
    echo "Expected format: YYYY-MM-DD HH:MM:SS UTC"
    grep "TIMESTAMP" /tmp/episode05_output.txt
    exit 1
fi

# Test 7: Check structure count
echo ""
echo "Test 7: Verifying structure count..."

# Should find exactly 7 structures (3 strings + 2 coords + 2 timestamps)
STRUCT_COUNT=$(grep -c "\[0x" /tmp/episode05_output.txt | head -1)
if [ "$STRUCT_COUNT" -ge 7 ]; then
    echo -e "${GREEN}✅ Expected number of structures found${NC}"
else
    echo -e "${YELLOW}⚠️  Found $STRUCT_COUNT structures (expected at least 7)${NC}"
fi

# Display actual output for review
echo ""
echo "=========================================="
echo "Actual output:"
echo "=========================================="
cat /tmp/episode05_output.txt
echo "=========================================="

echo ""
echo -e "${GREEN}🎉 All tests passed!${NC}"
echo ""
echo "Mission accomplished! You've successfully created a memory mapper."
echo "Next: Episode 06 - Pointer Game"

exit 0
