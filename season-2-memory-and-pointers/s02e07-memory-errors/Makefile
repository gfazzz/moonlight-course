# OPERATION MOONLIGHT — s02e07
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/safe.c
SOL     := solution/safe.c
BIN     := safe

.PHONY: all run test solution asan-buggy clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol

# Посмотреть, как ASan ловит ошибки в учебном buggy.c:
asan-buggy:
	@$(CC) -fsanitize=address -g -o buggy buggy.c && ./buggy || true

clean:
	@rm -f $(BIN) $(BIN)_sol buggy
	@echo "clean."
