# Episode 25: Processes & Daemons - SOLUTION Makefile
# Operation MOONLIGHT - Season 7

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread

TARGET = processes_daemons
SOURCES = processes_daemons.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) /tmp/moonlight_daemon.pid /tmp/moonlight_daemon.log

test: $(TARGET)
	@echo "Running process management tests..."
	./$(TARGET) --all

test-daemon: $(TARGET)
	@echo "Starting daemon..."
	./$(TARGET) --daemon

run: $(TARGET)
	./$(TARGET) --all

.PHONY: all clean test test-daemon run

