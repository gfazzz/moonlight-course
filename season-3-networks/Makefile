# ═══════════════════════════════════════════════════════════════
# Season 3: Networks - Makefile
# ═══════════════════════════════════════════════════════════════
# 
# Этот Makefile управляет:
# 1. Компиляцией эпизодов (Episode 09-12)
# 2. Сборкой Season Project (network_interceptor)
# 
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# ═══════════════════════════════════════════════════════════════
# Season Project: network_interceptor
# ═══════════════════════════════════════════════════════════════

# TODO: После создания модулей раскомментируй эти строки!
# 
# После Episode 09: раскомментируй season-project/ip_tools.c
# После Episode 10: раскомментируй season-project/tcp_client.c
# После Episode 11: раскомментируй season-project/packet_sniffer.c
# После Episode 12: раскомментируй season-project/session_reconstructor.c

PROJECT = network_interceptor
PROJECT_SRC = season_project_starter.c

# TODO: Раскомментируй после создания модулей:
# PROJECT_SRC += season-project/ip_tools.c                  # Из Episode 09
# PROJECT_SRC += season-project/tcp_client.c                # Из Episode 10
# PROJECT_SRC += season-project/packet_sniffer.c            # Из Episode 11
# PROJECT_SRC += season-project/session_reconstructor.c     # Из Episode 12

PROJECT_OBJ = $(PROJECT_SRC:.c=.o)

# ═══════════════════════════════════════════════════════════════
# Основные цели
# ═══════════════════════════════════════════════════════════════

.PHONY: all episodes season_project clean help test_leaks

# По умолчанию собираем только эпизоды
all: episodes

# Собрать все эпизоды
episodes:
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Компиляция Episode 09-12"
	@echo "═══════════════════════════════════════════════════════════════"
	$(MAKE) -C episode-09-network-basics
	$(MAKE) -C episode-10-socket-programming
	$(MAKE) -C episode-11-packet-analysis
	$(MAKE) -C episode-12-encrypted-communications
	@echo ""
	@echo "✅ Все эпизоды скомпилированы!"
	@echo ""

# Собрать Season Project
season_project: $(PROJECT)

$(PROJECT): $(PROJECT_OBJ)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Линковка $(PROJECT)"
	@echo "═══════════════════════════════════════════════════════════════"
	$(CC) $(CFLAGS) -o $@ $^
	@echo ""
	@echo "✅ $(PROJECT) собран успешно!"
	@echo "   Запустите: ./$(PROJECT) --help"
	@echo "   (Требуется sudo для режимов sniff/full)"
	@echo ""

# Компиляция объектных файлов
%.o: %.c
	@echo "[CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

# ═══════════════════════════════════════════════════════════════
# Утилиты
# ═══════════════════════════════════════════════════════════════

# Очистка всех скомпилированных файлов
clean:
	@echo "Очистка Season 3..."
	rm -f $(PROJECT) $(PROJECT_OBJ)
	rm -f *.o *.out a.out
	rm -f season-project/*.o
	$(MAKE) -C episode-09-network-basics clean
	$(MAKE) -C episode-10-socket-programming clean
	$(MAKE) -C episode-11-packet-analysis clean
	$(MAKE) -C episode-12-encrypted-communications clean
	@echo "✅ Очистка завершена"

# Тестирование Season Project
test_project: $(PROJECT)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Тестирование $(PROJECT)"
	@echo "═══════════════════════════════════════════════════════════════"
	./$(PROJECT) --mode ip --host 203.0.113.42
	@echo ""

# Проверка утечек памяти
test_leaks: $(PROJECT)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Проверка утечек памяти"
	@echo "═══════════════════════════════════════════════════════════════"
	@# Определяем ОС
	@if [ "$$(uname)" = "Darwin" ]; then \
		echo "Используем leaks (macOS)..."; \
		leaks --atExit -- ./$(PROJECT) --mode ip --host 127.0.0.1; \
	elif [ "$$(uname)" = "Linux" ]; then \
		echo "Используем valgrind (Linux)..."; \
		valgrind --leak-check=full --show-leak-kinds=all ./$(PROJECT) --mode ip --host 127.0.0.1; \
	else \
		echo "⚠️  Автоматическая проверка недоступна на этой ОС"; \
		echo "   Запустите: ./$(PROJECT) --mode ip --host 127.0.0.1"; \
	fi
	@echo ""

# Справка
help:
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Season 3: Networks - Makefile Help"
	@echo "═══════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Основные команды:"
	@echo "  make               - Собрать все эпизоды"
	@echo "  make episodes      - Собрать все эпизоды"
	@echo "  make season_project - Собрать network_interceptor"
	@echo "  make clean         - Удалить скомпилированные файлы"
	@echo "  make test_project  - Тестировать network_interceptor"
	@echo "  make test_leaks    - Проверить утечки памяти (valgrind/leaks)"
	@echo "  make help          - Показать эту справку"
	@echo ""
	@echo "Работа с эпизодами:"
	@echo "  cd episode-09-network-basics && make"
	@echo "  cd episode-10-socket-programming && make"
	@echo "  cd episode-11-packet-analysis && make"
	@echo "  cd episode-12-encrypted-communications && make"
	@echo ""
	@echo "Season Project:"
	@echo "  1. Создай папку season-project/"
	@echo "  2. Создай модули (ip_tools, tcp_client, packet_sniffer, session_reconstructor)"
	@echo "  3. Раскомментируй PROJECT_SRC в Makefile"
	@echo "  4. Запусти: make season_project"
	@echo "  5. Используй: ./network_interceptor --help"
	@echo "  6. Режимы:"
	@echo "     ./network_interceptor --mode ip --host 203.0.113.42"
	@echo "     ./network_interceptor --mode tcp --host HOST --port PORT"
	@echo "     sudo ./network_interceptor --mode sniff --filter PORT"
	@echo "     sudo ./network_interceptor --mode full"
	@echo "  7. Проверь утечки: make test_leaks"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════"

# ═══════════════════════════════════════════════════════════════
# ИНСТРУКЦИИ ПО SEASON PROJECT
# ═══════════════════════════════════════════════════════════════
#
# После прохождения всех эпизодов:
#
# 1. Создай папку season-project/ и модули из своего кода:
#    $ mkdir season-project
#    - season-project/ip_tools.c/.h           (из Episode 09)
#    - season-project/tcp_client.c/.h         (из Episode 10)
#    - season-project/packet_sniffer.c/.h     (из Episode 11)
#    - season-project/session_reconstructor.c/.h (из Episode 12)
#
# 2. Раскомментируй строки PROJECT_SRC выше
#
# 3. Собери проект:
#    $ make season_project
#
# 4. Протестируй:
#    $ ./network_interceptor --mode ip --host 203.0.113.42
#    $ ./network_interceptor --mode tcp --host 203.0.113.42 --port 31337
#    $ sudo ./network_interceptor --mode sniff --filter 9999
#    $ sudo ./network_interceptor --mode full
#
# 5. Проверь утечки:
#    $ make test_leaks
#
# 6. Поздравляю! У тебя есть инструмент для Season 4-10! 🚀
#    network_interceptor будет использоваться в:
#    - Season 5: Financial (анализ HFT трафика)
#    - Season 6: Embedded/IoT (перехват команд дронов)
#    - Season 7: System Programming (мониторинг операций)
#    - Season 10: ФИНАЛ (satellite tracking + intercept)
#
# ═══════════════════════════════════════════════════════════════



