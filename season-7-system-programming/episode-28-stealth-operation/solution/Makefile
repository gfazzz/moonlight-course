# Episode 28: Stealth Operation - SOLUTION Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread

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
