#!/bin/bash
# OPERATION MOONLIGHT — s08e07 автотест
# backprop.c: сеть со скрытым слоем решает XOR. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/backprop.c"
FALLBACK="../solution/backprop.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/backprop.c";
else SRC="$FALLBACK"; WHO="solution/backprop.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (exp реализуй сам, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "верно: 4 из 4" "$OUT"; then
    echo "FAIL: сеть не решила XOR."
    echo "Подсказка: d_h[j] = d_out * w2[j] * h[j] * (1 - h[j]); веса стартуют несимметрично."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: XOR решён скрытым слоем. Дальше — s08e08 (luna_ai, финал)."
rm -f "$BIN" "$OUT"
exit 0
