# Episode 26: Threads & Parallelism - SOLUTION Makefile
# Operation MOONLIGHT - Season 7

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread

TARGET = threads_parallelism
SOURCES = threads_parallelism.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running multithreading tests..."
	./$(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run

