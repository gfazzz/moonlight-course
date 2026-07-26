#!/bin/bash
# OPERATION MOONLIGHT — s08e06 автотест
# perceptron.c: нейрон с сигмоидой; AND/OR решаются, XOR — нет. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/perceptron.c"
FALLBACK="../solution/perceptron.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s08e06.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/perceptron.c";
else SRC="$FALLBACK"; WHO="solution/perceptron.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (exp реализуй сам, без -lm)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# AND и OR обязаны обучиться полностью, XOR — нет (это и есть урок серии).
if [ "$(grep -c 'верно: 4 из 4' "$OUT")" -ne 2 ]; then
    echo "FAIL: AND и OR должны решаться на 100% — проверь градиент (p - y)."
    rm -f "$BIN" "$OUT"; exit 1
fi
if ! grep -q "верно: 2 из 4" "$OUT"; then
    echo "FAIL: XOR не должен решаться одним нейроном — проверь данные."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: предел перцептрона показан. Дальше — s08e07 (backprop)."
rm -f "$BIN" "$OUT"
exit 0
