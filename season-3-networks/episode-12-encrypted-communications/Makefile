CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = comm_interceptor

all: $(TARGET)

$(TARGET): starter.c
	$(CC) $(CFLAGS) -o $(TARGET) starter.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
