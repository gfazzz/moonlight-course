# OPERATION MOONLIGHT — s07e03
CC      := gcc
CFLAGS  := -Wall -Wextra -std=gnu11
ART     := artifacts/daemon.c
SOL     := solution/daemon.c
BIN     := daemon_demo

.PHONY: all run test solution clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol

clean:
	@rm -f $(BIN) $(BIN)_sol
	@echo "clean."
