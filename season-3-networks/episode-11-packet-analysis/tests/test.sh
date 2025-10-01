#!/bin/bash

# Episode 11: Packet Analysis - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 11: Packet Analysis - Running Tests"
echo "==============================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./packet_sniffer" ]; then
    echo -e "${RED}❌ Error: packet_sniffer not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ packet_sniffer compiled successfully${NC}"

# Test 1: Check if program requires root
echo ""
echo "Test 1: Checking root privileges requirement..."
if ./packet_sniffer > /tmp/episode11_test.txt 2>&1; then
    if grep -qi "root\|permission\|sudo" /tmp/episode11_test.txt; then
        echo -e "${GREEN}✅ Root privilege check implemented${NC}"
    else
        echo -e "${YELLOW}⚠️  Program ran without sudo check (may need implementation)${NC}"
    fi
else
    if grep -qi "root\|permission\|sudo\|operation not permitted" /tmp/episode11_test.txt; then
        echo -e "${GREEN}✅ Root privilege required (as expected)${NC}"
    else
        echo -e "${YELLOW}⚠️  Error unclear${NC}"
    fi
fi

# Test 2: Check for help or usage
echo ""
echo "Test 2: Checking help/usage message..."
./packet_sniffer --help > /tmp/episode11_help.txt 2>&1 || true
./packet_sniffer -h > /tmp/episode11_help.txt 2>&1 || true

if grep -qi "usage\|help\|option\|port\|interface" /tmp/episode11_help.txt; then
    echo -e "${GREEN}✅ Help message available${NC}"
else
    echo -e "${YELLOW}⚠️  Help message not found (optional)${NC}"
fi

# Test 3: Check if raw socket is mentioned in code
echo ""
echo "Test 3: Verifying raw socket implementation..."
if [ -f "starter.c" ]; then
    if grep -q "SOCK_RAW\|socket(AF_PACKET\|socket(AF_INET.*SOCK_RAW" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Raw socket code found in starter.c${NC}"
    else
        echo -e "${YELLOW}⚠️  Raw socket not implemented yet (check starter.c)${NC}"
    fi
fi

# Test 4: Check for IP/TCP header structures
echo ""
echo "Test 4: Checking IP/TCP header structures..."
if [ -f "starter.c" ]; then
    HAS_IP_HEADER=$(grep -c "struct.*ip\|iphdr\|ip_header" starter.c 2>/dev/null || echo "0")
    HAS_TCP_HEADER=$(grep -c "struct.*tcp\|tcphdr\|tcp_header" starter.c 2>/dev/null || echo "0")
    
    if [ "$HAS_IP_HEADER" -gt 0 ] && [ "$HAS_TCP_HEADER" -gt 0 ]; then
        echo -e "${GREEN}✅ IP and TCP header structures found${NC}"
    else
        echo -e "${YELLOW}⚠️  Header structures need implementation${NC}"
    fi
fi

# Test 5: Generate test capture file if possible
echo ""
echo "Test 5: Checking for test capture file..."
if [ -f "artifacts/test_capture.pcap" ] || [ -f "capture.pcap" ]; then
    echo -e "${GREEN}✅ Test capture file exists${NC}"
else
    echo -e "${YELLOW}⚠️  No test capture file found${NC}"
    echo "   You can create one with: sudo tcpdump -i lo -w capture.pcap port 9999"
fi

# Test 6: Verify code has packet parsing functions
echo ""
echo "Test 6: Checking packet parsing functions..."
if [ -f "starter.c" ]; then
    PARSE_FUNCS=$(grep -c "parse.*packet\|print.*packet\|process.*packet\|hex.*dump" starter.c 2>/dev/null || echo "0")
    
    if [ "$PARSE_FUNCS" -gt 0 ]; then
        echo -e "${GREEN}✅ Packet parsing functions found${NC}"
    else
        echo -e "${YELLOW}⚠️  Packet parsing functions need implementation${NC}"
    fi
fi

# Test 7: Check for port filtering
echo ""
echo "Test 7: Checking port filtering capability..."
if [ -f "starter.c" ]; then
    if grep -q "port.*9999\|filter.*port\|tcp.*dport\|tcp.*sport" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Port filtering code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Port filtering needs implementation${NC}"
    fi
fi

# Test 8: Check for hex dump functionality
echo ""
echo "Test 8: Checking hex dump functionality..."
if [ -f "starter.c" ]; then
    if grep -q "hex.*dump\|print.*hex\|%02x\|printf.*hex" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Hex dump functionality found${NC}"
    else
        echo -e "${YELLOW}⚠️  Hex dump needs implementation${NC}"
    fi
fi

# Display any output
if [ -s /tmp/episode11_test.txt ]; then
    echo ""
    echo "==============================================="
    echo "Program output:"
    echo "==============================================="
    head -20 /tmp/episode11_test.txt
    echo "==============================================="
fi

# Summary
echo ""
echo -e "${GREEN}🎉 Compilation tests passed!${NC}"
echo ""
echo "📝 Implementation checklist:"
echo "  ☐ Create raw socket (SOCK_RAW)"
echo "  ☐ Parse IP header (source, dest, protocol)"
echo "  ☐ Parse TCP header (ports, seq, flags)"
echo "  ☐ Extract payload data"
echo "  ☐ Filter by port 9999"
echo "  ☐ Hex dump packets"
echo "  ☐ Find hidden message"
echo ""
echo "⚠️  To run packet sniffer: sudo ./packet_sniffer"
echo ""
echo "Next: Episode 12 - Encrypted Communications"

exit 0
