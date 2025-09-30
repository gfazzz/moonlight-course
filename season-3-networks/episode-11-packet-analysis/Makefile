CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = packet_sniffer

all: $(TARGET)

$(TARGET): starter.c
	$(CC) $(CFLAGS) -o $(TARGET) starter.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
