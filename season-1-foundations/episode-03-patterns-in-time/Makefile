# Makefile for Episode 03: Patterns in Time

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
MATHLIB = -lm
ARTIFACTS = artifacts
BUILD = build
DATA = data.txt

all: pattern sequence

$(BUILD):
	mkdir -p $(BUILD)

pattern: $(BUILD)
	@if [ -f $(ARTIFACTS)/pattern_finder.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/pattern $(ARTIFACTS)/pattern_finder.c; \
		echo "✓ pattern_finder compiled"; \
	else \
		echo "✗ $(ARTIFACTS)/pattern_finder.c not found"; \
		exit 1; \
	fi

sequence: $(BUILD)
	@if [ -f $(ARTIFACTS)/sequence_analyzer.c ]; then \
		$(CC) $(CFLAGS) $(MATHLIB) -o $(BUILD)/sequence $(ARTIFACTS)/sequence_analyzer.c; \
		echo "✓ sequence_analyzer compiled (with -lm)"; \
	else \
		echo "! $(ARTIFACTS)/sequence_analyzer.c not found (optional)"; \
	fi

bruteforce: $(BUILD)
	@if [ -f $(ARTIFACTS)/bruteforce.c ]; then \
		$(CC) $(CFLAGS) -o $(BUILD)/bruteforce $(ARTIFACTS)/bruteforce.c; \
		echo "✓ bruteforce compiled"; \
	else \
		echo "! $(ARTIFACTS)/bruteforce.c not found (optional)"; \
	fi

run-pattern: pattern
	@echo "=== Running pattern finder ==="
	@$(BUILD)/pattern < $(DATA)

run-sequence: sequence
	@echo "=== Running sequence analyzer ==="
	@$(BUILD)/sequence < $(DATA)

run-bruteforce: bruteforce
	@echo "=== Running bruteforce ==="
	@$(BUILD)/bruteforce

test: pattern sequence
	@echo "=== Running automated tests ==="
	@cd tests && ./test.sh

clean:
	rm -rf $(BUILD)
	@echo "✓ Cleaned build directory"

help:
	@echo "Episode 03 - Makefile"
	@echo "===================="
	@echo ""
	@echo "Targets:"
	@echo "  make             - Compile all"
	@echo "  make pattern     - Compile pattern_finder"
	@echo "  make sequence    - Compile sequence_analyzer"
	@echo "  make bruteforce  - Compile bruteforce"
	@echo "  make run-pattern - Run pattern finder"
	@echo "  make run-sequence- Run sequence analyzer"
	@echo "  make test        - Run automated tests"
	@echo "  make clean       - Remove build files"
	@echo ""
	@echo "Note: sequence_analyzer requires -lm (math library)"

.PHONY: all pattern sequence bruteforce run-pattern run-sequence run-bruteforce test clean help
