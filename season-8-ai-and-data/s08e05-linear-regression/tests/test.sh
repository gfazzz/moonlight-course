#!/bin/bash
# OPERATION MOONLIGHT — s08e05 автотест
# regression.c: градиентный спуск против точного решения. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/regression.c"
FALLBACK="../solution/regression.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/regression.c";
else SRC="$FALLBACK"; WHO="solution/regression.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "спуск сошёлся к точному решению: да" "$OUT"; then
    echo "FAIL: спуск не сошёлся — проверь градиенты (dw = 2/N·Σe·x, db = 2/N·Σe)."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! grep -q "модель разошлась: да" "$OUT"; then
    echo "FAIL: при lr = 1.0 модель обязана разойтись — проверь шаг обновления."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: модель обучена. Дальше — s08e06 (перцептрон)."
rm -f "$BIN" "$OUT"
exit 0
