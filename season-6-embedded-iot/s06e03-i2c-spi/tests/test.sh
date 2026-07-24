#!/bin/bash
# OPERATION MOONLIGHT — s06e03 автотест
# i2c.c: адресация на шине, чтение/запись регистров, ACK/NACK. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/i2c.c"
FALLBACK="../solution/i2c.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/i2c.c";
else SRC="$FALLBACK"; WHO="solution/i2c.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: датчик опрошен по шине. Дальше — s06e04 (Wiegand/СКУД)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
