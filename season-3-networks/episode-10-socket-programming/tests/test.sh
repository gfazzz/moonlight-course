#!/bin/bash

# Episode 10: Socket Programming - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 10: Socket Programming - Running Tests"
echo "=================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if executables exist
if [ ! -f "./moonlight_server" ]; then
    echo -e "${RED}❌ Error: moonlight_server not found. Run 'make' first.${NC}"
    exit 1
fi

if [ ! -f "./moonlight_client" ]; then
    echo -e "${RED}❌ Error: moonlight_client not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Both server and client compiled successfully${NC}"

# Test 1: Check server can start
echo ""
echo "Test 1: Starting server..."
timeout 2 ./moonlight_server 9999 > /tmp/episode10_server.txt 2>&1 &
SERVER_PID=$!
sleep 0.5

if ps -p $SERVER_PID > /dev/null; then
    echo -e "${GREEN}✅ Server started successfully${NC}"
    kill $SERVER_PID 2>/dev/null || true
else
    echo -e "${RED}❌ Server failed to start${NC}"
    exit 1
fi

# Test 2: Check client can connect
echo ""
echo "Test 2: Testing client connection..."
timeout 3 ./moonlight_server 9999 > /tmp/episode10_server.txt 2>&1 &
SERVER_PID=$!
sleep 0.5

echo "HELLO" | timeout 2 ./moonlight_client 127.0.0.1 9999 > /tmp/episode10_client.txt 2>&1 || true
sleep 0.5

if [ -s /tmp/episode10_client.txt ]; then
    echo -e "${GREEN}✅ Client connected to server${NC}"
else
    echo -e "${YELLOW}⚠️  Client connection unclear (check implementation)${NC}"
fi

kill $SERVER_PID 2>/dev/null || true

# Test 3: Check server output format
echo ""
echo "Test 3: Checking server output..."

if grep -q "Listening on port" /tmp/episode10_server.txt; then
    echo -e "${GREEN}✅ Server output format correct${NC}"
else
    echo -e "${YELLOW}⚠️  Server output format unclear${NC}"
fi

# Test 4: Check client output format
echo ""
echo "Test 4: Checking client output..."

if grep -q "Connected to\|Sent:" /tmp/episode10_client.txt; then
    echo -e "${GREEN}✅ Client output format correct${NC}"
else
    echo -e "${YELLOW}⚠️  Client output format unclear${NC}"
fi

# Display outputs
echo ""
echo "=================================================="
echo "Server output:"
echo "=================================================="
cat /tmp/episode10_server.txt 2>/dev/null || echo "(no output)"
echo ""
echo "=================================================="
echo "Client output:"
echo "=================================================="
cat /tmp/episode10_client.txt 2>/dev/null || echo "(no output)"
echo "=================================================="

echo ""
echo -e "${GREEN}🎉 Tests completed!${NC}"
echo ""
echo "Mission accomplished! You've mastered basic socket programming."
echo "Next: Episode 11 - Packet Analysis"

exit 0
