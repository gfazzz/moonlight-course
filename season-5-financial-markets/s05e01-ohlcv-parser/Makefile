# OPERATION MOONLIGHT — s05e01
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/parse.c
SOL     := solution/parse.c
DATA    := data/prices.csv
BIN     := parse

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
