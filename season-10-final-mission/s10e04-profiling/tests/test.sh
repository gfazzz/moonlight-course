#!/bin/bash
# OPERATION MOONLIGHT — s10e04 автотест
# profile.c: профилирование конвейера, оптимизация горячего пути, закон Амдала.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/profile.c"; FALLBACK="../solution/profile.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e04.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/profile.c"; else SRC="$FALLBACK"; WHO="solution/profile.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Главное правило оптимизации: результат обязан остаться прежним.
if grep -q "РАСХОЖДЕНИЕ" "$OUT"; then
    echo "FAIL: оптимизация изменила результат — это не оптимизация, а баг."
    rm -f "$BIN" "$OUT"; exit 1
fi

# Ускорение должно быть существенным (иначе горячий путь не найден).
python3 - "$OUT" <<'PY' || { echo "FAIL: ускорение меньше 10x — горячий путь не устранён."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
m = re.search(r'ускорение: ([\d.]+) раза', t)
sys.exit(0 if m and float(m.group(1)) >= 10.0 else 1)
PY

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: горячий путь найден и устранён. Дальше — s10e05 (защита)."
rm -f "$BIN" "$OUT"; exit 0
