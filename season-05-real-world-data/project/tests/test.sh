#!/bin/bash
# OPERATION MOONLIGHT — SEASON PROJECT trading_system: автотест
# Проверяет факт межсезонной сборки (S4 -> S5), переиспользование модулей
# и поведение конвейера.

set -u
cd "$(dirname "$0")" || exit 2
PROJ=".."; ROOT="../../.."
S4="$ROOT/season-04-crypto-and-algorithms/lib"; S5="$PROJ/../lib"

echo "--- проверка 1: модули подключены, а не скопированы ---"
for m in "$S4/ml_hash.c" "$S5/ml_csv.c" "$S5/ml_money.c" "$S5/ml_time.c" \
         "$S5/ml_text.c" "$S5/ml_tick.c"; do
    [ -f "$m" ] || { echo "FAIL: нет $m"; exit 1; }
done
for h in ml_hash ml_csv ml_money ml_time ml_text ml_tick; do
    grep -q "#include \"$h.h\"" "$PROJ/main.c" || { echo "FAIL: main.c не включает $h.h"; exit 1; }
done

# Реализация, а не вызов: определение функции модуля внутри main.c запрещено.
for fn in ml_hash_djb2 ml_csv_parse ml_money_mul_ppm ml_utc_from_local \
          ml_skeleton ml_tick_write; do
    grep -qE "^[A-Za-z_].*[[:space:]]\**$fn\(.*\)[[:space:]]*\{" "$PROJ/main.c" && {
        echo "FAIL: реализация $fn скопирована в main.c"; exit 1; }
done
echo "  шесть модулей подключены из двух сезонов"

echo "--- проверка 2: сборка нескольких единиц трансляции ---"
BIN="$(mktemp -u /tmp/trading_system.XXXX)"
gcc -Wall -Wextra -std=gnu11 -I"$S4" -I"$S5" -o "$BIN" \
    "$S4/ml_hash.c" "$S5/ml_csv.c" "$S5/ml_money.c" "$S5/ml_time.c" \
    "$S5/ml_text.c" "$S5/ml_tick.c" "$PROJ/main.c" || { echo "FAIL: не линкуется."; exit 1; }
echo "  слинковано: 6 модулей + main.c"

echo "--- проверка 3: поведение ---"
OUT="$(mktemp /tmp/ts_out.XXXX)"
OUT2="$(mktemp /tmp/ts_out2.XXXX)"
cleanup() { rm -f "$BIN" "$OUT" "$OUT2"; }

"$BIN" > "$OUT" 2>&1; rc=$?
[ $rc -eq 0 ] || { echo "FAIL: код возврата $rc"; cat "$OUT"; cleanup; exit 1; }

# Свойства, а не только совпадение с эталоном.
grep -q "сверка сходится: да"              "$OUT" || { echo "FAIL: суммы не сошлись."; cleanup; exit 1; }
grep -q "двойник найден:               да" "$OUT" || { echo "FAIL: двойник не найден."; cleanup; exit 1; }
grep -q "несуществующая метка найдена: да" "$OUT" || { echo "FAIL: метка вне календаря пропущена."; cleanup; exit 1; }
grep -q "выгрузка обратима:            да" "$OUT" || { echo "FAIL: бинарная выгрузка не обратима."; cleanup; exit 1; }
grep -q "сборка работоспособна: да"        "$OUT" || { echo "FAIL: самопроверка не пройдена."; cleanup; exit 1; }

diff -u expected.txt "$OUT" || { echo "FAIL: вывод не совпал."; cleanup; exit 1; }

# Конвейер целочисленный и не зависит от уровня оптимизации.
gcc -Wall -Wextra -std=gnu11 -O2 -I"$S4" -I"$S5" -o "$BIN" \
    "$S4/ml_hash.c" "$S5/ml_csv.c" "$S5/ml_money.c" "$S5/ml_time.c" \
    "$S5/ml_text.c" "$S5/ml_tick.c" "$PROJ/main.c" 2>/dev/null || {
    echo "FAIL: не собирается с -O2."; cleanup; exit 1; }
"$BIN" > "$OUT2" 2>&1
diff -u "$OUT" "$OUT2" || { echo "FAIL: вывод изменился при -O2."; cleanup; exit 1; }

cleanup
echo; echo "PASS: trading_system собран из модулей S4 + S5."
exit 0
