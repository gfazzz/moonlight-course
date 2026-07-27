# OPERATION MOONLIGHT — SEASON PROJECT: moonlight_core
#
# Здесь видно главное: программа собирается из МОДУЛЕЙ РАЗНЫХ СЕЗОНОВ.
# Компилируются пять отдельных единиц трансляции и линкуются вместе.

CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11

ROOT   := ../..
S4     := $(ROOT)/season-4-crypto-and-algorithms/lib
S6     := $(ROOT)/season-6-embedded-iot/lib
S8     := $(ROOT)/season-8-ai-and-data/lib
S9     := $(ROOT)/season-9-advanced-systems/lib

INCLUDES := -I$(S4) -I$(S6) -I$(S8) -I$(S9)
MODULES  := $(S4)/ml_hash.c $(S6)/ml_crc.c $(S8)/ml_stats.c $(S9)/ml_kalman.c
BIN      := moonlight_core

.PHONY: all run test modules clean

all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (5 единиц трансляции из 4 сезонов)"

run: all
	@./$(BIN)

# Показать, из чего собрана программа
modules:
	@echo "Модули, подключаемые из других сезонов:"
	@for m in $(MODULES); do echo "  $$m"; done
	@echo "  main.c (этот сезон)"

test:
	@cd tests && ./test.sh

clean:
	@rm -f $(BIN) *.o
	@echo "clean."
