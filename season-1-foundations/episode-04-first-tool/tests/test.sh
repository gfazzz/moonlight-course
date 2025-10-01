#!/bin/bash
# Test script for Episode 04: First Tool

echo "=== MOONLIGHT Episode 04 - Automated Tests ==="
echo ""

ARTIFACTS_DIR="../artifacts"
BUILD_DIR="$ARTIFACTS_DIR/build"
DECODER="$BUILD_DIR/decoder"
TEST_DATA="../test_encrypted.dat"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Create build directory
mkdir -p "$BUILD_DIR"

# Test 1: Check if all source files exist
echo "=== Test 1: Source Files Check ==="
REQUIRED_FILES=(
    "$ARTIFACTS_DIR/decoder.c"
    "$ARTIFACTS_DIR/crypto.c"
    "$ARTIFACTS_DIR/crypto.h"
    "$ARTIFACTS_DIR/utils.c"
    "$ARTIFACTS_DIR/utils.h"
)

MISSING=0
for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$file" ]; then
        echo -e "${RED}✗ $file not found${NC}"
        MISSING=1
    else
        echo -e "${GREEN}✓ $file exists${NC}"
    fi
done

if [ $MISSING -eq 1 ]; then
    echo ""
    echo "Create all required files first!"
    echo "See mission.md for details."
    exit 1
fi

echo ""

# Test 2: Compile crypto module
echo "=== Test 2: Compile Crypto Module ==="
gcc -Wall -Wextra -std=c11 -c "$ARTIFACTS_DIR/crypto.c" -o "$BUILD_DIR/crypto.o" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ crypto.c compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ crypto.o compiled${NC}"
echo ""

# Test 3: Compile utils module
echo "=== Test 3: Compile Utils Module ==="
gcc -Wall -Wextra -std=c11 -c "$ARTIFACTS_DIR/utils.c" -o "$BUILD_DIR/utils.o" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ utils.c compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ utils.o compiled${NC}"
echo ""

# Test 4: Compile main decoder
echo "=== Test 4: Compile Decoder ==="
gcc -Wall -Wextra -std=c11 -c "$ARTIFACTS_DIR/decoder.c" -o "$BUILD_DIR/decoder.o" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ decoder.c compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ decoder.o compiled${NC}"
echo ""

# Test 5: Link all modules
echo "=== Test 5: Link Modules ==="
gcc -Wall -Wextra -std=c11 -o "$DECODER" \
    "$BUILD_DIR/decoder.o" \
    "$BUILD_DIR/crypto.o" \
    "$BUILD_DIR/utils.o" 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Linking failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ decoder linked successfully${NC}"
echo ""

# Test 6: Run decoder with help flag
echo "=== Test 6: Help Message ==="
"$DECODER" --help > /tmp/episode04_help.txt 2>&1

if [ $? -eq 0 ] || [ -s /tmp/episode04_help.txt ]; then
    if grep -qi "usage\|help\|option" /tmp/episode04_help.txt; then
        echo -e "${GREEN}✓ Help message works${NC}"
    else
        echo -e "${YELLOW}? Help message unclear${NC}"
    fi
else
    echo -e "${YELLOW}? Help not implemented (optional)${NC}"
fi

echo ""

# Test 7: Test decryption with test data
echo "=== Test 7: Decryption Test ==="
if [ -f "$TEST_DATA" ]; then
    "$DECODER" -i "$TEST_DATA" -o /tmp/episode04_output.txt -k 0x42 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Decryption executed${NC}"
        
        if [ -f /tmp/episode04_output.txt ] && [ -s /tmp/episode04_output.txt ]; then
            echo -e "${GREEN}✓ Output file created${NC}"
            
            # Check if output is readable text
            if file /tmp/episode04_output.txt | grep -q "text"; then
                echo -e "${GREEN}✓ Output is text${NC}"
            fi
        else
            echo -e "${RED}✗ Output file empty or not created${NC}"
        fi
    else
        echo -e "${RED}✗ Decryption failed${NC}"
        echo "Check your decoder implementation"
    fi
else
    echo -e "${YELLOW}? test_encrypted.dat not found (skipping decrypt test)${NC}"
fi

echo ""

# Test 8: Module separation test
echo "=== Test 8: Module Separation ==="
CRYPTO_FUNCS=$(nm "$BUILD_DIR/crypto.o" 2>/dev/null | grep -c " T " || echo "0")
UTILS_FUNCS=$(nm "$BUILD_DIR/utils.o" 2>/dev/null | grep -c " T " || echo "0")

if [ "$CRYPTO_FUNCS" -gt 0 ] && [ "$UTILS_FUNCS" -gt 0 ]; then
    echo -e "${GREEN}✓ Modules properly separated${NC}"
    echo "  crypto.o: $CRYPTO_FUNCS functions"
    echo "  utils.o:  $UTILS_FUNCS functions"
else
    echo -e "${YELLOW}? Module separation unclear${NC}"
fi

echo ""

# Summary
echo "=== Test Summary ==="
echo -e "${GREEN}✓✓✓ All compilation tests passed!${NC}"
echo ""
echo "Your first modular C tool is ready!"
echo "Try running:"
echo "  $DECODER --help"
echo "  $DECODER -i test_encrypted.dat -o output.txt -k 0x42"
echo ""
echo "You may now proceed to Season 2."

# Cleanup (optional)
# rm -rf "$BUILD_DIR"

exit 0

