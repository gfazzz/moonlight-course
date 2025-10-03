# Episode 22: Industrial Protocols - SOLUTION Makefile
# Operation MOONLIGHT - Season 6

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = industrial_solution
SOURCES = industrial_solution.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) test_cards.txt

test: $(TARGET)
	@echo "Running full test suite..."
	./$(TARGET)

test-acs: $(TARGET)
	@echo "Running Access Control System test..."
	./$(TARGET) --acs

run: $(TARGET)
	./$(TARGET) --acs

.PHONY: all clean test test-acs run

