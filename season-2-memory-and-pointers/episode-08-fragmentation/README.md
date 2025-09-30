# Episode 08: "Fragmentation" 🔍

> *"Memory leaks are silent killers. Learn to hunt them down before they hunt you."*

## 📚 Теория: Поиск утечек и оптимизация памяти

### 1. Что такое утечка памяти?

**Утечка памяти (memory leak)** — ситуация, когда выделенная память не освобождается и не может быть использована повторно.

```c
// ❌ Утечка
void leak_example() {
    int *p = malloc(1000 * sizeof(int));
    // Забыли free(p)
}  // p исчезает, память потеряна навсегда (до завершения программы)

// После 1000 вызовов: 4MB памяти утекло!
for (int i = 0; i < 1000; i++) {
    leak_example();
}
```

**Последствия**:
- Постоянный рост потребления памяти
- Замедление системы (swap)
- Краш при исчерпании памяти
- Невозможность выделить новую память

---

### 2. Типы утечек

**1. Прямая утечка (Direct leak)**:
```c
int *p = malloc(100);
// Забыли free(p)
```

**2. Потерянный указатель**:
```c
int *p = malloc(100);
p = malloc(200);  // Старый блок потерян!
```

**3. Косвенная утечка (Indirect leak)**:
```c
struct Node {
    int *data;
    struct Node *next;
};

Node *head = malloc(sizeof(Node));
head->data = malloc(100);  // Выделили
free(head);  // Освободили Node, но data утекла!
```

**4. Условная утечка**:
```c
int *p = malloc(100);
if (error) {
    return;  // Выход без free!
}
free(p);
```

---

### 3. Инструменты поиска утечек

#### **macOS: `leaks`**

Встроенная утилита Apple:

```bash
# Запустить с проверкой утечек при выходе
leaks --atExit -- ./my_program

# Или проверить запущенный процесс по PID
leaks <pid>
```

**Интерпретация вывода**:
```
leaks Report Version: 4.0
Process 12345: 512 nodes malloced for 8192 KB
Process 12345: 1 leak for 1024 total leaked bytes.

Leak: 0x7f8a12345000  size=1024  zone: DefaultMallocZone_0x...
    0x00007fff12345678  malloc
    0x0000000100001234  my_function
    0x0000000100001567  main
```

#### **Linux: `valgrind`**

```bash
# Полная проверка утечек
valgrind --leak-check=full --show-leak-kinds=all ./my_program

# Только утечки без деталей
valgrind --leak-check=summary ./my_program

# С трекингом источников
valgrind --track-origins=yes ./my_program
```

**Вывод valgrind**:
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 1,024 bytes in 1 blocks
==12345==   total heap usage: 10 allocs, 9 frees, 10,240 bytes allocated
==12345==
==12345== 1,024 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2AB80: malloc (in /usr/lib/valgrind/...)
==12345==    by 0x400567: my_function (main.c:42)
==12345==    by 0x4005A3: main (main.c:56)
```

---

### 4. Паттерны предотвращения утечек

**1. Правило: Кто выделил — тот и освобождает**

```c
// ✅ Хорошо
char* create_string() {
    char *s = malloc(100);
    strcpy(s, "Hello");
    return s;
}

void use_string() {
    char *s = create_string();
    printf("%s\n", s);
    free(s);  // Вызывающий освобождает
}
```

**2. Обнуление после free**

```c
// ✅ Безопасно
free(p);
p = NULL;  // Теперь повторный free безопасен (ничего не делает)

if (p != NULL) {
    free(p);  // Не выполнится, если уже освобождено
}
```

**3. Cleanup-функции**

```c
typedef struct {
    char *name;
    int *data;
} Resource;

Resource* create_resource() {
    Resource *r = malloc(sizeof(Resource));
    r->name = malloc(100);
    r->data = malloc(1000 * sizeof(int));
    return r;
}

// ✅ Централизованная очистка
void destroy_resource(Resource *r) {
    if (r) {
        free(r->name);
        free(r->data);
        free(r);
    }
}
```

**4. RAII-подобные макросы (продвинуто)**

```c
#define CLEANUP_FREE __attribute__((cleanup(cleanup_free)))

void cleanup_free(void *p) {
    free(*(void**)p);
}

void example() {
    CLEANUP_FREE char *str = malloc(100);
    // str автоматически освободится при выходе из scope!
}
```

---

### 5. Защита от двойного освобождения (Double Free)

**Проблема**:
```c
int *p = malloc(100);
free(p);
free(p);  // КРАШ!
```

**Решение 1: Обнулять**:
```c
free(p);
p = NULL;
free(p);  // OK (free(NULL) безопасен)
```

**Решение 2: Безопасный макрос**:
```c
#define SAFE_FREE(p) do { \
    free(p); \
    p = NULL; \
} while(0)

SAFE_FREE(p);
SAFE_FREE(p);  // Безопасно
```

---

### 6. Оптимизация использования памяти

**1. Переиспользование буферов**

```c
// ❌ Плохо (много аллокаций)
for (int i = 0; i < 1000; i++) {
    char *buf = malloc(1024);
    // работа с buf
    free(buf);
}

