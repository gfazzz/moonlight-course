# OPERATION MOONLIGHT — SEASON PROJECT: iot_controller (S4 + S6)
CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11
ROOT   := ../..
S4     := $(ROOT)/season-4-crypto-and-algorithms/lib
S6     := ../lib
INCLUDES := -I$(S4) -I$(S6)
MODULES  := $(S4)/ml_hash.c $(S6)/ml_crc.c $(S6)/ml_wiegand.c
BIN      := iot_controller

.PHONY: all run test modules clean
all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (4 единицы трансляции из 2 сезонов)"
run: all
	@./$(BIN)
modules:
	@echo "Модули сборки:"; for m in $(MODULES); do echo "  $$m"; done; echo "  main.c (этот сезон)"
test:
	@cd tests && ./test.sh
clean:
	@rm -f $(BIN) *.o; echo "clean."
