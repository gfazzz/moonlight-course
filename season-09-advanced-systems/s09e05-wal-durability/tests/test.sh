#!/bin/bash
# OPERATION MOONLIGHT — s09e05 автотест
# wal.c: журнал впереди данных, undo/redo, инвариант при сбое на любом шаге.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/wal.c"
FALLBACK="../solution/wal.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e05.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/wal.c";
else SRC="$FALLBACK"; WHO="solution/wal.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Главное: деньги не теряются НИ В ОДНОМ сценарии.
if grep -q "ПОТЕРЯ ДЕНЕГ" "$OUT"; then
    echo "FAIL: инвариант нарушен — WAL не защитил данные при сбое."
    rm -f "$BIN" "$OUT"; exit 1
fi
if [ "$(grep -c "инвариант сохранён" "$OUT")" -ne 5 ]; then
    echo "FAIL: должно быть 5 сценариев с сохранённым инвариантом."
    rm -f "$BIN" "$OUT"; exit 1
fi
# Обе ветви восстановления обязаны встречаться.
grep -q "undo (откат)" "$OUT" || { echo "FAIL: нет ни одного отката (undo)."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "redo (транзакция зафиксирована)" "$OUT" || { echo "FAIL: нет доигрывания (redo)."; rm -f "$BIN" "$OUT"; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: данные переживают сбой на любом шаге. Дальше — s09e06 (TUI)."
rm -f "$BIN" "$OUT"
exit 0
