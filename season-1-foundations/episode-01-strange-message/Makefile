# Makefile for Episode 01: Strange Message

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
ARTIFACTS = artifacts
BUILD = build

# Default target
all: decoder warmup

# Create build directory
$(BUILD):
	mkdir -p $(BUILD)

# Decoder (main task)
decoder: $(BUILD)
	@if [ -f $(ARTIFACTS)/decoder.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/decoder $(ARTIFACTS)/decoder.c; \
		echo "✓ decoder compiled successfully"; \
	else \
		echo "✗ $(ARTIFACTS)/decoder.c not found"; \
		exit 1; \
	fi

# Warmup
warmup: $(BUILD)
	@if [ -f $(ARTIFACTS)/warmup.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/warmup $(ARTIFACTS)/warmup.c; \
		echo "✓ warmup compiled successfully"; \
	else \
		echo "! $(ARTIFACTS)/warmup.c not found (optional)"; \
	fi

# Validator (bonus)
validator: $(BUILD)
	@if [ -f $(ARTIFACTS)/validator.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/validator $(ARTIFACTS)/validator.c; \
		echo "✓ validator compiled successfully"; \
	else \
		echo "! $(ARTIFACTS)/validator.c not found (optional)"; \
	fi

# Run decoder
run: decoder
	@echo "=== Running decoder ==="
	@$(BUILD)/decoder

# Run tests
test: decoder
	@echo "=== Running automated tests ==="
	@cd tests && ./test.sh

# Clean build artifacts
clean:
	rm -rf $(BUILD)
	@echo "✓ Cleaned build directory"

# Show help
help:
	@echo "MOONLIGHT Episode 01 - Makefile"
	@echo "================================"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Compile all programs"
	@echo "  make decoder  - Compile decoder only"
	@echo "  make warmup   - Compile warmup only"
	@echo "  make run      - Compile and run decoder"
	@echo "  make test     - Run automated tests"
	@echo "  make clean    - Remove build files"
	@echo "  make help     - Show this help"
	@echo ""
	@echo "Quick start:"
	@echo "  1. Create artifacts/decoder.c"
	@echo "  2. Run 'make' to compile"
	@echo "  3. Run 'make test' to verify"

.PHONY: all decoder warmup validator run test clean help
