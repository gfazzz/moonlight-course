#!/bin/bash
# OPERATION MOONLIGHT — s02e08 автотест (финал Season 2)
# fixed_logger.c: логгер без утечек. Вывод + чистота под AddressSanitizer/LSan.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/fixed_logger.c"
FALLBACK="../solution/fixed_logger.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/fixed_logger.c";
else SRC="$FALLBACK"; WHO="solution/fixed_logger.c (fallback)"; fi
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

# Главная проверка финала: НИ ОДНОЙ утечки.
ASAN="$(mktemp -u /tmp/moonlight_s02e08_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s02e08_asan.log; then
        echo "AddressSanitizer/LSan: без утечек."
    else
        echo "FAIL: санитайзер нашёл проблему:"
        grep -oE "(ERROR|SUMMARY): AddressSanitizer.*|LeakSanitizer.*" /tmp/s02e08_asan.log | head -2
        rm -f "$ASAN"; exit 1
    fi
    rm -f "$ASAN"
else
    echo "(ASan недоступен — пропускаю проверку утечек)"
fi

echo; echo "PASS: логгер обезврежен, утечек нет. Season 2 пройден. Season 3 разблокирован."
exit 0
