# OPERATION MOONLIGHT — SEASON PROJECT: stealth_agent (S1 + S6 + S7)
CC     := gcc
CFLAGS := -Wall -Wextra -std=gnu11
ROOT   := ../..
S1     := $(ROOT)/season-1-foundations/lib
S6     := $(ROOT)/season-6-embedded-iot/lib
S7     := ../lib
INCLUDES := -I$(S1) -I$(S6) -I$(S7)
MODULES  := $(S1)/ml_crypto.c $(S6)/ml_crc.c $(S7)/ml_proc.c
BIN      := stealth_agent

.PHONY: all run test modules clean
all:
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(MODULES) main.c && echo "OK: $(BIN) (4 единицы трансляции из 3 сезонов)"
run: all
	@./$(BIN)
modules:
	@echo "Модули сборки:"; for m in $(MODULES); do echo "  $$m"; done; echo "  main.c (этот сезон)"
test:
	@cd tests && ./test.sh
clean:
	@rm -f $(BIN) *.o; echo "clean."
