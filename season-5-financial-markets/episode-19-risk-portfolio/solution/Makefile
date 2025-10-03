# Episode 19: Risk & Portfolio - SOLUTION Makefile
# Operation MOONLIGHT - Season 5

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
LDFLAGS = -lm

TARGET = risk_portfolio
SOURCES = risk_portfolio.c
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
	./$(TARGET) ../artifacts/portfolio_holdings.csv

run: $(TARGET)
	./$(TARGET) ../artifacts/portfolio_holdings.csv

.PHONY: all clean test run

