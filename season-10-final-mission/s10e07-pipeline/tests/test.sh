#!/bin/bash
# OPERATION MOONLIGHT — s10e07 автотест
# pipeline.c: сквозной конвейер. Ключевая проверка — сведение баланса.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/pipeline.c"; FALLBACK="../solution/pipeline.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e07.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/pipeline.c"; else SRC="$FALLBACK"; WHO="solution/pipeline.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

grep -q "баланс сходится: да"      "$OUT" || { echo "FAIL: баланс не сходится — записи теряются молча."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "хранилище == агрегаты:    да" "$OUT" || { echo "FAIL: хранилище и агрегаты разошлись."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "журнал == записи:         да"  "$OUT" || { echo "FAIL: журнал не соответствует записям (WAL нарушен)."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "конвейер продолжил работу после переполнения: да" "$OUT" || {
  echo "FAIL: переполнение хранилища не должно останавливать конвейер."; rm -f "$BIN" "$OUT"; exit 1; }

# Баланс проверяем ещё и арифметически, а не только по строке.
python3 - "$OUT" <<'PY' || { echo "FAIL: арифметика баланса не сходится."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
m = re.search(r'принято \+ отвергнуто = (\d+) \+ (\d+) = (\d+)', t)
n = re.search(r'подано на вход:\s+(\d+)', t)
a, r, s = map(int, m.groups()); total = int(n.group(1))
sys.exit(0 if a + r == s == total else 1)
PY

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: конвейер сведён, потерь нет. Дальше — s10e08 (финальная операция)."
rm -f "$BIN" "$OUT"; exit 0
