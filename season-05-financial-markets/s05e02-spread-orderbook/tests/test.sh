#!/bin/bash
# OPERATION MOONLIGHT — s05e02 автотест
# book.c читает стакан, считает best bid/ask, спред, mid, imbalance. Тест: <bin> ../data/book.csv

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/book.c"
FALLBACK="../solution/book.c"
DATA="../data/book.csv"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/book.c";
else SRC="$FALLBACK"; WHO="solution/book.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: спред и дисбаланс посчитаны. Дальше — s05e03 (скользящие средние)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
