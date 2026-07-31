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
ERR="$(mktemp /tmp/moonlight_err.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/bigdata.c";
else SRC="$FALLBACK"; WHO="solution/bigdata.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# stderr НЕ попадает в сравниваемый вывод: туда программа пишет величины,
# которые зависят от порядка операций (наивная дисперсия) — они для глаз.
"$BIN" > "$OUT" 2> "$ERR" || { echo "FAIL: падение при запуске."; cat "$OUT" "$ERR"; exit 1; }

if ! grep -q "дисперсия (Уэлфорд): 835.355023" "$OUT"; then
    echo "FAIL: устойчивая дисперсия посчитана неверно — проверь метод Уэлфорда."
    rm -f "$BIN" "$OUT" "$ERR"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT" "$ERR"; exit 1
fi

echo; echo "PASS: файл обработан за один проход. Дальше — s08e02 (статистика и выбросы)."
rm -f "$BIN" "$OUT" "$ERR"
exit 0
