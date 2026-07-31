# OPERATION MOONLIGHT — SEASON PROJECT: advanced_systems (S4 + S6 + S9)
CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11
ROOT   := ../..
S4     := $(ROOT)/season-04-crypto-and-algorithms/lib
S6     := $(ROOT)/season-06-embedded-iot/lib
S9     := ../lib
INCLUDES := -I$(S4) -I$(S6) -I$(S9)
MODULES  := $(S4)/ml_hash.c $(S6)/ml_crc.c $(S9)/ml_btree.c $(S9)/ml_kalman.c
BIN      := advanced_systems

.PHONY: all run test modules clean
all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (5 единиц трансляции из 3 сезонов)"
run: all
	@./$(BIN)
modules:
	@echo "Модули сборки:"; for m in $(MODULES); do echo "  $$m"; done; echo "  main.c (этот сезон)"
test:
	@cd tests && ./test.sh
clean:
	@rm -f $(BIN) *.o; echo "clean."
