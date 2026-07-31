#!/bin/bash
# OPERATION MOONLIGHT — s07e05 автотест
# race.c: data race и мьютекс.
# ВАЖНО: stdout содержит только инварианты (детерминирован), а плавающие числа
# программа пишет в stderr — поэтому здесь stderr намеренно отбрасывается.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/race.c"
FALLBACK="../solution/race.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/race.c";
else SRC="$FALLBACK"; WHO="solution/race.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -pthread -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (не забудь -pthread)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>/dev/null || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "итог в точности равен ожидаемому:                      да" "$OUT"; then
    echo "FAIL: версия с мьютексом не даёт точный результат — проверь lock/unlock."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    echo "Подсказка: плавающие числа печатай в stderr, в stdout — только инварианты."
    rm -f "$BIN" "$OUT"; exit 1
fi

# Пять прогонов: планировщик меняется, инварианты — нет.
for i in 1 2 3 4 5; do
    "$BIN" > "$OUT" 2>/dev/null
    if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
        echo; echo "FAIL: stdout нестабилен (прогон $i)."
        rm -f "$BIN" "$OUT"; exit 1
    fi
done

echo; echo "PASS: гонка показана, мьютекс чинит. Дальше — s07e06 (каналы/IPC)."
rm -f "$BIN" "$OUT"
exit 0
