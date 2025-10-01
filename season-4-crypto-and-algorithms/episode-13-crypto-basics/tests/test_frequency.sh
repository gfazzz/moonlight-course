#!/bin/bash

# ═══════════════════════════════════════════════════════════════
# Episode 13: Crypto Basics — Test для frequency_analyzer
# ═══════════════════════════════════════════════════════════════

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EPISODE_DIR="$(dirname "$SCRIPT_DIR")"

echo "═══════════════════════════════════════════════════════════"
echo "Testing frequency_analyzer.c"
echo "═══════════════════════════════════════════════════════════"
echo ""

# Компиляция
echo "[1/4] Compiling frequency_analyzer.c..."
cd "$EPISODE_DIR/artifacts"
gcc -Wall -Wextra -std=c11 -g frequency_analyzer.c -o frequency_analyzer

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi
echo "✅ Compilation successful"
echo ""

# Тест 1: Обычный английский текст
echo "[2/4] Test 1: English text (sample_english.txt)"
./frequency_analyzer "$EPISODE_DIR/tests/sample_english.txt" > /tmp/freq_test1.txt

# Проверяем что E — самая частая буква
if grep -q "Most frequent letter: E" /tmp/freq_test1.txt; then
    echo "✅ Test 1 passed: 'E' is most frequent"
else
    echo "❌ Test 1 failed: Expected 'E' as most frequent"
    cat /tmp/freq_test1.txt
    exit 1
fi
echo ""

# Тест 2: Caesar cipher (shift=7)
echo "[3/4] Test 2: Caesar cipher text (sample_caesar.txt, shift=7)"
./frequency_analyzer "$EPISODE_DIR/tests/sample_caesar.txt" > /tmp/freq_test2.txt

# В Caesar shift=7: E → L, поэтому L должна быть самой частой
if grep -q "Most frequent letter: L" /tmp/freq_test2.txt; then
    echo "✅ Test 2 passed: 'L' is most frequent (E shifted by 7)"
else
    echo "❌ Test 2 failed: Expected 'L' as most frequent"
    cat /tmp/freq_test2.txt
    exit 1
fi

# Проверяем автоопределение shift
if grep -q "Detected Caesar shift: 7" /tmp/freq_test2.txt; then
    echo "✅ Test 2 passed: Detected shift=7"
else
    echo "❌ Test 2 failed: Expected shift=7"
    cat /tmp/freq_test2.txt
    exit 1
fi
echo ""

# Тест 3: Проверка вывода гистограммы
echo "[4/4] Test 3: Histogram output format"
if grep -E "^[A-Z]: .*%.*\([0-9]+\)" /tmp/freq_test1.txt > /dev/null; then
    echo "✅ Test 3 passed: Histogram format correct"
else
    echo "❌ Test 3 failed: Histogram format incorrect"
    exit 1
fi
echo ""

# Cleanup
rm -f /tmp/freq_test1.txt /tmp/freq_test2.txt

echo "═══════════════════════════════════════════════════════════"
echo "✅ All tests passed!"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Frequency analyzer работает корректно!"
echo "Вы готовы к основной миссии Episode 13."
echo ""

