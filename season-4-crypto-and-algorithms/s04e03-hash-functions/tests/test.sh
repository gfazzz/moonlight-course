#!/bin/bash
# OPERATION MOONLIGHT — s04e03 автотест
# hashes.c: djb2 + FNV-1a, лавинный эффект, корзины. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/hashes.c"
FALLBACK="../solution/hashes.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/hashes.c";
else SRC="$FALLBACK"; WHO="solution/hashes.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: отпечатки считаются, лавина измерена. Дальше — s04e04 (хеш-таблица)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
