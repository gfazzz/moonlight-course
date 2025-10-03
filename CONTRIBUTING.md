# 🤝 Contributing to OPERATION MOONLIGHT
## Руководство для контрибьюторов

> *"Код пишется один раз, читается тысячи раз. Делайте его понятным."*

Спасибо за интерес к развитию курса! Мы рады любому вкладу — от исправления опечаток до создания целых эпизодов.

**Последнее обновление:** 4 октября 2025  
**Версия:** v0.3.5  
**Статус:** 50% готовности (21/42 эпизода)

---

## 📊 Текущий статус курса

**Готово к использованию:**
- 🟢 **Season 1:** 85% (v3.1 LUNA Edition, 4 эпизода)
- 🟢 **Season 2-4:** 80% (v3.0 Expanded Cast, 12 эпизодов)

**В разработке:**
- 🟡 **Season 5-8:** 20% (16 эпизодов, базовая структура)
- 🔴 **Season 9-10:** 5% (10 эпизодов, минимальная структура)

**Общая готовность:** 50% = (85% + 80×3 + 20×4 + 5×2) / 10

**Цель v1.0:** Все 10 сезонов на уровне 80%

---

## 🎯 Приоритеты контрибуций

### Приоритет 1 — КРИТИЧЕСКИЙ 🔥

**Season 5-8: от 20% до 80%**

Что нужно для каждого эпизода:
1. ✅ Полные `solution/*.c` файлы (300-500 строк)
2. ✅ Comprehensive `starter.c` (80-150 строк с TODO)
3. ✅ Artifacts (3-4 файла, 600-1200 строк реалистичных данных)
4. ✅ Автотесты `tests/test.sh`
5. ✅ Расширенные README (драматический сюжет, 150-250 строк)
6. ✅ Детализированные `mission.md` (100-150 строк)

**Зачем это важно:**
- Season 5-8 охватывают FinTech, Embedded/IoT, System Programming, AI/ML
- Это практические навыки для реальных карьер
- Базовая структура уже есть, нужно наполнение

**Примерное время:** 8-12 часов на эпизод

---

### Приоритет 2 — ВЫСОКИЙ 🔶

**Season Projects для Season 1-8**

Концепция cumulative code reuse требует Season Projects:

**Необходимо создать:**
1. `season-1-foundations/season_project_solution.c` (реализация `moonlight_decoder`)
2. `season-2-memory-and-pointers/season_project_solution.c` (реализация `memory_manager`)
3. `season-3-networks/season_project_solution.c` (реализация `network_interceptor`)
4. `season-4-crypto-and-algorithms/season_project_solution.c` (реализация `crypto_toolkit`)
5. Season 5-8: Аналогично

**Структура Season Project:**
```
season-X-название/
├── season_project_mission.md    # ✅ Уже есть
├── season_project_starter.c      # ✅ Уже есть
├── season_project_solution.c     # ❌ НУЖНО СОЗДАТЬ
├── season_project_Makefile       # ❌ НУЖНО СОЗДАТЬ
└── season_project_tests/         # ❌ НУЖНО СОЗДАТЬ
    └── test.sh
```

**Примерное время:** 4-6 часов на Season Project

---

### Приоритет 3 — СРЕДНИЙ 🟡

**Season 9-10: от 5% до 20%**

- Расширенные README с теорией
- Детализированные mission.md
- Starter.c с comprehensive TODOs
- Artifacts (минимум 3-4 файла)

**Примерное время:** 6-8 часов на эпизод

---

### Приоритет 4 — НИЗКИЙ 🔵

**Улучшения и полировка:**
- Expanded Cast методология для Season 5-8 (6 персонажей на сезон)
- Interleaving для Season 5-8 (теория разбита на блоки)
- Cliffhangers и сюжетные связки
- Easter eggs и отсылки
- Тестирование студентами
- CI/CD автоматизация

---

## 📋 Типы контрибуций

### 1. 🆕 Создание новых эпизодов

**Что нужно для полного эпизода:**

```
episode-XX-название/
├── README.md              # 150-250 строк (теория + сюжет)
├── mission.md             # 100-150 строк (детальное ТЗ)
├── starter.c              # 80-150 строк (comprehensive TODOs)
├── Makefile               # Build infrastructure
├── solution/              # Полные решения
│   ├── program.c          # 300-500 строк
│   └── (дополнительные файлы)
├── artifacts/             # 3-4 файла реалистичных данных
│   ├── data1.csv          # 600-1200 строк ВСЕГО
│   ├── data2.log
│   └── config.json
└── tests/
    └── test.sh            # Автоматические тесты
```

