# Makefile for Episode 21: Arduino & GPIO

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = gpio_system

# Source files
SRC = starter.c
OBJ = $(SRC:.c=.o)

# Directories
ARTIFACTS = artifacts
SOLUTION = solution
TESTS = tests

.PHONY: all clean test run help

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "✅ Built $(TARGET)"

# Build solution
solution: $(SOLUTION)/gpio_solution.c
	$(CC) $(CFLAGS) -o $(SOLUTION)/gpio_solution $(SOLUTION)/gpio_solution.c
	@echo "✅ Solution built"

# Run program
run: $(TARGET)
	@echo "🚀 Running GPIO system simulation..."
	./$(TARGET)

# Run tests
test: $(TARGET)
	@echo "🧪 Running tests..."
	@bash $(TESTS)/test.sh

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJ)
	rm -f $(SOLUTION)/gpio_solution
	@echo "🧹 Cleaned"

help:
	@echo "Episode 21: Arduino & GPIO"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the program"
	@echo "  make run      - Run the simulation"
	@echo "  make test     - Run tests"
	@echo "  make solution - Build solution"
	@echo "  make clean    - Clean build files"
	@echo "  make help     - Show this help"
