# Episode 32: ML Prediction & Deployment - SOLUTION Makefile
# Cross-platform: Linux, macOS, FreeBSD
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm
# Note: Real-time inference system

TARGET = prediction_system
SOURCES = prediction_system.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	./$(TARGET) ../artifacts/live_traffic.csv

deploy: $(TARGET)
	@echo "🚀 Deploying prediction system..."
	./$(TARGET) ../artifacts/live_traffic.csv

.PHONY: all clean test deploy

