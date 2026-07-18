#!/bin/bash
# OPERATION MOONLIGHT — s04e04 автотест
# hashtable.c: таблица с цепочками. Вывод + проверка утечек (ASan).

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/hashtable.c"
FALLBACK="../solution/hashtable.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/hashtable.c";
else SRC="$FALLBACK"; WHO="solution/hashtable.c (fallback)"; fi
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

# Таблица выделяет память под узлы и ключи — проверяем, что всё освобождено.
ASAN="$(mktemp -u /tmp/moonlight_s04e04_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s04e04_asan.log; then
        echo "AddressSanitizer: чисто (таблица освобождена)."
    else
        echo "FAIL: санитайзер нашёл проблему:"
        grep -oE "(ERROR|SUMMARY): AddressSanitizer.*|LeakSanitizer.*" /tmp/s04e04_asan.log | head -2
        rm -f "$ASAN"; exit 1
    fi
    rm -f "$ASAN"
fi

echo; echo "PASS: коллизии обработаны цепочками. Дальше — s04e05 (сортировки)."
exit 0
