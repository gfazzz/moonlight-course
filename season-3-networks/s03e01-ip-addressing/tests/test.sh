#!/bin/bash
# OPERATION MOONLIGHT — s03e01 автотест
# ipinfo.c: разбор IPv4 из файла, uint32/hex/класс. Тест: <bin> ../addresses.txt

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/ipinfo.c"
FALLBACK="../solution/ipinfo.c"
DATA="../addresses.txt"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/ipinfo.c";
else SRC="$FALLBACK"; WHO="solution/ipinfo.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: адреса разобраны и классифицированы. Дальше — s03e02 (подсети)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
