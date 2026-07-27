#!/bin/bash
# OPERATION MOONLIGHT — s10e05 автотест
# harden.c: разбор недоверенного пакета. Проверяется и санитайзерами.

set -u
cd "$(dirname "$0")" || exit 2

ART="../artifacts/harden.c"; FALLBACK="../solution/harden.c"; EXPECTED="expected.txt"
BIN="$(mktemp -u /tmp/moonlight_s10e05.XXXX)"; SAN="$(mktemp -u /tmp/moonlight_s10e05san.XXXX)"
OUT="$(mktemp /tmp/moonlight_out.XXXX)"; SOUT="$(mktemp /tmp/moonlight_san.XXXX)"

if [ -f "$ART" ]; then SRC="$ART"; WHO="artifacts/harden.c"; else SRC="$FALLBACK"; WHO="solution/harden.c (fallback)"; fi
echo "Источник: $WHO"

echo "Компиляция..."
gcc -Wall -Wextra -std=gnu11 -o "$BIN" "$SRC" || { echo "FAIL: не компилируется."; exit 1; }
echo "OK: собралось."

"$BIN" > "$OUT" 2>&1 || { echo "FAIL: падение при запуске."; cat "$OUT"; exit 1; }

# Ни одна атака не должна проходить.
for bad in "длина 200" "199 байт" "канал 250" "обрезанный" "неверная сигнатура" "испорченная контрольная"; do
  if ! grep -A1 "$bad" "$OUT" | grep -q "отвергнут"; then
      echo "FAIL: атака не отражена — «$bad» должна отвергаться."
      rm -f "$BIN" "$OUT" "$SOUT"; exit 1
  fi
done

# Санитайзеры: настоящая проверка защиты — не текст, а отсутствие нарушений памяти.
echo "Сборка под ASan/UBSan..."
if gcc -Wall -Wextra -std=gnu11 -fsanitize=address,undefined -o "$SAN" "$SRC" 2>/dev/null; then
    if ! "$SAN" > "$SOUT" 2>&1; then
        echo "FAIL: санитайзеры нашли нарушение:"; tail -20 "$SOUT"
        rm -f "$BIN" "$SAN" "$OUT" "$SOUT"; exit 1
    fi
    if grep -qE "runtime error|AddressSanitizer" "$SOUT"; then
        echo "FAIL: обнаружено неопределённое поведение:"; grep -m5 -E "runtime error|AddressSanitizer" "$SOUT"
        rm -f "$BIN" "$SAN" "$OUT" "$SOUT"; exit 1
    fi
    echo "OK: санитайзеры чисты."
    rm -f "$SAN"
else
    echo "(санитайзеры недоступны — пропускаю)"
fi

diff -u "$EXPECTED" "$OUT" || { echo; echo "FAIL: вывод не совпал с expected.txt."; rm -f "$BIN" "$OUT" "$SOUT"; exit 1; }

echo; echo "PASS: разбор устойчив к недоверенному вводу. Дальше — s10e06 (дашборд)."
rm -f "$BIN" "$OUT" "$SOUT"; exit 0
