#!/bin/bash
# OPERATION MOONLIGHT — s06e04 автотест
# wiegand.c: разбор Wiegand-26, проверка двух чётностей, решение о доступе. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/wiegand.c"
FALLBACK="../solution/wiegand.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/wiegand.c";
else SRC="$FALLBACK"; WHO="solution/wiegand.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: карты разобраны, доступ решён. Дальше — s06e05 (Modbus RTU)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
