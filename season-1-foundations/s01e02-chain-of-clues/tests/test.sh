#!/bin/bash
# OPERATION MOONLIGHT — s01e02 автотест
# classifier.c читает записи из stdin, валидирует, печатает статистику.
# Берём artifacts/classifier.c; если его нет — solution/classifier.c.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/classifier.c"
FALLBACK="../solution/classifier.c"
DATA="../data.dat"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/classifier.c";
else SRC="$FALLBACK"; WHO="solution/classifier.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" < "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: цепочка разобрана, подделки помечены. Дальше — s01e03."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
