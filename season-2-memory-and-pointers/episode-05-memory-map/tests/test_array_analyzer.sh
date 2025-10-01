#!/bin/bash

# OPERATION MOONLIGHT
# Episode 05: Memory Map
# Тест для промежуточной задачи array_analyzer

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "╔══════════════════════════════════════════════════════════╗"
echo "║  Episode 05: Array Analyzer Test                        ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

# Проверка наличия исходного файла
if [ ! -f "artifacts/array_analyzer.c" ]; then
    echo -e "${YELLOW}[!] artifacts/array_analyzer.c не найден${NC}"
    echo -e "${YELLOW}[!] Используем reference solution для демонстрации${NC}"
    echo ""
    
    # Компилируем reference solution
    gcc -Wall -Wextra -std=c11 -o array_analyzer solution/array_analyzer_solution.c
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Compilation failed${NC}"
        exit 1
    fi
else
    # Компилируем версию студента
    echo "[*] Компилируем artifacts/array_analyzer.c..."
    gcc -Wall -Wextra -std=c11 -o array_analyzer artifacts/array_analyzer.c
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Compilation failed${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓ Compilation successful${NC}"
    echo ""
fi

# Запускаем программу и сохраняем вывод
echo "[*] Запуск программы..."
OUTPUT=$(./array_analyzer)

echo "$OUTPUT"
echo ""

# Проверяем критические элементы вывода
echo "[*] Проверка вывода..."

TESTS_PASSED=0
TESTS_TOTAL=0

# Тест 1: Проверка размера массива
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Array size: 5 bytes"; then
    echo -e "${GREEN}✓ Тест 1: Размер массива корректен${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 1: Размер массива неверен${NC}"
fi

# Тест 2: Проверка суммы
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Sum of all bytes: 473"; then
    echo -e "${GREEN}✓ Тест 2: Сумма вычислена правильно${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 2: Неверная сумма (должна быть 473)${NC}"
fi

# Тест 3: Проверка максимума
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Max byte: 0xFF (255)"; then
    echo -e "${GREEN}✓ Тест 3: Максимум найден правильно${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 3: Неверный максимум (должен быть 0xFF)${NC}"
fi

# Тест 4: Проверка минимума
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Min byte: 0x00 (0)"; then
    echo -e "${GREEN}✓ Тест 4: Минимум найден правильно${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 4: Неверный минимум (должен быть 0x00)${NC}"
fi

# Тест 5: Проверка индекса максимума
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Index of max: 2"; then
    echo -e "${GREEN}✓ Тест 5: Индекс максимума правильный${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 5: Неверный индекс максимума (должен быть 2)${NC}"
fi

# Тест 6: Проверка индекса минимума
((TESTS_TOTAL++))
if echo "$OUTPUT" | grep -q "Index of min: 3"; then
    echo -e "${GREEN}✓ Тест 6: Индекс минимума правильный${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ Тест 6: Неверный индекс минимума (должен быть 3)${NC}"
fi

echo ""
echo "════════════════════════════════════════════════════════════"

# Итоговый результат
if [ $TESTS_PASSED -eq $TESTS_TOTAL ]; then
    echo -e "${GREEN}✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ: $TESTS_PASSED/$TESTS_TOTAL${NC}"
    echo ""
    echo "🎯 Array Analyzer работает корректно!"
    echo "🚀 Можете переходить к основной задаче memory_mapper"
    echo ""
    exit 0
else
    echo -e "${RED}✗ ТЕСТЫ НЕ ПРОЙДЕНЫ: $TESTS_PASSED/$TESTS_TOTAL${NC}"
    echo ""
    echo "Проверьте свой код и попробуйте снова."
    echo ""
    exit 1
fi

