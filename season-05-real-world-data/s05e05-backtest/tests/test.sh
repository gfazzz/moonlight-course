#!/bin/bash
# OPERATION MOONLIGHT — s05e05 автотест
# backtest.c: SMA-crossover бэктест + grid search + OOS-проверка. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/backtest.c"
FALLBACK="../solution/backtest.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/backtest.c";
else SRC="$FALLBACK"; WHO="solution/backtest.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: overfitting виден — in-sample winner проваливается на OOS. Дальше — s05e06 (тиковый поток)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
