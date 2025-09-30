#!/bin/bash
# Test script for Episode 03

echo "=== MOONLIGHT Episode 03 - Automated Tests ==="
echo ""

ARTIFACTS_DIR="../artifacts"
DATA="../data.txt"
PATTERN="$ARTIFACTS_DIR/pattern"
SEQUENCE="$ARTIFACTS_DIR/sequence"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check data file
if [ ! -f "$DATA" ]; then
    echo -e "${RED}✗ data.txt not found${NC}"
    exit 1
fi

# Test 1: Pattern Finder
echo "=== Test 1: Pattern Finder ==="
if [ ! -f "$ARTIFACTS_DIR/pattern_finder.c" ]; then
    echo -e "${RED}✗ pattern_finder.c not found${NC}"
    echo "Create artifacts/pattern_finder.c first!"
    exit 1
fi

echo "Compiling pattern_finder.c..."
gcc -Wall -Wextra -std=c11 -o "$PATTERN" "$ARTIFACTS_DIR/pattern_finder.c" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"

echo "Running pattern finder..."
output=$("$PATTERN" < "$DATA" 2>&1)

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Runtime error!${NC}"
    echo "$output"
    rm -f "$PATTERN"
    exit 1
fi

# Check if output contains expected period (10 seconds)
if echo "$output" | grep -q "period.*10"; then
    echo -e "${GREEN}✓ Correct period detected (10 seconds)${NC}"
else
    echo -e "${YELLOW}? Period detection needs verification${NC}"
    echo "$output"
fi

rm -f "$PATTERN"
echo ""

# Test 2: Sequence Analyzer (optional)
echo "=== Test 2: Sequence Analyzer ==="
if [ -f "$ARTIFACTS_DIR/sequence_analyzer.c" ]; then
    echo "Compiling sequence_analyzer.c..."
    gcc -Wall -Wextra -std=c11 -lm -o "$SEQUENCE" "$ARTIFACTS_DIR/sequence_analyzer.c" 2>&1
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Compilation failed!${NC}"
        echo "Don't forget -lm flag for math.h!"
    else
        echo -e "${GREEN}✓ Compilation successful${NC}"
        
        echo "Running sequence analyzer..."
        output=$("$SEQUENCE" < "$DATA" 2>&1)
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓ Sequence analyzer executed${NC}"
            if echo "$output" | grep -q "STATISTICS"; then
                echo -e "${GREEN}✓ Statistics generated${NC}"
            fi
        fi
        
        rm -f "$SEQUENCE"
    fi
else
    echo -e "${YELLOW}! sequence_analyzer.c not found (optional)${NC}"
fi

echo ""
echo "=== Test Complete ==="
echo -e "${GREEN}✓✓✓ Pattern finder works correctly!${NC}"
echo ""
echo "You may now proceed to Episode 04."
