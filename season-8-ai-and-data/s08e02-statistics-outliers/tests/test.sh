#!/bin/bash
# OPERATION MOONLIGHT — s08e02 автотест
# stats.c: устойчивые оценки, IQR/Тьюки против z-score. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/stats.c"
FALLBACK="../solution/stats.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/stats.c";
else SRC="$FALLBACK"; WHO="solution/stats.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (sqrt реализуй сам, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "найдено выбросов (Тьюки): 2" "$OUT"; then
    echo "FAIL: правило Тьюки должно найти оба выброса — проверь Q1/Q3 и IQR."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: выбросы найдены устойчиво. Дальше — s08e03 (ГПСЧ и Кнут)."
rm -f "$BIN" "$OUT"
exit 0
