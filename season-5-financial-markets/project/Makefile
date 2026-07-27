# OPERATION MOONLIGHT — SEASON PROJECT: trading_system
# Собирается из модулей Season 4 и Season 5: три единицы трансляции.

CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11

ROOT   := ../..
S4     := $(ROOT)/season-4-crypto-and-algorithms/lib
S5     := ../lib

INCLUDES := -I$(S4) -I$(S5)
MODULES  := $(S4)/ml_hash.c $(S5)/ml_indicators.c
BIN      := trading_system

.PHONY: all run test modules clean

all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (3 единицы трансляции из 2 сезонов)"

run: all
	@./$(BIN)

modules:
	@echo "Модули сборки:"
	@for m in $(MODULES); do echo "  $$m"; done
	@echo "  main.c (этот сезон)"

test:
	@cd tests && ./test.sh

clean:
	@rm -f $(BIN) *.o
	@echo "clean."
