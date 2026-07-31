# OPERATION MOONLIGHT — s03e08 (финал Season 3)
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/intercept.c
SOL     := solution/intercept.c
DATA    := channel.dat
BIN     := intercept

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
