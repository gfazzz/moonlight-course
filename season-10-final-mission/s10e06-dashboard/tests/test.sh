#!/bin/bash
# OPERATION MOONLIGHT — s10e06 автотест
# dashboard.c: модель/представление, возраст данных, явная деградация. Без TTY.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/dashboard.c"; FALLBACK="../solution/dashboard.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e06.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/dashboard.c"; else SRC="$FALLBACK"; WHO="solution/dashboard.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Все строки экрана обязаны быть одной ширины (кириллица внутри ячеек ломает вёрстку).
python3 - "$OUT" <<'PY' || { echo "FAIL: строки экрана разной ширины — подписи внутри экрана должны быть ASCII."; rm -f "$BIN" "$OUT"; exit 1; }
import sys
rows = [l.rstrip('\n') for l in open(sys.argv[1], encoding='utf-8')
        if l.startswith('|') and l.rstrip('\n').endswith('|')]
sys.exit(0 if rows and len({len(r) for r in rows}) == 1 else 1)
PY

grep -q "STATUS: NOMINAL"    "$OUT" || { echo "FAIL: нет штатного состояния."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "STATUS: STALE DATA" "$OUT" || { echo "FAIL: устаревание данных не отражается на экране."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "STATUS: DEGRADED"   "$OUT" || { echo "FAIL: деградация не отражается."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "no data"            "$OUT" || { echo "FAIL: отсутствующий источник должен показывать 'no data', а не старое значение."; rm -f "$BIN" "$OUT"; exit 1; }

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: экран собран. Дальше — s10e07 (конвейер moonlight_core)."
rm -f "$BIN" "$OUT"; exit 0