**Структура README.md:**
```markdown
# Episode XX: "Название"
## Season X: Тема сезона

> *"Драматическая цитата из сюжета"*

**Местоположение:** Город, страна (координаты)  
**Время:** Дата и время по сюжету  
**Длительность:** 2-4 часа  

---

## 🎬 BRIEFING

[Драматическая завязка эпизода, связь с предыдущими]

**Цель миссии:**
- Основная цель
- Дополнительные задачи

**Таймлайн:**
- 14:00 — Событие 1
- 16:30 — Событие 2
- 19:00 — Дедлайн

---

## 📞 ПЕРСОНАЖИ

**Появляются:**
- Имя (роль) — описание

**Упоминаются:**
- Связи с другими эпизодами

---

## 📚 THEORY

### 1. Концепция А

[Объяснение с метафорами]

**Метафора:** [Понятная аналогия из жизни]

**Код:**
```c
// Пример использования
```

**Визуализация:**
```
[ASCII-диаграмма]
```

### 2. Концепция Б

[Аналогично]

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ

1. Вопрос 1 (проверка понимания)
2. Вопрос 2 (применение)
3. Вопрос 3 (анализ)

---

## 🎯 ПРОМЕЖУТОЧНАЯ ЗАДАЧА

**Разминка перед основной миссией:**

```bash
cd artifacts/
nano warmup.c
```

[Описание небольшой задачи]

---

## 🎯 ОСНОВНАЯ МИССИЯ

**Задача:** [Детальное описание]

**Входные данные:**
- `artifacts/data.csv` — [описание]

**Ожидаемый результат:**
- [Конкретные критерии успеха]

**Технические требования:**
- Требование 1
- Требование 2

---

## 💡 ПОДСКАЗКИ

<details>
<summary>Подсказка 1: С чего начать</summary>

[Направление мысли, НЕ готовое решение]
</details>

<details>
<summary>Подсказка 2: Алгоритм</summary>

[Псевдокод или шаги]
</details>

---

## 🧪 ТЕСТИРОВАНИЕ

```bash
make
make test
```

**Критерии прохождения:**
- ✅ Все тесты зелёные
- ✅ Valgrind чист (для эпизодов с памятью)
- ✅ Компиляция с `-Wall -Wextra -Werror`

---

## 🎓 ЧТО ВЫ ИЗУЧИЛИ

После прохождения эпизода вы:
- ✅ Освоили [навык 1]
- ✅ Научились [навык 2]
- ✅ Понимаете [концепция]

---

## 🔗 СВЯЗЬ С ДРУГИМИ ЭПИЗОДАМИ

**Использует знания из:**
- Episode XX (тема)
- Episode YY (тема)

**Потребуется в:**
- Episode ZZ (применение)

---

## 🎬 DEBRIEFING

[Драматическое завершение эпизода]

**Результаты миссии:**
- [Что удалось обнаружить/достичь]

**Cliffhanger:**
> [Интригующая концовка для следующего эпизода]

**Следующий эпизод:** Episode XX — "Название"

---

## 🥚 Easter Eggs

[Скрытые отсылки к фильмам, книгам, реальным событиям]

---

*Время прохождения: ~2-4 часа*  
*Сложность: ⭐⭐⭐☆☆ (3/5)*
```

---

### 2. 🔧 Улучшение существующих эпизодов

**Можно улучшить:**
- 📝 Исправление опечаток и грамматики
- 🎨 Улучшение объяснений (больше метафор)
- 💡 Добавление подсказок и примеров
- 🐛 Исправление багов в solution коде
- 📊 Добавление ASCII-диаграмм
- 🎭 Расширение сюжетных элементов
- 🥚 Добавление Easter eggs

**Качество улучшений:**
- Метафоры должны быть понятными (не технические)
- ASCII-диаграммы чёткие и информативные
- Примеры кода рабочие и протестированные

---

### 3. 🌍 Перевод на другие языки

Курс сейчас на русском. Переводы ОЧЕНЬ приветствуются!

