#!/bin/bash
# OPERATION MOONLIGHT — s05e03 автотест
# csv.c разбирает журнал сделок автоматом и сравнивает результат с наивным
# разбором по запятой. Тест: <bin> ../data/journal.csv

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/csv.c"
FALLBACK="../solution/csv.c"
DATA="../data/journal.csv"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s05e03.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"
OUT2="$(mktemp /tmp/moonlight_out2.XXXX)"
cleanup() { rm -f "$BIN" "$OUT" "$OUT2"; }

# Файл данных намеренно содержит и CRLF, и LF, и перевод строки внутри поля.
# Если система контроля версий нормализовала переводы строк, разбор потеряет
# смысл — поэтому размер проверяется до всего остального.
SIZE=$(wc -c < "$DATA" | tr -d ' ')
if [ "$SIZE" != "446" ]; then
    echo "FAIL: $DATA изменился в размере ($SIZE вместо 446)."
    echo "      Скорее всего, переводы строк нормализовались при выгрузке."
    echo "      В каталоге серии есть .gitattributes, который это запрещает."
    exit 1
fi

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/csv.c";
else SRC="$FALLBACK"; WHO="solution/csv.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=c11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" "$DATA" > "$OUT" 2>/dev/null || { echo "FAIL: падение при запуске."; cat "$OUT"; cleanup; exit 1; }

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."; cleanup; exit 1
fi

# Разбор определяется байтами файла, а не уровнем оптимизации.
gcc -Wall -Wextra -std=c11 -O2 -o "$BIN" "$SRC" 2>/dev/null || {
    echo "FAIL: не собирается с -O2."; cleanup; exit 1; }
"$BIN" "$DATA" > "$OUT2" 2>/dev/null
if ! diff -u "$OUT" "$OUT2"; then
    echo; echo "FAIL: вывод изменился при -O2."; cleanup; exit 1
fi

echo; echo "PASS: журнал разобран, битые записи найдены. Дальше — s05e04 (время)."
cleanup
exit 0
