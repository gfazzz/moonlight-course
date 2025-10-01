# Episode 20: Crypto & DeFi - FINALE
# Makefile for crypto analyzer

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = crypto_analyzer
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
	./$(TARGET) artifacts/pump_dump_history.csv artifacts/arbitrage_logs.csv

.PHONY: all clean test run

