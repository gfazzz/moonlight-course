#!/bin/bash
# OPERATION MOONLIGHT — s01e05 автотест (финальная многофайловая сборка)
# Собирает crypto.c + decoder.c и сверяет отчёт с expected.txt.

set -u
cd "$(dirname "$0")" || exit 2

EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "../artifacts/decoder.c" ] && [ -f "../artifacts/crypto.c" ]; then
    SRCS="../artifacts/crypto.c ../artifacts/decoder.c"; INC="../artifacts"; WHO="artifacts/"
else
    SRCS="../solution/crypto.c ../solution/decoder.c"; INC="../solution"; WHO="solution/ (fallback)"
fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -I "$INC" -o "$BIN" $SRCS; then
    echo "FAIL: не компилируется/не линкуется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: moonlight_decoder собран. Season 1 пройден. Season 2 разблокирован."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
