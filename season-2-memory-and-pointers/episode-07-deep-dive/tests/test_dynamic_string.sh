#!/bin/bash

# OPERATION MOONLIGHT - Episode 07: Dynamic String Test

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "╔══════════════════════════════════════════════════════════╗"
echo "║  Episode 07: Dynamic String Test                        ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

# Компиляция
if [ ! -f "artifacts/dynamic_string.c" ]; then
    echo "[!] Используем reference solution"
    gcc -Wall -Wextra -std=c11 -o dynamic_string solution/dynamic_string_solution.c
else
    echo "[*] Компилируем artifacts/dynamic_string.c..."
    gcc -Wall -Wextra -std=c11 -o dynamic_string artifacts/dynamic_string.c
fi

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"
echo ""

# Тест с коротким вводом
echo "[*] Тест 1: Короткая строка (< 10 символов)"
OUTPUT=$(echo "Hello" | ./dynamic_string)

if echo "$OUTPUT" | grep -q "Length: 5 characters" && \
   echo "$OUTPUT" | grep -q 'Content: "Hello"'; then
    echo -e "${GREEN}✓ Тест 1 пройден${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}✗ Тест 1 не пройден${NC}"
fi

# Тест с длинным вводом (должен вызвать realloc)
echo "[*] Тест 2: Длинная строка (> 10 символов, триггер realloc)"
LONG_TEXT="This is a very long string that will trigger realloc multiple times"
OUTPUT=$(echo "$LONG_TEXT" | ./dynamic_string)

if echo "$OUTPUT" | grep -q "Realloc:" && \
   echo "$OUTPUT" | grep -q "Length: ${#LONG_TEXT} characters"; then
    echo -e "${GREEN}✓ Тест 2 пройден (realloc сработал)${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}✗ Тест 2 не пройден${NC}"
fi

# Тест на освобождение памяти
echo "[*] Тест 3: Проверка освобождения памяти"
if echo "$OUTPUT" | grep -q "Memory freed"; then
    echo -e "${GREEN}✓ Тест 3 пройден (память освобождена)${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}✗ Тест 3 не пройден${NC}"
fi

echo ""
echo "════════════════════════════════════════════════════════════"

if [ ${TESTS_PASSED:-0} -eq 3 ]; then
    echo -e "${GREEN}✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ: 3/3${NC}"
    echo ""
    echo "🎯 Dynamic String работает корректно!"
    echo "🚀 Можете переходить к основной задаче dynamic_loader"
    exit 0
else
    echo -e "${RED}✗ ТЕСТЫ НЕ ПРОЙДЕНЫ: ${TESTS_PASSED:-0}/3${NC}"
    exit 1
fi

