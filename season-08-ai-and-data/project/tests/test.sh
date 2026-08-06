#!/bin/bash
# OPERATION MOONLIGHT — SEASON PROJECT luna_ai: автотест
# Проверяет не только вывод, но и САМ ФАКТ сборки из модулей других сезонов.

set -u
cd "$(dirname "$0")" || exit 2
PROJ=".."

echo "--- проверка 1: модули на месте и подключены заголовками ---"
MODS=$(make -s -C "$PROJ" modules | grep -v "Модули\|main.c")
for m in $MODS; do
    [ -f "$PROJ/$m" ] || { echo "FAIL: нет модуля $m"; exit 1; }
    h="$(basename $m .c).h"
    grep -q "#include \"$h\"" "$PROJ/main.c" || { echo "FAIL: main.c не включает $h"; exit 1; }
    echo "  $m + #include \"$h\""
done

echo "--- проверка 2: в main.c нет копий реализаций ---"
for m in $MODS; do
    h="$PROJ/$(dirname $m)/$(basename $m .c).h"
    # каждая функция из заголовка не должна иметь ТЕЛА в main.c
    grep -oE '^[a-z].*\bml_[a-z_0-9]+\(' "$h" 2>/dev/null | grep -oE 'ml_[a-z_0-9]+' | sort -u | while read -r fn; do
        if grep -qE "^[a-z].*\b$fn\s*\(.*\)\s*\{" "$PROJ/main.c"; then
            echo "FAIL: в main.c найдена копия реализации $fn"; exit 1
        fi
    done
done
echo "  копий нет — только вызовы"

echo "--- проверка 3: сборка и запуск ---"
make -s -C "$PROJ" clean >/dev/null 2>&1
make -s -C "$PROJ" all 2>&1 | grep -q "OK:" || { echo "FAIL: не собирается."; exit 1; }
OUT="$(mktemp /tmp/proj_out.XXXX)"
"$PROJ/luna_ai" > "$OUT" 2>&1; rc=$?
[ $rc -eq 0 ] || { echo "FAIL: код возврата $rc"; cat "$OUT"; rm -f "$OUT"; exit 1; }
grep -q "сборка работоспособна: да" "$OUT" || { echo "FAIL: самопроверка не пройдена."; cat "$OUT"; rm -f "$OUT"; exit 1; }
# Число единиц трансляции берётся из Makefile, а не зашивается в тест.
NMOD=$(echo "$MODS" | wc -w | tr -d ' ')
grep -q "$((NMOD + 1)) единиц трансляции" "$OUT" || {
    echo "FAIL: программа сообщает не о $((NMOD + 1)) единицах трансляции."; rm -f "$OUT"; exit 1; }
grep -q "битая строка отброшена, а не подставлена нулями: да" "$OUT" || {
    echo "FAIL: битая запись контрольной выборки не отброшена."; rm -f "$OUT"; exit 1; }
grep -q "внешняя проверка пройдена: да" "$OUT" || {
    echo "FAIL: модель не прошла проверку на внешних данных."; rm -f "$OUT"; exit 1; }

if [ -f expected.txt ]; then
    diff -u expected.txt "$OUT" || { echo "FAIL: вывод не совпал с expected.txt."; rm -f "$OUT"; exit 1; }
fi
rm -f "$OUT"
echo
echo "PASS: luna_ai собран из модулей нескольких сезонов и работает."
exit 0
