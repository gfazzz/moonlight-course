#!/bin/bash
# OPERATION MOONLIGHT — s10e02 автотест
# orbit.c: уравнение Кеплера, положение спутника, законы сохранения. Без -lm.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/orbit.c"; FALLBACK="../solution/orbit.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e02.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/orbit.c"; else SRC="$FALLBACK"; WHO="solution/orbit.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется (тригонометрию и sqrt пиши сам, без -lm)."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

grep -q "законы сохранения выполняются: да" "$OUT" || {
  echo "FAIL: энергия/момент импульса не сохраняются — проверь решение уравнения Кеплера."
  rm -f "$BIN" "$OUT"; exit 1; }
grep -q "секторная скорость постоянна: да" "$OUT" || {
  echo "FAIL: второй закон Кеплера не выполняется (v*r должно быть одинаково в перигее и апогее)."
  rm -f "$BIN" "$OUT"; exit 1; }

# Свойство: спутник обязан вернуться в исходную точку ровно через период.
python3 - "$OUT" <<'PY' || { echo "FAIL: за полный период спутник не вернулся в исходную точку."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
rows = re.findall(r'^\s*([\d.]+)\s+(-?[\d.]+)\s+(-?[\d.]+)\s+(\d+)\s+([\d.]+)', 
                  open(sys.argv[1], encoding='utf-8').read(), re.M)
sys.exit(0 if rows and abs(float(rows[0][4]) - float(rows[-1][4])) < 1e-6 else 1)
PY

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: орбита посчитана. Дальше — s10e03 (окно связи)."
rm -f "$BIN" "$OUT"; exit 0
