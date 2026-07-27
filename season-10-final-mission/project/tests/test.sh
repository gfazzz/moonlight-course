#!/bin/bash
# OPERATION MOONLIGHT — SEASON PROJECT moonlight_core: автотест
#
# Проверяет не только вывод, но и САМ ФАКТ межсезонной сборки:
# программа обязана собираться из модулей других сезонов, а не содержать
# их копии.

set -u
cd "$(dirname "$0")" || exit 2
PROJ=".."
ROOT="../../.."

S4="$ROOT/season-4-crypto-and-algorithms/lib"
S6="$ROOT/season-6-embedded-iot/lib"
S8="$ROOT/season-8-ai-and-data/lib"
S9="$ROOT/season-9-advanced-systems/lib"

echo "--- проверка 1: модули существуют в своих сезонах ---"
for m in "$S4/ml_hash.c" "$S6/ml_crc.c" "$S8/ml_stats.c" "$S9/ml_kalman.c"; do
    [ -f "$m" ] || { echo "FAIL: нет модуля $m"; exit 1; }
    echo "  есть: ${m#$ROOT/}"
done

echo "--- проверка 2: main.c подключает их заголовки ---"
for h in ml_hash.h ml_crc.h ml_stats.h ml_kalman.h; do
    grep -q "#include \"$h\"" "$PROJ/main.c" || { echo "FAIL: main.c не включает $h"; exit 1; }
    echo "  #include \"$h\""
done

echo "--- проверка 3: main.c НЕ содержит копий реализаций ---"
# Реализации обязаны жить в сезонах-источниках, а не быть скопированы сюда.
for sym in "ml_hash_djb2(const char" "ml_crc16_modbus(const unsigned" "ml_welford_push(MlWelford" "ml_kalman_step(MlKalman"; do
    if grep -q "^[a-z].*$sym.*{" "$PROJ/main.c"; then
        echo "FAIL: в main.c найдена копия реализации: $sym"; exit 1
    fi
done
echo "  копий реализаций нет — только вызовы"

echo "--- проверка 4: сборка из нескольких единиц трансляции ---"
BIN="$(mktemp -u /tmp/moonlight_core.XXXX)"
if ! gcc -Wall -Wextra -std=gnu11 -I"$S4" -I"$S6" -I"$S8" -I"$S9" \
     -o "$BIN" "$S4/ml_hash.c" "$S6/ml_crc.c" "$S8/ml_stats.c" "$S9/ml_kalman.c" "$PROJ/main.c"; then
    echo "FAIL: не линкуется."; exit 1
fi
echo "  слинковано: 4 модуля + main.c"

echo "--- проверка 5: поведение ---"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"
"$BIN" > "$OUT" 2>&1
rc=$?
[ $rc -eq 0 ] || { echo "FAIL: программа вернула $rc"; cat "$OUT"; rm -f "$BIN" "$OUT"; exit 1; }

grep -q "баланс сходится: да"        "$OUT" || { echo "FAIL: баланс кадров не сошёлся."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "фильтр точнее измерения:  да" "$OUT" || { echo "FAIL: модуль Калмана работает неверно."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "сборка работоспособна: да"  "$OUT" || { echo "FAIL: сборка неработоспособна."; rm -f "$BIN" "$OUT"; exit 1; }

diff -u expected.txt "$OUT" || { echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

rm -f "$BIN" "$OUT"
echo
echo "PASS: moonlight_core собран из модулей S4+S6+S8+S9 и работает."
exit 0
