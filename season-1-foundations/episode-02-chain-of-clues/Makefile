# Makefile for Episode 02: Chain of Clues

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
ARTIFACTS = artifacts
BUILD = build

all: classifier router

$(BUILD):
	mkdir -p $(BUILD)

classifier: $(BUILD)
	@if [ -f $(ARTIFACTS)/classifier.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/classifier $(ARTIFACTS)/classifier.c; \
		echo "✓ classifier compiled"; \
	else \
		echo "✗ $(ARTIFACTS)/classifier.c not found"; \
		exit 1; \
	fi

router: $(BUILD)
	@if [ -f $(ARTIFACTS)/router.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/router $(ARTIFACTS)/router.c; \
		echo "✓ router compiled"; \
	else \
		echo "! $(ARTIFACTS)/router.c not found (optional)"; \
	fi

pattern: $(BUILD)
	@if [ -f $(ARTIFACTS)/pattern_finder.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/pattern_finder $(ARTIFACTS)/pattern_finder.c; \
		echo "✓ pattern_finder compiled"; \
	else \
		echo "! $(ARTIFACTS)/pattern_finder.c not found (optional)"; \
	fi

run: classifier
	@echo "=== Running classifier with test data ==="
	@$(BUILD)/classifier < data.dat

test: classifier
	@echo "=== Running automated tests ==="
	@cd tests && ./test.sh

clean:
	rm -rf $(BUILD)
	@echo "✓ Cleaned build directory"

help:
	@echo "Episode 02 - Makefile"
	@echo "===================="
	@echo ""
	@echo "Targets:"
	@echo "  make           - Compile all"
	@echo "  make classifier - Compile classifier only"
	@echo "  make run        - Compile and run with data.dat"
	@echo "  make test       - Run automated tests"
	@echo "  make clean      - Remove build files"
	@echo ""

.PHONY: all classifier router pattern run test clean help
