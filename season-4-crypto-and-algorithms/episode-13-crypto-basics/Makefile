# Makefile for Episode 13: Crypto Basics
# MOONLIGHT OPERATION

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = crypto_tool
SOLUTION_TARGET = crypto_tool_solution

# Source files
STARTER_SRC = starter.c
SOLUTION_SRC = solution/crypto_solution.c

# Test files
TEST_SCRIPT = tests/test.sh
ENCRYPTED_FILES = message1.enc message2.enc message3.enc

.PHONY: all clean test solution debug help

# Default target
all: $(TARGET)
	@echo "✓ crypto_tool compiled successfully!"
	@echo "Run './crypto_tool xor message1.enc' to start"

# Compile starter code
$(TARGET): $(STARTER_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(STARTER_SRC)

# Compile solution (for reference)
solution: $(SOLUTION_SRC)
	$(CC) $(CFLAGS) -o $(SOLUTION_TARGET) $(SOLUTION_SRC)
	@echo "✓ Solution compiled: $(SOLUTION_TARGET)"

# Debug build with extra symbols
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)
	@echo "✓ Debug build ready for GDB"

# Run tests
test: $(TARGET)
	@echo "Running Episode 13 tests..."
	@bash $(TEST_SCRIPT)

# Generate test encrypted files
generate-test-files: solution
	@echo "Generating test encrypted files..."
	@./$(SOLUTION_TARGET) generate
	@echo "✓ Test files created: $(ENCRYPTED_FILES)"

# Clean build artifacts
clean:
	rm -f $(TARGET) $(SOLUTION_TARGET)
	rm -f *.o
	rm -f artifacts/*.txt
	@echo "✓ Cleaned build artifacts"

# Deep clean (including test files)
distclean: clean
	rm -f $(ENCRYPTED_FILES)
	@echo "✓ Deep clean complete"

# Help message
help:
	@echo "MOONLIGHT Episode 13 - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make              - Compile crypto_tool"
	@echo "  make solution     - Compile solution (for reference)"
	@echo "  make test         - Run automated tests"
	@echo "  make debug        - Compile with debug symbols"
	@echo "  make clean        - Remove compiled files"
	@echo "  make distclean    - Remove all generated files"
	@echo "  make help         - Show this message"
	@echo ""
	@echo "Usage:"
	@echo "  ./crypto_tool xor message1.enc"
	@echo "  ./crypto_tool caesar message2.enc"
	@echo "  ./crypto_tool multi message3.enc"
