# OPERATION MOONLIGHT — s05e06
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/tick.c
SOL     := solution/tick.c
BIN     := tick

.PHONY: all run test solution clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN) $(BIN).bin

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol $(BIN).bin

clean:
	@rm -f $(BIN) $(BIN)_sol $(BIN).bin
	@echo "clean."
