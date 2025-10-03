# Episode 27: IPC - SOLUTION Makefile
# Cross-platform: Linux, macOS, FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lpthread
# Note: -lrt not needed (POSIX shm_open works without it on Linux/macOS/FreeBSD)

TARGET = ipc
SOURCES = ipc.c
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
