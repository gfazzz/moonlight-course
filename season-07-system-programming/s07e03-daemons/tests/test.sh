#!/bin/bash
# OPERATION MOONLIGHT — s07e03 автотест
# daemon.c: демонизация (fork/setsid/fork), отвязка от терминала. Детерминирован.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/daemon.c"
FALLBACK="../solution/daemon.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/daemon.c";
else SRC="$FALLBACK"; WHO="solution/daemon.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Ключевая проверка: вывод демона в stdout обязан уходить в /dev/null.
if grep -q "НЕ ДОЛЖНА ПОЯВИТЬСЯ" "$OUT"; then
    echo "FAIL: stdout демона не перенаправлен в /dev/null."
    rm -f "$BIN" "$OUT"; exit 1
fi

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

"$BIN" > "$OUT" 2>&1
if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
    echo; echo "FAIL: вывод нестабилен между запусками."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: демон отвязался от терминала. Дальше — s07e04 (потоки)."
rm -f "$BIN" "$OUT"
exit 0
