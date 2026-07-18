#!/bin/bash
# OPERATION MOONLIGHT — s04e07 автотест
# list.c: список, разворот, цикл Флойда. Вывод + проверка утечек (ASan).

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/list.c"
FALLBACK="../solution/list.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/list.c";
else SRC="$FALLBACK"; WHO="solution/list.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# Таймаут: незакрытый цикл в списке -> бесконечный обход.
if command -v timeout >/dev/null 2>&1; then RUN="timeout 10 $BIN"; else RUN="$BIN"; fi
$RUN > "$OUT" 2>&1
rc_run=$?
if [ $rc_run -eq 124 ]; then echo "FAIL: зависание (похоже, цикл не разорван)."; rm -f "$BIN" "$OUT"; exit 1; fi
if [ $rc_run -ne 0 ]; then echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1; fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rm -f "$BIN" "$OUT"; exit 1
fi
rm -f "$BIN" "$OUT"

ASAN="$(mktemp -u /tmp/moonlight_s04e07_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s04e07_asan.log; then echo "AddressSanitizer: чисто (список освобождён).";
    else echo "FAIL: санитайзер нашёл проблему:"; grep -oE "(ERROR|SUMMARY): AddressSanitizer.*|LeakSanitizer.*" /tmp/s04e07_asan.log | head -2; rm -f "$ASAN"; exit 1; fi
    rm -f "$ASAN"
fi

echo; echo "PASS: список, разворот и цикл Флойда работают. Дальше — s04e08 (блокчейн, финал)."
exit 0
