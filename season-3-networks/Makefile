# OPERATION MOONLIGHT — Season 3 (агрегатор v2.0)
#   make test  — прогнать автотесты всех серий s03e01…s03e08
#   make list  — показать серии
#   make clean — очистить бинарники

EPISODES := s03e01-ip-addressing \
            s03e02-subnets-cidr \
            s03e03-byte-order \
            s03e04-sockets \
            s03e05-packet-structure \
            s03e06-checksum \
            s03e07-reassembly \
            s03e08-encrypted-channel

.PHONY: test list clean

test:
	@fail=0; \
	for ep in $(EPISODES); do \
		printf "%-26s " "$$ep"; \
		if $(MAKE) -s -C $$ep test >/tmp/s3_$$ep.log 2>&1 && grep -q PASS /tmp/s3_$$ep.log; then \
			echo "PASS"; \
		else echo "FAIL"; tail -6 /tmp/s3_$$ep.log; fail=1; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "Season 3: все серии зелёные." || { echo "Season 3: есть падения."; exit 1; }

list:
	@echo "Season 3 — Networks:"; for ep in $(EPISODES); do echo "  $$ep"; done

clean:
	@for ep in $(EPISODES); do $(MAKE) -s -C $$ep clean >/dev/null 2>&1 || true; done
	@echo "clean."
