#!/bin/bash
# Test script for byte_printer.c (Task 1.5)

echo "=== MOONLIGHT Episode 01 - byte_printer.c Test ==="
echo ""

ARTIFACTS_DIR="../artifacts"
BYTE_PRINTER="$ARTIFACTS_DIR/byte_printer"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if byte_printer.c exists
if [ ! -f "$ARTIFACTS_DIR/byte_printer.c" ]; then
    echo -e "${YELLOW}! byte_printer.c not found in artifacts/${NC}"
    echo "This is an optional intermediate task."
    echo "Create artifacts/byte_printer.c if you want to practice!"
    exit 0
fi

echo "Compiling byte_printer.c..."
gcc -Wall -Wextra -std=c11 -o "$BYTE_PRINTER" "$ARTIFACTS_DIR/byte_printer.c" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo "Fix compilation errors and try again."
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"
echo ""

# Run byte_printer and capture output
echo "Running byte_printer..."
OUTPUT=$("$BYTE_PRINTER" 2>&1)

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Runtime error!${NC}"
    echo "$OUTPUT"
    exit 1
fi

echo -e "${GREEN}✓ Program executed successfully${NC}"
echo ""

# Display output
echo "Output:"
echo "----------------------------------------"
echo "$OUTPUT"
echo "----------------------------------------"
echo ""

# Validate output (check for expected patterns)
PASS=0

if echo "$OUTPUT" | grep -q "0x42 (66"; then
    echo -e "${GREEN}✓ Byte 0 (0x42) found${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ Byte 0 (0x42) not found${NC}"
fi

if echo "$OUTPUT" | grep -q "0xFF (255"; then
    echo -e "${GREEN}✓ Byte 2 (0xFF) found${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ Byte 2 (0xFF) not found${NC}"
fi

if echo "$OUTPUT" | grep -q "Sum.*473"; then
    echo -e "${GREEN}✓ Correct sum (473)${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ Sum should be 473${NC}"
fi

if echo "$OUTPUT" | grep -q "Max.*0xFF\|Max.*255"; then
    echo -e "${GREEN}✓ Correct max (0xFF/255)${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ Max should be 0xFF (255)${NC}"
fi

echo ""

# Final result
if [ $PASS -eq 4 ]; then
    echo -e "${GREEN}✓✓✓ ALL CHECKS PASSED! ✓✓✓${NC}"
    echo ""
    echo "Great job! You've mastered byte array processing."
    echo "This prepares you for the decoder task."
else
    echo -e "${YELLOW}⚠ Some checks failed ($PASS/4 passed)${NC}"
    echo ""
    echo "Expected output should include:"
    echo "  - Byte 0: 0x42 (66 decimal)"
    echo "  - Byte 2: 0xFF (255 decimal)"
    echo "  - Sum of all bytes: 473"
    echo "  - Max byte: 0xFF (255)"
fi

# Cleanup
rm -f "$BYTE_PRINTER"

echo ""
echo "=== Test Complete ==="

