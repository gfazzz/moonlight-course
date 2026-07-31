# OPERATION MOONLIGHT — s07e01
# -std=gnu11: системные вызовы POSIX (fork/waitpid) без ручных feature-макросов
CC      := gcc
CFLAGS  := -Wall -Wextra -std=gnu11
ART     := artifacts/fork.c
SOL     := solution/fork.c
BIN     := fork_demo

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
