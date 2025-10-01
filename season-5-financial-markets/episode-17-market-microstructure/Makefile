# Episode 17: Market Microstructure
# Makefile for OHLCV analyzer

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = market_analyzer
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
	./$(TARGET) artifacts/market_data.csv

.PHONY: all clean test run

