#!/bin/bash
# OPERATION MOONLIGHT — s05e07 автотест
# montecarlo.c: секвенциальная симуляция P&L на seeded LCG. Детерминирован, без -lm.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/montecarlo.c"
FALLBACK="../solution/montecarlo.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/montecarlo.c";
else SRC="$FALLBACK"; WHO="solution/montecarlo.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: распределение P&L смоделировано. Дальше — s05e08 (арбитраж, финал)."; rc=0
else
    echo; echo "FAIL: вывод не совпал (проверь seeded LCG: библиотечный rand() не воспроизводим)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
