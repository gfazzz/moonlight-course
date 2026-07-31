# OPERATION MOONLIGHT — s07e05
CC      := gcc
CFLAGS  := -Wall -Wextra -std=gnu11 -pthread
ART     := artifacts/race.c
SOL     := solution/race.c
BIN     := race_demo

.PHONY: all run test solution race clean

all:
	@if [ -f $(ART) ]; then $(CC) $(CFLAGS) -o $(BIN) $(ART) && echo "OK: $(BIN)"; \
	else echo "Нет $(ART). Скопируй: cp starter.c $(ART)"; fi

run: all
	@./$(BIN)

# Показать «плавающие» числа гонки: пять запусков подряд.
# Работает и до того, как ты написал свой код (тогда берётся эталон).
race:
	@src=$(ART); [ -f $$src ] || src=$(SOL); \
	$(CC) $(CFLAGS) -o $(BIN)_race $$src && \
	for i in 1 2 3 4 5; do ./$(BIN)_race 2>&1 >/dev/null | head -1; done; \
	rm -f $(BIN)_race

test:
	@cd tests && ./test.sh

solution:
	@$(CC) $(CFLAGS) -o $(BIN)_sol $(SOL) && ./$(BIN)_sol

clean:
	@rm -f $(BIN) $(BIN)_sol
	@echo "clean."
