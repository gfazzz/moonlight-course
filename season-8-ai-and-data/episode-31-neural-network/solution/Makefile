# Episode 31: Neural Network from Scratch - SOLUTION Makefile
# Cross-platform: Linux, macOS, FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm
# Note: Neural network uses standard C math library

TARGET = neural_network
SOURCES = neural_network.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

demo: $(TARGET)
	./$(TARGET) --demo

train: $(TARGET)
	./$(TARGET) --train ../artifacts/training_data.csv

.PHONY: all clean demo train

