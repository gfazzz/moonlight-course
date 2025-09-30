# Makefile for Episode 22: Industrial Protocols
# Wiegand & Modbus RTU

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = industrial_protocols

# Основная цель
all: $(TARGET)

$(TARGET): starter.c
	$(CC) $(CFLAGS) -o $(TARGET) starter.c

# Альтернативное имя
protocols: $(TARGET)

# Запуск всех тестов
run: $(TARGET)
	./$(TARGET)

# Отдельные тесты
test-wiegand: $(TARGET)
	./$(TARGET) --wiegand

test-modbus: $(TARGET)
	./$(TARGET) --modbus

test-rfid: $(TARGET)
	./$(TARGET) --rfid

# Автоматические тесты
test: $(TARGET)
	@echo "🧪 Running Episode 22 tests..."
	@./tests/test.sh

# Проверка на утечки памяти
memcheck: $(TARGET)
	@if command -v valgrind > /dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET); \
	elif command -v leaks > /dev/null 2>&1; then \
		leaks --atExit -- ./$(TARGET); \
	else \
		echo "⚠️  valgrind/leaks not found, skipping memory check"; \
	fi

# Очистка
clean:
	rm -f $(TARGET)
	rm -f test_cards.txt
	rm -f artifacts/*.o

# Справка
help:
	@echo "Episode 22: Industrial Protocols"
	@echo ""
	@echo "Targets:"
	@echo "  make           - Build program"
	@echo "  make run       - Run all tests"
	@echo "  make test      - Run automated tests"
	@echo "  make test-wiegand - Test Wiegand only"
	@echo "  make test-modbus  - Test Modbus only"
	@echo "  make test-rfid    - Test RFID emulator only"
	@echo "  make memcheck  - Check for memory leaks"
	@echo "  make clean     - Remove build artifacts"

.PHONY: all run test test-wiegand test-modbus test-rfid memcheck clean help protocols