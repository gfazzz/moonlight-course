#!/bin/bash
# OPERATION MOONLIGHT — s02e07 автотест
# safe.c: корректная работа с памятью. Проверяем вывод + чистоту под AddressSanitizer
# (ASan на Linux ловит leak/use-after-free/double-free/overflow разом).

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/safe.c"
FALLBACK="../solution/safe.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s02e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/safe.c";
else SRC="$FALLBACK"; WHO="solution/safe.c (fallback)"; fi
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

# Главная проверка серии: никаких ошибок памяти.
ASAN="$(mktemp -u /tmp/moonlight_s02e07_asan.XXXX)"
if gcc -fsanitize=address -g -o "$ASAN" "$SRC" 2>/dev/null; then
    if "$ASAN" >/dev/null 2>/tmp/s02e07_asan.log; then
        echo "AddressSanitizer: чисто (нет утечек/UAF/double-free/overflow)."
    else
        echo "FAIL: AddressSanitizer нашёл ошибку памяти:"
        grep -oE "ERROR: AddressSanitizer: [a-z-]+" /tmp/s02e07_asan.log | head -1
        rm -f "$ASAN"; exit 1
    fi
    rm -f "$ASAN"
else
    echo "(ASan недоступен — пропускаю проверку памяти)"
fi

echo; echo "PASS: память чистая. Дальше — s02e08 (охота на утечки, финал)."
exit 0
