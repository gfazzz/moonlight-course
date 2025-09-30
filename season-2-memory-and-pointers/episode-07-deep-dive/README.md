# Episode 07: "Deep Dive" 💾

> *"Static memory is a cage. Dynamic memory is freedom — and responsibility."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Безопасная квартира, Тагарнка, Москва          ║
║  ВРЕМЯ:   19:30, среда, 1 октября                        ║
║  СТАТУС:  🔴 FILE SIZE UNKNOWN - DYNAMIC ALLOC REQUIRED  ║
╚══════════════════════════════════════════════════════════╝
```

**ВРЕМЕННАЯ ЛИНИЯ:**
```
13:45  ✅ Цепочка указателей пройдена
15:00  📧 Новый файл: archive.dat
17:00  ⚠️  Попытка загрузить в статический массив — переполнение
18:15  💡 Анализ структуры файла
19:30  🎯 Решение: динамическое выделение памяти
```

### 🔐 СИТУАЦИЯ

После расшифровки цепочки указателей вы получили адрес: `0x600000000000` — область heap-памяти. Это не случайность. V. подготовил вас к следующему шагу.

На защищённый канал приходит файл — `archive.dat`. Но есть проблема: **размер файла неизвестен заранее**.

**Метаданные файла:**
```
Имя:     archive.dat
Размер:  ??? (переменный, до 10MB)
Тип:     Structured archive with variable blocks
Отправитель: V. (зашифрованный канал)
Сообщение: "Статические массивы больше не работают. Heap или ничего."
```

Вы пробуете загрузить в статический массив:

```c
char buffer[4096];  // 4KB
FILE *f = fopen("archive.dat", "rb");
fread(buffer, 1, sizeof(buffer), f);  // ❌ ПЕРЕПОЛНЕНИЕ! Файл > 4KB!
```

**Ошибка:**
```
*** buffer overflow detected ***
Aborted (core dumped)
```

**Что вы понимаете:**
- Файл имеет **переменный размер** (зависит от миссии)
- Статические массивы **ограничены** размером, известным на этапе компиляции
- Нужна **динамическая память** (heap) — выделение во время выполнения
- Это **единственный способ** работать с данными неизвестного размера

Виктор оставил подсказку: *"Heap — это океан памяти. Malloc — твой корабль. Free — твой якорь. Не забудь вернуться."*

### 🎯 ЗАДАЧА

Создать **`dynamic_loader`** — загрузчик файлов произвольного размера:
1. ✅ Определить размер файла динамически
2. ✅ Выделить память в heap (malloc)
3. ✅ Загрузить блоки переменного размера
4. ✅ Построить индекс для быстрого доступа
5. ✅ Корректно освободить ВСЮ память (free)

**Сложность:** 🔴 Hard  
**Новые навыки:** malloc, free, calloc, realloc, динамические структуры, управление heap

```
╔══════════════════════════════════════════════════════════╗
║  [!] КРИТИЧЕСКИ ВАЖНО:                                   ║
║  Каждый malloc ДОЛЖЕН иметь соответствующий free!        ║
║  Утечка памяти в продакшене = катастрофа.                ║
║  В Episode 08 вы научитесь их находить.                  ║
╚══════════════════════════════════════════════════════════╝
```

---

## 📚 Теория: Динамическая память в C

### 💡 Почему статические массивы не всегда подходят?

**Проблемы статической памяти:**
- 🚫 **Фиксированный размер** — объявляется при компиляции
- 🚫 **Ограничена стеком** — обычно ~8MB
- 🚫 **Не растёт** — нельзя изменить размер
- 🚫 **Умирает со scope** — нельзя вернуть из функции

**Когда нужна динамическая память:**
- 📁 **Файлы неизвестного размера** — загрузка данных
- 🌐 **Сетевые буферы** — пакеты переменной длины
- 🗂️ **Структуры данных** — деревья, графы, списки
- 🎮 **Игры** — динамические объекты, частицы
- 🤖 **AI/ML** — матрицы весов, датасеты

### 1. Стек vs Куча (Stack vs Heap)

**Стек (Stack)**:
- Автоматическое управление
- Ограниченный размер (~8MB)
- Быстрый доступ
- Переменные умирают при выходе из функции

```c
void function() {
    int local = 42;  // Выделено на стеке
    // local автоматически освободится при выходе
}
```

**Куча (Heap)**:
- Ручное управление (`malloc`/`free`)
- Большой размер (зависит от ОС)
- Медленнее стека
- Живёт до явного освобождения

```c
void function() {
    int *p = malloc(sizeof(int));  // Выделено в куче
    *p = 42;
    // Память НЕ освободится автоматически!
    free(p);  // Надо освободить вручную
}
```

---

### 2. Выделение памяти: `malloc()`

```c
#include <stdlib.h>

