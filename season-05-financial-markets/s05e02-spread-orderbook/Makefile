# OPERATION MOONLIGHT — s05e02
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/book.c
SOL     := solution/book.c
DATA    := data/book.csv
BIN     := book

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