**Приоритетные языки:**
1. 🇬🇧 English (глобальная аудитория)
2. 🇪🇸 Español (вторая по распространённости)
3. 🇨🇳 中文 (огромная аудитория разработчиков)
4. 🇫🇷 Français
5. 🇩🇪 Deutsch
6. 🇯🇵 日本語

**Структура перевода:**
```
season-X-название/
├── episode-XX/
│   ├── README.md              # Русский (оригинал)
│   ├── README.en.md           # English
│   ├── README.es.md           # Español
│   └── README.zh.md           # 中文
```

**Важно:**
- Переводите сюжет, теорию, комментарии
- Код остаётся на английском (переменные, функции)
- Сохраняйте стиль и тон оригинала
- Метафоры адаптируйте под культуру

---

### 4. 🧪 Тестирование

**Платформы для тестирования:**
- ✅ macOS (darwin 24.6.0) — основная платформа разработки
- ✅ Ubuntu/Debian (22.04+)
- ✅ Fedora/RHEL (38+)
- ✅ FreeBSD (13.0+) — для Season 7
- ⏳ Windows (WSL2) — частичная поддержка

**Что тестировать:**
1. Компиляция (`make`)
2. Автотесты (`make test`)
3. Работоспособность решений
4. Valgrind/leaks (memory safety)
5. Cross-platform compatibility

**Как сообщить о проблеме:**
См. секцию "🐛 Reporting Bugs" ниже

---

### 5. 📚 Документация

**Можно улучшить:**
- README.md (главный)
- CURRICULUM.md (детальная программа)
- GETTING_STARTED.md (инструкции)
- PROJECTS.md (Season Projects)
- THEORY_MAP.md (карта зависимостей)

**Стандарты документации:**
- Markdown с правильным форматированием
- Примеры кода в блоках ```c ... ```
- Эмодзи для визуального разделения
- Ссылки на связанные документы
- Актуальная информация (проверяйте даты!)

---

## 🔄 Процесс контрибуции

### Шаг 1: Fork репозитория

Нажмите **Fork** вверху страницы на GitHub.

### Шаг 2: Clone к себе

```bash
git clone git@github.com:YOUR_USERNAME/moonlight-course.git
cd moonlight-course
```

### Шаг 3: Настройка окружения

```bash
# Проверьте компилятор
gcc --version   # или clang --version

# Установите зависимости (если нужно)
# macOS:
xcode-select --install

# Ubuntu/Debian:
sudo apt install build-essential valgrind

# Fedora:
sudo dnf install gcc make valgrind
```

### Шаг 4: Создайте ветку

```bash
git checkout -b feature/episode-XX-название
# или
git checkout -b fix/season-X-typo
# или
git checkout -b docs/improve-readme
```

**Naming convention:**
- `feature/*` — новые эпизоды/функции
- `fix/*` — исправления багов
- `docs/*` — документация
- `test/*` — тесты и CI/CD
- `i18n/*` — переводы (internationalization)
- `refactor/*` — рефакторинг кода
- `perf/*` — оптимизация производительности

### Шаг 5: Внесите изменения

Следуйте стандартам кода (см. ниже).

### Шаг 6: Тестирование

```bash
# Для эпизода
cd season-X-название/episode-XX-название/
make clean
make
make test

# Проверка памяти (Linux)
valgrind --leak-check=full ./program

# Проверка памяти (macOS)
leaks --atExit -- ./program

# Компиляция со всеми предупреждениями
gcc -Wall -Wextra -Werror -pedantic *.c -o program
```

### Шаг 7: Commit

**Хорошие commit messages:**

```bash
git add .
git commit -m "feat(S5E17): Add OHLCV parser solution

- Implemented CSV parser with error handling
- Added validation for price data
- Created comprehensive test suite (15 test cases)
- Added artifacts: sample_ohlcv.csv (1000 lines)

Time complexity: O(n)
Memory: O(1) streaming approach
Tested on: macOS, Ubuntu 22.04"
```

**Плохие commit messages:**
```bash
git commit -m "fix"           # ❌ Не информативно
git commit -m "updated stuff" # ❌ Нет деталей
git commit -m "asdf"          # ❌ Бессмыслица
```

**Формат commit message:**
```
<type>(<scope>): <short summary>

<detailed description>

<footer: Breaking changes, issues, etc.>
```

