#!/bin/bash
# OPERATION MOONLIGHT — s07e04 автотест
# threads.c: pthread, параллельный Monte Carlo, побитовая воспроизводимость.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/threads.c"
FALLBACK="../solution/threads.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/threads.c";
else SRC="$FALLBACK"; WHO="solution/threads.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -pthread -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется (не забудь -pthread)."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! grep -q "совпадают побитово: да" "$OUT"; then
    echo "FAIL: параллельный результат разошёлся с последовательным."
    echo "Подсказка: сид ГПСЧ должен зависеть от НОМЕРА траектории, а не быть общим."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

# Трижды: планировщик не должен влиять на результат.
for i in 1 2 3; do
    "$BIN" > "$OUT" 2>&1
    if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
        echo; echo "FAIL: вывод нестабилен между запусками (прогон $i)."
        rm -f "$BIN" "$OUT"; exit 1
    fi
done

echo; echo "PASS: параллельно и воспроизводимо. Дальше — s07e05 (гонки и мьютексы)."
rm -f "$BIN" "$OUT"
exit 0
