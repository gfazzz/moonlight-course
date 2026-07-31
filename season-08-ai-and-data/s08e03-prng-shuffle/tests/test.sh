#!/bin/bash
# OPERATION MOONLIGHT — s08e03 автотест
# shuffle.c: смещение наивного тасования, Фишер–Йетс, modulo bias. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/shuffle.c"
FALLBACK="../solution/shuffle.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/shuffle.c";
else SRC="$FALLBACK"; WHO="solution/shuffle.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "максимальное отклонение: 179" "$OUT"; then
    echo "FAIL: перемешивание Кнута должно давать почти равномерные перестановки."
    echo "Подсказка: на шаге i меняем местами с индексом из [0, i], а не из всего массива."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: перемешивание честное. Дальше — s08e04 (Shell и heapsort)."
rm -f "$BIN" "$OUT"
exit 0