**Types:**
- `feat:` — новая функция/эпизод
- `fix:` — исправление бага
- `docs:` — документация
- `test:` — добавление/исправление тестов
- `refactor:` — рефакторинг без изменения поведения
- `perf:` — оптимизация производительности
- `style:` — форматирование кода
- `chore:` — обновление зависимостей, настроек

**Scope:**
- `S1E01` — Season 1, Episode 01
- `S5` — весь Season 5
- `docs` — документация
- `ci` — CI/CD

### Шаг 8: Push

```bash
git push origin feature/episode-XX-название
```

### Шаг 9: Pull Request

1. Откройте ваш fork на GitHub
2. Нажмите **Pull Request**
3. Заполните template (если есть)
4. Опишите изменения **детально**
5. Прикрепите screenshots (если применимо)
6. Дождитесь review

**Хороший PR description:**
```markdown
## Описание
Добавлен полный Episode 17 (Market Microstructure) для Season 5.

## Что включено
- ✅ README.md (234 строки, драматический сюжет)
- ✅ solution/ohlcv_parser.c (412 строк)
- ✅ starter.c (98 строк с TODOs)
- ✅ artifacts/sample_ohlcv.csv (1000 строк)
- ✅ tests/test.sh (15 тест-кейсов)
- ✅ Makefile

## Тестирование
- ✅ macOS (clang 15.0.0)
- ✅ Ubuntu 22.04 (gcc 11.3.0)
- ✅ Все тесты проходят
- ✅ Valgrind чист (0 leaks)

## Checklist
- [x] Код компилируется без warnings
- [x] Все тесты проходят
- [x] Документация обновлена
- [x] Нет опечаток
- [x] Следую code style
```

---

## 📐 Стандарты кода

### C Code Style

**Общие правила:**
- Отступы: 4 пробела (НЕ табы)
- Максимальная длина строки: 100 символов
- Encoding: UTF-8
- Line endings: Unix (LF, `\n`)

**Naming conventions:**
```c
// Переменные: snake_case
int user_count = 0;
char *file_name = "data.txt";

// Функции: snake_case, глаголы
int calculate_checksum(unsigned char *data, int length);
void process_input(FILE *fp);
char *read_line(FILE *fp);

// Константы: UPPER_CASE
#define MAX_BUFFER_SIZE 1024
#define PI 3.14159265359

// Типы (struct, typedef): CamelCase
typedef struct User {
    char *name;
    int age;
} User;

// Enum: CamelCase для типа, UPPER_CASE для значений
typedef enum Status {
    STATUS_OK,
    STATUS_ERROR,
    STATUS_PENDING
} Status;
```

**Структура файла:**
```c
// 1. Includes (system, затем local)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_header.h"

// 2. Defines и макросы
#define MAX_SIZE 1024
#define DEBUG 1

// 3. Типы (struct, enum, typedef)
typedef struct Point {
    int x;
    int y;
} Point;

// 4. Function prototypes
int process_data(char *input);
void cleanup(void);

// 5. Global variables (избегать!)
// static int g_counter = 0;  // Только если ДЕЙСТВИТЕЛЬНО нужно

// 6. Function implementations
int main(int argc, char *argv[]) {
    // ...
}

int process_data(char *input) {
    // ...
}
```

**Комментарии:**
```c
// Хорошие комментарии объясняют "ПОЧЕМУ", не "ЧТО"

// ❌ Плохо (очевидно из кода)
int count = 0;  // Инициализируем count в 0

// ✅ Хорошо (объясняет причину)
int count = 0;  // Start from 0 because loop is 0-indexed

// ✅ Отлично (объясняет неочевидное решение)
// Using XOR swap to avoid extra variable and save stack space
// in tight embedded environment (only 2KB RAM available)
a ^= b;
b ^= a;
a ^= b;

// Многострочные комментарии для сложной логики
/*
 * Floyd's Cycle Detection Algorithm (Tortoise and Hare)
 * 
 * Time:  O(n)
 * Space: O(1)
 * 
 * Why not hash table? Memory constraint in embedded systems.
 * This algorithm uses only 2 pointers.
 */
```

**Error handling:**
```c
// ✅ Всегда проверяйте возвращаемые значения
FILE *fp = fopen("data.txt", "r");
if (fp == NULL) {
    fprintf(stderr, "Error: Cannot open file 'data.txt'\n");
    return 1;
}

char *buffer = malloc(1024);
if (buffer == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    fclose(fp);
    return 1;
}

// ✅ Используйте perror для системных ошибок
FILE *fp = fopen("data.txt", "r");
if (fp == NULL) {
    perror("fopen");  // Выведет: "fopen: No such file or directory"
    return 1;
}
```

