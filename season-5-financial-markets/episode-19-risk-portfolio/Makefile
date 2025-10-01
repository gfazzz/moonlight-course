# Episode 19: Risk & Portfolio Management
# Makefile for portfolio optimizer

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = portfolio_manager
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
	./$(TARGET) artifacts/portfolio_holdings.csv

.PHONY: all clean test run

