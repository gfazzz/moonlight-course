#!/bin/bash
# OPERATION MOONLIGHT — s09e03 автотест
# interp.c: интерпретатор MOON — переменные, if/else, while, print.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/interp.c"
FALLBACK="../solution/interp.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/interp.c";
else SRC="$FALLBACK"; WHO="solution/interp.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# Бесконечный цикл — типичная ошибка при неверном возврате лексера к условию.
if command -v timeout >/dev/null 2>&1; then RUN="timeout 15 $BIN"; else RUN="$BIN"; fi

$RUN > "$OUT" 2>&1
rc=$?
if [ $rc -eq 124 ]; then
    echo "FAIL: зависание. Проверь, что while перечитывает условие, возвращая позицию лексера."
    rm -f "$BIN" "$OUT"; exit 1
elif [ $rc -ne 0 ]; then
    echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1
fi

grep -q "print: 15" "$OUT"  || { echo "FAIL: сумма 1..5 должна быть 15 (while)."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "print: 120" "$OUT" || { echo "FAIL: факториал 5 должен быть 120."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "не определена" "$OUT" || { echo "FAIL: неизвестная переменная должна давать ошибку."; rm -f "$BIN" "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: язык MOON исполняется. Дальше — s09e04 (хранилище и B-дерево)."
rm -f "$BIN" "$OUT"
exit 0