**Memory management:**
```c
// ✅ Всегда освобождайте память
char *buffer = malloc(1024);
if (buffer == NULL) {
    return 1;
}

// Use buffer...

free(buffer);
buffer = NULL;  // Хорошая практика: обнуляем указатель

// ✅ Проверяйте границы массивов
int arr[10];
for (int i = 0; i < 10; i++) {  // НЕ i <= 10!
    arr[i] = i;
}

// ✅ Используйте const для защиты данных
void process_data(const char *input) {
    // input нельзя изменить (compile-time защита)
}
```

**Примеры качественного кода:**

```c
// ✅ ОТЛИЧНЫЙ КОД (Season 1-4 стандарт)

/**
 * Calculate checksum of data buffer using XOR
 * 
 * @param data    Pointer to data buffer
 * @param length  Length of buffer in bytes
 * @return        XOR checksum (0-255)
 * 
 * Time:  O(n)
 * Space: O(1)
 */
unsigned char calculate_checksum(const unsigned char *data, size_t length) {
    if (data == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to calculate_checksum\n");
        return 0;
    }
    
    unsigned char checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    
    return checksum;
}

/**
 * Read entire file into memory
 * 
 * @param filename  Path to file
 * @param size      Output: file size in bytes
 * @return          Pointer to allocated buffer, or NULL on error
 * 
 * NOTE: Caller must free() returned pointer!
 */
char *read_file(const char *filename, size_t *size) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("fopen");
        return NULL;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (fsize < 0) {
        fprintf(stderr, "Error: Cannot determine file size\n");
        fclose(fp);
        return NULL;
    }
    
    // Allocate buffer
    char *buffer = malloc(fsize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Cannot allocate %ld bytes\n", fsize);
        fclose(fp);
        return NULL;
    }
    
    // Read file
    size_t read_bytes = fread(buffer, 1, fsize, fp);
    if (read_bytes != (size_t)fsize) {
        fprintf(stderr, "Error: Read only %zu of %ld bytes\n", read_bytes, fsize);
        free(buffer);
        fclose(fp);
        return NULL;
    }
    
    buffer[fsize] = '\0';  // Null-terminate
    fclose(fp);
    
    if (size != NULL) {
        *size = (size_t)fsize;
    }
    
    return buffer;
}
```

---

### Markdown Style

**Заголовки:**
```markdown
# H1 — только для названия эпизода
## H2 — для основных секций (BRIEFING, THEORY, MISSION)
### H3 — для подсекций
#### H4 — редко, для деталей
```

**Списки:**
```markdown
<!-- Ненумерованные -->
- Пункт 1
- Пункт 2
  - Вложенный пункт (2 пробела)
  - Ещё вложенный

<!-- Нумерованные -->
1. Шаг 1
2. Шаг 2
3. Шаг 3
```

**Код:**
````markdown
```c
// C code block
int main(void) {
    printf("Hello\n");
    return 0;
}
```

```bash
# Bash commands
make
make test
```

`inline code` для коротких фрагментов
````

**Таблицы:**
```markdown
| Колонка 1 | Колонка 2 | Колонка 3 |
|-----------|-----------|-----------|
| Данные 1  | Данные 2  | Данные 3  |
| Ещё       | Ещё       | Ещё       |
```

**Эмодзи:**
```markdown
Используйте для визуального разделения:
🎯 Цели
📚 Теория
🎬 Сюжет
💡 Подсказки
✅ Успех
❌ Ошибка
🔧 Инструменты
```

---

## ✅ Checklist для нового эпизода

Перед созданием PR убедитесь:

### Структура
- [ ] `README.md` создан (150-250 строк)
- [ ] `mission.md` создан (100-150 строк)
- [ ] `starter.c` создан (80-150 строк)
- [ ] `solution/*.c` созданы (300-500 строк)
- [ ] `artifacts/` созданы (3-4 файла, 600-1200 строк ВСЕГО)
- [ ] `tests/test.sh` создан (минимум 10 тест-кейсов)
- [ ] `Makefile` создан

