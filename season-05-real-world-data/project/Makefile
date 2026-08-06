# OPERATION MOONLIGHT — SEASON PROJECT: trading_system
# Собирается из модулей Season 4 и Season 5: семь единиц трансляции.

CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11

ROOT   := ../..
S4     := $(ROOT)/season-04-crypto-and-algorithms/lib
S5     := ../lib

INCLUDES := -I$(S4) -I$(S5)
MODULES  := $(S4)/ml_hash.c \
            $(S5)/ml_csv.c $(S5)/ml_money.c $(S5)/ml_time.c \
            $(S5)/ml_text.c $(S5)/ml_tick.c
BIN      := trading_system

.PHONY: all run test modules clean

all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (7 единиц трансляции из 2 сезонов)"

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
