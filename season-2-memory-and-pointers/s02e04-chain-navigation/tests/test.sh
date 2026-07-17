#!/bin/bash
# OPERATION MOONLIGHT — s02e04 автотест
# navigator.c читает pointers.dat и идёт по цепочке next. Тест: <bin> ../pointers.dat

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/navigator.c"
FALLBACK="../solution/navigator.c"
DATA="../pointers.dat"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/navigator.c";
else SRC="$FALLBACK"; WHO="solution/navigator.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: цепочка пройдена, сообщение собрано. Дальше — s02e05 (куча)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
