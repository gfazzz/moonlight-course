#!/bin/bash
# OPERATION MOONLIGHT — s03e05 автотест
# dissect.c: разбор IP/TCP заголовков захваченного пакета. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/dissect.c"
FALLBACK="../solution/dissect.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/dissect.c";
else SRC="$FALLBACK"; WHO="solution/dissect.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: пакет разобран по полям. Дальше — s03e06 (контрольная сумма)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
