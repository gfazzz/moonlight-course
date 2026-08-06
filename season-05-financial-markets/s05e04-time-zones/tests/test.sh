#!/bin/bash
# OPERATION MOONLIGHT — s05e04 автотест
# tstamp.c сводит выписку со счётом и показывает, куда девается недостача.
# Данных на входе нет: выписка зашита в исходник, поток сделок порождается ЛКГ.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/tstamp.c"
FALLBACK="../solution/tstamp.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"
OUT2="$(mktemp /tmp/moonlight_out2.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/tstamp.c";
else SRC="$FALLBACK"; WHO="solution/tstamp.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>/dev/null || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

rc=0
if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT" "$OUT2"; exit 1
fi

# Деньги считаются целыми, поэтому вывод обязан быть одинаковым при любом
# уровне оптимизации. Если -O2 напечатал другое — где-то в расчёт затесался
# double, и это ошибка серии, а не придирка теста.
if ! gcc -Wall -Wextra -std=c11 -O2 -o "$BIN" "$SRC" 2>/dev/null; then
    echo "FAIL: не собирается с -O2."; rm -f "$BIN" "$OUT" "$OUT2"; exit 1
fi
"$BIN" > "$OUT2" 2>/dev/null
if ! diff -u "$OUT" "$OUT2"; then
    echo; echo "FAIL: вывод изменился при -O2 — значит, в расчёт попал double или системные часы."
    rm -f "$BIN" "$OUT" "$OUT2"; exit 1
fi

echo; echo "PASS: метки разобраны, пропавший час найден. Дальше — s05e05 (бэктест)."
rm -f "$BIN" "$OUT" "$OUT2"
exit $rc
