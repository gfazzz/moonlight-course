# ═══════════════════════════════════════════════════════════════
# Episode 16: Data Structures — Промежуточная задача
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = linked_list_cycle

.PHONY: all clean test

all: $(TARGET)

$(TARGET): linked_list_cycle.c
	$(CC) $(CFLAGS) -o $(TARGET) linked_list_cycle.c

test: $(TARGET)
	@echo "Running linked list cycle test..."
	../tests/test_linked_list_cycle.sh

clean:
	rm -f $(TARGET) *.o

help:
	@echo "Usage:"
	@echo "  make          - Compile linked_list_cycle"
	@echo "  make test     - Run tests"
	@echo "  make clean    - Remove compiled files"

