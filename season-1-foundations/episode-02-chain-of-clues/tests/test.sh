#!/bin/bash
# Test script for Episode 02

echo "=== MOONLIGHT Episode 02 - Automated Tests ==="
echo ""

ARTIFACTS_DIR="../artifacts"
DATA="../data.dat"
CLASSIFIER="$ARTIFACTS_DIR/classifier"
EXPECTED="expected.txt"
OUTPUT="output.txt"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if classifier.c exists
if [ ! -f "$ARTIFACTS_DIR/classifier.c" ]; then
    echo -e "${RED}✗ classifier.c not found in artifacts/${NC}"
    echo "Please create artifacts/classifier.c first!"
    exit 1
fi

echo "Compiling classifier.c..."
gcc -Wall -Wextra -std=c11 -o "$CLASSIFIER" "$ARTIFACTS_DIR/classifier.c" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo "Fix compilation errors and try again."
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"
echo ""

# Run classifier
echo "Running classifier with test data..."
"$CLASSIFIER" < "$DATA" > "$OUTPUT" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Runtime error!${NC}"
    cat "$OUTPUT"
    exit 1
fi

echo -e "${GREEN}✓ Classifier executed successfully${NC}"
echo ""

# Check output
if [ ! -f "$EXPECTED" ]; then
    echo -e "${YELLOW}! Expected output file not found${NC}"
    echo "Current output:"
    cat "$OUTPUT"
    echo ""
    echo "If this looks correct, save it as expected.txt"
    rm -f "$CLASSIFIER"
    exit 0
fi

# Compare (ignore exact formatting, check key content)
echo "Checking output..."

# Check for keywords
if grep -q "STATISTICS" "$OUTPUT" && \
   grep -q "Valid" "$OUTPUT" && \
   grep -q "Invalid" "$OUTPUT"; then
    echo -e "${GREEN}✓✓✓ ALL TESTS PASSED! ✓✓✓${NC}"
    echo ""
    echo "Output summary:"
    grep -E "(Coordinates|Time|IDs|Valid|Invalid)" "$OUTPUT"
    echo ""
    echo "You may now proceed to Episode 03."
else
    echo -e "${RED}✗ Output missing required sections${NC}"
    echo ""
    echo "Expected sections: STATISTICS, Valid, Invalid"
    echo ""
    echo "Your output:"
    cat "$OUTPUT"
fi

# Cleanup
rm -f "$OUTPUT" "$CLASSIFIER"

echo ""
echo "=== Test Complete ==="
