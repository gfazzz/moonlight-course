# Episode 07: Deep Dive - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = dynamic_loader
SOLUTION_DIR = solution
ARTIFACTS_DIR = artifacts

# Source files
STARTER = starter.c
SOLUTION = $(SOLUTION_DIR)/dynamic_loader_solution.c
GENERATOR = $(SOLUTION_DIR)/generate_archive.c

# Default target: build starter
all: $(TARGET)

$(TARGET): $(STARTER)
	$(CC) $(CFLAGS) -o $(TARGET) $(STARTER)
	@echo "✅ Dynamic loader built successfully!"

# Build solution version
solution: $(SOLUTION)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/dynamic_loader_solution $(SOLUTION)
	@echo "✅ Solution built!"

# Generate test data
generate: $(GENERATOR)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/generate_archive $(GENERATOR)
	./$(ARTIFACTS_DIR)/generate_archive
	@echo "✅ Test data generated: archive.dat"

# Run tests
test: $(TARGET)
	@echo "🧪 Running tests..."
	@bash tests/test.sh

# Test solution
test-solution: solution
	@echo "🧪 Testing solution..."
	@./$(ARTIFACTS_DIR)/dynamic_loader_solution archive.dat

# Check for memory leaks (macOS)
leaks: $(TARGET)
	@echo "🔍 Checking for memory leaks..."
	@leaks --atExit -- ./$(TARGET) archive.dat || true

# Check for memory leaks (Linux/valgrind)
valgrind: $(TARGET)
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) archive.dat || echo "valgrind not installed"

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -f $(ARTIFACTS_DIR)/dynamic_loader_solution
	rm -f $(ARTIFACTS_DIR)/generate_archive
	rm -f $(ARTIFACTS_DIR)/*.o
	@echo "🧹 Cleaned!"

# Help
help:
	@echo "Available targets:"
	@echo "  make           - Build dynamic_loader from starter.c"
	@echo "  make solution  - Build solution version"
	@echo "  make generate  - Generate test archive file"
	@echo "  make test      - Run automated tests"
	@echo "  make leaks     - Check for memory leaks (macOS)"
	@echo "  make valgrind  - Check for memory leaks (Linux)"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make help      - Show this help"

.PHONY: all solution generate test test-solution leaks valgrind clean help
