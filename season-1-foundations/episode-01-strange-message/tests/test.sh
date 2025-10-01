#!/bin/bash
# Test script for Episode 01

echo "=== MOONLIGHT Episode 01 - Automated Tests ==="
echo ""

ARTIFACTS_DIR="../artifacts"
DECODER="$ARTIFACTS_DIR/decoder"
EXPECTED="expected.txt"
OUTPUT="output.txt"

# Check if byte_printer.c exists and run its test first
if [ -f "$ARTIFACTS_DIR/byte_printer.c" ]; then
    echo "Found byte_printer.c - running optional test first..."
    echo ""
    ./test_byte_printer.sh
    echo ""
    echo "─────────────────────────────────────────"
    echo ""
fi

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if decoder exists
if [ ! -f "$ARTIFACTS_DIR/decoder.c" ]; then
    echo -e "${RED}✗ decoder.c not found in artifacts/${NC}"
    echo "Please create artifacts/decoder.c first!"
    exit 1
fi

echo "Compiling decoder.c..."
gcc -Wall -Wextra -std=c11 -o "$DECODER" "$ARTIFACTS_DIR/decoder.c" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo "Fix compilation errors and try again."
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"
echo ""

# Run decoder
echo "Running decoder..."
"$DECODER" > "$OUTPUT" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Runtime error!${NC}"
    cat "$OUTPUT"
    exit 1
fi

echo -e "${GREEN}✓ Decoder executed successfully${NC}"
echo ""

# Check if expected.txt exists
if [ ! -f "$EXPECTED" ]; then
    echo -e "${YELLOW}! Expected output file not found${NC}"
    echo "Current output:"
    cat "$OUTPUT"
    echo ""
    echo "If this looks correct, save it as expected.txt"
    exit 0
fi

# Compare output
echo "Comparing output with expected result..."
if diff -q "$OUTPUT" "$EXPECTED" > /dev/null; then
    echo -e "${GREEN}✓✓✓ ALL TESTS PASSED! ✓✓✓${NC}"
    echo ""
    echo "Congratulations! Message decrypted successfully."
    echo "You may now proceed to Episode 02."
else
    echo -e "${RED}✗ Output doesn't match expected result${NC}"
    echo ""
    echo "Expected:"
    cat "$EXPECTED"
    echo ""
    echo "Got:"
    cat "$OUTPUT"
    echo ""
    echo "Differences:"
    diff "$EXPECTED" "$OUTPUT"
fi

# Cleanup
rm -f "$OUTPUT" "$DECODER"

echo ""
echo "=== Test Complete ==="
