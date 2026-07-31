#!/bin/bash
# OPERATION MOONLIGHT — s01e03 автотест
# pattern_finder.c читает метки времени из stdin, ищет период и аномалии.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/pattern_finder.c"
FALLBACK="../solution/pattern_finder.c"
DATA="../data.txt"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/pattern_finder.c";
else SRC="$FALLBACK"; WHO="solution/pattern_finder.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" < "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: период найден, сбой расписания локализован. Дальше — s01e05."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
