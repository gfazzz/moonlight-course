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

# Все строки экрана обязаны быть одной ширины В ЗНАКОМЕСТАХ — не в байтах
# и не в символах. Строка с иероглифами короче остальных по числу символов
# и длиннее по числу байт, а на экране обязана вставать вровень.
python3 - "$OUT" <<'PY' || { echo "FAIL: строки экрана разной ширины — проверь отсечение и учёт ширины символов."; rm -f "$BIN" "$OUT"; exit 1; }
import sys, unicodedata
def width(s):
    w = 0
    for c in s:
        if unicodedata.combining(c): continue
        w += 2 if unicodedata.east_asian_width(c) in 'WF' else 1
    return w
rows = [l.rstrip('\n') for l in open(sys.argv[1], encoding='utf-8')
        if l.startswith('|') and l.rstrip('\n').endswith('|')]
if not rows: sys.exit(1)
widths = {width(r) for r in rows}
if len(widths) != 1:
    print("  ширины строк:", sorted(widths), file=sys.stderr)
    sys.exit(1)
# И отдельно: хотя бы одна строка обязана быть не-ASCII, иначе проверка пуста.
sys.exit(0 if any(any(ord(c) > 127 for c in r) for r in rows) else 1)
PY

grep -q "изменено ячеек: 0 из 480" "$OUT" || {
    echo "FAIL: одинаковый кадр должен давать 0 изменённых ячеек."
    rm -f "$BIN" "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: экран рисуется, диффится и не боится не-ASCII. Дальше — s09e07 (радар и фильтр Калмана)."
rm -f "$BIN" "$OUT"
exit 0
