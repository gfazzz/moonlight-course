#!/bin/bash
# OPERATION MOONLIGHT — s06e05 автотест
# modbus.c: кадр Modbus RTU, CRC-16, разбор регистров, отбраковка. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/modbus.c"
FALLBACK="../solution/modbus.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/modbus.c";
else SRC="$FALLBACK"; WHO="solution/modbus.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: Modbus-кадр собран и проверен по CRC. Дальше — s06e06 (датчики/FSM)."; rc=0
else
    echo; echo "FAIL: вывод не совпал (проверь CRC-16: init 0xFFFF, полином 0xA001, low-байт первым)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
