# Episode 24: IoT Integration - SOLUTION Makefile
# Operation MOONLIGHT - Season 6 (FINALE)

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = iot_integration
SOURCES = iot_integration.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running IoT attack simulation..."
	./$(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run

