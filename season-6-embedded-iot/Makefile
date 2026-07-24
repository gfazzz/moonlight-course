# OPERATION MOONLIGHT — Season 6 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s06e01…s06e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s06e01-gpio-registers \
            s06e02-uart \
            s06e03-i2c-spi \
            s06e04-wiegand-access \
            s06e05-modbus \
            s06e06-sensors-fsm \
            s06e07-ip-cameras \
            s06e08-mavlink-drone

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-24s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s6_$$ep.log 2>&1 && grep -q PASS /tmp/s6_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s6_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 6: все серии зелёные." || { echo "Season 6: есть падения."; exit 1; }

list:
	@echo "Season 6 — Embedded & IoT:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