// malloc - memory allocation
void *malloc(size_t size);

// Пример
int *p = malloc(sizeof(int));  // Выделить 4 байта (обычно)
if (p == NULL) {
    fprintf(stderr, "Memory allocation failed!\n");
    return 1;
}
*p = 42;

free(p);  // Освободить
```

**Важно**:
- `malloc()` возвращает `void*` (нужно приведение типа в старом C)
- `malloc()` может вернуть `NULL` — ВСЕГДА проверяйте!
- Память не инициализирована (содержит мусор)

---

### 3. Массивы динамического размера

```c
int n;
printf("Enter array size: ");
scanf("%d", &n);

// Выделить массив из n элементов
int *arr = malloc(n * sizeof(int));
if (!arr) {
    perror("malloc failed");
    return 1;
}

// Использовать как обычный массив
for (int i = 0; i < n; i++) {
    arr[i] = i * 10;
}

// Освободить
free(arr);
```

---

### 4. Другие функции выделения

**`calloc()` — выделить и обнулить**:
```c
// calloc(количество, размер элемента)
int *arr = calloc(10, sizeof(int));  // 10 элементов, все = 0
free(arr);
```

**`realloc()` — изменить размер**:
```c
int *arr = malloc(10 * sizeof(int));

// Увеличить до 20 элементов
arr = realloc(arr, 20 * sizeof(int));
if (!arr) {
    // Ошибка! Старый указатель потерян!
}

free(arr);
```

**⚠️ Опасность `realloc()`**:
```c
// ❌ ПЛОХО
arr = realloc(arr, new_size);  // Если вернёт NULL, arr потеряется!

// ✅ ХОРОШО
int *temp = realloc(arr, new_size);
if (temp) {
    arr = temp;
} else {
    // arr ещё валиден, можно освободить
    free(arr);
}
```

---

### 5. Строки динамической длины

```c
// Прочитать строку неизвестной длины
char *read_line() {
    int capacity = 10;
    int length = 0;
    char *buffer = malloc(capacity);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *temp = realloc(buffer, capacity);
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[length++] = c;
    }
    buffer[length] = '\0';
    
    return buffer;  // Вызывающий должен free()!
}
```

---

### 6. Двумерные динамические массивы

**Метод 1: Массив указателей**:
```c
int rows = 3, cols = 4;

// Выделить массив указателей
int **matrix = malloc(rows * sizeof(int*));

// Выделить каждую строку
for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
}

// Использование
matrix[1][2] = 42;

// Освобождение (в обратном порядке!)
for (int i = 0; i < rows; i++) {
    free(matrix[i]);
}
free(matrix);
```

**Метод 2: Один блок памяти** (эффективнее):
```c
int *matrix = malloc(rows * cols * sizeof(int));

// Доступ как matrix[i][j]:
#define INDEX(i, j) ((i) * cols + (j))
matrix[INDEX(1, 2)] = 42;

// Или через функцию
int* get_element(int *matrix, int cols, int i, int j) {
    return &matrix[i * cols + j];
}

free(matrix);  // Одно освобождение!
```

---

### 7. Утечки памяти (Memory Leaks)

**Утечка памяти** — выделенная память не освобождена.

```c
// ❌ Утечка
void leak() {
    int *p = malloc(100 * sizeof(int));
    // Забыли free(p)!
}  // p исчезает, но память не освобождена!

// ❌ Потеря указателя
int *p = malloc(100);
p = malloc(200);  // Старый блок потерян!

// ✅ Правильно
int *p = malloc(100);
free(p);
p = malloc(200);
free(p);
```

---

### 8. Распространённые ошибки

```c
// ❌ Double free
int *p = malloc(100);
free(p);
free(p);  // КРАШ!

// ❌ Use after free
int *p = malloc(100);
free(p);
*p = 42;  // КРАШ!

// ❌ Освобождение не-malloc памяти
int x = 42;
int *p = &x;
free(p);  // КРАШ!

