# OPERATION MOONLIGHT — Season 8 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s08e01…s08e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s08e01-bigdata-mmap \
            s08e02-statistics-outliers \
            s08e03-prng-shuffle \
            s08e04-shell-heapsort \
            s08e05-linear-regression \
            s08e06-perceptron \
            s08e07-backprop \
            s08e08-luna-ai

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-30s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s8_$$ep.log 2>&1 && grep -q PASS /tmp/s8_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s8_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 8: все серии зелёные." || { echo "Season 8: есть падения."; exit 1; }

list:
	@echo "Season 8 — AI & Data Science:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
