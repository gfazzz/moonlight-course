#!/bin/bash
# OPERATION MOONLIGHT — s04e05 автотест
# sorting.c: insertion vs merge со счётчиком сравнений. Детерминирован (фиксированный LCG).

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/sorting.c"
FALLBACK="../solution/sorting.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/sorting.c";
else SRC="$FALLBACK"; WHO="solution/sorting.c (fallback)"; fi
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

ASAN="$(mktemp -u /tmp/moonlight_s04e05_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s04e05_asan.log; then echo "AddressSanitizer: чисто.";
    else echo "FAIL: санитайзер нашёл проблему:"; grep -oE "(ERROR|SUMMARY): AddressSanitizer.*" /tmp/s04e05_asan.log | head -1; rm -f "$ASAN"; exit 1; fi
    rm -f "$ASAN"
fi

echo; echo "PASS: разрыв O(n^2) и O(n log n) измерен. Дальше — s04e06 (бинарный поиск)."
exit 0
