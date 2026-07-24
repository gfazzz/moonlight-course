#!/bin/bash
# OPERATION MOONLIGHT — s06e08 автотест (финал Season 6)
# mavlink.c: разбор MAVLink-кадра, X.25 checksum, телеметрия, интеграция. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/mavlink.c"
FALLBACK="../solution/mavlink.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s06e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/mavlink.c";
else SRC="$FALLBACK"; WHO="solution/mavlink.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: телеметрия дрона декодирована. Season 6 пройден. Season 7 разблокирован."; rc=0
else
    echo; echo "FAIL: вывод не совпал (проверь X.25 checksum: init 0xFFFF + CRC_EXTRA)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
