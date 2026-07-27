# OPERATION MOONLIGHT — SEASON PROJECT: crypto_toolkit (S1 + S4)
CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11
ROOT   := ../..
S1     := $(ROOT)/season-1-foundations/lib
S4     := ../lib
INCLUDES := -I$(S1) -I$(S4)
MODULES  := $(S1)/ml_crypto.c $(S4)/ml_hash.c
BIN      := crypto_toolkit

.PHONY: all run test modules clean
all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (3 единицы трансляции из 2 сезонов)"
run: all
	@./$(BIN)
modules:
	@echo "Модули сборки:"; for m in $(MODULES); do echo "  $$m"; done; echo "  main.c (этот сезон)"
test:
	@cd tests && ./test.sh
clean:
	@rm -f $(BIN) *.o; echo "clean."
