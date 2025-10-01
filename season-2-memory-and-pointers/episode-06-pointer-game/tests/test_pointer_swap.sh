#!/bin/bash

# OPERATION MOONLIGHT - Episode 06: Pointer Swap Test

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "╔══════════════════════════════════════════════════════════╗"
echo "║  Episode 06: Pointer Swap Test                          ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

# Компиляция
if [ ! -f "artifacts/pointer_swap.c" ]; then
    echo "[!] Используем reference solution"
    gcc -Wall -Wextra -std=c11 -o pointer_swap solution/pointer_swap_solution.c
else
    echo "[*] Компилируем artifacts/pointer_swap.c..."
    gcc -Wall -Wextra -std=c11 -o pointer_swap artifacts/pointer_swap.c
fi

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# Запуск и проверка
OUTPUT=$(./pointer_swap)
echo "$OUTPUT"
echo ""

if echo "$OUTPUT" | grep -q "After swap:  x=10, y=5" && \
   echo "$OUTPUT" | grep -q "✓ Swap successful!"; then
    echo -e "${GREEN}✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ${NC}"
    echo "🎯 Pointer swap работает корректно!"
    exit 0
else
    echo -e "${RED}✗ ТЕСТ НЕ ПРОЙДЕН${NC}"
    exit 1
fi

