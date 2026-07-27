#!/bin/bash
# OPERATION MOONLIGHT — s10e01 автотест
# core.c: реестр модулей, слои, топологическая сортировка, каскад отказов.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/core.c"; FALLBACK="../solution/core.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e01.XXXX)"; OUT="$(mktemp /tmp/moonlight_out.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/core.c"; else SRC="$FALLBACK"; WHO="solution/core.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Свойство 1: зависимость обязана стоять в порядке запуска РАНЬШЕ зависящего.
python3 - "$OUT" <<'PY' || { echo "FAIL: порядок инициализации нарушает зависимости."; rm -f "$BIN" "$OUT"; exit 1; }
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
order = re.search(r'порядок инициализации:(.*)', t).group(1).split()
pos = {m: i for i, m in enumerate(order)}
deps = {"telemetry": ["net", "storage"], "analytics": ["storage"],
        "tracker": ["telemetry", "analytics"], "dashboard": ["tracker"]}
sys.exit(0 if all(pos[d] < pos[m] for m, ds in deps.items() for d in ds) else 1)
PY

grep -q "нарушений слоёв: 0" "$OUT" || { echo "FAIL: корректная система не должна иметь нарушений слоёв."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "цикл обнаружен: да" "$OUT"  || { echo "FAIL: цикл в зависимостях должен обнаруживаться."; rm -f "$BIN" "$OUT"; exit 1; }
grep -q "запущено модулей: 0 из 3" "$OUT" || { echo "FAIL: отказ нижнего модуля должен каскадно блокировать зависимых."; rm -f "$BIN" "$OUT"; exit 1; }

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT"; exit 1; }

echo; echo "PASS: ядро собирается в правильном порядке. Дальше — s10e02 (орбиты)."
rm -f "$BIN" "$OUT"; exit 0
