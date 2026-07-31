#!/bin/bash
# OPERATION MOONLIGHT — SEASON PROJECT trading_system: автотест
# Проверяет факт межсезонной сборки (S4 -> S5) и поведение.

set -u
cd "$(dirname "$0")" || exit 2
PROJ=".."; ROOT="../../.."
S4="$ROOT/season-04-crypto-and-algorithms/lib"; S5="$PROJ/../lib"

echo "--- проверка 1: модуль Season 4 используется, а не скопирован ---"
[ -f "$S4/ml_hash.c" ] || { echo "FAIL: нет $S4/ml_hash.c"; exit 1; }
grep -q '#include "ml_hash.h"' "$PROJ/main.c" || { echo "FAIL: main.c не включает ml_hash.h"; exit 1; }
grep -q "^[a-z].*ml_hash_djb2(const char.*{" "$PROJ/main.c" && { echo "FAIL: реализация djb2 скопирована в main.c"; exit 1; }
echo "  ml_hash подключён из Season 4"

echo "--- проверка 2: сборка нескольких единиц трансляции ---"
BIN="$(mktemp -u /tmp/trading_system.XXXX)"
gcc -Wall -Wextra -std=gnu11 -I"$S4" -I"$S5" -o "$BIN" \
    "$S4/ml_hash.c" "$S5/ml_indicators.c" "$PROJ/main.c" || { echo "FAIL: не линкуется."; exit 1; }
echo "  слинковано: ml_hash.c + ml_indicators.c + main.c"

echo "--- проверка 3: поведение ---"
OUT="$(mktemp /tmp/ts_out.XXXX)"
"$BIN" > "$OUT" 2>&1; rc=$?
[ $rc -eq 0 ] || { echo "FAIL: код возврата $rc"; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "RSI в диапазоне \[0, 100\]:      да" "$OUT" || { echo "FAIL: RSI вне диапазона."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "EMA быстрее SMA на тренде:     да" "$OUT" || { echo "FAIL: EMA должна опережать SMA на тренде."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "сборка работоспособна: да" "$OUT" || { echo "FAIL: самопроверка не пройдена."; rm -f "$BIN" "$OUT"; exit 1; }

diff -u expected.txt "$OUT" || { echo "FAIL: вывод не совпал."; rm -f "$BIN" "$OUT"; exit 1; }
rm -f "$BIN" "$OUT"
echo; echo "PASS: trading_system собран из модулей S4 + S5."
exit 0