// ❌ Забыть проверить NULL
int *p = malloc(1000000000000);  // Может вернуть NULL!
*p = 42;  // КРАШ!

// ✅ Правильно
int *p = malloc(size);
if (p == NULL) {
    return 1;
}
*p = 42;
free(p);
p = NULL;  // Опционально, но хорошая практика
```

---

## 🎯 Миссия: "Динамический загрузчик"

### Легенда

Файлы операции MOONLIGHT имеют **переменный размер** и специальную структуру:
- **Заголовок** с метаданными
- **Блоки данных** различных типов и размеров
- **Индексная таблица** для быстрого доступа

Невозможно заранее знать размер файла или количество блоков. Нужен **динамический загрузчик**, который:
1. Читает заголовок
2. Выделяет память под данные
3. Загружает блоки
4. Строит индекс
5. Позволяет извлекать данные по индексу

---

### Формат файла `archive.dat`

**Структура**:
```
[HEADER]
  - magic (4 байта): "MOON"
  - version (uint16_t): версия формата
  - block_count (uint16_t): количество блоков
  
[BLOCKS] (block_count штук)
  - type (uint8_t): тип данных
  - size (uint32_t): размер данных
  - data (size байт): содержимое
  
[INDEX] (block_count записей)
  - offset (uint32_t): смещение блока от начала файла
  - type (uint8_t): дубликат типа для быстрого доступа
```

**Типы блоков**:
- `0x01` — TEXT (строка)
- `0x02` — BINARY (произвольные данные)
- `0x03` — COMPRESSED (сжатые данные, в этом задании не распаковываем)

---

### Технические требования

**Входные данные**: файл `archive.dat`

**Выходные данные**:
```
=== DYNAMIC LOADER ===
Archive: archive.dat
Version: 1
Blocks: 5

Loading blocks...
[1/5] TEXT (42 bytes) at 0x0010
[2/5] BINARY (1024 bytes) at 0x0040
[3/5] TEXT (15 bytes) at 0x0440
[4/5] COMPRESSED (500 bytes) at 0x0450
[5/5] TEXT (8 bytes) at 0x0640

Index built successfully.

Block 1 (TEXT): "This is a secret message..."
Block 2 (BINARY): [binary data, 1024 bytes]
Block 3 (TEXT): "V. was here."
Block 4 (COMPRESSED): [compressed, 500 bytes]
Block 5 (TEXT): "END"

Total memory allocated: 1589 bytes
Memory freed successfully.
```

**Функционал**:
1. Динамически выделить память под массив блоков
2. Прочитать каждый блок, выделяя память под его данные
3. Построить индекс (массив указателей/смещений)
4. Вывести содержимое блоков
5. Корректно освободить всю память

---

## 📝 Задание

### Структуры данных

```c
typedef struct {
    char magic[4];
    uint16_t version;
    uint16_t block_count;
} Header;

typedef struct {
    uint8_t type;
    uint32_t size;
    uint8_t *data;  // Динамически выделенная память!
} Block;

typedef struct {
    uint32_t offset;
    uint8_t type;
} IndexEntry;
```

### Starter Code

Файл `starter.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TYPE_TEXT       0x01
#define TYPE_BINARY     0x02
#define TYPE_COMPRESSED 0x03

// TODO: Определите структуры Header, Block, IndexEntry

// TODO: Реализуйте функции

// Прочитать заголовок
Header* read_header(FILE *f) {
    // ваш код
}

// Прочитать блок (с выделением памяти под data!)
Block* read_block(FILE *f) {
    // ваш код
}

// Вывести блок
void print_block(Block *block, int index) {
    // ваш код
}

// Освободить блок
void free_block(Block *block) {
    // ваш код
}

int main(int argc, char *argv[]) {
    // TODO:
    // 1. Открыть файл
    // 2. Прочитать заголовок
    // 3. Выделить массив блоков (malloc)
    // 4. Загрузить каждый блок
    // 5. Вывести содержимое
    // 6. Освободить всю память
    
    return 0;
}
```

---

### Подсказки

<details>
<summary>Как выделить массив структур?</summary>

```c
Block *blocks = malloc(block_count * sizeof(Block));
if (!blocks) {
    perror("malloc failed");
    return 1;
}

// Использование
for (int i = 0; i < block_count; i++) {
    blocks[i] = read_block(file);
}

