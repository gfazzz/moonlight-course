# OPERATION MOONLIGHT — s01e05 (сборка через объектные файлы)
# Демонстрирует .c -> .o -> бинарь: раздельная компиляция + линковка.
#
# make / make run / make test / make solution / make clean

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -Iartifacts
BIN     := moonlight_decoder
OBJS    := artifacts/crypto.o artifacts/decoder.o

.PHONY: all run test solution clean

all: check $(BIN)

check:
	@[ -f artifacts/decoder.c ] && [ -f artifacts/crypto.c ] || \
	 { echo "Нет artifacts/. Скопируй: cp -r starter/* artifacts/"; exit 1; }

# Правило: каждый .c компилируется в свой .o (раздельная компиляция).
artifacts/%.o: artifacts/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка объектных файлов в исполняемый.
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)
	@echo "OK: $(BIN)"

run: all
	@./$(BIN)

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS:artifacts=solution) -Isolution -o $(BIN)_sol solution/crypto.c solution/decoder.c && ./$(BIN)_sol

clean:
	@rm -f $(BIN) $(BIN)_sol artifacts/*.o
	@echo "clean."
