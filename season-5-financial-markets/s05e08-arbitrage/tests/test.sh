#!/bin/bash
# OPERATION MOONLIGHT — s05e08 автотест (финал Season 5)
# arbitrage.c: арбитраж между биржами + детект pump&dump. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/arbitrage.c"
FALLBACK="../solution/arbitrage.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/arbitrage.c";
else SRC="$FALLBACK"; WHO="solution/arbitrage.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: арбитраж и схема pump&dump раскрыты. Season 5 пройден. Season 6 разблокирован."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
