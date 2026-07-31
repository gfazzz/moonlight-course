# OPERATION MOONLIGHT — Season 9 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s09e01…s09e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s09e01-lexer \
            s09e02-parser-ast \
            s09e03-interpreter \
            s09e04-btree-storage \
            s09e05-wal-durability \
            s09e06-tui \
            s09e07-radar-kalman \
            s09e08-quantum

.PHONY: test list clean project

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-24s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s9_$$ep.log 2>&1 && grep -q PASS /tmp/s9_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s9_$$ep.log; fail=1; fi; \
	done; \
	printf "%-26s " "project/"; \
	if $(MAKE) -s -C project test >/tmp/proj_$$$$.log 2>&1 && grep -q PASS /tmp/proj_$$$$.log; then \
		echo "PASS"; else echo "FAIL"; tail -6 /tmp/proj_$$$$.log; fail=1; fi; \
	[ $$fail -eq 0 ] && echo "Season 9: все серии зелёные." || { echo "Season 9: есть падения."; exit 1; }

list:
	@echo "Season 9 — Advanced Topics:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."

# Season Project: собирается из модулей нескольких сезонов
project:
	@$(MAKE) -s -C project run
