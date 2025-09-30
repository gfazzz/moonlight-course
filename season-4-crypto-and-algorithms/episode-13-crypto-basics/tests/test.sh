#!/bin/bash
# Test script for Episode 13: Crypto Basics
# MOONLIGHT OPERATION

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EPISODE_DIR="$(dirname "$SCRIPT_DIR")"
cd "$EPISODE_DIR"

echo "════════════════════════════════════════════════"
echo "  Episode 13: Crypto Basics - Test Suite"
echo "════════════════════════════════════════════════"
echo ""

# Create artifacts directory
mkdir -p artifacts

# Check if executable exists
if [ ! -f "./crypto_tool" ]; then
    echo -e "${RED}✗ crypto_tool not found!${NC}"
    echo "Run 'make' first to compile."
    exit 1
fi

# Generate test files if they don't exist
if [ ! -f "message1.enc" ] || [ ! -f "message2.enc" ] || [ ! -f "message3.enc" ]; then
    echo -e "${YELLOW}⚠ Test files not found. Generating...${NC}"
    
    # Compile solution to generate test files
    if [ -f "solution/crypto_solution.c" ]; then
        gcc -Wall -Wextra -std=c11 -o crypto_tool_gen solution/crypto_solution.c
        ./crypto_tool_gen generate
        rm -f crypto_tool_gen
        echo ""
    else
        echo -e "${RED}✗ Cannot generate test files (solution not found)${NC}"
        exit 1
    fi
fi

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0

# Test 1: XOR decryption
echo "─────────────────────────────────────────────────"
echo "Test 1: XOR Decryption (message1.enc)"
echo "─────────────────────────────────────────────────"
./crypto_tool xor message1.enc > artifacts/test1_output.txt 2>&1

if grep -q "URGENT:" artifacts/test1_output.txt; then
    echo -e "${GREEN}✓ PASS${NC} - Message decrypted successfully"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - XOR decryption failed"
    ((TESTS_FAILED++))
fi
echo ""

# Test 2: Caesar cipher
echo "─────────────────────────────────────────────────"
echo "Test 2: Caesar Cipher (message2.enc)"
echo "─────────────────────────────────────────────────"
./crypto_tool caesar message2.enc > artifacts/test2_output.txt 2>&1

if grep -qi "package\|genesis\|warehouse" artifacts/test2_output.txt; then
    echo -e "${GREEN}✓ PASS${NC} - Caesar cipher broken"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Caesar cipher decryption failed"
    ((TESTS_FAILED++))
fi
echo ""

# Test 3: Multi-layer decryption
echo "─────────────────────────────────────────────────"
echo "Test 3: Multi-layer Decryption (message3.enc)"
echo "─────────────────────────────────────────────────"
./crypto_tool multi message3.enc > artifacts/test3_output.txt 2>&1

if grep -qi "MOONLIGHT\|blockchain\|database" artifacts/test3_output.txt; then
    echo -e "${GREEN}✓ PASS${NC} - Multi-layer decryption successful"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Multi-layer decryption failed"
    ((TESTS_FAILED++))
fi
echo ""

# Final results
echo "════════════════════════════════════════════════"
echo "  Test Results"
echo "════════════════════════════════════════════════"
echo -e "Passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Failed: ${RED}${TESTS_FAILED}${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed! Episode 13 complete!${NC}"
    echo ""
    echo "Next: Episode 14 - Hash Functions & Blockchain"
    exit 0
else
    echo -e "${RED}✗ Some tests failed. Keep trying!${NC}"
    echo ""
    echo "Hints:"
    echo "- Check your XOR implementation (remember: XOR is self-inverse)"
    echo "- For Caesar: try both brute-force and frequency analysis"
    echo "- Multi-layer: decrypt in REVERSE order (XOR first, then Caesar)"
    exit 1
fi
