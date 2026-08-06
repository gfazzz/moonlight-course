#!/bin/bash
# OPERATION MOONLIGHT — s05e06 автотест
# tick.c пишет бинарный поток тиков, читает его обратно и сравнивает размеры.
# Путь файла передаётся аргументом: два экземпляра на одной машине не дерутся.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/tick.c"
FALLBACK="../solution/tick.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e06.XXXX)"
DAT="$(mktemp -u /tmp/moonlight_ticks.XXXX.bin)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"
OUT2="$(mktemp /tmp/moonlight_out2.XXXX)"
cleanup() { rm -f "$BIN" "$DAT" "$OUT" "$OUT2"; }

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/tick.c";
else SRC="$FALLBACK"; WHO="solution/tick.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# stderr в сравнение не идёт: туда программа пишет раскладку байт этой
# машины — величину, которая на разных платформах разная (см. s08e01).
"$BIN" "$DAT" > "$OUT" 2>/dev/null || { echo "FAIL: падение при запуске."; cat "$OUT"; cleanup; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; cleanup; exit 1
fi

if [ -f "$DAT" ]; then
    echo "FAIL: временный файл $DAT остался после работы."; cleanup; exit 1
fi

gcc -Wall -Wextra -std=c11 -O2 -o "$BIN" "$SRC" 2>/dev/null || {
    echo "FAIL: не собирается с -O2."; cleanup; exit 1; }
"$BIN" "$DAT" > "$OUT2" 2>/dev/null
if ! diff -u "$OUT" "$OUT2"; then
    echo; echo "FAIL: вывод изменился при -O2."; cleanup; exit 1
fi

echo; echo "PASS: формат разобран, лишнее поле найдено. Дальше — s05e07 (Монте-Карло)."
cleanup
exit 0
