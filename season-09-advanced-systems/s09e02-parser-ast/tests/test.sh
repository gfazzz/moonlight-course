#!/bin/bash
# OPERATION MOONLIGHT — s09e02 автотест
# parser.c: рекурсивный спуск, AST, приоритеты и ассоциативность.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/parser.c"
FALLBACK="../solution/parser.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/parser.c";
else SRC="$FALLBACK"; WHO="solution/parser.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q '(+ 3 (\* 4 2))' "$OUT"; then
    echo "FAIL: приоритет операций нарушен — умножение должно связывать сильнее сложения."
    rm -f "$BIN" "$OUT"; exit 1
fi
if ! grep -q '(- (- 100 20) 5)' "$OUT"; then
    echo "FAIL: сложение/вычитание должны быть ЛЕВОассоциативны: (100-20)-5."
    rm -f "$BIN" "$OUT"; exit 1
fi
if [ "$(grep -c 'ошибка разбора' "$OUT")" -ne 2 ]; then
    echo "FAIL: обе некорректные строки должны дать ошибку разбора."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: дерево разбора построено. Дальше — s09e03 (интерпретатор)."
rm -f "$BIN" "$OUT"
exit 0
