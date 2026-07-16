# OPERATION MOONLIGHT — s01e04 (многофайловая сборка)
# make / make run / make test / make solution / make clean

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
BIN     := decoder

.PHONY: all run test solution clean

all:
	@if [ -f artifacts/decoder.c ] && [ -f artifacts/crypto.c ]; then \
		$(CC) $(CFLAGS) -Iartifacts -o $(BIN) artifacts/crypto.c artifacts/decoder.c && echo "OK: $(BIN)"; \
	else echo "Нет artifacts/. Скопируй: cp -r starter/* artifacts/"; fi

run: all
	@./$(BIN)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -Isolution -o $(BIN)_sol solution/crypto.c solution/decoder.c && ./$(BIN)_sol

clean:
	@rm -f $(BIN) $(BIN)_sol
	@echo "clean."
