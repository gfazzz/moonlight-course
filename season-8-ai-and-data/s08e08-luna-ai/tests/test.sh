#!/bin/bash
# OPERATION MOONLIGHT — s08e08 автотест (финал Season 8)
# luna_ai.c: полный цикл ML на C — данные, шаффл, обучение, метрики.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/luna_ai.c"
FALLBACK="../solution/luna_ai.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/luna_ai.c";
else SRC="$FALLBACK"; WHO="solution/luna_ai.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (exp и sqrt — свои, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "F1:        0.947" "$OUT"; then
    echo "FAIL: качество модели не совпало — проверь нормировку, шаффл и backprop."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: luna_ai обучена и честно оценена. Season 8 пройден. Season 9 разблокирован."
rm -f "$BIN" "$OUT"
exit 0
