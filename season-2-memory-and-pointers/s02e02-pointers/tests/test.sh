#!/bin/bash
# OPERATION MOONLIGHT — s02e02 автотест
# swap.c: чтение/запись через указатель и swap двух значений функцией.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/swap.c"
FALLBACK="../solution/swap.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/swap.c";
else SRC="$FALLBACK"; WHO="solution/swap.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: указатели читают и меняют данные. Дальше — s02e03 (арифметика)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
