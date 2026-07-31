#!/bin/bash
# OPERATION MOONLIGHT — s06e07 автотест
# motion.c: детект движения разностью кадров, пороги, зоны. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/motion.c"
FALLBACK="../solution/motion.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/motion.c";
else SRC="$FALLBACK"; WHO="solution/motion.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: движение найдено, слепые зоны определены. Дальше — s06e08 (MAVLink, финал)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
