#!/bin/bash

# Test script for header_parser.c
# Episode 11: Packet Analysis

set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SOLUTION_DIR="../solution"
SOURCE_FILE="${SOLUTION_DIR}/header_parser_solution.c"
EXECUTABLE="./header_parser_test"

echo "════════════════════════════════════════════════════════"
echo "  TEST: header_parser.c"
echo "════════════════════════════════════════════════════════"
echo

# ────────────────────────────────────────
# 1. Compilation
# ────────────────────────────────────────
echo "[1/3] Compiling header_parser_solution.c..."

if [ ! -f "$SOURCE_FILE" ]; then
    echo -e "${RED}✗ File not found: $SOURCE_FILE${NC}"
    exit 1
fi

gcc -Wall -Wextra -std=c11 "$SOURCE_FILE" -o "$EXECUTABLE" 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
else
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi

echo

# ────────────────────────────────────────
# 2. Create sample packet
# ────────────────────────────────────────
echo "[2/3] Creating sample TCP packet..."

# Sample packet: TCP SYN packet
# IP: 192.168.1.100:54321 → 203.0.113.42:31337
# Flags: SYN
# Payload: "HELLO MOONLIGHT"

SAMPLE_PACKET="sample_packet.bin"

# Create binary packet using printf
# IP header (20 bytes):
printf "\x45" > "$SAMPLE_PACKET"  # Version (4) + IHL (5)
printf "\x00" >> "$SAMPLE_PACKET"  # TOS
printf "\x00\x43" >> "$SAMPLE_PACKET"  # Total length: 67 bytes
printf "\x00\x01" >> "$SAMPLE_PACKET"  # ID
printf "\x40\x00" >> "$SAMPLE_PACKET"  # Flags + Fragment offset
printf "\x40" >> "$SAMPLE_PACKET"  # TTL: 64
printf "\x06" >> "$SAMPLE_PACKET"  # Protocol: TCP (6)
printf "\x00\x00" >> "$SAMPLE_PACKET"  # Checksum (placeholder)
printf "\xc0\xa8\x01\x64" >> "$SAMPLE_PACKET"  # Source IP: 192.168.1.100
printf "\xcb\x00\x71\x2a" >> "$SAMPLE_PACKET"  # Dest IP: 203.0.113.42

# TCP header (20 bytes):
printf "\xd4\x31" >> "$SAMPLE_PACKET"  # Source port: 54321
printf "\x7a\x69" >> "$SAMPLE_PACKET"  # Dest port: 31337
printf "\x00\x00\x03\xe8" >> "$SAMPLE_PACKET"  # Sequence: 1000
printf "\x00\x00\x00\x00" >> "$SAMPLE_PACKET"  # Ack: 0
printf "\x50\x02" >> "$SAMPLE_PACKET"  # Data offset (5*4=20) + Flags (SYN)
printf "\x20\x00" >> "$SAMPLE_PACKET"  # Window: 8192
printf "\x00\x00" >> "$SAMPLE_PACKET"  # Checksum (placeholder)
printf "\x00\x00" >> "$SAMPLE_PACKET"  # Urgent pointer

# TCP payload (15 bytes):
printf "HELLO MOONLIGHT" >> "$SAMPLE_PACKET"

echo -e "${GREEN}✓ Sample packet created (67 bytes)${NC}"
echo

# Cleanup function
cleanup() {
    rm -f "$EXECUTABLE" "$SAMPLE_PACKET"
}

trap cleanup EXIT

# ────────────────────────────────────────
# 3. Test Cases
# ────────────────────────────────────────
echo "[3/3] Running test cases..."
echo

TEST_COUNT=1
PASSED_TESTS=0
FAILED_TESTS=0

run_test() {
    local test_name=$1
    local packet_file=$2
    local expected_pattern=$3

    echo -n "Test $TEST_COUNT: $test_name "
    TEST_COUNT=$((TEST_COUNT + 1))

    if [ ! -f "$packet_file" ]; then
        echo -e "  ${RED}✗ FAIL (packet file not found)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi

    # Run parser and capture output
    output=$("$EXECUTABLE" "$packet_file" 2>&1 || true)

    if echo "$output" | grep -qE "$expected_pattern"; then
        echo -e "  ${GREEN}✓ PASS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "  ${RED}✗ FAIL${NC}"
        echo "    Expected pattern: $expected_pattern"
        echo "    Actual output:"
        echo "$output" | sed 's/^/      /'
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Test 1: Parse sample packet
run_test "Parse TCP SYN packet" "$SAMPLE_PACKET" "Source IP:.*192\.168\.1\.100.*Dest IP:.*203\.0\.113\.42.*Flags:.*SYN"

# Test 2: Check port parsing
run_test "Port parsing (54321 → 31337)" "$SAMPLE_PACKET" "Source Port:.*54321.*Dest Port:.*31337"

# Test 3: Check payload detection
run_test "Payload detection (HELLO MOONLIGHT)" "$SAMPLE_PACKET" "ASCII:.*HELLO MOONLIGHT"

echo

# ────────────────────────────────────────
# 4. Summary
# ────────────────────────────────────────
echo "════════════════════════════════════════════════════════"
echo "  TEST SUMMARY"
echo "════════════════════════════════════════════════════════"
echo "Total tests:  $((PASSED_TESTS + FAILED_TESTS))"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"
echo "════════════════════════════════════════════════════════"
echo

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    echo "You've mastered packet header parsing!"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    echo "Review IP/TCP header structures and try again."
    exit 1
fi

