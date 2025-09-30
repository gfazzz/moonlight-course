# Episode 05: Memory Map - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = memory_mapper
SOLUTION_DIR = solution
ARTIFACTS_DIR = artifacts

# Source files
STARTER = starter.c
SOLUTION = $(SOLUTION_DIR)/memory_mapper_solution.c
GENERATOR = $(SOLUTION_DIR)/generate_dump.c

# Default target: build starter
all: $(TARGET)

$(TARGET): $(STARTER)
	$(CC) $(CFLAGS) -o $(TARGET) $(STARTER)
	@echo "✅ Memory mapper built successfully!"

# Build solution version
solution: $(SOLUTION)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/memory_mapper_solution $(SOLUTION)
	@echo "✅ Solution built!"

# Generate test data
generate: $(GENERATOR)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/generate_dump $(GENERATOR)
	./$(ARTIFACTS_DIR)/generate_dump
	@echo "✅ Test data generated: memory_dump.dat"

# Run tests
test: $(TARGET)
	@echo "🧪 Running tests..."
	@bash tests/test.sh

# Test solution
test-solution: solution
	@echo "🧪 Testing solution..."
	@./$(ARTIFACTS_DIR)/memory_mapper_solution memory_dump.dat

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -f $(ARTIFACTS_DIR)/memory_mapper_solution
	rm -f $(ARTIFACTS_DIR)/generate_dump
	rm -f $(ARTIFACTS_DIR)/*.o
	@echo "🧹 Cleaned!"

# Run with valgrind (if available)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) memory_dump.dat

# Help
help:
	@echo "Available targets:"
	@echo "  make           - Build memory_mapper from starter.c"
	@echo "  make solution  - Build solution version"
	@echo "  make generate  - Generate test data file"
	@echo "  make test      - Run automated tests"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make valgrind  - Run with memory checker"
	@echo "  make help      - Show this help"

.PHONY: all solution generate test test-solution clean valgrind help
