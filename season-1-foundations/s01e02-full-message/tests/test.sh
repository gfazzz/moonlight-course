#!/bin/bash
# OPERATION MOONLIGHT — s01e01 автотест
# Проверяет: decoder.c компилируется, запускается и печатает расшифровку V.
#
# Логика: берём artifacts/decoder.c (твой код). Если его ещё нет —
# падаем на solution/decoder.c, чтобы harness можно было прогнать сразу.

set -u
cd "$(dirname "$0")" || exit 2

ARTIFACT="../artifacts/decoder.c"
FALLBACK="../solution/decoder.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s01e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ARTIFACT" ]; then
    SRC="$ARTIFACT"; WHO="artifacts/decoder.c"
else
    SRC="$FALLBACK"; WHO="solution/decoder.c (fallback — своего решения пока нет)"
fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется. Почини ошибки и запусти снова."
    exit 1
fi
echo "OK: собралось без ошибок."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if diff -u "$EXPECTED" "$OUT"; then
    echo
    echo "PASS: сообщение V. расшифровано верно. Переходи к s01e03."
    rc=0
else
    echo
    echo "FAIL: вывод не совпал с expected.txt (см. diff выше)."
    rc=1
fi

rm -f "$BIN" "$OUT"
exit $rc
