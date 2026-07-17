# OPERATION MOONLIGHT — Season 1 (агрегатор v2.0)
#
#   make test    — прогнать автотесты всех серий s01e01…s01e08
#   make list    — показать серии
#   make clean   — очистить бинарники во всех сериях

EPISODES := s01e01-strange-message \
            s01e02-full-message \
            s01e03-chain-of-clues \
            s01e04-patterns-in-time \
            s01e05-functions \
            s01e06-modules \
            s01e07-files-and-args \
            s01e08-assemble-tool

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s1_$$ep.log 2>&1 && grep -q PASS /tmp/s1_$$ep.log; then \
			echo "PASS"; \
		else \
			echo "FAIL"; tail -5 /tmp/s1_$$ep.log; fail=1; \
		fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 1: все серии зелёные." || { echo "Season 1: есть падения."; exit 1; }

list:
	@echo "Season 1 — Foundations:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
