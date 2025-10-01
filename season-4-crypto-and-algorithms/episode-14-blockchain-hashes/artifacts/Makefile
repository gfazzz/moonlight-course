# ═══════════════════════════════════════════════════════════════
# Episode 14: Blockchain & Hashes — Промежуточная задача
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = hash_avalanche_tester

.PHONY: all clean test

all: $(TARGET)

$(TARGET): hash_avalanche_tester.c
	$(CC) $(CFLAGS) -o $(TARGET) hash_avalanche_tester.c

test: $(TARGET)
	@echo "Running hash avalanche tester test..."
	../tests/test_avalanche.sh

clean:
	rm -f $(TARGET) *.o

help:
	@echo "Usage:"
	@echo "  make          - Compile hash_avalanche_tester"
	@echo "  make test     - Run tests"
	@echo "  make clean    - Remove compiled files"

