#!/bin/bash
# OPERATION MOONLIGHT — s04e08 автотест (финал Season 4)
# blockchain.c: цепочка с PoW, проверка целостности, обнаружение подделки.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/blockchain.c"
FALLBACK="../solution/blockchain.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s04e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/blockchain.c";
else SRC="$FALLBACK"; WHO="solution/blockchain.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

# Майнинг перебирает nonce — ставим таймаут на случай слишком высокой сложности.
if command -v timeout >/dev/null 2>&1; then RUN="timeout 30 $BIN"; else RUN="$BIN"; fi
$RUN > "$OUT" 2>&1
rc_run=$?
if [ $rc_run -eq 124 ]; then echo "FAIL: майнинг не завершился (проверь DIFFICULTY)."; rm -f "$BIN" "$OUT"; exit 1; fi
if [ $rc_run -ne 0 ]; then echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1; fi

if diff -u "$EXPECTED" "$OUT"; then
    echo; echo "PASS: реестр проверен, подделка найдена. Season 4 пройден. Season 5 разблокирован."; rc=0
else
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; rc=1
fi
rm -f "$BIN" "$OUT"
exit $rc
