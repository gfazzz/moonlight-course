# OPERATION MOONLIGHT — s02e08 (финал Season 2)
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/fixed_logger.c
SOL     := solution/fixed_logger.c
BIN     := fixed_logger

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

# Увидеть утечки в буgiom логгере:
asan-buggy:
	@$(CC) -fsanitize=address -g -o buggy buggy_logger.c && ./buggy || true

clean:
	@rm -f $(BIN) $(BIN)_sol buggy
	@echo "clean."
