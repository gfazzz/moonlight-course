#!/bin/bash

# Test script for Episode 21

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "🧪 Testing GPIO System..."
echo ""

# Test 1: Program compiles
echo "Test 1: Compilation..."
if [ -f "./gpio_system" ]; then
    echo -e "${GREEN}✅ Program compiled${NC}"
else
    echo -e "${RED}❌ Program not compiled${NC}"
    exit 1
fi

# Test 2: Program runs
echo "Test 2: Execution..."
if ./gpio_system > /dev/null 2>&1; then
    echo -e "${GREEN}✅ Program runs${NC}"
else
    echo -e "${RED}❌ Program failed to run${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}✅ All tests passed!${NC}"
echo ""
echo "Next steps:"
echo "1. Implement actual GPIO functions"
echo "2. Test with Arduino hardware"
echo "3. Add sensor reading logic"
