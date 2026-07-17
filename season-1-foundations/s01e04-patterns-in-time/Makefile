# OPERATION MOONLIGHT — s01e03
# make / make run / make test / make solution / make clean

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/pattern_finder.c
SOL     := solution/pattern_finder.c
DATA    := data.txt
BIN     := pattern_finder

.PHONY: all run test solution clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN) < $(DATA)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol < $(DATA)

clean:
	@rm -f $(BIN) $(BIN)_sol
	@echo "clean."
