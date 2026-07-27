#!/bin/bash
# OPERATION MOONLIGHT — s10e08 автотест (ФИНАЛ КУРСА)
# operation.c: приёмочная проверка навыков всех десяти сезонов.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/operation.c"; FALLBACK="../solution/operation.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e08.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/operation.c"; else SRC="$FALLBACK"; WHO="solution/operation.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

# Код возврата — часть контракта: непройденная фаза = ненулевой выход.
"$BIN" > "$OUT" 2>&1
rc=$?

if grep -q "\[!!\]" "$OUT"; then
    echo "FAIL: есть непройденные фазы:"; grep "\[!!\]" "$OUT"
    rm -f "$BIN" "$OUT"; exit 1
fi
if [ $rc -ne 0 ]; then
    echo "FAIL: программа обязана вернуть 0 при успешной приёмке (получено $rc)."
    rm -f "$BIN" "$OUT"; exit 1
fi

grep -q "операция завершена: УСПЕШНО" "$OUT" || { echo "FAIL: приёмка не пройдена."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "MOONLIGHT closed" "$OUT" || { echo "FAIL: нет финального подтверждения."; rm -f "$BIN" "$OUT"; exit 1; }

# Все десять сезонов обязаны быть представлены в приёмке.
python3 - "$OUT" <<'PY' || { echo "FAIL: в приёмке представлены не все сезоны S1..S10."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
seen = set(re.findall(r'\[OK\] (S\d+)', t))
need = {f"S{i}" for i in range(1, 11)}
sys.exit(0 if need <= seen else 1)
PY

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: операция завершена. КУРС ПРОЙДЕН."
rm -f "$BIN" "$OUT"; exit 0
