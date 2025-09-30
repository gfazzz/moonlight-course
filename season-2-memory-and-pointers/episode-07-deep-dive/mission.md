# Миссия: "Динамический загрузчик"

## 🎯 Цель

Создать `dynamic_loader` — систему загрузки файлов с переменной структурой и динамическим выделением памяти.

## 📋 Технические требования

### Входные данные

- Файл: `archive.dat` (бинарный формат)
- Структура: HEADER + BLOCKS (переменное количество и размер)

### Формат архива

**HEADER** (8 байт):
```c
typedef struct {
    char magic[4];        // "MOON" (0x4D 0x4F 0x4F 0x4E)
    uint16_t version;     // Версия формата (1)
    uint16_t block_count; // Количество блоков
} Header;
```

**BLOCK** (переменный размер):
```c
typedef struct {
    uint8_t type;         // Тип блока (1 байт)
    uint32_t size;        // Размер данных (4 байта)
    uint8_t *data;        // Данные (size байт)
} Block;
```

**Типы блоков**:
- `0x01` — TEXT: текстовые данные (можно вывести как строку)
- `0x02` — BINARY: произвольные байты (вывести hex dump)
- `0x03` — COMPRESSED: сжатые данные (не распаковываем, только показываем размер)

### Выходные данные

```
=== DYNAMIC LOADER ===
Archive: archive.dat
Magic: MOON
Version: 1
Blocks: N

Loading blocks...
[1/N] TEXT (XX bytes) loaded
[2/N] BINARY (XXX bytes) loaded
...

Block #1 (TEXT):
"Text content here..."

Block #2 (BINARY):
00000000: 48 65 6C 6C 6F 20 57 6F  72 6C 64 00 00 00 00 00  |Hello World.....|
...

Total memory allocated: XXXX bytes
Freeing memory...
Memory freed successfully. No leaks!
```

### Функционал

1. **Чтение заголовка**:
   - Открыть файл
   - Прочитать и валидировать magic ("MOON")
   - Получить количество блоков

2. **Динамическая загрузка блоков**:
   - Выделить массив Block'ов (malloc)
   - Для каждого блока:
     * Прочитать type и size
     * Выделить память под data (malloc)
     * Прочитать data

3. **Вывод содержимого**:
   - TEXT: вывести как строку
   - BINARY: hex dump (опционально, можно просто размер)
   - COMPRESSED: только размер и тип

4. **Освобождение памяти**:
   - Освободить data каждого блока
   - Освободить массив блоков
   - Проверить отсутствие утечек

---

## 🔧 Реализация

### Обязательные функции

```c
// Чтение и валидация заголовка
Header* read_header(FILE *f);

// Чтение одного блока (с malloc для data!)
Block* read_block(FILE *f);

// Вывод блока (разный формат для разных типов)
void print_block(Block *block, int index);

// Освобождение блока (free data!)
void free_block(Block *block);

// Подсчёт выделенной памяти
size_t calculate_total_memory(Block *blocks, int count);
```

### Структура main()

```c
int main(int argc, char *argv[]) {
    // 1. Открыть файл
    // 2. Прочитать заголовок
    // 3. Проверить magic
    // 4. Выделить массив блоков
    // 5. Загрузить все блоки
    // 6. Вывести содержимое
    // 7. Подсчитать память
    // 8. Освободить всё
    // 9. Вывести статистику
    return 0;
}
```

---

## ✅ Критерии приёмки

- [ ] Программа компилируется без предупреждений
- [ ] Корректно читает и валидирует заголовок
- [ ] Динамически выделяет память для блоков
- [ ] Динамически выделяет память для данных блоков
- [ ] Правильно обрабатывает все типы блоков
- [ ] Корректно освобождает всю память
- [ ] Проверяет все вызовы malloc на NULL
- [ ] Обрабатывает ошибки открытия/чтения файла
- [ ] Valgrind не показывает утечек
- [ ] `make test` проходит успешно

---

## 💡 Подсказки

<details>
<summary>Как валидировать magic?</summary>

```c
Header* read_header(FILE *f) {
    Header *h = malloc(sizeof(Header));
    if (!h) return NULL;
    
    fread(h, sizeof(Header), 1, f);
    
    if (memcmp(h->magic, "MOON", 4) != 0) {
        fprintf(stderr, "Invalid magic number!\n");
        free(h);
        return NULL;
    }
    
    return h;
}
```
</details>

<details>
<summary>Как читать блок?</summary>

```c
Block* read_block(FILE *f) {
    Block *b = malloc(sizeof(Block));
    if (!b) return NULL;
    
    // Читаем type и size
    if (fread(&b->type, 1, 1, f) != 1 ||
        fread(&b->size, sizeof(uint32_t), 1, f) != 1) {
        free(b);
        return NULL;
    }
    
    // Выделяем память под данные
    b->data = malloc(b->size);
    if (!b->data) {
        free(b);
        return NULL;
    }
    
    // Читаем данные
    if (fread(b->data, 1, b->size, f) != b->size) {
        free(b->data);
        free(b);
        return NULL;
    }
    
    return b;
}
```
</details>

<details>
<summary>Как освободить все?</summary>

```c
// Порядок важен!

// 1. Освобождаем данные каждого блока
for (int i = 0; i < block_count; i++) {
    if (blocks[i].data) {
        free(blocks[i].data);
        blocks[i].data = NULL;
    }
}

// 2. Освобождаем массив блоков
free(blocks);
blocks = NULL;

// 3. Освобождаем заголовок
free(header);
header = NULL;
```
</details>

<details>
<summary>Как подсчитать память?</summary>

```c
size_t total = sizeof(Header);  // Заголовок
total += block_count * sizeof(Block);  // Массив блоков

for (int i = 0; i < block_count; i++) {
    total += blocks[i].size;  // Данные каждого блока
}

printf("Total allocated: %zu bytes\n", total);
```
</details>

---

**Удачи, агент!** 🕵️‍♂️
