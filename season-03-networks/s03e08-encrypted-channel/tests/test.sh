#!/bin/bash
# OPERATION MOONLIGHT — s03e08 автотест (финал Season 3)
# intercept.c: реассемблинг зашифрованного потока + XOR-расшифровка. Тест: <bin> ../channel.dat

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/intercept.c"
FALLBACK="../solution/intercept.c"
DATA="../channel.dat"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s03e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/intercept.c";
else SRC="$FALLBACK"; WHO="solution/intercept.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: канал перехвачен и расшифрован. Season 3 пройден. Season 4 разблокирован."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
