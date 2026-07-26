# OPERATION MOONLIGHT — Season 7 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s07e01…s07e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s07e01-processes-fork \
            s07e02-exec \
            s07e03-daemons \
            s07e04-threads \
            s07e05-race-mutex \
            s07e06-pipes-ipc \
            s07e07-mmap-shared \
            s07e08-signals-stealth

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s7_$$ep.log 2>&1 && grep -q PASS /tmp/s7_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s7_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 7: все серии зелёные." || { echo "Season 7: есть падения."; exit 1; }

list:
	@echo "Season 7 — Systems Programming:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
