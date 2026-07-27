#!/bin/bash
# OPERATION MOONLIGHT — s09e07 автотест
# kalman.c: сопровождение цели по шумному радару. Детерминирован, без -lm.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/kalman.c"
FALLBACK="../solution/kalman.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e07.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/kalman.c";
else SRC="$FALLBACK"; WHO="solution/kalman.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (sqrt реализуй сам, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Главный критерий: фильтр обязан быть ТОЧНЕЕ сырого измерения.
python3 - "$OUT" <<'PY' || { echo "FAIL: оценка фильтра не точнее сырого радара — проверь ковариацию 2x2 и K1."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
m = float(re.search(r'ошибка измерения:\s*([\d.]+)', t).group(1))
e = float(re.search(r'ошибка оценки:\s*([\d.]+)', t).group(1))
sys.exit(0 if e < m else 1)
PY

# И должен выучить скорость, измеряя только координату.
python3 - "$OUT" <<'PY' || { echo "FAIL: скорость не сошлась к истинной — внедиагональные элементы P не работают."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
v = float(re.search(r'оценённая скорость:\s*([\d.]+)', t).group(1))
sys.exit(0 if abs(v - 12.0) < 1.0 else 1)
PY

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: цель сопровождается точнее радара. Дальше — s09e08 (квант, финал)."
rm -f "$BIN" "$OUT"
exit 0
