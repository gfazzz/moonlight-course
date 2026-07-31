#!/bin/bash
# OPERATION MOONLIGHT — s05e06 автотест
# var.c: волатильность + VaR (исторический и параметрический). Детерминирован, без -lm.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/var.c"
FALLBACK="../solution/var.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/var.c";
else SRC="$FALLBACK"; WHO="solution/var.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (подсказка: реализуй sqrt сам, не тяни -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: волатильность и VaR посчитаны. Дальше — s05e07 (Monte Carlo)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
