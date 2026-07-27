# OPERATION MOONLIGHT — Season 10 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s10e01…s10e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s10e01-architecture \
            s10e02-orbital \
            s10e03-comm-window \
            s10e04-profiling \
            s10e05-hardening \
            s10e06-dashboard \
            s10e07-pipeline \
            s10e08-final-operation

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s10_$$ep.log 2>&1 && grep -q PASS /tmp/s10_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s10_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 10: все серии зелёные." || { echo "Season 10: есть падения."; exit 1; }

list:
	@echo "Season 10 — Final Mission:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
