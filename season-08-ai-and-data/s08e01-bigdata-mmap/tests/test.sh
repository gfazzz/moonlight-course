#!/bin/bash
# OPERATION MOONLIGHT — s08e01 автотест
# bigdata.c: mmap + однопроходная статистика (Уэлфорд). Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/bigdata.c"
FALLBACK="../solution/bigdata.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/bigdata.c";
else SRC="$FALLBACK"; WHO="solution/bigdata.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "дисперсия (Уэлфорд): 835.355023" "$OUT"; then
    echo "FAIL: устойчивая дисперсия посчитана неверно — проверь метод Уэлфорда."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: файл обработан за один проход. Дальше — s08e02 (статистика и выбросы)."
rm -f "$BIN" "$OUT"
exit 0
