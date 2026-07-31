#!/bin/bash
# OPERATION MOONLIGHT — s04e06 автотест
# search.c: линейный vs двоичный поиск со счётчиком шагов. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/search.c"
FALLBACK="../solution/search.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/search.c";
else SRC="$FALLBACK"; WHO="solution/search.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }
if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rm -f "$BIN" "$OUT"; exit 1
fi
rm -f "$BIN" "$OUT"

ASAN="$(mktemp -u /tmp/moonlight_s04e06_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s04e06_asan.log; then echo "AddressSanitizer: чисто.";
    else echo "FAIL: санитайзер нашёл проблему:"; grep -oE "(ERROR|SUMMARY): AddressSanitizer.*" /tmp/s04e06_asan.log | head -1; rm -f "$ASAN"; exit 1; fi
    rm -f "$ASAN"
fi

echo; echo "PASS: log n подтверждён замером. Дальше — s04e07 (связный список)."
exit 0
