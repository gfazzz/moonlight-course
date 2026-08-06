#!/bin/bash
# OPERATION MOONLIGHT — s05e08 автотест
# unicode.c сводит выписку со счётом и показывает, куда девается недостача.
# Данных на входе нет: выписка зашита в исходник, поток сделок порождается ЛКГ.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/unicode.c"
FALLBACK="../solution/unicode.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e08.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"
OUT2="$(mktemp /tmp/moonlight_out2.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/unicode.c";
else SRC="$FALLBACK"; WHO="solution/unicode.c (fallback)"; fi
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
    echo; echo "FAIL: вывод изменился при -O2 — в расчёт попала локаль или системная таблица."
    rm -f "$BIN" "$OUT" "$OUT2"; exit 1
fi

echo; echo "PASS: двойник найден. Season 5 закрыт — дальше Season 6."
rm -f "$BIN" "$OUT" "$OUT2"
exit $rc
