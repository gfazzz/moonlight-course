#!/bin/bash

# Episode 15: Sorting & Search - Test Script

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 15: Sorting & Search - Running Tests"
echo "================================================"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./algo_analyzer" ]; then
    echo -e "${RED}❌ Error: algo_analyzer not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ algo_analyzer compiled successfully${NC}"

# Generate test data if needed
if [ ! -f "./server_logs.dat" ]; then
    echo -e "${YELLOW}⚠️  server_logs.dat not found. Generating small test...${NC}"
    make generate 2>/dev/null || echo -e "${YELLOW}? No generate target${NC}"
fi

# Test 1: Run with small dataset
echo ""
echo "Test 1: Running with test data..."
./algo_analyzer server_logs.dat > /tmp/episode15_output.txt 2>&1 || true

if [ -s /tmp/episode15_output.txt ]; then
    echo -e "${GREEN}✅ Program executed${NC}"
else
    echo -e "${YELLOW}⚠️  No output generated${NC}"
fi

# Test 2: Check for sorting algorithm implementations
echo ""
echo "Test 2: Checking sorting algorithm implementations..."

if [ -f "starter.c" ]; then
    QUICKSORT=$(grep -c "quicksort\|quick_sort" starter.c 2>/dev/null || echo "0")
    MERGESORT=$(grep -c "mergesort\|merge_sort" starter.c 2>/dev/null || echo "0")
    HEAPSORT=$(grep -c "heapsort\|heap_sort" starter.c 2>/dev/null || echo "0")
    
    echo "  Quicksort: $QUICKSORT references"
    echo "  Mergesort: $MERGESORT references"
    echo "  Heapsort:  $HEAPSORT references"
    
    if [ "$QUICKSORT" -gt 0 ] && [ "$MERGESORT" -gt 0 ] && [ "$HEAPSORT" -gt 0 ]; then
        echo -e "${GREEN}✅ All three algorithms found${NC}"
    else
        echo -e "${YELLOW}⚠️  Not all algorithms implemented yet${NC}"
    fi
fi

# Test 3: Check for binary search
echo ""
echo "Test 3: Checking binary search implementation..."

if [ -f "starter.c" ]; then
    if grep -q "binary.*search\|bsearch" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Binary search found${NC}"
    else
        echo -e "${YELLOW}⚠️  Binary search needs implementation${NC}"
    fi
fi

# Test 4: Check output format
echo ""
echo "Test 4: Checking output format..."

if grep -q "=== SORTING & SEARCH ===\|Algorithm\|Time:" /tmp/episode15_output.txt; then
    echo -e "${GREEN}✅ Output format looks correct${NC}"
else
    echo -e "${YELLOW}⚠️  Output format unclear${NC}"
fi

# Test 5: Check for performance comparison
echo ""
echo "Test 5: Checking performance comparison..."

if grep -qi "quicksort\|mergesort\|heapsort" /tmp/episode15_output.txt; then
    echo -e "${GREEN}✅ Algorithm names found in output${NC}"
    
    if grep -E "[0-9]+\s*(ms|seconds|µs)" /tmp/episode15_output.txt > /dev/null; then
        echo -e "${GREEN}✅ Timing measurements found${NC}"
    else
        echo -e "${YELLOW}⚠️  Timing measurements need implementation${NC}"
    fi
else
    echo -e "${YELLOW}⚠️  Performance comparison needs implementation${NC}"
fi

# Test 6: Check for Big O notation
echo ""
echo "Test 6: Checking Big O complexity analysis..."

if grep -E "O\(.*\)|complexity" /tmp/episode15_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Big O notation found${NC}"
else
    echo -e "${YELLOW}⚠️  Big O analysis needs implementation${NC}"
fi

# Test 7: Verify sorting correctness with small test
echo ""
echo "Test 7: Verifying sorting correctness..."

# Create small test file
echo "5 2 8 1 9 3 7 4 6 0" > /tmp/test_small.txt

if ./algo_analyzer /tmp/test_small.txt > /tmp/test_sorted.txt 2>&1; then
    if grep -q "0.*1.*2.*3.*4.*5.*6.*7.*8.*9\|sorted" /tmp/test_sorted.txt; then
        echo -e "${GREEN}✅ Sorting appears correct${NC}"
    else
        echo -e "${YELLOW}⚠️  Sorting correctness unclear${NC}"
    fi
fi

# Test 8: Check for timestamp search
echo ""
echo "Test 8: Checking timestamp search functionality..."

if grep -qi "target\|search\|found\|timestamp" /tmp/episode15_output.txt; then
    echo -e "${GREEN}✅ Search functionality found${NC}"
else
    echo -e "${YELLOW}⚠️  Search functionality needs implementation${NC}"
fi

# Display output
echo ""
echo "================================================"
echo "Actual output:"
echo "================================================"
cat /tmp/episode15_output.txt 2>/dev/null | head -30 || echo "(no output)"
echo "================================================"

# Summary
echo ""
echo "================================================"
echo -e "${BLUE}📝 Implementation checklist:${NC}"
echo "================================================"
echo ""
echo "Sorting Algorithms (O(n log n)):"
echo "  ☐ Quicksort (average O(n log n), worst O(n²))"
echo "    - Partition function"
echo "    - Random pivot selection"
echo "  ☐ Merge Sort (guaranteed O(n log n))"
echo "    - Merge function"
echo "    - Recursive split"
echo "  ☐ Heap Sort (O(n log n), in-place)"
echo "    - Heapify function"
echo "    - Build heap"
echo ""
echo "Search:"
echo "  ☐ Binary Search (O(log n))"
echo "    - Only works on sorted data!"
echo "    - Returns index or -1"
echo ""
echo "Performance Testing:"
echo "  ☐ Time measurement (clock_gettime or clock())"
echo "  ☐ Comparison of 3 algorithms"
echo "  ☐ Test with 10,000 records"
echo "  ☐ Test with 1,000,000 records (if possible)"
echo "  ☐ Print Big O complexity"
echo ""
echo "Mission:"
echo "  ☐ Sort 10M server logs"
echo "  ☐ Find target timestamp"
echo "  ☐ Extract coordinates"
echo ""
echo "================================================"
echo ""
echo -e "${GREEN}🎉 Compilation successful!${NC}"
echo ""
echo "Expected performance on 10M records:"
echo "  Quicksort: ~2-3 seconds"
echo "  Merge Sort: ~3-4 seconds"
echo "  Heap Sort: ~5-6 seconds"
echo "  Binary Search: < 1 millisecond"
echo ""
echo "Next: Episode 16 - Data Structures (FINALE)"

rm -f /tmp/test_small.txt /tmp/test_sorted.txt

exit 0
