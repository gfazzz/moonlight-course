#!/bin/bash
#
# test_ip_converter.sh
# 
# Тест для промежуточной задачи ip_converter
# Episode 09: Network Basics
#
# Использование:
#   bash tests/test_ip_converter.sh

set -e  # Выход при ошибке

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Счётчики
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

# Функция для заголовка
print_header() {
    echo -e "${CYAN}╔═══════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║  IP CONVERTER TEST SUITE                      ║${NC}"
    echo -e "${CYAN}║  Episode 09: Network Basics                   ║${NC}"
    echo -e "${CYAN}╚═══════════════════════════════════════════════╝${NC}"
    echo ""
}

# Функция для проверки наличия программы
check_program() {
    if [ ! -f "$1" ]; then
        echo -e "${RED}❌ Error: Program $1 not found!${NC}"
        echo -e "${YELLOW}Please compile first:${NC}"
        echo -e "  gcc -Wall -Wextra -std=c11 -o ip_converter solution/ip_converter_solution.c"
        exit 1
    fi
}

# Функция для одного теста
run_test() {
    local test_name="$1"
    local ip_addr="$2"
    local expected_pattern="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test ${TOTAL_TESTS}: ${test_name}${NC}"
    echo -e "  Input: ${ip_addr}"
    
    # Запустить программу с IP-адресом
    output=$(./ip_converter "$ip_addr" 2>&1)
    
    # Проверить, содержит ли вывод ожидаемый паттерн
    if echo "$output" | grep -q "$expected_pattern"; then
        echo -e "  ${GREEN}✓ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "  ${RED}✗ FAIL${NC}"
        echo -e "  Expected pattern: ${expected_pattern}"
        echo -e "  Actual output:"
        echo "$output" | sed 's/^/    /'
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Функция для теста невалидного IP
run_invalid_test() {
    local test_name="$1"
    local ip_addr="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test ${TOTAL_TESTS}: ${test_name}${NC}"
    echo -e "  Input: ${ip_addr}"
    
    # Запустить программу с невалидным IP
    output=$(./ip_converter "$ip_addr" 2>&1)
    
    # Должно быть сообщение об ошибке
    if echo "$output" | grep -qi "invalid\|error"; then
        echo -e "  ${GREEN}✓ PASS (correctly rejected)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "  ${RED}✗ FAIL (should reject invalid IP)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Главная функция
main() {
    print_header
    
    # Проверка наличия программы
    check_program "./ip_converter"
    
    echo -e "${CYAN}Running tests...${NC}\n"
    
    # Тест 1: Private IP (192.168.x.x)
    run_test \
        "Private IP (192.168.1.100)" \
        "192.168.1.100" \
        "0xC0A80164"
    
    # Тест 2: Private IP (10.0.0.5)
    run_test \
        "Private IP (10.0.0.5)" \
        "10.0.0.5" \
        "0x0A000005"
    
    # Тест 3: Public IP (8.8.8.8)
    run_test \
        "Public IP (8.8.8.8)" \
        "8.8.8.8" \
        "0x08080808"
    
    # Тест 4: Loopback (127.0.0.1)
    run_test \
        "Loopback (127.0.0.1)" \
        "127.0.0.1" \
        "Loopback"
    
    # Тест 5: MOONLIGHT IP (203.0.113.42)
    run_test \
        "MOONLIGHT IP (203.0.113.42)" \
        "203.0.113.42" \
        "0xCB00712A"
    
    # Тест 6: Private classification
    run_test \
        "Private classification (172.16.0.1)" \
        "172.16.0.1" \
        "Private"
    
    # Тест 7: Public classification
    run_test \
        "Public classification (1.1.1.1)" \
        "1.1.1.1" \
        "Public"
    
    # Тест 8: Invalid IP (256 in octet)
    run_invalid_test \
        "Invalid IP (10.0.0.256)" \
        "10.0.0.256"
    
    # Тест 9: Invalid IP (not an IP)
    run_invalid_test \
        "Invalid IP (not.an.ip.address)" \
        "not.an.ip.address"
    
    # Тест 10: Edge case (0.0.0.0)
    run_test \
        "Edge case (0.0.0.0)" \
        "0.0.0.0" \
        "0x00000000"
    
    # Результаты
    echo -e "${CYAN}═══════════════════════════════════════════════${NC}"
    echo -e "${CYAN}TEST RESULTS:${NC}"
    echo -e "  Total tests:  ${TOTAL_TESTS}"
    echo -e "  ${GREEN}Passed:       ${TESTS_PASSED}${NC}"
    
    if [ $TESTS_FAILED -gt 0 ]; then
        echo -e "  ${RED}Failed:       ${TESTS_FAILED}${NC}"
        echo -e "${CYAN}═══════════════════════════════════════════════${NC}"
        echo -e "${RED}❌ SOME TESTS FAILED${NC}"
        exit 1
    else
        echo -e "  ${RED}Failed:       ${TESTS_FAILED}${NC}"
        echo -e "${CYAN}═══════════════════════════════════════════════${NC}"
        echo -e "${GREEN}✅ ALL TESTS PASSED!${NC}"
        echo -e "${GREEN}🎯 IP Converter работает корректно!${NC}"
        exit 0
    fi
}

# Запуск
main

