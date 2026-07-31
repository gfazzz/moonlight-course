#!/bin/bash
# OPERATION MOONLIGHT — s03e07 автотест
# reassemble.c собирает поток из сегментов по seq. Тест: <bin> ../segments.dat

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/reassemble.c"
FALLBACK="../solution/reassemble.c"
DATA="../segments.dat"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/reassemble.c";
else SRC="$FALLBACK"; WHO="solution/reassemble.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: поток собран по seq. Дальше — s03e08 (шифрованный канал, финал)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
