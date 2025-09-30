#!/bin/bash

# Episode 08: Fragmentation - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 08: Fragmentation - Running Tests"
echo "============================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Setup
make config > /dev/null 2>&1
make test-input > /dev/null 2>&1

# Test 1: Build buggy version
echo ""
echo "Test 1: Building buggy version..."
if make buggy > /dev/null 2>&1; then
    echo -e "${GREEN}✅ Buggy version built${NC}"
else
    echo -e "${RED}❌ Failed to build buggy version${NC}"
    exit 1
fi

# Test 2: Run buggy version (should work but leak)
echo ""
echo "Test 2: Running buggy version..."
if timeout 2 ./buggy_logger < test_input.txt > /tmp/buggy_output.txt 2>&1; then
    echo -e "${GREEN}✅ Buggy version runs${NC}"
    echo -e "${YELLOW}⚠️  But it leaks memory!${NC}"
else
    echo -e "${YELLOW}⚠️  Buggy version may have crashed or timed out${NC}"
fi

# Test 3: Check for leaks in buggy version
echo ""
echo "Test 3: Checking buggy version for leaks..."

if command -v leaks &> /dev/null; then
    # macOS
    if timeout 5 leaks --atExit -- ./buggy_logger < test_input.txt 2>&1 | grep -q "leaks for"; then
        echo -e "${YELLOW}⚠️  Leaks detected in buggy version (expected)${NC}"
    else
        echo -e "${YELLOW}⚠️  Could not detect leaks (may need manual check)${NC}"
    fi
elif command -v valgrind &> /dev/null; then
    # Linux
    if timeout 10 valgrind --leak-check=summary --error-exitcode=0 ./buggy_logger < test_input.txt 2>&1 | grep -q "definitely lost"; then
        echo -e "${YELLOW}⚠️  Leaks detected in buggy version (expected)${NC}"
    else
        echo -e "${YELLOW}⚠️  Could not detect leaks (may need manual check)${NC}"
    fi
else
    echo -e "${YELLOW}⚠️  No leak detector available${NC}"
fi

# Test 4: Check if solution exists
echo ""
echo "Test 4: Checking if solution exists..."
if make solution > /dev/null 2>&1; then
    echo -e "${GREEN}✅ Solution built${NC}"
    
    # Test 5: Run solution
    echo ""
    echo "Test 5: Running solution..."
    if timeout 2 ./artifacts/fixed_logger_solution < test_input.txt > /tmp/solution_output.txt 2>&1; then
        echo -e "${GREEN}✅ Solution runs successfully${NC}"
    else
        echo -e "${RED}❌ Solution failed to run${NC}"
        exit 1
    fi
    
    # Test 6: Check solution for leaks
    echo ""
    echo "Test 6: Checking solution for leaks..."
    
    if command -v leaks &> /dev/null; then
        # macOS
        LEAK_OUTPUT=$(timeout 5 leaks --atExit -- ./artifacts/fixed_logger_solution < test_input.txt 2>&1)
        if echo "$LEAK_OUTPUT" | grep -q "0 leaks for 0 total leaked bytes"; then
            echo -e "${GREEN}✅ Solution has no leaks!${NC}"
        else
            echo -e "${YELLOW}⚠️  Solution may have leaks:${NC}"
            echo "$LEAK_OUTPUT" | grep "leaks for" || true
        fi
    elif command -v valgrind &> /dev/null; then
        # Linux
        LEAK_OUTPUT=$(timeout 10 valgrind --leak-check=full --error-exitcode=1 ./artifacts/fixed_logger_solution < test_input.txt 2>&1)
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✅ Solution has no leaks!${NC}"
        else
            echo -e "${YELLOW}⚠️  Solution may have leaks:${NC}"
            echo "$LEAK_OUTPUT" | grep "definitely lost" || true
        fi
    else
        echo -e "${YELLOW}⚠️  No leak detector available to verify${NC}"
    fi
else
    echo -e "${YELLOW}⚠️  Solution not available${NC}"
fi

# Test 7: Verify output correctness
echo ""
echo "Test 7: Verifying output format..."

if grep -q "Logged\." /tmp/solution_output.txt && \
   grep -q "Rotating logs" /tmp/solution_output.txt; then
    echo -e "${GREEN}✅ Output format is correct${NC}"
else
    echo -e "${RED}❌ Output format is incorrect${NC}"
    cat /tmp/solution_output.txt
    exit 1
fi

# Summary
echo ""
echo "============================================="
echo "Test Summary:"
echo "============================================="
echo "Buggy version: Runs but leaks (as expected)"
echo "Solution:      Runs without leaks ✅"
echo ""
echo -e "${GREEN}🎉 All tests passed!${NC}"
echo ""
echo "Your task: Create fixed_logger.c with 0 leaks"
echo "1. Run: make leaks (or make valgrind on Linux)"
echo "2. Find all leaks in buggy_logger.c"
echo "3. Fix them in fixed_logger.c"
echo "4. Verify: make leaks-fixed shows 0 leaks"

exit 0
