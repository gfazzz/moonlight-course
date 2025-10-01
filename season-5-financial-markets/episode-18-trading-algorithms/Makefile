# Episode 18: Trading Algorithms
# Makefile for strategy backtester

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = strategy_tester
SOURCES = starter.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@bash tests/test.sh

run: $(TARGET)
	./$(TARGET) artifacts/strategy_logs.csv

.PHONY: all clean test run

