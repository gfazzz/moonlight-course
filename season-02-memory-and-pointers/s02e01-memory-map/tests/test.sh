#!/bin/bash
# OPERATION MOONLIGHT — s02e01 автотест
# memmap.c: размеры типов и непрерывность массива. Вывод детерминирован на x86-64.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/memmap.c"
FALLBACK="../solution/memmap.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/memmap.c";
else SRC="$FALLBACK"; WHO="solution/memmap.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: карта памяти построена. Дальше — s02e02 (указатели)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    echo "Подсказка: на не-x86-64 размеры типов могут отличаться."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
