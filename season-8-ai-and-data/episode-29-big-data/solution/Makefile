# Episode 29: Big Data Processing - SOLUTION Makefile
# Cross-platform: Linux, macOS, FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread -lm
# Note: mmap is POSIX standard (works on all UNIX systems)

TARGET = big_data
SOURCES = big_data.c
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

