# OPERATION MOONLIGHT — Season 5 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s05e01…s05e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s05e01-ohlcv-parser \
            s05e02-spread-orderbook \
            s05e03-moving-averages \
            s05e04-rsi \
            s05e05-backtest \
            s05e06-volatility-var \
            s05e07-monte-carlo \
            s05e08-arbitrage

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s5_$$ep.log 2>&1 && grep -q PASS /tmp/s5_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s5_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 5: все серии зелёные." || { echo "Season 5: есть падения."; exit 1; }

list:
	@echo "Season 5 — Financial Markets:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
