#!/bin/bash
# OPERATION MOONLIGHT — s07e01 автотест
# fork.c: порождение процесса, родство, wait и код возврата.
# Детерминизм: печатаем свойства (да/нет), а не PID — они меняются каждый запуск.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/fork.c"
FALLBACK="../solution/fork.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s07e01.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/fork.c";
else SRC="$FALLBACK"; WHO="solution/fork.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
# -std=gnu11: POSIX-функции (fork/waitpid) видны без ручных feature-макросов
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

# Повторный прогон: вывод не должен зависеть от PID/планировщика.
"$BIN" > "$OUT" 2>&1
if ! diff -q "$EXPECTED" "$OUT" >/dev/null; then
    echo; echo "FAIL: вывод нестабилен между запусками (не печатай сырые PID)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: процесс раздвоился, потомок дождан. Дальше — s07e02 (exec)."
rm -f "$BIN" "$OUT"
exit 0
