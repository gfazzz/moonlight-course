#!/bin/bash
# Test script for Episode 22: Industrial Protocols

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TESTS_PASSED=0
TESTS_FAILED=0

echo "🧪 Episode 22: Industrial Protocols - Automated Tests"
echo "======================================================"

# Функция для проверки результата
check_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ PASS${NC}: $2"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC}: $2"
        ((TESTS_FAILED++))
    fi
}

# Проверка существования программы
if [ ! -f "./industrial_protocols" ]; then
    echo -e "${RED}ERROR: Program not found. Run 'make' first.${NC}"
    exit 1
fi

# Test 1: Программа запускается
echo ""
echo "Test 1: Program execution..."
./industrial_protocols > /dev/null 2>&1
check_result $? "Program runs without crashing"

# Test 2: Wiegand test
echo ""
echo "Test 2: Wiegand protocol..."
OUTPUT=$(./industrial_protocols --wiegand 2>&1)

# Проверка наличия ключевых слов в выводе
echo "$OUTPUT" | grep -q "WIEGAND TEST"
check_result $? "Wiegand test header present"

echo "$OUTPUT" | grep -q "Generated"
check_result $? "Wiegand generation works"

echo "$OUTPUT" | grep -q "Parsed"
check_result $? "Wiegand parsing works"

# Test 3: Modbus test
echo ""
echo "Test 3: Modbus protocol..."
OUTPUT=$(./industrial_protocols --modbus 2>&1)

echo "$OUTPUT" | grep -q "MODBUS TEST"
check_result $? "Modbus test header present"

echo "$OUTPUT" | grep -q "CRC16"
check_result $? "CRC16 calculation present"

# Проверка эталонного CRC (0xC5CD для запроса 01 03 00 00 00 0A)
echo "$OUTPUT" | grep -q "C5CD"
check_result $? "CRC16 correct (expected 0xC5CD)"

# Test 4: RFID Emulator test
echo ""
echo "Test 4: RFID Emulator..."
OUTPUT=$(./industrial_protocols --rfid 2>&1)

echo "$OUTPUT" | grep -q "RFID EMULATOR TEST"
check_result $? "RFID emulator test header present"

echo "$OUTPUT" | grep -q "Loaded.*cards"
check_result $? "Cards loading works"

# Проверка создания файла test_cards.txt
if [ -f "test_cards.txt" ]; then
    check_result 0 "Test cards file created"
else
    check_result 1 "Test cards file created"
fi

# Test 5: Memory leaks check (опционально)
echo ""
echo "Test 5: Memory check..."
if command -v valgrind > /dev/null 2>&1; then
    valgrind --leak-check=full --error-exitcode=1 --quiet ./industrial_protocols > /dev/null 2>&1
    check_result $? "No memory leaks (valgrind)"
elif command -v leaks > /dev/null 2>&1; then
    leaks --atExit -- ./industrial_protocols > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        check_result 0 "No memory leaks (leaks)"
    else
        echo -e "${YELLOW}⚠ WARNING${NC}: Possible memory leaks detected"
    fi
else
    echo -e "${YELLOW}⚠ SKIP${NC}: No memory checker available (install valgrind or use macOS leaks)"
fi

# Итоги
echo ""
echo "======================================================"
echo "Results: ${GREEN}$TESTS_PASSED passed${NC}, ${RED}$TESTS_FAILED failed${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed! You can proceed to Episode 23.${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed. Check your implementation.${NC}"
    exit 1
fi
