#!/bin/bash
# OPERATION MOONLIGHT — s09e04 автотест
# btree.c: B-дерево, расщепление узлов, поиск по страницам.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/btree.c"
FALLBACK="../solution/btree.c"
EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s09e04.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/btree.c";
else SRC="$FALLBACK"; WHO="solution/btree.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
if ! gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC"; then
    echo "FAIL: не компилируется."; exit 1
fi
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

grep -q "высота дерева: 4" "$OUT" || {
    echo "FAIL: высота дерева должна быть 4 — проверь расщепление и рост корня."
    rm -f "$BIN" "$OUT"; exit 1; }
grep -q "всего ключей в дереве: 200" "$OUT" || {
    echo "FAIL: обход должен найти все 200 ключей — часть потеряна при расщеплении."
    rm -f "$BIN" "$OUT"; exit 1; }

# Ключи из обхода обязаны идти по неубыванию.
line=$(grep -A1 "обход по порядку" "$OUT" | tail -1)
python3 - "$line" <<'PY' || { echo "FAIL: обход выдал ключи не по порядку."; rm -f "$BIN" "$OUT"; exit 1; }
import sys
ks = [int(x) for x in sys.argv[1].split()]
sys.exit(0 if ks == sorted(ks) else 1)
PY

if ! diff -u "$EXPECTED" "$OUT"; then
    echo; echo "FAIL: вывод не совпал с expected.txt (см. diff)."
    rm -f "$BIN" "$OUT"; exit 1
fi

echo; echo "PASS: индекс построен, поиск быстрый. Дальше — s09e05 (журнал и надёжность)."
rm -f "$BIN" "$OUT"
exit 0
