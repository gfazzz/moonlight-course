#!/bin/bash
# OPERATION MOONLIGHT — s09e08 автотест (финал Season 9)
# quantum.c: симулятор кубитов, запутанность, поиск Гровера.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/quantum.c"
FALLBACK="../solution/quantum.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/quantum.c";
else SRC="$FALLBACK"; WHO="solution/quantum.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (sqrt реализуй сам, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Нормировка: сумма вероятностей всегда 1 — иначе гейты реализованы неверно.
if grep -q "сумма вероятностей:" "$OUT"; then
  if grep "сумма вероятностей:" "$OUT" | grep -qv "1.000"; then
      echo "FAIL: нарушена нормировка состояния — проверь коэффициент 1/sqrt(2) в H."
      rm -f "$BIN" "$OUT"; exit 1
  fi
else
  echo "FAIL: нет вывода вероятностей."; rm -f "$BIN" "$OUT"; exit 1
fi

# Состояние Белла: |01> и |10> должны отсутствовать.
python3 - "$OUT" <<'PY' || { echo "FAIL: запутанность не получилась — в состоянии Белла не должно быть |001>/|010> у пары."; rm -f "$BIN" "$OUT"; exit 1; }
import sys
t = open(sys.argv[1], encoding='utf-8').read()
block = t.split('состояние Белла) ---')[1].split('\n\n')[0]
sys.exit(0 if ('|000>' in block and '|011>' in block and '|001>' not in block and '|010>' not in block) else 1)
PY

grep -q "вероятность найти цель: 94.5%" "$OUT" || {
    echo "FAIL: Гровер должен усилить цель до 94.5% за 2 итерации."
    rm -f "$BIN" "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: кубиты симулированы, Гровер работает. Season 9 пройден. Season 10 разблокирован."
rm -f "$BIN" "$OUT"
exit 0
