# OPERATION MOONLIGHT — корневой Makefile
#
#   make            — справка
#   make list       — все сезоны и серии
#   make test       — прогнать ВЕСЬ курс (80 серий + 7 проектов)
#   make projects   — запустить сквозные проекты всех сезонов
#   make clean      — убрать собранные бинарники

SEASONS := season-01-foundations \
           season-02-memory-and-pointers \
           season-03-networks \
           season-04-crypto-and-algorithms \
           season-05-real-world-data \
           season-06-embedded-iot \
           season-07-system-programming \
           season-08-ai-and-data \
           season-09-advanced-systems \
           season-10-final-mission

.PHONY: help list test progress projects links check clean

help:
	@echo "OPERATION MOONLIGHT — курс C через расследование"
	@echo ""
	@echo "  make progress  — где ты сейчас: пройденные серии по сезонам"
	@echo "  make list      — сезоны и серии"
	@echo "  make test      — прогнать весь курс"
	@echo "  make links     — проверить ссылки между документами"
	@echo "  make check     — links + test (как в CI)"
	@echo "  make projects  — запустить сквозные проекты"
	@echo "  make clean     — убрать бинарники"
	@echo ""
	@echo "Начать:"
	@echo "  cd season-01-foundations/s01e01-strange-message"
	@echo "  cat README.md"
	@echo ""
	@echo "Внутри серии: make test — зелёный тест = серия пройдена."

# Сводка по операции: какие серии решены (тест зелёный на ТВОЁМ коде в artifacts/),
# какие ещё нет. Серия считается пройденной, только если в artifacts/ есть твой
# исходник и тест на нём проходит — наличие solution/ роли не играет.
progress:
	@echo ""
	@echo "  ┌──────────────────────────────────────────────────────────────┐"
	@echo "  │  OPERATION MOONLIGHT — СВОДКА ПО ОПЕРАЦИИ                    │"
	@echo "  └──────────────────────────────────────────────────────────────┘"
	@echo ""
	@total=0; done_all=0; next=""; \
	for s in $(SEASONS); do \
		n=0; d=0; \
		for e in $$s/s[0-9][0-9]e[0-9][0-9]*; do \
			[ -d "$$e" ] || continue; \
			n=$$((n+1)); total=$$((total+1)); \
			if ls $$e/artifacts/*.c >/dev/null 2>&1 && \
			   $(MAKE) -s -C $$e test >/dev/null 2>&1; then \
				d=$$((d+1)); done_all=$$((done_all+1)); \
			elif [ -z "$$next" ]; then next="$$e"; fi; \
		done; \
		[ $$n -eq 0 ] && continue; \
		bar=""; i=0; \
		while [ $$i -lt $$n ]; do \
			[ $$i -lt $$d ] && bar="$$bar#" || bar="$$bar."; i=$$((i+1)); \
		done; \
		printf "  %-32s [%s] %d/%d\n" "$$(echo $$s | sed 's/season-//;s/-/ /g')" "$$bar" "$$d" "$$n"; \
	done; \
	echo ""; \
	printf "  Всего пройдено: %d из %d серий\n" "$$done_all" "$$total"; \
	if [ -n "$$next" ]; then \
		echo ""; printf "  Следующая цель: %s\n" "$$next"; \
		printf "  → cd %s && cat README.md\n" "$$next"; \
	else \
		echo ""; echo "  Все серии пройдены. MOONLIGHT closed."; \
	fi; \
	echo ""

list:
	@for s in $(SEASONS); do \
		echo ""; echo "== $$s"; \
		ls -d $$s/s[0-9]* 2>/dev/null | sed 's|.*/|   |'; \
		[ -d $$s/project ] && echo "   project/ (сквозной проект сезона)" || true; \
	done

# Прогон курса. Лог каждого сезона сохраняется в tests/logs/<сезон>.log —
# из общего вывода не видно, КАКАЯ серия упала, а из лога видно.
#
#   make test                            — весь курс
#   make test SEASON=season-08-ai-and-data   — один сезон
LOGDIR := tests/logs

test:
	@mkdir -p $(LOGDIR)
	@fail=0; \
	for s in $(if $(SEASON),$(SEASON),$(SEASONS)); do \
		[ -d "$$s" ] || { echo "нет такого сезона: $$s"; exit 2; }; \
		printf "%-34s " "$$s"; \
		if $(MAKE) -s -C $$s test >$(LOGDIR)/$$s.log 2>&1; then \
			echo "OK"; \
		else \
			echo "FAIL   (лог: $(LOGDIR)/$$s.log)"; \
			echo "  ── упавшие серии:"; \
			grep -E "^[a-z0-9-]+ +FAIL|FAIL:" $(LOGDIR)/$$s.log | head -20 | sed 's/^/     /'; \
			echo "  ── подробности:"; \
			grep -A6 -E "FAIL:" $(LOGDIR)/$$s.log | head -30 | sed 's/^/     /'; \
			fail=1; \
		fi; \
	done; \
	echo ""; \
	if [ $$fail -eq 0 ]; then echo "$(if $(SEASON),Сезон зелёный.,Весь курс зелёный.)"; else \
		echo "Есть падения. Полные логи: $(LOGDIR)/*.log"; exit 1; fi

links:
	@bash tools/check_links.sh

# То же, что гоняет CI: ссылки и весь курс.
check: links test

projects:
	@for s in $(SEASONS); do \
		[ -d $$s/project ] || continue; \
		echo ""; echo "===== $$s/project ====="; \
		$(MAKE) -s -C $$s/project run; \
	done

clean:
	@for s in $(SEASONS); do $(MAKE) -s -C $$s clean >/dev/null 2>&1 || true; done
	@echo "clean."
