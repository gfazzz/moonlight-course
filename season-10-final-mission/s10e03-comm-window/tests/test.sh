#!/bin/bash
# OPERATION MOONLIGHT — s10e03 автотест
# window.c: угол места, границы окна связи, доплеровский сдвиг. Без -lm.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/window.c"; FALLBACK="../solution/window.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e03.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/window.c"; else SRC="$FALLBACK"; WHO="solution/window.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется (тригонометрию пиши сам, без -lm)."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Свойство 1: окно должно быть найдено и быть правдоподобным для НОО (5-15 мин).
python3 - "$OUT" <<'PY' || { echo "FAIL: длительность окна нереалистична для низкой орбиты."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
m = re.search(r'длительность окна: ([\d.]+) с', t)
sys.exit(0 if m and 300 < float(m.group(1)) < 900 else 1)
PY

# Свойство 2: доплеровский сдвиг обязан менять знак (сближение -> удаление).
python3 - "$OUT" <<'PY' || { echo "FAIL: доплеровский сдвиг не меняет знак — проверь радиальную скорость."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
blk = t.split('доплеровский сдвиг')[1]
vals = [float(x) for x in re.findall(r'(-?\d+\.\d+)\s*$', blk, re.M)]
sys.exit(0 if vals and max(vals) > 0 and min(vals) < 0 else 1)
PY

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: окно связи вычислено. Дальше — s10e04 (профилирование)."
rm -f "$BIN" "$OUT"; exit 0
