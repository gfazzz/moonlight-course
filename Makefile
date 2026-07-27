# OPERATION MOONLIGHT — корневой Makefile
#
#   make            — справка
#   make list       — все сезоны и серии
#   make test       — прогнать ВЕСЬ курс (80 серий + 7 проектов)
#   make projects   — запустить сквозные проекты всех сезонов
#   make clean      — убрать собранные бинарники

SEASONS := season-1-foundations \
           season-2-memory-and-pointers \
           season-3-networks \
           season-4-crypto-and-algorithms \
           season-5-financial-markets \
           season-6-embedded-iot \
           season-7-system-programming \
           season-8-ai-and-data \
           season-9-advanced-systems \
           season-10-final-mission

.PHONY: help list test projects clean

help:
	@echo "OPERATION MOONLIGHT — курс C через расследование"
	@echo ""
	@echo "  make list      — сезоны и серии"
	@echo "  make test      — прогнать весь курс"
	@echo "  make projects  — запустить сквозные проекты"
	@echo "  make clean     — убрать бинарники"
	@echo ""
	@echo "Начать:"
	@echo "  cd season-1-foundations/s01e01-strange-message"
	@echo "  cat README.md"
	@echo ""
	@echo "Внутри серии: make test — зелёный тест = серия пройдена."

list:
	@for s in $(SEASONS); do \
		echo ""; echo "== $$s"; \
		ls -d $$s/s[0-9]* 2>/dev/null | sed 's|.*/|   |'; \
		[ -d $$s/project ] && echo "   project/ (сквозной проект сезона)" || true; \
	done

test:
	@fail=0; \
	for s in $(SEASONS); do \
		printf "%-34s " "$$s"; \
		if $(MAKE) -s -C $$s test >/tmp/ml_$$s.log 2>&1; then \
			echo "OK"; \
		else echo "FAIL"; tail -8 /tmp/ml_$$s.log; fail=1; fi; \
	done; \
	echo ""; \
	[ $$fail -eq 0 ] && echo "Весь курс зелёный." || { echo "Есть падения."; exit 1; }

projects:
	@for s in $(SEASONS); do \
		[ -d $$s/project ] || continue; \
		echo ""; echo "===== $$s/project ====="; \
		$(MAKE) -s -C $$s/project run; \
	done

clean:
	@for s in $(SEASONS); do $(MAKE) -s -C $$s clean >/dev/null 2>&1 || true; done
	@echo "clean."