### Контент
- [ ] Теория объяснена с метафорами
- [ ] Сюжет связан с общей историей
- [ ] Персонажи упомянуты (если применимо)
- [ ] Таймлайн указан (дата, время, локация)
- [ ] Контрольные вопросы добавлены (минимум 8)
- [ ] Промежуточная задача создана
- [ ] Основная миссия детализирована
- [ ] Подсказки НЕ дают готовое решение
- [ ] Debriefing с cliffhanger
- [ ] Easter eggs добавлены (опционально)

### Код
- [ ] Solution компилируется без warnings (`-Wall -Wextra -Werror`)
- [ ] Все функции прокомментированы
- [ ] Error handling присутствует
- [ ] Memory leaks отсутствуют (Valgrind/leaks)
- [ ] Код следует style guide
- [ ] Starter.c содержит comprehensive TODOs

### Тестирование
- [ ] Протестировано на macOS
- [ ] Протестировано на Linux (Ubuntu/Debian)
- [ ] Все автотесты проходят
- [ ] Makefile работает (clean, all, test)
- [ ] Artifacts реалистичны и достаточны

### Документация
- [ ] Нет опечаток
- [ ] Markdown правильно отформатирован
- [ ] Все ссылки работают
- [ ] Code blocks с правильным синтаксисом
- [ ] Эмодзи используются уместно

---

## 🐛 Reporting Bugs

### Нашли баг?

Создайте **Issue** на GitHub с детальным описанием.

**Template для bug report:**

```markdown
**Заголовок:** [Bug] Season X Episode XX: Краткое описание

## 🐛 Описание проблемы

Чёткое описание что не работает.

## 📝 Шаги воспроизведения

1. `cd season-X/.../episode-XX/`
2. `make`
3. `./program`
4. Ошибка появляется

## ✅ Ожидаемое поведение

Что ДОЛЖНО было произойти.

## ❌ Фактическое поведение

Что произошло ВМЕСТО этого.

```
[Вставьте вывод ошибки]
```

## 💻 Окружение

- **OS:** macOS 14.0 / Ubuntu 22.04 / etc.
- **Компилятор:** gcc 11.3.0 / clang 15.0.0
- **Make:** GNU Make 4.3
- **Valgrind:** 3.19.0 (если применимо)

## 🔍 Дополнительная информация

Screenshots, логи, дополнительный контекст.
```

### Примеры качественных bug reports:

**Хороший:**
```
[Bug] Season 2 Episode 07: Memory leak in dynamic_loader.c

Программа теряет 1024 байта при каждом вызове load_archive().

Шаги:
1. cd season-2-memory-and-pointers/episode-07-deep-dive/
2. make
3. valgrind --leak-check=full ./dynamic_loader artifacts/archive.dat

Вывод Valgrind:
==12345== 1,024 bytes in 1 blocks are definitely lost
==12345==    at malloc (vg_replace_malloc.c:307)
==12345==    by load_archive (dynamic_loader.c:42)

OS: Ubuntu 22.04, gcc 11.3.0
```

**Плохой:**
```
не работает

помогите
```

---

## 💡 Идеи для контрибуций

### Season 5: Financial Markets (20% → 80%)

**Episode 17: Market Microstructure**
- ✅ README есть (базовый)
- ❌ solution/ohlcv_parser.c НУЖЕН
- ❌ artifacts/sample_ohlcv.csv НУЖЕН (1000+ строк)
- ❌ tests/test.sh НУЖЕН

**Episode 18: Trading Algorithms**
- ✅ README есть (базовый)
- ❌ solution/indicators.c НУЖЕН (SMA, RSI, MACD)
- ❌ artifacts/price_history.csv НУЖЕН
- ❌ tests/test.sh НУЖЕН

**Episode 19: Risk & Portfolio**
- ✅ README есть (базовый)
- ❌ solution/var_monte_carlo.c НУЖЕН
- ❌ artifacts/portfolio.csv НУЖЕН
- ❌ tests/test.sh НУЖЕН

**Episode 20: Crypto & DeFi**
- ✅ README есть (базовый)
- ❌ solution/arbitrage_finder.c НУЖЕН
- ❌ artifacts/exchange_data.json НУЖЕН
- ❌ tests/test.sh НУЖЕН

---

### Season 6: Embedded & IoT (20% → 80%)

**Episode 21-24:** Аналогично Season 5

---

### Season 7-8: Аналогично

---

