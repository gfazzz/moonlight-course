# Episode 16: Data Structures
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

# Targets
TARGETS = linked_list stack_queue bst hash_table database_engine

all: $(TARGETS)

linked_list: artifacts/linked_list.c
	$(CC) $(CFLAGS) $< -o $@

stack_queue: artifacts/stack_queue.c
	$(CC) $(CFLAGS) $< -o $@

bst: artifacts/bst.c
	$(CC) $(CFLAGS) $< -o $@

hash_table: artifacts/hash_table.c
	$(CC) $(CFLAGS) $< -o $@

database_engine: artifacts/database_engine.c
	$(CC) $(CFLAGS) $< -o $@

test: all
	@echo "Running data structure tests..."
	./linked_list
	./stack_queue
	./bst
	./hash_table

clean:
	rm -f $(TARGETS) *.o

.PHONY: all test clean
