#!/bin/bash
# OPERATION MOONLIGHT — s07e08 автотест (финал Season 7)
# signals.c: sigaction, sig_atomic_t, чистое завершение, неперехватываемость SIGKILL.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/signals.c"
FALLBACK="../solution/signals.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/signals.c";
else SRC="$FALLBACK"; WHO="solution/signals.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

if command -v timeout >/dev/null 2>&1; then RUN="timeout 15 $BIN"; else RUN="$BIN"; fi

$RUN > "$OUT" 2>&1
rc=$?
if [ $rc -eq 124 ]; then
    echo "FAIL: зависание. Проверь барьер готовности потомка и waitpid."
    rm -f "$BIN" "$OUT"; exit 1
elif [ $rc -ne 0 ]; then
    echo "FAIL: падение при запуске."; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1
fi

if ! grep -q "это был SIGKILL (перехват невозможен): да" "$OUT"; then
    echo "FAIL: SIGKILL-демонстрация не сработала."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

for i in 1 2 3; do
    $RUN > "$OUT" 2>&1
    if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
        echo; echo "FAIL: вывод нестабилен (прогон $i)."
        rm -f "$BIN" "$OUT"; exit 1
    fi
done

echo; echo "PASS: агент реагирует и завершается чисто. Season 7 пройден. Season 8 разблокирован."
rm -f "$BIN" "$OUT"
exit 0
