# Episode 15: Sorting & Search Algorithms
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

# Targets
TARGETS = sorting_demo search_demo algorithm_suite

all: $(TARGETS)

sorting_demo: artifacts/sorting_demo.c
	$(CC) $(CFLAGS) $< -o $@

search_demo: artifacts/search_demo.c
	$(CC) $(CFLAGS) $< -o $@

algorithm_suite: artifacts/algorithm_suite.c
	$(CC) $(CFLAGS) $< -o $@

test: all
	@echo "Running sorting tests..."
	./sorting_demo
	@echo "\nRunning search tests..."
	./search_demo

clean:
	rm -f $(TARGETS) *.o

.PHONY: all test clean
