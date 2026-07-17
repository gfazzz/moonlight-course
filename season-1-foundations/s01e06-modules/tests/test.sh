#!/bin/bash
# OPERATION MOONLIGHT — s01e04 автотест (многофайловая сборка)
# Собирает crypto.c + decoder.c и сверяет вывод с expected.txt.
# Берём artifacts/*.c, если там есть decoder.c; иначе solution/*.c.

set -u
cd "$(dirname "$0")" || exit 2

EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "../artifacts/decoder.c" ] && [ -f "../artifacts/crypto.c" ]; then
    SRCS="../artifacts/crypto.c ../artifacts/decoder.c"; INC="../artifacts"; WHO="artifacts/"
else
    SRCS="../solution/crypto.c ../solution/decoder.c"; INC="../solution"; WHO="solution/ (fallback)"
fi
echo "Источник: $WHO"

echo "Компиляция ($SRCS)..."
if ! gcc -Wall -Wextra -std=c11 -I "$INC" -o "$BIN" $SRCS; then
    echo "FAIL: не компилируется/не линкуется."; exit 1
fi
echo "OK: собралось и слинковалось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: инструмент собран из модулей. Дальше — s01e07 (файлы и аргументы)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
