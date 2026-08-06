# OPERATION MOONLIGHT — s05e03
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/csv.c
SOL     := solution/csv.c
DATA    := data/journal.csv
BIN     := csv

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
