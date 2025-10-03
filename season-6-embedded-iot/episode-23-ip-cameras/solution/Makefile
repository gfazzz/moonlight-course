# Episode 23: IP Cameras - SOLUTION Makefile
# Operation MOONLIGHT - Season 6

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = ip_cameras
SOURCES = ip_cameras.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running IP camera analysis..."
	./$(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run

