# OPERATION MOONLIGHT — Season 4 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s04e01…s04e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s04e01-caesar-cipher \
            s04e02-frequency-analysis \
            s04e03-hash-functions \
            s04e04-hash-table \
            s04e05-sorting \
            s04e06-binary-search \
            s04e07-linked-list \
            s04e08-blockchain

.PHONY: test list clean project

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-28s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s4_$$ep.log 2>&1 && grep -q PASS /tmp/s4_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s4_$$ep.log; fail=1; fi; \
	done; \
	printf "%-26s " "project/"; \
	if $(MAKE) -s -C project test >/tmp/proj_$$$$.log 2>&1 && grep -q PASS /tmp/proj_$$$$.log; then \
		echo "PASS"; else echo "FAIL"; tail -6 /tmp/proj_$$$$.log; fail=1; fi; \
	[ $$fail -eq 0 ] && echo "Season 4: все серии зелёные." || { echo "Season 4: есть падения."; exit 1; }

list:
	@echo "Season 4 — Crypto & Algorithms:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."

# Season Project: собирается из модулей нескольких сезонов
project:
	@$(MAKE) -s -C project run
