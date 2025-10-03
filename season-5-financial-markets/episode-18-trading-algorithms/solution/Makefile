# Episode 18: Trading Algorithms - SOLUTION Makefile
# Operation MOONLIGHT - Season 5

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = trading_algorithms
SOURCES = trading_algorithms.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running solution test..."
	./$(TARGET) ../artifacts/historical_prices.csv

run: $(TARGET)
	./$(TARGET) ../artifacts/historical_prices.csv

.PHONY: all clean test run