// Освобождение
for (int i = 0; i < block_count; i++) {
    free_block(&blocks[i]);
}
free(blocks);
```
</details>

<details>
<summary>Как прочитать блок переменного размера?</summary>

```c
Block* read_block(FILE *f) {
    Block *b = malloc(sizeof(Block));
    
    fread(&b->type, 1, 1, f);
    fread(&b->size, sizeof(uint32_t), 1, f);
    
    // Выделить память под данные
    b->data = malloc(b->size);
    if (!b->data) {
        free(b);
        return NULL;
    }
    
    fread(b->data, 1, b->size, f);
    
    return b;
}
```
</details>

<details>
<summary>Как освободить все?</summary>

```c
// Сначала освободить вложенные данные
for (int i = 0; i < block_count; i++) {
    free(blocks[i].data);  // Данные блока
}
free(blocks);  // Массив блоков
free(header);  // Заголовок
```
</details>

---

## ✅ Проверка

```bash
make
make test
```

**Критерии успеха**:
- Память выделяется динамически
- Все блоки загружаются правильно
- TEXT блоки выводятся как строки
- Вся память корректно освобождается
- Нет утечек (проверить valgrind/leaks)

---

## 🎬 DEBRIEFING

```
╔═══════════════════════════════════════════════════════════╗
║     🎯 АРХИВ ЗАГРУЖЕН! ПАМЯТЬ УПРАВЛЯЕТСЯ ДИНАМИЧЕСКИ     ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  ✓ Файл прочитан: 2.7 MB (переменный размер)             ║
║  ✓ Блоков загружено: 142                                 ║
║  ✓ Память выделена: 2,834,560 байт                       ║
║  ✓ Индекс построен: 142 записи                           ║
║  ✓ Память освобождена: 100% (утечек нет)                 ║
║                                                           ║
║  [!] СОДЕРЖИМОЕ АРХИВА:                                  ║
║                                                           ║
║     → 47 текстовых сообщений от V.                       ║
║     → 28 бинарных файлов (шифроключи?)                   ║
║     → 67 сжатых блоков (данные операции MOONLIGHT)       ║
║                                                           ║
║  📍 Последнее сообщение V.:                              ║
║     "Память освоена. Но умеешь ли ты находить утечки?    ║
║      Episode 08 — последний экзамен."                    ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

### 📊 Статистика прохождения:
- ⏱️ Среднее время: 75-90 минут
- 🎓 Новые навыки: **malloc**, **free**, **calloc**, **realloc**, **heap management**
- 🏆 Достижение разблокировано: **"Heap Master"** 

### 🔓 Skill Unlocked: **Dynamic Memory Mastery**
```
[████████████████████] 100%

Вы освоили:
✓ Выделение памяти (malloc, calloc)
✓ Изменение размера (realloc)
✓ Освобождение памяти (free)
✓ Работу с динамическими структурами
✓ Двумерные динамические массивы
✓ Правильное управление lifecycle памяти
```

### 🎨 Easter Eggs

🔍 **Скрытые детали:**
- Heap-аллокатор в glibc использует алгоритм ptmalloc2
- Адрес 0x600000000000 — область mmap для больших аллокаций
- realloc может перемещать данные — поэтому старый указатель недействителен
- calloc быстрее, чем malloc + memset на некоторых системах

🎬 **Отсылки:**
- "Heap — океан памяти" — метафора из книги "Expert C Programming"
- Динамические структуры — основа ядра Linux
- Управление памятью — как в NASA Mission Critical Software

🔬 **Интересный факт:**
Windows и Linux управляют heap по-разному! Windows использует HeapAlloc, Linux — brk/mmap. Но malloc скрывает эти различия.

---

## 🎁 Артефакт

После выполнения миссии вы получите:
- **Навык**: динамическое управление памятью на уровне эксперта
- **Инструмент**: `dynamic_loader` — загрузчик файлов произвольного размера
- **Фундамент**: готовность к Episode 08 (детективная работа с утечками)

**Вы готовы к финальному испытанию Season 2!**

---

## 🔗 Навигация

- [← Episode 06: Pointer Game](../episode-06-pointer-game/README.md)
- [→ Episode 08: Fragmentation](../episode-08-fragmentation/README.md)
- [📋 Season 2: Memory & Pointers](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"With great power comes great responsibility. Manage your memory wisely."* — V.

```
// DYNAMIC MEMORY MASTERED
// HEAP ALLOCATED: 2,834,560 BYTES
// ALL FREED: NO LEAKS
// READY FOR THE FINAL TEST
```
