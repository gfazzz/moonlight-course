#!/bin/bash
# OPERATION MOONLIGHT — s02e03 автотест
# ptr_arith.c: обход массива через арифметику указателей.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/ptr_arith.c"
FALLBACK="../solution/ptr_arith.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/ptr_arith.c";
else SRC="$FALLBACK"; WHO="solution/ptr_arith.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: массив пройден указателями. Дальше — s02e04 (навигация по цепочке)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
