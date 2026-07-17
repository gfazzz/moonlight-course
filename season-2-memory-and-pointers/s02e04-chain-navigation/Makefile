# OPERATION MOONLIGHT — s02e04
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/navigator.c
SOL     := solution/navigator.c
DATA    := pointers.dat
BIN     := navigator

.PHONY: all run test solution clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN) $(DATA)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol $(DATA)

clean:
	@rm -f $(BIN) $(BIN)_sol
	@echo "clean."
