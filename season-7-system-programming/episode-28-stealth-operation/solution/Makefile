# Episode 28: Stealth Operation - SOLUTION Makefile
# Cross-platform: Linux, macOS (x86-64 + Apple Silicon), FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread
# Note: Cache/timing code includes platform detection (__x86_64__, __aarch64__)

TARGET = stealth_operation
SOURCES = stealth_operation.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run
