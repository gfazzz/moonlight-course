# Episode 30: Statistical Analysis - SOLUTION Makefile
# Cross-platform: Linux, macOS, FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm
# Note: All functions use standard C math library (no GSL required)

TARGET = statistical_analysis
SOURCES = statistical_analysis.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	./$(TARGET) --all

run: $(TARGET)
	./$(TARGET) --all

.PHONY: all clean test run

