#!/bin/bash

# ═══════════════════════════════════════════════════════════════
# Episode 15: Sorting & Search — Test для sorting_race
# ═══════════════════════════════════════════════════════════════

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EPISODE_DIR="$(dirname "$SCRIPT_DIR")"

echo "═══════════════════════════════════════════════════════════"
echo "Testing sorting_race.c"
echo "═══════════════════════════════════════════════════════════"
echo ""

# Компиляция
echo "[1/3] Compiling sorting_race.c..."
cd "$EPISODE_DIR/artifacts"
gcc -Wall -Wextra -std=c11 -O2 -g sorting_race.c -o sorting_race

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi
echo "✅ Compilation successful"
echo ""

# Тест 1: Запуск программы
echo "[2/3] Running sorting_race..."
OUTPUT=$(./sorting_race 2>&1)

if [ $? -ne 0 ]; then
    echo "❌ Program crashed!"
    exit 1
fi
echo "✅ Program runs successfully"
echo ""

# Тест 2: Проверка что все алгоритмы протестированы
echo "[3/3] Checking algorithm coverage..."

REQUIRED_ALGOS=("Bubble Sort" "Insertion Sort" "Quick Sort" "Heapsort")

for algo in "${REQUIRED_ALGOS[@]}"; do
    if echo "$OUTPUT" | grep -q "$algo"; then
        echo "  ✓ $algo tested"
    else
        echo "  ✗ $algo NOT found"
        exit 1
    fi
done

echo "✅ All algorithms tested"
echo ""

# Cleanup
rm -f sorting_race

echo "═══════════════════════════════════════════════════════════"
echo "✅ All tests passed!"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Sorting race работает корректно!"
echo "Вы готовы к основной миссии Episode 15."
echo ""

