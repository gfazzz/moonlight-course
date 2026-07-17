#!/bin/bash
# OPERATION MOONLIGHT — s03e04 автотест
# echo.c: сервер+клиент через fork на loopback. Печатает только родитель -> детерминизм.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/echo.c"
FALLBACK="../solution/echo.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/echo.c";
else SRC="$FALLBACK"; WHO="solution/echo.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1
rc_run=$?
if [ $rc_run -ne 0 ]; then echo "FAIL: обмен не удался (exit $rc_run)."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1; fi

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: сокет-диалог состоялся. Дальше — s03e05 (структура пакета)."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
