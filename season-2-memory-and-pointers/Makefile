# ═══════════════════════════════════════════════════════════════
# Season 2: Memory & Pointers - Makefile
# ═══════════════════════════════════════════════════════════════
# 
# Этот Makefile управляет:
# 1. Компиляцией эпизодов (Episode 05-08)
# 2. Сборкой Season Project (memory_manager)
# 
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# ═══════════════════════════════════════════════════════════════
# Season Project: memory_manager
# ═══════════════════════════════════════════════════════════════

# TODO: После создания модулей раскомментируй эти строки!
# 
# После Episode 05: раскомментируй array_tools.c
# После Episode 06: раскомментируй pointer_utils.c
# После Episode 07: раскомментируй dynamic_alloc.c
# После Episode 08: раскомментируй leak_detector.c

PROJECT = memory_manager
PROJECT_SRC = season_project_starter.c

# TODO: Раскомментируй после создания модулей:
# PROJECT_SRC += array_tools.c       # Из Episode 05
# PROJECT_SRC += pointer_utils.c     # Из Episode 06
# PROJECT_SRC += dynamic_alloc.c     # Из Episode 07
# PROJECT_SRC += leak_detector.c     # Из Episode 08

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
	@echo "  Компиляция Episode 05-08"
	@echo "═══════════════════════════════════════════════════════════════"
	$(MAKE) -C episode-05-memory-map
	$(MAKE) -C episode-06-pointer-game
	$(MAKE) -C episode-07-deep-dive
	$(MAKE) -C episode-08-fragmentation
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
	@echo "   Запустите: ./$(PROJECT)"
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
	@echo "Очистка Season 2..."
	rm -f $(PROJECT) $(PROJECT_OBJ)
	rm -f *.o *.out a.out
	rm -f array_tools.o pointer_utils.o dynamic_alloc.o leak_detector.o
	$(MAKE) -C episode-05-memory-map clean
	$(MAKE) -C episode-06-pointer-game clean
	$(MAKE) -C episode-07-deep-dive clean
	$(MAKE) -C episode-08-fragmentation clean
	@echo "✅ Очистка завершена"

# Тестирование Season Project
test_project: $(PROJECT)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Тестирование $(PROJECT)"
	@echo "═══════════════════════════════════════════════════════════════"
	./$(PROJECT)
	@echo ""

# Проверка утечек памяти
test_leaks: $(PROJECT)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Проверка утечек памяти"
	@echo "═══════════════════════════════════════════════════════════════"
	@# Определяем ОС
	@if [ "$$(uname)" = "Darwin" ]; then \
		echo "Используем leaks (macOS)..."; \
		leaks --atExit -- ./$(PROJECT); \
	elif [ "$$(uname)" = "Linux" ]; then \
		echo "Используем valgrind (Linux)..."; \
		valgrind --leak-check=full --show-leak-kinds=all ./$(PROJECT); \
	else \
		echo "⚠️  Автоматическая проверка недоступна на этой ОС"; \
		echo "   Запустите: ./$(PROJECT)"; \
	fi
	@echo ""

# Справка
help:
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Season 2: Memory & Pointers - Makefile Help"
	@echo "═══════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Основные команды:"
	@echo "  make               - Собрать все эпизоды"
	@echo "  make episodes      - Собрать все эпизоды"
	@echo "  make season_project - Собрать memory_manager"
	@echo "  make clean         - Удалить скомпилированные файлы"
	@echo "  make test_project  - Тестировать memory_manager"
	@echo "  make test_leaks    - Проверить утечки памяти (valgrind/leaks)"
	@echo "  make help          - Показать эту справку"
	@echo ""
	@echo "Работа с эпизодами:"
	@echo "  cd episode-05-memory-map && make"
	@echo "  cd episode-06-pointer-game && make"
	@echo "  cd episode-07-deep-dive && make"
	@echo "  cd episode-08-fragmentation && make"
	@echo ""
	@echo "Season Project:"
	@echo "  1. Создай модули (array_tools, pointer_utils, dynamic_alloc, leak_detector)"
	@echo "  2. Раскомментируй PROJECT_SRC в Makefile"
	@echo "  3. Запусти: make season_project"
	@echo "  4. Используй: ./memory_manager"
	@echo "  5. Проверь утечки: make test_leaks"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════"

# ═══════════════════════════════════════════════════════════════
# ИНСТРУКЦИИ ПО SEASON PROJECT
# ═══════════════════════════════════════════════════════════════
#
# После прохождения всех эпизодов:
#
# 1. Создай модули из своего кода:
#    - array_tools.c/.h     (из Episode 05)
#    - pointer_utils.c/.h   (из Episode 06)
#    - dynamic_alloc.c/.h   (из Episode 07)
#    - leak_detector.c/.h   (из Episode 08)
#
# 2. Раскомментируй строки PROJECT_SRC выше
#
# 3. Собери проект:
#    $ make season_project
#
# 4. Протестируй:
#    $ ./memory_manager
#
# 5. Проверь утечки:
#    $ make test_leaks
#
# 6. Поздравляю! У тебя есть инструмент для Season 3-10! 🚀
#
# ═══════════════════════════════════════════════════════════════