// ✅ Хорошо (одна аллокация)
char *buf = malloc(1024);
for (int i = 0; i < 1000; i++) {
    // работа с buf
}
free(buf);
```

**2. Memory pools (пулы памяти)**

```c
typedef struct {
    void *memory;
    size_t size;
    size_t used;
} MemoryPool;

MemoryPool* pool_create(size_t size) {
    MemoryPool *pool = malloc(sizeof(MemoryPool));
    pool->memory = malloc(size);
    pool->size = size;
    pool->used = 0;
    return pool;
}

void* pool_alloc(MemoryPool *pool, size_t size) {
    if (pool->used + size > pool->size) {
        return NULL;  // Пул переполнен
    }
    void *ptr = (char*)pool->memory + pool->used;
    pool->used += size;
    return ptr;
}

void pool_destroy(MemoryPool *pool) {
    free(pool->memory);
    free(pool);
}
```

**3. Кэширование размеров**

```c
// ❌ Плохо
for (int i = 0; i < strlen(str); i++) {  // strlen вызывается каждый раз!
    process(str[i]);
}

// ✅ Хорошо
size_t len = strlen(str);  // Один раз
for (int i = 0; i < len; i++) {
    process(str[i]);
}
```

---

### 7. Best Practices

1. **Всегда проверяйте malloc**:
```c
int *p = malloc(size);
if (!p) {
    perror("malloc failed");
    return -1;
}
```

2. **Один malloc — один free**:
```c
// Трекайте все аллокации
allocated++;
// ...
freed++;
// В конце: allocated == freed?
```

3. **Используйте статический анализ**:
```bash
# clang static analyzer
scan-build make

# cppcheck
cppcheck --enable=all .
```

4. **Добавляйте ассерты**:
```c
#include <assert.h>

void function(int *p) {
    assert(p != NULL);  // Краш в дебаге, если NULL
    *p = 42;
}
```

5. **Логируйте аллокации (дебаг)**:
```c
#ifdef DEBUG
#define MALLOC(size) (printf("MALLOC %zu at %s:%d\n", size, __FILE__, __LINE__), malloc(size))
#define FREE(p) (printf("FREE %p at %s:%d\n", p, __FILE__, __LINE__), free(p))
#else
#define MALLOC malloc
#define FREE free
#endif
```

---

## 🎯 Миссия: "Детектив утечек"

### Легенда

Вам досталась программа от предыдущего агента — **система логирования MOONLIGHT**. Она работает, но есть проблема: память постоянно растёт!

После 1000 операций программа съедает 100MB+. Очевидно, где-то утечки.

**Ваша задача**:
1. Запустить программу с детектором утечек
2. Найти все утечки памяти
3. Исправить их
4. Оптимизировать использование памяти
5. Доказать отсутствие утечек

---

### Программа `logger.c` (с багами!)

**Предоставлена**: `buggy_logger.c` — программа с утечками

**Симптомы**:
- Память растёт при каждом логе
- Через 1000 операций: ~50MB утекло
- Valgrind/leaks показывают утечки

**Функционал**:
- Чтение конфига из файла
- Запись логов с временными метками
- Ротация логов (создание новых файлов)
- Буферизация сообщений

---

### Технические требования

**Входные данные**: 
- `config.txt` (конфигурация)
- Команды через stdin или аргументы

**Задачи**:
1. Запустить с valgrind/leaks
2. Идентифицировать утечки (файл:строка)
3. Исправить каждую утечку
4. Добавить cleanup-функции
5. Проверить: 0 leaks

**Критерии успеха**:
```
valgrind --leak-check=full ./logger
...
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: N allocs, N frees, X bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
```

---

## 📝 Задание

### Starter Code

Файл `buggy_logger.c` содержит утечки. Найдите и исправьте!

Файл `fixed_logger.c` — ваша исправленная версия.

### Подсказки

1. **Найдите утечки**:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./buggy_logger
# Или на macOS:
leaks --atExit -- ./buggy_logger
```

2. **Типичные места**:
   - Чтение конфига (строки)
   - Добавление лог-записей
   - Ротация файлов
   - Буферы

3. **Чеклист проверки**:
   - [ ] Каждый malloc имеет соответствующий free
   - [ ] Нет потерянных указателей
   - [ ] Освобождаются вложенные структуры
   - [ ] free вызывается даже при ошибках

---

## ✅ Проверка

```bash
# Исправить
nano fixed_logger.c

# Собрать
make

# Проверить
make test
make valgrind  # или make leaks
```

**Критерии успеха**:
- 0 байт утекло
- Все allocs == все frees
- Программа работает корректно
- Оптимизация: меньше аллокаций чем в оригинале

---

## 🎁 Артефакт

После выполнения миссии вы получите:
- **Навык**: мастерство поиска и устранения утечек
- **Инструмент**: `leak_detector` — отлаженная система
- **Сертификат**: Memory Management Expert 🏆

**Season 2 завершён!** Вы освоили память на уровне профессионала.

---

## 🔗 Навигация

- [← Episode 07: Deep Dive](../episode-07-deep-dive/README.md)
- [→ Season 3: Networks](../../season-3-networks/README.md)
- [📋 Season 2: Memory & Pointers](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"Clean code is not just about logic. It's about resources — allocated, used, and freed with precision."* — V.
