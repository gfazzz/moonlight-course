# Episode 06: Pointer Game - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = pointer_navigator
SOLUTION_DIR = solution
ARTIFACTS_DIR = artifacts

# Source files
STARTER = starter.c
SOLUTION = $(SOLUTION_DIR)/pointer_navigator_solution.c
GENERATOR = $(SOLUTION_DIR)/generate_pointers.c

# Default target: build starter
all: $(TARGET)

$(TARGET): $(STARTER)
	$(CC) $(CFLAGS) -o $(TARGET) $(STARTER)
	@echo "✅ Pointer navigator built successfully!"

# Build solution version
solution: $(SOLUTION)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/pointer_navigator_solution $(SOLUTION)
	@echo "✅ Solution built!"

# Generate test data
generate: $(GENERATOR)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/generate_pointers $(GENERATOR)
	./$(ARTIFACTS_DIR)/generate_pointers
	@echo "✅ Test data generated: pointers.dat"

# Run tests
test: $(TARGET)
	@echo "🧪 Running tests..."
	@bash tests/test.sh

# Test solution
test-solution: solution
	@echo "🧪 Testing solution..."
	@./$(ARTIFACTS_DIR)/pointer_navigator_solution pointers.dat

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -f $(ARTIFACTS_DIR)/pointer_navigator_solution
	rm -f $(ARTIFACTS_DIR)/generate_pointers
	rm -f $(ARTIFACTS_DIR)/*.o
	@echo "🧹 Cleaned!"

# Run with valgrind (if available)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) pointers.dat

# Help
help:
	@echo "Available targets:"
	@echo "  make           - Build pointer_navigator from starter.c"
	@echo "  make solution  - Build solution version"
	@echo "  make generate  - Generate test data file"
	@echo "  make test      - Run automated tests"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make valgrind  - Run with memory checker"
	@echo "  make help      - Show this help"

.PHONY: all solution generate test test-solution clean valgrind help
