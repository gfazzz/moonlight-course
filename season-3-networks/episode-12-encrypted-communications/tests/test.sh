#!/bin/bash

# Episode 12: Encrypted Communications - Test Script  
# SEASON 3 FINALE

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 12: Encrypted Communications - Running Tests"
echo "======================================================="
echo "🎯 SEASON 3 FINALE"
echo "======================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./comm_interceptor" ]; then
    echo -e "${RED}❌ Error: comm_interceptor not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ comm_interceptor compiled successfully${NC}"

# Test 1: Check for root privileges requirement
echo ""
echo "Test 1: Checking root privileges requirement..."
if ./comm_interceptor > /tmp/episode12_test.txt 2>&1; then
    if grep -qi "root\|permission\|sudo" /tmp/episode12_test.txt; then
        echo -e "${GREEN}✅ Root privilege check implemented${NC}"
    fi
else
    if grep -qi "root\|permission\|sudo\|operation not permitted" /tmp/episode12_test.txt; then
        echo -e "${GREEN}✅ Root privilege required (as expected)${NC}"
    fi
fi

# Test 2: Check help message
echo ""
echo "Test 2: Checking help/usage message..."
./comm_interceptor --help > /tmp/episode12_help.txt 2>&1 || true

if grep -qi "usage\|help\|port.*4433\|intercept" /tmp/episode12_help.txt; then
    echo -e "${GREEN}✅ Help message available${NC}"
else
    echo -e "${YELLOW}⚠️  Help message not found (optional)${NC}"
fi

# Test 3: Verify integration of Episode 01 (XOR)
echo ""
echo "Test 3: Checking XOR decryption integration (Episode 01)..."
if [ -f "starter.c" ]; then
    if grep -q "xor\|XOR\|decrypt" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ XOR decryption code found${NC}"
    else
        echo -e "${YELLOW}⚠️  XOR decryption needs implementation${NC}"
    fi
fi

# Test 4: Verify TCP session tracking
echo ""
echo "Test 4: Checking TCP session reconstruction..."
if [ -f "starter.c" ]; then
    if grep -q "seq\|sequence.*number\|session\|track.*packet" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ TCP session tracking code found${NC}"
    else
        echo -e "${YELLOW}⚠️  TCP session tracking needs implementation${NC}"
    fi
fi

# Test 5: Check for packet ordering by seq
echo ""
echo "Test 5: Checking packet ordering by sequence numbers..."
if [ -f "starter.c" ]; then
    if grep -q "sort.*seq\|order.*packet\|qsort" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Packet ordering code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Packet ordering needs implementation${NC}"
    fi
fi

# Test 6: Verify stream assembly
echo ""
echo "Test 6: Checking stream assembly (payload concatenation)..."
if [ -f "starter.c" ]; then
    if grep -q "assemble\|concat\|payload\|stream" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Stream assembly code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Stream assembly needs implementation${NC}"
    fi
fi

# Test 7: Check for key extraction from first packet
echo ""
echo "Test 7: Checking key extraction from first packet..."
if [ -f "starter.c" ]; then
    if grep -q "key.*extract\|first.*packet\|key.*=.*packet" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Key extraction code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Key extraction needs implementation${NC}"
    fi
fi

# Test 8: Check for port 4433 filtering
echo ""
echo "Test 8: Checking port 4433 filtering..."
if [ -f "starter.c" ]; then
    if grep -q "4433\|port.*==.*4433" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Port 4433 filtering found${NC}"
    else
        echo -e "${YELLOW}⚠️  Port filtering needs implementation${NC}"
    fi
fi

# Test 9: Verify test capture file
echo ""
echo "Test 9: Checking for test capture file..."
if [ -f "artifacts/encrypted_session.pcap" ] || [ -f "session.pcap" ]; then
    echo -e "${GREEN}✅ Test capture file exists${NC}"
else
    echo -e "${YELLOW}⚠️  No test capture file found${NC}"
    echo "   Create with: sudo tcpdump -i lo -w session.pcap port 4433"
fi

# Display output
if [ -s /tmp/episode12_test.txt ]; then
    echo ""
    echo "======================================================="
    echo "Program output:"
    echo "======================================================="
    head -20 /tmp/episode12_test.txt
    echo "======================================================="
fi

# Final summary
echo ""
echo "======================================================="
echo -e "${BLUE}📝 SEASON 3 FINALE CHECKLIST:${NC}"
echo "======================================================="
echo ""
echo "Episode 01 Skills: XOR Decryption"
echo "  ☐ XOR decrypt function implemented"
echo ""
echo "Episode 09 Skills: IP Addressing"
echo "  ☐ IP address filtering (10.0.0.1)"
echo ""
echo "Episode 10 Skills: Sockets"
echo "  ☐ TCP connection understanding"
echo ""
echo "Episode 11 Skills: Packet Sniffing"
echo "  ☐ Raw socket creation (SOCK_RAW)"
echo "  ☐ TCP/IP header parsing"
echo "  ☐ Payload extraction"
echo ""
echo "Episode 12 Skills: Session Reconstruction"
echo "  ☐ TCP session tracking"
echo "  ☐ Packet ordering by seq number"
echo "  ☐ Stream assembly"
echo "  ☐ Key extraction from first packet"
echo "  ☐ Full message decryption"
echo ""
echo "======================================================="
echo ""
echo -e "${GREEN}🎉 Compilation successful!${NC}"
echo ""
echo "⚠️  To intercept encrypted session: sudo ./comm_interceptor"
echo ""
echo -e "${BLUE}🏆 Complete this to finish Season 3!${NC}"
echo "Next: Season 4 - Cryptography & Algorithms"

exit 0
