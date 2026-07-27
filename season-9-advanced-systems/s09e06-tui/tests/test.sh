#!/bin/bash
# OPERATION MOONLIGHT — s09e06 автотест
# tui.c: буфер экрана, рамки, диффер кадров. Не требует терминала — вывод в память.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/tui.c"
FALLBACK="../solution/tui.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/tui.c";
else SRC="$FALLBACK"; WHO="solution/tui.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# Вывод идёт в канал, а не в терминал: рендер обязан работать без TTY.
"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Все строки экрана обязаны быть одной ширины (иначе вёрстка поехала).
python3 - "$OUT" <<'PY' || { echo "FAIL: строки экрана разной ширины — проверь отсечение и ASCII-текст."; rm -f "$BIN" "$OUT"; exit 1; }
import sys
rows = [l.rstrip('\n') for l in open(sys.argv[1], encoding='utf-8') if l.startswith('|') and l.rstrip('\n').endswith('|')]
sys.exit(0 if rows and len({len(r) for r in rows}) == 1 else 1)
PY

grep -q "изменено ячеек: 0 из 480" "$OUT" || {
    echo "FAIL: одинаковый кадр должен давать 0 изменённых ячеек."
    rm -f "$BIN" "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: экран рисуется и диффится. Дальше — s09e07 (радар и фильтр Калмана)."
rm -f "$BIN" "$OUT"
exit 0
