# OPERATION MOONLIGHT — s01e01
# make        — собрать твой decoder из artifacts/
# make run    — собрать и запустить
# make test   — прогнать автотест (падает на solution/, если artifacts/ пуст)
# make solution — запустить эталон
# make clean  — убрать бинарники

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
ART     := artifacts/decoder.c
SOL     := solution/decoder.c
BIN     := decoder

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
