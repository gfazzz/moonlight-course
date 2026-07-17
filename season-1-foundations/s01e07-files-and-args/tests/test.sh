#!/bin/bash
# OPERATION MOONLIGHT — s01e07 автотест
# decoder.c читает зашифрованный файл, имя которого дано аргументом (argv[1]).
# Тест всегда запускает: <bin> ../message.enc

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/decoder.c"
FALLBACK="../solution/decoder.c"
DATA="../message.enc"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/decoder.c";
else SRC="$FALLBACK"; WHO="solution/decoder.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# argv[1] = ../message.enc (относительно tests/)
"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: файл прочитан и расшифрован. Дальше — s01e08 (struct + финал)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
