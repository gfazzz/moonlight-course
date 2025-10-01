#!/bin/bash

# Episode 16: Data Structures - Test Script
# SEASON 4 FINALE

set -e

EPISODE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$EPISODE_DIR"

echo "🧪 Episode 16: Data Structures - Running Tests"
echo "==============================================="
echo "🎯 SEASON 4 FINALE"
echo "==============================================="

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "./data_structure_analyzer" ]; then
    echo -e "${RED}❌ Error: data_structure_analyzer not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ data_structure_analyzer compiled successfully${NC}"

# Test 1: Run program
echo ""
echo "Test 1: Running data structure analyzer..."
./data_structure_analyzer > /tmp/episode16_output.txt 2>&1 || true

if [ -s /tmp/episode16_output.txt ]; then
    echo -e "${GREEN}✅ Program executed${NC}"
else
    echo -e "${YELLOW}⚠️  No output generated${NC}"
fi

# Test 2: Check for Hash Table implementation
echo ""
echo "Test 2: Checking Hash Table implementation..."

if [ -f "starter.c" ]; then
    if grep -q "hash.*table\|HashTable\|hash_insert\|hash_search" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Hash Table code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Hash Table needs implementation${NC}"
    fi
fi

# Test 3: Check for BST implementation
echo ""
echo "Test 3: Checking Binary Search Tree implementation..."

if [ -f "starter.c" ]; then
    if grep -q "BST\|bst_insert\|bst_search\|tree.*node" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ BST code found${NC}"
    else
        echo -e "${YELLOW}⚠️  BST needs implementation${NC}"
    fi
fi

# Test 4: Check for Linked List implementation
echo ""
echo "Test 4: Checking Linked List implementation..."

if [ -f "starter.c" ]; then
    if grep -q "linked.*list\|LinkedList\|list_append\|Node.*next" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Linked List code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Linked List needs implementation${NC}"
    fi
fi

# Test 5: Check for Stack implementation
echo ""
echo "Test 5: Checking Stack implementation..."

if [ -f "starter.c" ]; then
    if grep -q "Stack\|stack_push\|stack_pop\|LIFO" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Stack code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Stack needs implementation${NC}"
    fi
fi

# Test 6: Check for Queue implementation
echo ""
echo "Test 6: Checking Queue implementation..."

if [ -f "starter.c" ]; then
    if grep -q "Queue\|queue_enqueue\|queue_dequeue\|FIFO" starter.c 2>/dev/null; then
        echo -e "${GREEN}✅ Queue code found${NC}"
    else
        echo -e "${YELLOW}⚠️  Queue needs implementation${NC}"
    fi
fi

# Test 7: Check output format
echo ""
echo "Test 7: Checking output format..."

if grep -q "=== DATABASE ENGINE ===\|Hash Table\|BST\|Documents" /tmp/episode16_output.txt; then
    echo -e "${GREEN}✅ Output format looks correct${NC}"
else
    echo -e "${YELLOW}⚠️  Output format unclear${NC}"
fi

# Test 8: Check for CRUD operations
echo ""
echo "Test 8: Checking CRUD operations..."

if grep -qi "insert\|search\|delete\|update" /tmp/episode16_output.txt; then
    echo -e "${GREEN}✅ CRUD operations found${NC}"
else
    echo -e "${YELLOW}⚠️  CRUD operations need implementation${NC}"
fi

# Test 9: Check for performance stats
echo ""
echo "Test 9: Checking performance statistics..."

if grep -E "O\(1\)|O\(log n\)|O\(n\)|operations|time:" /tmp/episode16_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Performance analysis found${NC}"
else
    echo -e "${YELLOW}⚠️  Performance analysis needs implementation${NC}"
fi

# Display output
echo ""
echo "==============================================="
echo "Actual output:"
echo "==============================================="
cat /tmp/episode16_output.txt 2>/dev/null | head -40 || echo "(no output)"
echo "==============================================="

# Final summary
echo ""
echo "==============================================="
echo -e "${BLUE}📝 SEASON 4 FINALE CHECKLIST:${NC}"
echo "==============================================="
echo ""
echo "Data Structures to Implement:"
echo ""
echo "1. Hash Table (O(1) average):"
echo "  ☐ Hash function (modulo with prime)"
echo "  ☐ Collision handling (chaining or open addressing)"
echo "  ☐ Insert, search, delete operations"
echo ""
echo "2. Binary Search Tree (O(log n) average):"
echo "  ☐ Node structure (left, right pointers)"
echo "  ☐ Insert (recursive or iterative)"
echo "  ☐ Search (compare keys)"
echo "  ☐ Delete (3 cases: leaf, 1 child, 2 children)"
echo "  ☐ In-order traversal (sorted output)"
echo ""
echo "3. Linked List (O(1) append, O(n) search):"
echo "  ☐ Node structure (data, next pointer)"
echo "  ☐ Append (add to end)"
echo "  ☐ Prepend (add to start)"
echo "  ☐ Delete (find and unlink)"
echo "  ☐ Traverse (iterate all nodes)"
echo ""
echo "4. Stack (LIFO - Last In First Out):"
echo "  ☐ Push (add to top)"
echo "  ☐ Pop (remove from top)"
echo "  ☐ Peek (view top without removing)"
echo "  ☐ Use case: Undo operations"
echo ""
echo "5. Queue (FIFO - First In First Out):"
echo "  ☐ Enqueue (add to rear)"
echo "  ☐ Dequeue (remove from front)"
echo "  ☐ Peek (view front)"
echo "  ☐ Use case: Task scheduling"
echo ""
echo "Database Engine Integration:"
echo "  ☐ Use Hash Table for O(1) document lookup by ID"
echo "  ☐ Use BST for sorted access (by timestamp)"
echo "  ☐ Use Linked List for audit log (append-only)"
echo "  ☐ Use Stack for undo operations"
echo "  ☐ Use Queue for async task processing"
echo "  ☐ Handle 250,000 documents efficiently"
echo ""
echo "==============================================="
echo ""
echo -e "${GREEN}🎉 Compilation successful!${NC}"
echo ""
echo -e "${BLUE}🏆 Complete this to finish Season 4!${NC}"
echo ""
echo "Next: Season 5 - Financial Markets"

exit 0
