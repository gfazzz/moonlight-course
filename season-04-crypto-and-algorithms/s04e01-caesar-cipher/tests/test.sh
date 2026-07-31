#!/bin/bash
# OPERATION MOONLIGHT — s04e01 автотест
# caesar.c: шифр со сдвигом + брутфорс всех 25 ключей. Вывод детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/caesar.c"
FALLBACK="../solution/caesar.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/caesar.c";
else SRC="$FALLBACK"; WHO="solution/caesar.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: шифр Цезаря взломан перебором. Дальше — s04e02 (частотный анализ)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
