#!/bin/bash
# OPERATION MOONLIGHT — s07e02 автотест
# exec.c: замена образа процесса, fork+exec, коды возврата. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/exec.c"
FALLBACK="../solution/exec.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e02.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/exec.c";
else SRC="$FALLBACK"; WHO="solution/exec.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

"$BIN" > "$OUT" 2>&1
if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
    echo; echo "FAIL: вывод нестабилен между запусками."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: образ процесса заменён, коды сняты. Дальше — s07e03 (демоны)."
rm -f "$BIN" "$OUT"
exit 0
