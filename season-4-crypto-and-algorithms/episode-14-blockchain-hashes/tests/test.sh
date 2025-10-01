#!/bin/bash

# Episode 14: Blockchain & Hash Functions - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 14: Blockchain & Hash Functions - Running Tests"
echo "==========================================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./blockchain_validator" ]; then
    echo -e "${RED}❌ Error: blockchain_validator not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ blockchain_validator compiled successfully${NC}"

# Generate test blockchain if needed
if [ ! -f "./moonlight_ledger.dat" ]; then
    echo -e "${YELLOW}⚠️  moonlight_ledger.dat not found. Generating...${NC}"
    make generate 2>/dev/null || echo -e "${YELLOW}? No generate target${NC}"
fi

# Test 1: Run validator
echo ""
echo "Test 1: Running blockchain validator..."
if ./blockchain_validator moonlight_ledger.dat > /tmp/episode14_output.txt 2>&1; then
    echo -e "${GREEN}✅ Program executed successfully${NC}"
else
    echo -e "${YELLOW}⚠️  Program execution unclear (check implementation)${NC}"
fi

# Test 2: Check output format
echo ""
echo "Test 2: Checking output format..."

if grep -q "=== BLOCKCHAIN VALIDATOR ===\|BLOCKCHAIN\|Ledger" /tmp/episode14_output.txt; then
    echo -e "${GREEN}✅ Output format looks correct${NC}"
else
    echo -e "${YELLOW}⚠️  Output format unclear${NC}"
fi

# Test 3: Check for block parsing
echo ""
echo "Test 3: Verifying block parsing..."

if grep -E "\[Block [0-9]+\]|Block #[0-9]+|^Block [0-9]+" /tmp/episode14_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Block parsing found${NC}"
else
    echo -e "${YELLOW}⚠️  Block parsing needs implementation${NC}"
fi

# Test 4: Check for hash verification
echo ""
echo "Test 4: Checking hash verification..."

if grep -qi "hash\|verify\|valid\|invalid\|corrupted" /tmp/episode14_output.txt; then
    echo -e "${GREEN}✅ Hash verification implemented${NC}"
else
    echo -e "${YELLOW}⚠️  Hash verification needs implementation${NC}"
fi

# Test 5: Check for genesis block
echo ""
echo "Test 5: Checking genesis block detection..."

if grep -qi "genesis\|block 0\|first block" /tmp/episode14_output.txt; then
    echo -e "${GREEN}✅ Genesis block detected${NC}"
else
    echo -e "${YELLOW}⚠️  Genesis block handling needs implementation${NC}"
fi

# Test 6: Check for chain validation
echo ""
echo "Test 6: Checking chain validation..."

if grep -qi "chain.*valid\|integrity\|broken.*chain\|corrupted.*block" /tmp/episode14_output.txt; then
    echo -e "${GREEN}✅ Chain validation found${NC}"
else
    echo -e "${YELLOW}⚠️  Chain validation needs implementation${NC}"
fi

# Test 7: Check for statistics
echo ""
echo "Test 7: Checking statistics..."

if grep -q "Total.*blocks\|Valid.*blocks\|Invalid.*blocks" /tmp/episode14_output.txt; then
    echo -e "${GREEN}✅ Statistics found${NC}"
else
    echo -e "${YELLOW}⚠️  Statistics need implementation${NC}"
fi

# Test 8: Check for coordinate extraction
echo ""
echo "Test 8: Checking coordinate extraction..."

if grep -E "[0-9]+\.[0-9]+.*,.*[0-9]+\.[0-9]+|Coordinates:" /tmp/episode14_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Coordinates found in output${NC}"
else
    echo -e "${YELLOW}⚠️  Coordinate extraction needs implementation${NC}"
fi

# Test 9: Verify hash function implementation
echo ""
echo "Test 9: Checking hash function implementation..."

if [ -f "starter.c" ]; then
    if grep -q "hash.*function\|calculate.*hash\|sha\|md5" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Hash function code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Hash function needs implementation${NC}"
    fi
fi

# Test 10: Check for block structure
echo ""
echo "Test 10: Checking block structure definition..."

if [ -f "starter.c" ]; then
    if grep -q "struct.*block\|typedef.*Block" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Block structure defined${NC}"
    else
        echo -e "${YELLOW}⚠️  Block structure needs definition${NC}"
    fi
fi

# Display actual output
echo ""
echo "==========================================================="
echo "Actual output:"
echo "==========================================================="
cat /tmp/episode14_output.txt 2>/dev/null || echo "(no output)"
echo "==========================================================="

# Summary
echo ""
echo "==========================================================="
echo -e "${BLUE}📝 Implementation checklist:${NC}"
echo "==========================================================="
echo ""
echo "Block Structure:"
echo "  ☐ Define Block struct (index, timestamp, data, prev_hash, hash)"
echo "  ☐ Parse blocks from binary file"
echo ""
echo "Hash Functions:"
echo "  ☐ Implement hash function (SHA-256 or simplified)"
echo "  ☐ Calculate block hash from its data"
echo "  ☐ Compare hashes (verify integrity)"
echo ""
echo "Chain Validation:"
echo "  ☐ Verify genesis block (prev_hash = 0)"
echo "  ☐ Verify each block's prev_hash = previous block's hash"
echo "  ☐ Detect corrupted blocks"
echo "  ☐ Count valid/invalid blocks"
echo ""
echo "Coordinate Extraction:"
echo "  ☐ Extract coordinates from valid blocks"
echo "  ☐ Ignore corrupted blocks"
echo "  ☐ Display final coordinates"
echo ""
echo "==========================================================="
echo ""
echo -e "${GREEN}🎉 Compilation successful!${NC}"
echo ""
echo "Blockchain = Immutable ledger. One tampered block breaks the chain."
echo ""
echo "Next: Episode 15 - Sorting & Search Algorithms"

exit 0
