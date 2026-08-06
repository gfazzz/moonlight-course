#!/bin/bash
# OPERATION MOONLIGHT — s05e01 автотест
# parse.c читает OHLCV CSV, считает сводку и аномалии объёма. Тест: <bin> ../data/prices.csv

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/parse.c"
FALLBACK="../solution/parse.c"
DATA="../data/prices.csv"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/parse.c";
else SRC="$FALLBACK"; WHO="solution/parse.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: свечи распарсены, аномалии объёма найдены. Дальше — s05e02 (деньги)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
