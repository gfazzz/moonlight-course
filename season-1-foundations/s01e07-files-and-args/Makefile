# OPERATION MOONLIGHT — s01e07
# make / make run / make test / make solution / make clean

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/decoder.c
SOL     := solution/decoder.c
DATA    := message.enc
BIN     := decoder

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
