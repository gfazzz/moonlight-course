# Episode 08: Fragmentation - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
ARTIFACTS_DIR = artifacts
SOLUTION_DIR = solution

# Targets
BUGGY = buggy_logger
STARTER = leak_hunter
FIXED = fixed_logger
SOLUTION = $(SOLUTION_DIR)/fixed_logger_solution

# Default: build starter
all: $(STARTER)

# Buggy version (for demonstration)
buggy: buggy_logger.c
	$(CC) $(CFLAGS) -o $(BUGGY) buggy_logger.c
	@echo "✅ Buggy logger built (contains leaks!)"

# Starter version (student works on this)
$(STARTER): starter.c
	$(CC) $(CFLAGS) -o $(STARTER) starter.c
	@echo "✅ Starter version built. Fix the leaks!"

# Fixed version (student creates this)
fixed: fixed_logger.c
	$(CC) $(CFLAGS) -o $(FIXED) fixed_logger.c
	@echo "✅ Fixed logger built!"

# Solution version
solution: $(SOLUTION).c
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/fixed_logger_solution $(SOLUTION).c
	@echo "✅ Solution built!"

# Generate test config
config:
	@echo "log_dir=/tmp/moonlight_logs" > config.txt
	@echo "max_buffer_size=100" >> config.txt
	@echo "rotation_size=1024" >> config.txt
	@echo "enable_timestamps=1" >> config.txt
	@echo "✅ config.txt created"

# Generate test input
test-input:
	@echo "log Test message 1" > test_input.txt
	@echo "log Test message 2" >> test_input.txt
	@echo "log Test message 3" >> test_input.txt
	@echo "print" >> test_input.txt
	@echo "rotate" >> test_input.txt
	@echo "log After rotation" >> test_input.txt
	@echo "print" >> test_input.txt
	@echo "quit" >> test_input.txt
	@echo "✅ test_input.txt created"

# Run tests
test: $(STARTER) config test-input
	@echo "🧪 Running starter (may have leaks)..."
	@./$(STARTER) < test_input.txt

# Test buggy version (should show leaks)
test-buggy: buggy config test-input
	@echo "🧪 Running buggy version (will leak)..."
	@./$(BUGGY) < test_input.txt

# Test fixed version
test-fixed: fixed config test-input
	@echo "🧪 Running fixed version..."
	@./$(FIXED) < test_input.txt

# Test solution
test-solution: solution config test-input
	@echo "🧪 Running solution..."
	@./$(ARTIFACTS_DIR)/fixed_logger_solution < test_input.txt

# Memory leak check (macOS)
leaks: $(STARTER) config test-input
	@echo "🔍 Checking for leaks (macOS)..."
	@leaks --atExit -- ./$(STARTER) < test_input.txt || true

leaks-buggy: buggy config test-input
	@echo "🔍 Checking buggy version for leaks..."
	@leaks --atExit -- ./$(BUGGY) < test_input.txt || true

leaks-fixed: fixed config test-input
	@echo "🔍 Checking fixed version for leaks..."
	@leaks --atExit -- ./$(FIXED) < test_input.txt || true

# Memory leak check (Linux)
valgrind: $(STARTER) config test-input
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(STARTER) < test_input.txt || echo "valgrind not installed"

valgrind-buggy: buggy config test-input
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(BUGGY) < test_input.txt || echo "valgrind not installed"

valgrind-fixed: fixed config test-input
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(FIXED) < test_input.txt || echo "valgrind not installed"

# Clean
clean:
	rm -f $(BUGGY) $(STARTER) $(FIXED)
	rm -f $(ARTIFACTS_DIR)/fixed_logger_solution
	rm -f config.txt test_input.txt
	@echo "🧹 Cleaned!"

# Help
help:
	@echo "Available targets:"
	@echo "  make              - Build starter version (leak_hunter)"
	@echo "  make buggy        - Build buggy version (with leaks)"
	@echo "  make fixed        - Build your fixed version"
	@echo "  make solution     - Build solution version"
	@echo ""
	@echo "  make config       - Generate config.txt"
	@echo "  make test-input   - Generate test_input.txt"
	@echo ""
	@echo "  make test         - Run starter version"
	@echo "  make test-buggy   - Run buggy version"
	@echo "  make test-fixed   - Run fixed version"
	@echo ""
	@echo "  make leaks        - Check starter for leaks (macOS)"
	@echo "  make leaks-buggy  - Check buggy for leaks (macOS)"
	@echo "  make leaks-fixed  - Check fixed for leaks (macOS)"
	@echo ""
	@echo "  make valgrind     - Check starter for leaks (Linux)"
	@echo "  make valgrind-buggy - Check buggy for leaks (Linux)"
	@echo "  make valgrind-fixed - Check fixed for leaks (Linux)"
	@echo ""
	@echo "  make clean        - Remove all build files"

.PHONY: all buggy fixed solution config test-input test test-buggy test-fixed test-solution \
        leaks leaks-buggy leaks-fixed valgrind valgrind-buggy valgrind-fixed clean help
