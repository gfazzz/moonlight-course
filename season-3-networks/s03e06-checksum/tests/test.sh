#!/bin/bash
# OPERATION MOONLIGHT — s03e06 автотест
# checksum.c: Internet checksum IP-заголовка (compute/verify/corrupt). Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/checksum.c"
FALLBACK="../solution/checksum.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/checksum.c";
else SRC="$FALLBACK"; WHO="solution/checksum.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: целостность проверяется checksum'ом. Дальше — s03e07 (реассемблинг)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
