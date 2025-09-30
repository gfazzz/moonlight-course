# Миссия: "Детектив утечек"

## 🎯 Цель

Найти и устранить все утечки памяти в программе логирования MOONLIGHT.

## 📋 Задача

Вам досталась legacy-программа `buggy_logger.c`. Она **работает**, но **течёт как решето**:

- После 100 операций: ~5MB утекло
- После 1000 операций: ~50MB утекло  
- Valgrind кричит об утечках

**Ваша миссия**:
1. Запустить детектор утечек (valgrind/leaks)
2. Найти **все** утечки (файл:строка)
3. Исправить в `fixed_logger.c`
4. Доказать отсутствие утечек

---

## 🐛 Программа: Система логирования

### Функционал

- **Чтение конфига** из `config.txt`
- **Запись логов** с временными метками
- **Буферизация** сообщений
- **Ротация логов** (переключение файлов)

### Типичное использование

```bash
# Запустить
./buggy_logger

# Команды (stdin)
log "System started"
log "User logged in"
rotate
log "New file entry"
quit
```

### Формат конфига `config.txt`

```
log_dir=/tmp/logs
max_buffer_size=100
rotation_size=1024
enable_timestamps=1
```

---

## 🔧 Технические детали

### Структуры данных

```c
typedef struct {
    char *log_dir;
    int max_buffer_size;
    int rotation_size;
    int enable_timestamps;
} Config;

typedef struct LogEntry {
    char *message;
    time_t timestamp;
    struct LogEntry *next;
} LogEntry;

typedef struct {
    LogEntry *head;
    LogEntry *tail;
    int count;
} LogBuffer;
```

### Утечки (подсказки)

Программа содержит **минимум 5 утечек**:

1. **Конфиг**: строки из файла не освобождаются
2. **Лог-записи**: сообщения не очищаются
3. **Буфер**: при ротации остаются записи
4. **Временные буферы**: забыты free
5. **Косвенные**: вложенные структуры

---

## 📋 Требования к исправлению

### Обязательно:

- [ ] Все утечки устранены (0 bytes leaked)
- [ ] Программа работает идентично оригиналу
- [ ] Добавлены cleanup-функции
- [ ] Все ошибки malloc обрабатываются
- [ ] Нет double-free, use-after-free
- [ ] Код читаемый и документированный

### Опционально (бонус):

- [ ] Оптимизация: меньше аллокаций
- [ ] Memory pool для лог-записей
- [ ] SAFE_FREE макросы
- [ ] Статистика памяти

---

## ✅ Проверка

### 1. Найти утечки

```bash
# Linux
valgrind --leak-check=full --show-leak-kinds=all ./buggy_logger < test_input.txt

# macOS
leaks --atExit -- ./buggy_logger < test_input.txt
```

**Анализируйте вывод**:
```
==12345== 1,024 bytes in 1 blocks are definitely lost
==12345==    at 0x4C2AB80: malloc (vg_replace_malloc.c:299)
==12345==    by 0x400678: read_config (buggy_logger.c:45)
==12345==    by 0x400890: main (buggy_logger.c:123)
```

→ Утечка в `read_config()` на строке 45!

### 2. Исправить

Создайте `fixed_logger.c` с исправлениями.

```c
// Пример исправления
void cleanup_config(Config *cfg) {
    if (cfg) {
        free(cfg->log_dir);  // Было: утечка!
        // ...
    }
}
```

### 3. Проверить результат

```bash
make fixed
make test

# Финальная проверка
valgrind --leak-check=full ./fixed_logger < test_input.txt
# Должно быть: "All heap blocks were freed -- no leaks are possible"
```

---

## 💡 Подсказки

<details>
<summary>Как найти утечку в чтении конфига?</summary>

```c
// Проблема:
char* read_line(FILE *f) {
    char *line = malloc(256);
    fgets(line, 256, f);
    return line;  // Кто освободит?
}

// В main:
char *line = read_line(file);
// ... используем line ...
// УТЕЧКА! Забыли free(line)

// Решение:
free(line);
```
</details>

<details>
<summary>Как очистить связанный список?</summary>

```c
void free_log_buffer(LogBuffer *buf) {
    LogEntry *current = buf->head;
    while (current) {
        LogEntry *next = current->next;
        free(current->message);  // Сначала вложенное
        free(current);           // Потом структуру
        current = next;
    }
    buf->head = buf->tail = NULL;
    buf->count = 0;
}
```
</details>

<details>
<summary>Как обнулять указатели после free?</summary>

```c
#define SAFE_FREE(p) do { \
    if (p) { \
        free(p); \
        p = NULL; \
    } \
} while(0)

// Использование:
SAFE_FREE(config->log_dir);
SAFE_FREE(config);  // Теперь NULL
```
</details>

---

## 🎁 Критерии успеха

**100% выполнение**:
```
$ valgrind --leak-check=full ./fixed_logger < test_input.txt
...
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 42 allocs, 42 frees, 8,192 bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
==12345==
==12345== ERROR SUMMARY: 0 errors from 0 contexts
```

**Программа работает**:
```
$ ./fixed_logger
> log "Test message"
[2024-09-30 12:00:00] Test message
> quit
Logs flushed. Exiting.
```

---

## 📊 Отчёт (опционально)

Создайте `LEAK_REPORT.md`:

```markdown
# Leak Report

## Found Leaks

1. **read_config() line 45**
   - Type: Direct leak
   - Size: 256 bytes
   - Fix: Added free() in cleanup_config()

2. **add_log_entry() line 89**
   - Type: Indirect leak
   - Size: 1024 bytes (message string)
   - Fix: Free message before freeing entry

...

## Statistics

- Original: 42 allocs, 35 frees → 7 leaks
- Fixed: 42 allocs, 42 frees → 0 leaks
- Memory saved: ~5KB per 100 operations
```

---

**Удачи, детектив!** 🕵️‍♂️ Найдите все утечки!
