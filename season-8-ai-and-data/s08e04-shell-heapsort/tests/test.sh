#!/bin/bash
# OPERATION MOONLIGHT — s08e04 автотест
# sorts.c: Shell sort (шаги Кнута) и heapsort, честные замеры операций.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/sorts.c"
FALLBACK="../solution/sorts.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/sorts.c";
else SRC="$FALLBACK"; WHO="solution/sorts.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if grep -q "отсортирован: НЕТ" "$OUT"; then
    echo "FAIL: какой-то из алгоритмов не отсортировал массив."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    echo "Подсказка: шаги Кнута — h = 3h+1, затем h /= 3; в heapsort просеивай вниз."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: обе сортировки работают. Дальше — s08e05 (линейная регрессия)."
rm -f "$BIN" "$OUT"
exit 0
