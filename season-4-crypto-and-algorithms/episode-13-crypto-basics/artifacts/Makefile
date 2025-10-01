# ═══════════════════════════════════════════════════════════════
# Episode 13: Crypto Basics — Промежуточная задача
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = frequency_analyzer

.PHONY: all clean test

all: $(TARGET)

$(TARGET): frequency_analyzer.c
	$(CC) $(CFLAGS) -o $(TARGET) frequency_analyzer.c

test: $(TARGET)
	@echo "Running frequency analyzer test..."
	../tests/test_frequency.sh

clean:
	rm -f $(TARGET) *.o

help:
	@echo "Usage:"
	@echo "  make          - Compile frequency_analyzer"
	@echo "  make test     - Run tests"
	@echo "  make clean    - Remove compiled files"

