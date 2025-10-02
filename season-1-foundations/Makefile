# ═══════════════════════════════════════════════════════════════
# Season 1: Foundations - Makefile
# ═══════════════════════════════════════════════════════════════
# 
# Этот Makefile управляет:
# 1. Компиляцией эпизодов (Episode 01-04)
# 2. Сборкой Season Project (moonlight_decoder)
# 
# ═══════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# ═══════════════════════════════════════════════════════════════
# Season Project: moonlight_decoder
# ═══════════════════════════════════════════════════════════════

# TODO: После создания модулей раскомментируй эти строки!
# 
# После Episode 01: раскомментируй byte_ops.c
# После Episode 04: раскомментируй crypto.c, utils.c, decoder.c

PROJECT = moonlight_decoder
PROJECT_SRC = season_project_starter.c

# TODO: Раскомментируй после создания модулей:
# PROJECT_SRC += byte_ops.c      # Из Episode 01
# PROJECT_SRC += crypto.c         # Из Episode 04
# PROJECT_SRC += utils.c          # Валидация
# PROJECT_SRC += decoder.c        # Pattern analysis

PROJECT_OBJ = $(PROJECT_SRC:.c=.o)

# ═══════════════════════════════════════════════════════════════
# Основные цели
# ═══════════════════════════════════════════════════════════════

.PHONY: all episodes season_project clean help

# По умолчанию собираем только эпизоды
all: episodes

# Собрать все эпизоды
episodes:
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Компиляция Episode 01-04"
	@echo "═══════════════════════════════════════════════════════════════"
	$(MAKE) -C episode-01-strange-message
	$(MAKE) -C episode-02-chain-of-clues
	$(MAKE) -C episode-03-patterns-in-time
	$(MAKE) -C episode-04-first-tool
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
	@echo "   Запустите: ./$(PROJECT) <encrypted_file>"
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
	@echo "Очистка Season 1..."
	rm -f $(PROJECT) $(PROJECT_OBJ)
	rm -f *.o *.out a.out
	$(MAKE) -C episode-01-strange-message clean
	$(MAKE) -C episode-02-chain-of-clues clean
	$(MAKE) -C episode-03-patterns-in-time clean
	$(MAKE) -C episode-04-first-tool clean
	@echo "✅ Очистка завершена"

# Тестирование Season Project
test_project: $(PROJECT)
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Тестирование $(PROJECT)"
	@echo "═══════════════════════════════════════════════════════════════"
	@# TODO: Добавь свои тесты здесь
	@# Пример:
	@# ./$(PROJECT) test_data.enc
	@echo "TODO: Создай тестовые файлы и раскомментируй тесты"
	@echo ""

# Справка
help:
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "  Season 1: Foundations - Makefile Help"
	@echo "═══════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Основные команды:"
	@echo "  make               - Собрать все эпизоды"
	@echo "  make episodes      - Собрать все эпизоды"
	@echo "  make season_project - Собрать moonlight_decoder"
	@echo "  make clean         - Удалить скомпилированные файлы"
	@echo "  make test_project  - Тестировать decoder"
	@echo "  make help          - Показать эту справку"
	@echo ""
	@echo "Работа с эпизодами:"
	@echo "  cd episode-01-strange-message && make"
	@echo "  cd episode-02-chain-of-clues && make"
	@echo "  cd episode-03-patterns-in-time && make"
	@echo "  cd episode-04-first-tool && make"
	@echo ""
	@echo "Season Project:"
	@echo "  1. Создай модули (byte_ops, crypto, utils, decoder)"
	@echo "  2. Раскомментируй PROJECT_SRC в Makefile"
	@echo "  3. Запусти: make season_project"
	@echo "  4. Используй: ./moonlight_decoder <file>"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════"

# ═══════════════════════════════════════════════════════════════
# ИНСТРУКЦИИ ПО SEASON PROJECT
# ═══════════════════════════════════════════════════════════════
#
# После прохождения всех эпизодов:
#
# 1. Создай модули из своего кода:
#    - byte_ops.c/.h  (из Episode 01)
#    - crypto.c/.h    (из Episode 04, уже есть в episode-04/solution/)
#    - utils.c/.h     (валидация из Episode 02)
#    - decoder.c/.h   (pattern analysis из Episode 03)
#
# 2. Раскомментируй строки PROJECT_SRC выше
#
# 3. Собери проект:
#    $ make season_project
#
# 4. Протестируй:
#    $ ./moonlight_decoder test.enc
#
# 5. Поздравляю! У тебя есть инструмент для Season 2! 🚀
#
# ═══════════════════════════════════════════════════════════════

