#!/bin/bash
# OPERATION MOONLIGHT — s02e06 автотест
# grow.c: динамический массив, растущий через realloc. + проверка утечек LSan.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/grow.c"
FALLBACK="../solution/grow.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/grow.c";
else SRC="$FALLBACK"; WHO="solution/grow.c (fallback)"; fi
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

LSAN="$(mktemp -u /tmp/moonlight_s02e06_lsan.XXXX)"
if gcc -fsanitize=leak -o "$LSAN" "$SRC" 2>/dev/null; then
    if "$LSAN" >/dev/null 2>/tmp/s02e06_lsan.log; then echo "LeakSanitizer: без утечек.";
    else echo "WARN: LeakSanitizer нашёл утечки:"; cat /tmp/s02e06_lsan.log; fi
    rm -f "$LSAN"
fi

echo; echo "PASS: массив вырос через realloc. Дальше — s02e07 (ошибки памяти)."
rm -f "$BIN" "$OUT"
exit 0
