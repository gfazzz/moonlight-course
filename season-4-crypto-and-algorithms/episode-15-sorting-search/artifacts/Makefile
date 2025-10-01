# ═══════════════════════════════════════════════════════════════
# Episode 15: Sorting & Search — Промежуточная задача
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = sorting_race

.PHONY: all clean test

all: $(TARGET)

$(TARGET): sorting_race.c
	$(CC) $(CFLAGS) -o $(TARGET) sorting_race.c

test: $(TARGET)
	@echo "Running sorting race test..."
	../tests/test_sorting_race.sh

clean:
	rm -f $(TARGET) *.o

help:
	@echo "Usage:"
	@echo "  make          - Compile sorting_race"
	@echo "  make test     - Run tests"
	@echo "  make clean    - Remove compiled files"

