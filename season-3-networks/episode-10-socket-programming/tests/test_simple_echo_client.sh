#!/bin/bash

# Test script for simple_echo_client.c
# Episode 10: Socket Programming

set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SOLUTION_DIR="../solution"
SOURCE_FILE="${SOLUTION_DIR}/simple_echo_client_solution.c"
EXECUTABLE="./simple_echo_client_test"
ECHO_SERVER_PORT=7777
ECHO_SERVER_PID=""

echo "════════════════════════════════════════════════════════"
echo "  TEST: simple_echo_client.c"
echo "════════════════════════════════════════════════════════"
echo

# ────────────────────────────────────────
# 1. Compilation
# ────────────────────────────────────────
echo "[1/4] Compiling simple_echo_client_solution.c..."

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
# 2. Start Echo Server (using nc/netcat)
# ────────────────────────────────────────
echo "[2/4] Starting echo server on port $ECHO_SERVER_PORT..."

# Check if netcat (nc) is available
if ! command -v nc &> /dev/null; then
    echo -e "${YELLOW}⚠️  netcat (nc) not found. Skipping server tests.${NC}"
    echo -e "${YELLOW}   Install: sudo apt install netcat${NC}"
    rm -f "$EXECUTABLE"
    exit 0
fi

# Start simple echo server using netcat
# -l: listen, -k: keep listening, -p: port (for some nc versions)
# Different nc versions have different syntax
if nc -h 2>&1 | grep -q "\-k"; then
    # GNU netcat
    nc -l -k $ECHO_SERVER_PORT > /dev/null &
    ECHO_SERVER_PID=$!
elif nc -h 2>&1 | grep -q "OpenBSD"; then
    # OpenBSD netcat (macOS)
    while true; do nc -l $ECHO_SERVER_PORT; done &
    ECHO_SERVER_PID=$!
else
    # Try basic netcat
    nc -l $ECHO_SERVER_PORT &
    ECHO_SERVER_PID=$!
fi

# Give server time to start
sleep 0.5

if kill -0 $ECHO_SERVER_PID 2>/dev/null; then
    echo -e "${GREEN}✓ Echo server started (PID: $ECHO_SERVER_PID)${NC}"
else
    echo -e "${RED}✗ Failed to start echo server${NC}"
    rm -f "$EXECUTABLE"
    exit 1
fi

echo

# Cleanup function
cleanup() {
    if [ -n "$ECHO_SERVER_PID" ]; then
        kill $ECHO_SERVER_PID 2>/dev/null || true
        wait $ECHO_SERVER_PID 2>/dev/null || true
    fi
    rm -f "$EXECUTABLE"
}

trap cleanup EXIT

# ────────────────────────────────────────
# 3. Test Cases
# ────────────────────────────────────────
echo "[3/4] Running test cases..."
echo

TEST_COUNT=1
PASSED_TESTS=0
FAILED_TESTS=0

run_test() {
    local test_name=$1
    local server_ip=$2
    local server_port=$3
    local expected_pattern=$4

    echo -n "Test $TEST_COUNT: $test_name "
    TEST_COUNT=$((TEST_COUNT + 1))

    # Run client and capture output
    output=$("$EXECUTABLE" "$server_ip" "$server_port" 2>&1 || true)

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

# Test 1: Valid connection to echo server
run_test "Valid connection to 127.0.0.1:$ECHO_SERVER_PORT" "127.0.0.1" "$ECHO_SERVER_PORT" "Echo successful.*Match: ✓"

# Test 2: Invalid port (should fail gracefully)
run_test "Invalid port (99999)" "127.0.0.1" "99999" "Invalid port"

# Test 3: Connection to non-existent server (should timeout/fail)
run_test "Connection refused (port 9999)" "127.0.0.1" "9999" "connect.*failed|Connection refused"

echo

# ────────────────────────────────────────
# 4. Summary
# ────────────────────────────────────────
echo "[4/4] Test Summary"
echo "════════════════════════════════════════════════════════"
echo "Total tests:  $((PASSED_TESTS + FAILED_TESTS))"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"
echo "════════════════════════════════════════════════════════"
echo

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    echo "You've mastered basic socket programming!"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    echo "Review the socket API and try again."
    exit 1
fi

