#!/bin/bash
# OPERATION MOONLIGHT — s07e07 автотест
# shared.c: mmap MAP_SHARED vs MAP_PRIVATE, межпроцессный мьютекс. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/shared.c"
FALLBACK="../solution/shared.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/shared.c";
else SRC="$FALLBACK"; WHO="solution/shared.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -pthread -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (не забудь -pthread)."; exit 1
fi
echo "OK: собралось."

if command -v timeout >/dev/null 2>&1; then RUN="timeout 20 $BIN"; else RUN="$BIN"; fi

$RUN > "$OUT" 2>&1
rc=$?
if [ $rc -eq 124 ]; then
    echo "FAIL: зависание. Мьютекс в разделяемой памяти требует PTHREAD_PROCESS_SHARED."
    rm -f "$BIN" "$OUT"; exit 1
elif [ $rc -ne 0 ]; then
    echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1
fi

if ! grep -q "ожидалось 200000): да" "$OUT"; then
    echo "FAIL: счётчик в общей памяти неточен — проверь межпроцессный мьютекс."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

for i in 1 2 3; do
    $RUN > "$OUT" 2>&1
    if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
        echo; echo "FAIL: вывод нестабилен (прогон $i)."
        rm -f "$BIN" "$OUT"; exit 1
    fi
done

echo; echo "PASS: память разделена, гонок нет. Дальше — s07e08 (сигналы, финал)."
rm -f "$BIN" "$OUT"
exit 0