### Season Projects (КРИТИЧНО!)

**Season 1:** `moonlight_decoder`
- Интегрирует Episodes 01-04
- XOR cipher, Caesar, frequency analysis
- Модульная архитектура (crypto.h, utils.h)

**Season 2:** `memory_manager`
- Интегрирует Episodes 05-08
- Leak detector, memory profiler
- Wrapper functions for malloc/free

**Season 3:** `network_interceptor`
- Интегрирует Episodes 09-12
- TCP client, packet sniffer, protocol parser
- Raw sockets, session reconstruction

**Season 4:** `crypto_toolkit`
- Интегрирует Episodes 13-16
- CLI tool: encrypt, hash, sort, search
- Hash tables, BST, blockchain verifier

---

## 📞 Вопросы и обсуждения

### Есть вопросы?

- **GitHub Issues:** для bug reports и feature requests
- **GitHub Discussions:** для общих вопросов
- **Email:** (если указан в профиле maintainer)

### Хотите помочь, но не знаете с чего начать?

1. Найдите Issues с тегом `good first issue`
2. Прочитайте секцию "Приоритеты контрибуций"
3. Выберите что-то из "Идеи для контрибуций"
4. Создайте Issue "Хочу сделать Episode XX" для координации

---

## 🎖️ Contributors

Все контрибьюторы будут указаны в:
- `README.md` (главный список)
- `CREDITS.md` (детальные благодарности)
- GitHub Contributors page

**Типы вклада:**
- 🆕 New episodes
- 🔧 Bug fixes
- 📚 Documentation
- 🌍 Translations
- 🧪 Testing
- 🎨 Design/UX
- 💡 Ideas & suggestions

Спасибо за вклад в обучение программированию на C! 🚀

---

## 📜 Code of Conduct

Будьте вежливы и уважительны:

### ✅ Приветствуется:
- Конструктивная критика с предложениями улучшений
- Помощь новичкам и терпеливые объяснения
- Уважение к разным уровням знаний
- Профессиональное общение
- Благодарность за чужой труд
- Признание ошибок и готовность учиться

### ❌ Недопустимо:
- Токсичность и оскорбления
- Дискриминация по любому признаку
- Троллинг и флейм
- Спам и реклама
- Плагиат без указания источников
- Harassment в любой форме

**Нарушения Code of Conduct:**
Сообщайте maintainers через email или приватные сообщения.

---

## 🏆 Качество превыше количества

**Помните:**
- 1 качественный эпизод > 10 незавершённых
- Тестирование важнее скорости
- Документация — часть кода
- Код пишется один раз, читается тысячи

**Стандарты качества:**
- ⭐⭐⭐⭐⭐ Production-ready (Season 1-4 уровень)
- ⭐⭐⭐⭐☆ Хорошо (минимум для merge)
- ⭐⭐⭐☆☆ Нужна доработка
- ⭐⭐☆☆☆ WIP (work in progress)

---

## 🚀 Roadmap к v1.0

```
v0.3.5 (текущая) ─┐
                  │
                  ├─→ v0.4.0: Season 5 @ 80% (4 эпизода)
                  │
                  ├─→ v0.5.0: Season 6 @ 80% (4 эпизода)
                  │
                  ├─→ v0.6.0: Season 7 @ 80% (4 эпизода)
                  │
                  ├─→ v0.7.0: Season 8 @ 80% (4 эпизода)
                  │
                  ├─→ v0.8.0: Season Projects 1-8
                  │
                  ├─→ v0.9.0: Season 9-10 @ 80% (10 эпизодов)
                  │
                  └─→ v1.0.0: ALL COMPLETE! 🎉
```

**Ваш вклад приближает нас к v1.0!**

---

## 💡 О проекте

Этот курс создан на основе идеи с платформы [Eurecable](https://eurecable.com/ideas/971).

**Философия курса:**
- Обучение через драматический сюжет
- Практика важнее теории (70/30)
- Cumulative code reuse (Season Projects)
- AI как инструмент (Cursor, ChatGPT)
- Cross-platform compatibility
- Production-ready code

См. [CREDITS.md](CREDITS.md) для полной информации об источниках и благодарностях.

---

**Удачи с вашим первым PR!** 🎯

*— The Architect*

---

**Последнее обновление:** 4 октября 2025  
**Версия документа:** 2.0 (Comprehensive Edition)