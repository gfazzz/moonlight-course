# Makefile for Episode 14: Blockchain & Hash Functions
# MOONLIGHT OPERATION

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = blockchain_verifier
SOLUTION_TARGET = blockchain_verifier_solution
BLOCKCHAIN_FILE = moonlight_chain.dat

.PHONY: all clean test solution generate-blockchain debug help

# Default target
all: $(TARGET)
	@echo "✓ blockchain_verifier compiled successfully!"
	@echo "Run './blockchain_verifier $(BLOCKCHAIN_FILE)' to start"

# Compile starter code
$(TARGET): starter.c
	$(CC) $(CFLAGS) -o $(TARGET) starter.c

# Compile solution
solution: solution/blockchain_solution.c
	$(CC) $(CFLAGS) -o $(SOLUTION_TARGET) solution/blockchain_solution.c
	@echo "✓ Solution compiled"

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)
	@echo "✓ Debug build ready"

# Generate test blockchain
generate-blockchain: solution
	@echo "Generating test blockchain..."
	@./$(SOLUTION_TARGET) generate $(BLOCKCHAIN_FILE)
	@echo "✓ Blockchain generated: $(BLOCKCHAIN_FILE)"

# Run tests
test: $(TARGET) generate-blockchain
	@echo "Running Episode 14 tests..."
	@bash tests/test.sh

# Clean
clean:
	rm -f $(TARGET) $(SOLUTION_TARGET)
	rm -f *.o
	rm -f artifacts/*.txt
	@echo "✓ Cleaned"

# Deep clean
distclean: clean
	rm -f $(BLOCKCHAIN_FILE)
	@echo "✓ Deep clean complete"

# Help
help:
	@echo "MOONLIGHT Episode 14 - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make                    - Compile blockchain_verifier"
	@echo "  make solution           - Compile solution"
	@echo "  make generate-blockchain - Create test blockchain"
	@echo "  make test               - Run tests"
	@echo "  make debug              - Debug build"
	@echo "  make clean              - Remove binaries"
	@echo "  make distclean          - Remove all generated files"
	@echo ""
	@echo "Usage:"
	@echo "  ./blockchain_verifier moonlight_chain.dat"
