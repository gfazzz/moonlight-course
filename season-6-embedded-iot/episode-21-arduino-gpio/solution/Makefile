# Episode 21: Arduino & GPIO - SOLUTION Makefile
# Operation MOONLIGHT - Season 6

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = arduino_gpio
SOURCES = arduino_gpio.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running solution test..."
	./$(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run

