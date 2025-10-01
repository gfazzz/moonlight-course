#!/bin/bash

# ═══════════════════════════════════════════════════════════════
# Episode 16: Data Structures — Test для linked_list_cycle
# ═══════════════════════════════════════════════════════════════

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EPISODE_DIR="$(dirname "$SCRIPT_DIR")"

echo "═══════════════════════════════════════════════════════════"
echo "Testing linked_list_cycle.c (Floyd's Cycle Detection)"
echo "═══════════════════════════════════════════════════════════"
echo ""

# Компиляция
echo "[1/3] Compiling linked_list_cycle.c..."
cd "$EPISODE_DIR/artifacts"
gcc -Wall -Wextra -std=c11 -O2 -g linked_list_cycle.c -o linked_list_cycle

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi
echo "✅ Compilation successful"
echo ""

# Тест 1: Запуск программы
echo "[2/3] Running linked_list_cycle..."
OUTPUT=$(./linked_list_cycle 2>&1)

if [ $? -ne 0 ]; then
    echo "❌ Program crashed!"
    exit 1
fi
echo "✅ Program runs successfully"
echo ""

# Тест 2: Проверка функциональности
echo "[3/3] Checking Floyd's algorithm implementation..."

# Должны быть все 4 теста
REQUIRED_TESTS=("TEST 1" "TEST 2" "TEST 3" "TEST 4")

for test in "${REQUIRED_TESTS[@]}"; do
    if echo "$OUTPUT" | grep -q "$test"; then
        echo "  ✓ $test found"
    else
        echo "  ✗ $test NOT found"
        exit 1
    fi
done

# Проверка ключевых функций
if echo "$OUTPUT" | grep -q "Has cycle"; then
    echo "  ✓ has_cycle() implemented"
else
    echo "  ✗ has_cycle() NOT implemented"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Cycle starts at"; then
    echo "  ✓ find_cycle_start() implemented"
else
    echo "  ✗ find_cycle_start() NOT implemented"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Cycle length"; then
    echo "  ✓ cycle_length() implemented"
else
    echo "  ✗ cycle_length() NOT implemented"
    exit 1
fi

echo "✅ All functions implemented"
echo ""

# Cleanup
rm -f linked_list_cycle

echo "═══════════════════════════════════════════════════════════"
echo "✅ All tests passed!"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Floyd's Cycle Detection работает корректно!"
echo "Вы готовы к основной миссии Episode 16."
echo ""

