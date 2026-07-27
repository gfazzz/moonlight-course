#!/bin/bash
# OPERATION MOONLIGHT — s09e01 автотест
# lexer.c: поток символов -> поток токенов. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/lexer.c"
FALLBACK="../solution/lexer.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/lexer.c";
else SRC="$FALLBACK"; WHO="solution/lexer.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "всего токенов: 44" "$OUT"; then
    echo "FAIL: неверное число токенов."
    echo "Подсказка: '>=' и '!=' — ОДИН токен (longest match), комментарии пропускаются."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: поток нарезан на токены. Дальше — s09e02 (парсер и AST)."
rm -f "$BIN" "$OUT"
exit 0
