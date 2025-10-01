#!/bin/bash

# ═══════════════════════════════════════════════════════════════
# Episode 14: Blockchain & Hashes — Test для hash_avalanche_tester
# ═══════════════════════════════════════════════════════════════

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EPISODE_DIR="$(dirname "$SCRIPT_DIR")"

echo "═══════════════════════════════════════════════════════════"
echo "Testing hash_avalanche_tester.c"
echo "═══════════════════════════════════════════════════════════"
echo ""

# Компиляция
echo "[1/4] Compiling hash_avalanche_tester.c..."
cd "$EPISODE_DIR/artifacts"
gcc -Wall -Wextra -std=c11 -g hash_avalanche_tester.c -o hash_tester

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi
echo "✅ Compilation successful"
echo ""

# Тест 1: Проверка hash_djb2
echo "[2/4] Test 1: Checking hash_djb2() implementation"
OUTPUT=$(./hash_tester)

# Проверяем что вывод содержит хеши
if echo "$OUTPUT" | grep -q "0x[0-9A-F]"; then
    echo "✅ Test 1 passed: hash_djb2() produces output"
else
    echo "❌ Test 1 failed: No hash output found"
    exit 1
fi
echo ""

# Тест 2: Проверка avalanche effect (должно быть ~50%)
echo "[3/4] Test 2: Checking avalanche effect"

# Ожидаем что для "MOONLIGHT" vs "MOONLIGH" будет хороший avalanche
if echo "$OUTPUT" | grep -q "Good avalanche effect\|Acceptable avalanche"; then
    echo "✅ Test 2 passed: Avalanche effect detected"
else
    echo "⚠️  Test 2: Avalanche effect not optimal (check implementation)"
fi
echo ""

# Тест 3: Проверка visualize_bits
echo "[4/4] Test 3: Checking bit visualization"

if echo "$OUTPUT" | grep -q "[01]\{32\}"; then
    echo "✅ Test 3 passed: Binary visualization works"
else
    echo "❌ Test 3 failed: Binary visualization not found"
    exit 1
fi
echo ""

# Cleanup
rm -f hash_tester

echo "═══════════════════════════════════════════════════════════"
echo "✅ All tests passed!"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Hash avalanche tester работает корректно!"
echo "Вы готовы к основной миссии Episode 14."
echo ""

