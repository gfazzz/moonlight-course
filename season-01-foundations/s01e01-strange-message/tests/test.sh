#!/bin/bash
# OPERATION MOONLIGHT — s01e01 автотест
# bytecheck.c: XOR первого байта перехвата, проверка самообратимости.
# Берём artifacts/bytecheck.c; если нет — solution/bytecheck.c.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/bytecheck.c"
FALLBACK="../solution/bytecheck.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/bytecheck.c";
else SRC="$FALLBACK"; WHO="solution/bytecheck.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: ключ проверен на одном байте. Дальше — s01e02 (всё сообщение)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
