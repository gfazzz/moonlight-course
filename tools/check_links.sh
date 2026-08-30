#!/usr/bin/env bash
#
# check_links.sh — проверка внутренних ссылок во всех .md курса.
#
# Зачем: ручная вычитка не ловит ссылки на несуществующие файлы — здесь так
# пережили несколько вычиток ссылки на solution/sorts.c при фактическом sorting.c.
# Скрипт обходит все .md (кроме personal/ и .git/), достаёт markdown-ссылки вида
# [текст](путь) и проверяет существование цели относительно файла-источника.
#
# Пропускаются: внешние ссылки (http/https/mailto), якоря (#...), пути с плейсхолдерами.
#
# Использование:  bash tools/check_links.sh
# Код возврата:   0 — битых ссылок нет; 1 — есть (список в stdout).

set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT}"

broken=0
checked=0

while IFS= read -r file; do
    dir="$(dirname "${file}")"
    # достаём цели ссылок: [текст](цель)
    grep -oE '\]\([^)]+\)' "${file}" 2>/dev/null | sed 's/^](//; s/)$//' | while IFS= read -r target; do
        case "${target}" in
            http://*|https://*|mailto:*|'#'*|'') continue ;;
            *'<'*|*'>'*|*' '*) continue ;;   # плейсхолдеры вида <путь>
        esac
        # отрезаем якорь: file.md#section -> file.md
        path="${target%%#*}"
        [ -z "${path}" ] && continue
        if [ ! -e "${dir}/${path}" ]; then
            printf '%s -> %s\n' "${file#./}" "${target}"
        fi
    done
done < <(find . -name '*.md' -not -path './.git/*' -not -path './personal/*' | sort) > /tmp/ml_broken_links.$$ 2>/dev/null

broken=$(wc -l < /tmp/ml_broken_links.$$ | tr -d ' ')
checked=$(find . -name '*.md' -not -path './.git/*' -not -path './personal/*' | wc -l | tr -d ' ')

echo "════════════════════════════════════════════════════════════"
echo " Проверка внутренних ссылок: ${checked} файлов .md"
echo "════════════════════════════════════════════════════════════"

if [ "${broken}" -eq 0 ]; then
    echo " Битых ссылок нет."
    rm -f /tmp/ml_broken_links.$$
    echo "════════════════════════════════════════════════════════════"
    exit 0
fi

echo " БИТЫЕ ССЫЛКИ (${broken}):"
sed 's/^/   /' /tmp/ml_broken_links.$$
rm -f /tmp/ml_broken_links.$$
echo "════════════════════════════════════════════════════════════"
exit 1
