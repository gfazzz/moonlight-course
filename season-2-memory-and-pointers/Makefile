# OPERATION MOONLIGHT — Season 2 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s02e01…s02e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s02e01-memory-map \
            s02e02-pointers \
            s02e03-pointer-arithmetic \
            s02e04-chain-navigation \
            s02e05-heap \
            s02e06-realloc \
            s02e07-memory-errors \
            s02e08-leak-hunt

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s2_$$ep.log 2>&1 && grep -q PASS /tmp/s2_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s2_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 2: все серии зелёные." || { echo "Season 2: есть падения."; exit 1; }

list:
	@echo "Season 2 — Memory & Pointers:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
