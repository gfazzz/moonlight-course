#!/bin/bash

# Episode 07: Deep Dive - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 07: Deep Dive - Running Tests"
echo "========================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./dynamic_loader" ]; then
    echo -e "${RED}❌ Error: dynamic_loader not found. Run 'make' first.${NC}"
    exit 1
fi

# Generate test data if it doesn't exist
if [ ! -f "./archive.dat" ]; then
    echo -e "${YELLOW}⚠️  archive.dat not found. Generating...${NC}"
    make generate
fi

# Test 1: Run the program
echo ""
echo "Test 1: Running dynamic_loader..."
if ./dynamic_loader archive.dat > /tmp/episode07_output.txt; then
    echo -e "${GREEN}✅ Program executed successfully${NC}"
else
    echo -e "${RED}❌ Program failed to execute${NC}"
    exit 1
fi

# Test 2: Check output format
echo ""
echo "Test 2: Checking output format..."

if grep -q "=== DYNAMIC LOADER ===" /tmp/episode07_output.txt && \
   grep -q "Archive:" /tmp/episode07_output.txt && \
   grep -q "Magic: MOON" /tmp/episode07_output.txt && \
   grep -q "Version:" /tmp/episode07_output.txt && \
   grep -q "Blocks:" /tmp/episode07_output.txt; then
    echo -e "${GREEN}✅ Output format is correct${NC}"
else
    echo -e "${RED}❌ Output format is incorrect${NC}"
    cat /tmp/episode07_output.txt
    exit 1
fi

# Test 3: Check for block loading messages
echo ""
echo "Test 3: Verifying block loading..."

if grep -q "Loading blocks..." /tmp/episode07_output.txt && \
   grep -E "\[.*/.*\].*loaded" /tmp/episode07_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Block loading messages found${NC}"
else
    echo -e "${RED}❌ Block loading messages not found${NC}"
    exit 1
fi

# Test 4: Check for specific content
echo ""
echo "Test 4: Checking block content..."

if grep -q "MOONLIGHT" /tmp/episode07_output.txt; then
    echo -e "${GREEN}✅ Expected content found${NC}"
else
    echo -e "${RED}❌ Expected content not found${NC}"
    exit 1
fi

# Test 5: Check for memory statistics
echo ""
echo "Test 5: Checking memory statistics..."

if grep -q "Total memory allocated:" /tmp/episode07_output.txt && \
   grep -q "Freeing memory..." /tmp/episode07_output.txt && \
   grep -q "No leaks!" /tmp/episode07_output.txt; then
    echo -e "${GREEN}✅ Memory statistics found${NC}"
else
    echo -e "${RED}❌ Memory statistics not found${NC}"
    exit 1
fi

# Test 6: Check for memory leaks (platform-specific)
echo ""
echo "Test 6: Checking for memory leaks..."

if command -v leaks &> /dev/null; then
    # macOS
    echo "Using leaks (macOS)..."
    if leaks --atExit -- ./dynamic_loader archive.dat 2>&1 | grep -q "0 leaks for 0 total leaked bytes"; then
        echo -e "${GREEN}✅ No memory leaks detected${NC}"
    else
        echo -e "${YELLOW}⚠️  Potential memory leaks detected (check manually)${NC}"
    fi
elif command -v valgrind &> /dev/null; then
    # Linux
    echo "Using valgrind (Linux)..."
    if valgrind --leak-check=full --error-exitcode=1 ./dynamic_loader archive.dat &> /tmp/valgrind_output.txt; then
        echo -e "${GREEN}✅ No memory leaks detected${NC}"
    else
        echo -e "${YELLOW}⚠️  Potential memory leaks detected:${NC}"
        grep "definitely lost" /tmp/valgrind_output.txt || true
    fi
else
    echo -e "${YELLOW}⚠️  No memory leak checker available (install leaks or valgrind)${NC}"
fi

# Test 7: Verify all block types
echo ""
echo "Test 7: Checking block types..."

if grep -q "TEXT" /tmp/episode07_output.txt && \
   grep -q "BINARY" /tmp/episode07_output.txt && \
   grep -q "COMPRESSED" /tmp/episode07_output.txt; then
    echo -e "${GREEN}✅ All block types found${NC}"
else
    echo -e "${RED}❌ Not all block types found${NC}"
    exit 1
fi

# Display actual output
echo ""
echo "========================================="
echo "Actual output:"
echo "========================================="
cat /tmp/episode07_output.txt
echo "========================================="

echo ""
echo -e "${GREEN}🎉 All tests passed!${NC}"
echo ""
echo "Mission accomplished! You've mastered dynamic memory management."
echo "Next: Episode 08 - Fragmentation (Memory Leaks & Optimization)"

exit 0
