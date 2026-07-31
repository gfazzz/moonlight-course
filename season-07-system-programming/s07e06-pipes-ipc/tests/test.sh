#!/bin/bash
# OPERATION MOONLIGHT — s07e06 автотест
# pipes.c: pipe(), двусторонний обмен, конвейер через dup2. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/pipes.c"
FALLBACK="../solution/pipes.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/pipes.c";
else SRC="$FALLBACK"; WHO="solution/pipes.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# Таймаут: незакрытый конец канала = вечное ожидание EOF.
if command -v timeout >/dev/null 2>&1; then RUN="timeout 10 $BIN"; else RUN="$BIN"; fi

$RUN > "$OUT" 2>&1
rc=$?
if [ $rc -eq 124 ]; then
    echo "FAIL: программа зависла (10 c). Скорее всего, не закрыт неиспользуемый конец канала."
    rm -f "$BIN" "$OUT"; exit 1
elif [ $rc -ne 0 ]; then
    echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

$RUN > "$OUT" 2>&1
if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
    echo; echo "FAIL: вывод нестабилен между запусками."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: процессы поговорили через канал. Дальше — s07e07 (mmap)."
rm -f "$BIN" "$OUT"
exit 0
