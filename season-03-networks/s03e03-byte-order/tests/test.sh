#!/bin/bash
# OPERATION MOONLIGHT — s03e03 автотест
# byteorder.c: host vs network byte order (htons/htonl). Детерминирован на x86-64.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/byteorder.c"
FALLBACK="../solution/byteorder.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/byteorder.c";
else SRC="$FALLBACK"; WHO="solution/byteorder.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: порядок байтов понятен. Дальше — s03e04 (сокеты)."; rc=0
else
    echo; echo "FAIL: вывод не совпал (на не-x86-64 порядок байтов иной — это ожидаемо)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
