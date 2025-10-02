# Season 2: Season Project — memory_manager

> *"Собери свой инструмент управления памятью из компонентов миссий"*

```
╔══════════════════════════════════════════════════════════╗
║              SEASON PROJECT - MEMORY MANAGER             ║
║                  OPERATION MOONLIGHT                     ║
╠══════════════════════════════════════════════════════════╣
║                                                          ║
║  СТАТУС:    🎯 FINAL MISSION                             ║
║  СЛОЖНОСТЬ: 🔴🔴🔴 Hard                                   ║
║  ВРЕМЯ:     ~2-4 часа                                    ║
║  ЦЕЛЬ:      Создать универсальный memory manager         ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

---

## 🎬 BRIEFING ОТ VIKTOR

**Дата:** Пятница, 3 октября  
**Время:** 08:00  
**Место:** Красная площадь, у Спасской башни

После успешного обезвреживания трояна Z. и прохождения всех тестов Season 2, вы получаете финальное сообщение от Viktor:

```
╔═══════════════════════════════════════════════════════════╗
║  ОТ: Viktor Petrov                                        ║
║  ТЕМА: Финальное задание Season 2                        ║
║  ПРИОРИТЕТ: ⭐⭐⭐⭐⭐ КРИТИЧЕСКИЙ                          ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  "Поздравляю, агент. Ты прошёл все тесты.                ║
║                                                           ║
║   Ты научился работать с памятью на уровне мастера:      ║
║   - Episode 05: Массивы и парсинг структур               ║
║   - Episode 06: Указатели и навигация                    ║
║   - Episode 07: Динамическое выделение                   ║
║   - Episode 08: Поиск утечек и оптимизация               ║
║                                                           ║
║   Теперь финальный экзамен:                              ║
║   Собери ВСЕ свои инструменты в ЕДИНУЮ СИСТЕМУ.          ║
║                                                           ║
║   Это не просто учебная задача.                          ║
║   memory_manager станет КРИТИЧЕСКИ ВАЖНЫМ инструментом   ║
║   для Season 3, 4, 5... до самого финала Season 10.      ║
║                                                           ║
║   Твой код должен быть:                                  ║
║   ✅ Модульным (каждый компонент = отдельный .h/.c)      ║
║   ✅ Переиспользуемым (будем подключать через #include)  ║
║   ✅ Чистым (без утечек, без предупреждений)             ║
║   ✅ Профессиональным (как в реальных проектах)          ║
║                                                           ║
║   Время на выполнение: 2-4 часа.                         ║
║                                                           ║
║   Если справишься — получишь доступ к Season 3.          ║
║   Если нет — придётся пересдавать Season 2.              ║
║                                                           ║
║   Удачи. Это твой момент доказать, что ты готов.         ║
║                                                           ║
║                                              — Viktor"    ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🎯 ЗАДАНИЕ

### Цель
Создать **memory_manager** — универсальный инструмент для управления памятью, который:

1. ✅ **Отслеживает аллокации** — обёртка над malloc/calloc/realloc/free
2. ✅ **Обнаруживает утечки** — логирует каждую аллокацию и деаллокацию
3. ✅ **Анализирует heap** — статистика использования памяти
4. ✅ **Парсит структуры** — работает с бинарными данными
5. ✅ **Профессионален** — модульный код, чистая архитектура

### Компоненты
Ты извлекаешь код из своих решений Episodes 05-08 и создаёшь 4 модуля:

```
Episode 05: Memory Map
   ↓
array_tools.c / array_tools.h
   - Работа с массивами
   - Парсинг бинарных структур
   - Анализ данных (min/max/sum/avg)

Episode 06: Pointer Game
   ↓
pointer_utils.c / pointer_utils.h
   - Навигация по указателям
   - Обход linked lists
   - Обнаружение циклов (Floyd's)

Episode 07: Deep Dive
   ↓
dynamic_alloc.c / dynamic_alloc.h
   - Обёртка над malloc/calloc/realloc/free
   - Логирование аллокаций
   - Динамическое выделение памяти

Episode 08: Fragmentation
   ↓
leak_detector.c / leak_detector.h
   - Поиск утечек памяти
   - Статистика heap
   - Профилирование
   - Memory tracking система
```

### Итоговая структура проекта

```
season-2-memory-and-pointers/
├── season_project_mission.md       ← Вы здесь
├── season_project_starter.c        ← Шаблон main()
├── Makefile                         ← Сборка (обновить!)
│
├── array_tools.h                    ← TODO: создать из Episode 05
├── array_tools.c                    ← TODO: создать из Episode 05
│
├── pointer_utils.h                  ← TODO: создать из Episode 06
├── pointer_utils.c                  ← TODO: создать из Episode 06
│
├── dynamic_alloc.h                  ← TODO: создать из Episode 07
├── dynamic_alloc.c                  ← TODO: создать из Episode 07
│
├── leak_detector.h                  ← TODO: создать из Episode 08
└── leak_detector.c                  ← TODO: создать из Episode 08
```

---

## 📝 ПОШАГОВЫЙ ПЛАН

### Шаг 1: Изучи стартовый шаблон

```bash
cat season_project_starter.c
```

Там ты найдёшь:
- Комментарии с TODO
- Структуру main()
- Примеры использования модулей
- Закомментированные #include (раскомментируй после создания .h файлов)

### Шаг 2: Создай модуль array_tools

**Из Episode 05 извлеки:**
- Функции работы с массивами
- Парсинг бинарных структур
- Анализ данных (min, max, sum, average)

**Создай файлы:**

`array_tools.h`:
```c
#ifndef ARRAY_TOOLS_H
#define ARRAY_TOOLS_H

#include <stddef.h>
#include <stdint.h>

// Анализ массива байтов
typedef struct {
    uint8_t min;
    uint8_t max;
    double  average;
    size_t  sum;
} ArrayStats;

// Функции (API)
ArrayStats analyze_byte_array(const uint8_t *arr, size_t size);
void print_array_stats(const ArrayStats *stats);
// ... добавь свои функции из Episode 05

#endif
```

`array_tools.c`:
```c
#include "array_tools.h"
#include <stdio.h>
#include <limits.h>

ArrayStats analyze_byte_array(const uint8_t *arr, size_t size) {
    // TODO: Скопируй свою реализацию из Episode 05
    //       (из array_analyzer.c)
}

void print_array_stats(const ArrayStats *stats) {
    // TODO: Вывод статистики
}
```

### Шаг 3: Создай модуль pointer_utils

**Из Episode 06 извлеки:**
- Работа с указателями
- Linked list traversal
- Floyd's cycle detection

**Создай файлы:**

`pointer_utils.h`:
```c
#ifndef POINTER_UTILS_H
#define POINTER_UTILS_H

#include <stdbool.h>

// Узел linked list
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Функции (API)
bool has_cycle(Node *head);              // Floyd's algorithm
int  list_length(Node *head);
void print_list(Node *head);
void swap_pointers(int **a, int **b);
// ... добавь свои функции из Episode 06

#endif
```

`pointer_utils.c`:
```c
#include "pointer_utils.h"
#include <stdio.h>

bool has_cycle(Node *head) {
    // TODO: Скопируй Floyd's algorithm из Episode 06
}

// ... остальные функции
```

### Шаг 4: Создай модуль dynamic_alloc

**Из Episode 07 извлеки:**
- Wrapper над malloc/free/realloc
- Логирование аллокаций
- Динамическое выделение

**Создай файлы:**

`dynamic_alloc.h`:
```c
#ifndef DYNAMIC_ALLOC_H
#define DYNAMIC_ALLOC_H

#include <stddef.h>

// Wrapper над malloc с логированием
void* tracked_malloc(size_t size, const char *file, int line);
void* tracked_calloc(size_t num, size_t size, const char *file, int line);
void* tracked_realloc(void *ptr, size_t size, const char *file, int line);
void  tracked_free(void *ptr, const char *file, int line);

// Макросы для удобства
#define MALLOC(size)         tracked_malloc(size, __FILE__, __LINE__)
#define CALLOC(num, size)    tracked_calloc(num, size, __FILE__, __LINE__)
#define REALLOC(ptr, size)   tracked_realloc(ptr, size, __FILE__, __LINE__)
#define FREE(ptr)            tracked_free(ptr, __FILE__, __LINE__)

#endif
```

`dynamic_alloc.c`:
```c
#include "dynamic_alloc.h"
#include <stdio.h>
#include <stdlib.h>

void* tracked_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    printf("[ALLOC] %zu bytes at %p (%s:%d)\n", size, ptr, file, line);
    return ptr;
}

// TODO: Реализуй остальные функции
```

### Шаг 5: Создай модуль leak_detector

**Из Episode 08 извлеки:**
- Систему отслеживания аллокаций
- Поиск утечек
- Статистику памяти

**Создай файлы:**

`leak_detector.h`:
```c
#ifndef LEAK_DETECTOR_H
#define LEAK_DETECTOR_H

#include <stddef.h>

// Инициализация и очистка
void leak_detector_init(void);
void leak_detector_cleanup(void);

// Отчёт об утечках
void leak_detector_report(void);

// Статистика
typedef struct {
    size_t total_allocations;
    size_t total_frees;
    size_t current_bytes;
    size_t peak_bytes;
    size_t leaked_bytes;
} MemoryStats;

MemoryStats get_memory_stats(void);
void print_memory_stats(const MemoryStats *stats);

#endif
```

`leak_detector.c`:
```c
#include "leak_detector.h"
#include <stdio.h>

static MemoryStats stats = {0};

void leak_detector_init(void) {
    // TODO: Инициализация
}

void leak_detector_report(void) {
    // TODO: Вывод всех утечек
}

// ... остальные функции из Episode 08
```

### Шаг 6: Обнови season_project_starter.c

Раскомментируй #include и реализуй TODO:

```c
#include "array_tools.h"
#include "pointer_utils.h"
#include "dynamic_alloc.h"
#include "leak_detector.h"

int main(void) {
    leak_detector_init();
    
    // TODO 1: Тест array_tools
    uint8_t data[] = {10, 20, 30, 40, 50};
    ArrayStats stats = analyze_byte_array(data, 5);
    print_array_stats(&stats);
    
    // TODO 2: Тест pointer_utils
    // TODO 3: Тест dynamic_alloc
    // TODO 4: Финальный отчёт leak_detector
    
    leak_detector_cleanup();
    return 0;
}
```

### Шаг 7: Обнови Makefile

Раскомментируй закомментированные строки:

```makefile
# Season Project
season_project: season_project_starter.c array_tools.c pointer_utils.c dynamic_alloc.c leak_detector.c
	$(CC) $(CFLAGS) -o memory_manager $^ $(LDFLAGS)
	@echo "✅ memory_manager собран!"
```

### Шаг 8: Собери и протестируй

```bash
make season_project
./memory_manager
```

---

## 🎯 КРИТЕРИИ УСПЕХА

### Минимальные требования (ОБЯЗАТЕЛЬНО):

✅ **Компиляция:**
- Проект собирается без ошибок
- Нет warnings от компилятора

✅ **Модульность:**
- Каждый модуль = отдельный .h/.c файл
- API в .h файлах чистый и понятный
- Реализация в .c файлах

✅ **Функциональность:**
- array_tools: анализ массивов работает
- pointer_utils: обнаружение циклов работает
- dynamic_alloc: логирование malloc/free работает
- leak_detector: отчёт об утечках работает

✅ **Чистота кода:**
- Нет утечек памяти (все malloc → free)
- Valgrind/leaks не находит проблем
- Код читаемый с комментариями

✅ **Тестирование:**
- main() демонстрирует работу всех модулей
- Программа выводит осмысленные результаты

### Бонусные задачи (ОПЦИОНАЛЬНО):

🌟 **Бонус 1:** Визуализация heap
- Функция print_heap_map() рисует карту памяти

🌟 **Бонус 2:** Memory profiling
- Отслеживание peak memory usage
- График использования памяти

🌟 **Бонус 3:** Интеграция с valgrind
- Автоматический запуск valgrind из Makefile
- Парсинг вывода valgrind

🌟 **Бонус 4:** Документация
- README для Season Project
- Примеры использования каждого модуля

---

## 💡 ПОДСКАЗКИ

### Подсказка 1: Начни с простого
Не пытайся сделать всё идеально сразу. Начни с базовых функций, протестируй, потом улучшай.

### Подсказка 2: Используй свой код
У тебя уже есть рабочие решения из Episodes 05-08! Просто извлеки функции и адаптируй.

### Подсказка 3: API Design
Думай о том, как ТЫ САМ будешь использовать эти модули в Season 3-10. Делай API удобным.

### Подсказка 4: Комментируй
Через месяц ты забудешь, как работает твой код. Комментарии помогут вспомнить.

### Подсказка 5: Тестируй по частям
Не пиши все 4 модуля сразу. Создавай по одному, тестируй, потом следующий.

---

## 🔗 КАК ЭТО ПРИГОДИТСЯ В SEASON 3-10?

### Season 3: Networks
```c
#include "../season-2-memory-and-pointers/dynamic_alloc.h"
#include "../season-2-memory-and-pointers/leak_detector.h"

// Используй memory manager для буферов пакетов
void* packet_buffer = MALLOC(4096);
// ... работа с сетью
FREE(packet_buffer);
leak_detector_report();  // Проверка утечек
```

### Season 4: Crypto & Algorithms
```c
#include "../season-2-memory-and-pointers/array_tools.h"
#include "../season-2-memory-and-pointers/pointer_utils.h"

// Используй для работы с ключами и hash tables
```

### Season 5-10: Все проекты
Твой memory_manager станет СТАНДАРТНОЙ БИБЛИОТЕКОЙ для всех будущих проектов. Инвестиция в качество сейчас = экономия времени потом!

---

## 📊 ВРЕМЯ НА ВЫПОЛНЕНИЕ

**Ориентировочное время:**
- Изучение задания: 15 минут
- Создание array_tools: 30 минут
- Создание pointer_utils: 30 минут
- Создание dynamic_alloc: 45 минут
- Создание leak_detector: 60 минут
- Интеграция и тестирование: 30 минут
- **ИТОГО: ~3 часа**

Если сделаешь бонусные задачи — +1-2 часа.

---

## 🚀 ГОТОВ НАЧАТЬ?

1. ✅ Прочитал задание
2. ✅ Понял структуру проекта
3. ✅ Изучил season_project_starter.c
4. ✅ Готов создавать модули

**Удачи, агент!**

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║  "Код, который ты напишешь сейчас,                      ║
║   будет работать до самого финала Season 10.            ║
║                                                          ║
║   Сделай его достойным.                                 ║
║                                                          ║
║   Увидимся на другой стороне, агент."                   ║
║                                                          ║
║                                           — Viktor"      ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

---

*Финальное задание Season 2*  
*Operation MOONLIGHT continues...*


