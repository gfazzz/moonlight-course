#!/bin/bash
# OPERATION MOONLIGHT — s06e01 автотест
# gpio.c: битовые операции над регистрами DDR/PORT/PIN. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/gpio.c"
FALLBACK="../solution/gpio.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/gpio.c";
else SRC="$FALLBACK"; WHO="solution/gpio.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: регистры и биты под контролем. Дальше — s06e02 (UART)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
