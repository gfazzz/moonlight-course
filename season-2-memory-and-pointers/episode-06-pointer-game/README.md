# Episode 06: "Pointer Game" 🎯

> *"Pointers are not just addresses — they are the keys to unlock the system's deepest secrets."*

## 📚 Теория: Указатели в C

### 1. Что такое указатель?

**Указатель (pointer)** — это переменная, которая хранит адрес в памяти другой переменной.

```c
int x = 42;        // Обычная переменная
int *p = &x;       // Указатель на x

printf("Value: %d\n", x);      // 42
printf("Address: %p\n", &x);   // Адрес x в памяти (например, 0x7ffeea8b3c)
printf("Pointer: %p\n", p);    // То же что &x
printf("Dereferenced: %d\n", *p);  // 42 (значение по адресу)
```

**Два ключевых оператора**:
- `&` — взять адрес переменной ("адрес-оф")
- `*` — разыменовать указатель, получить значение ("звёздочка")

**Визуализация**:
```
Memory:
Address    | Variable | Value
-----------|----------|-------
0x1000     | x        | 42
0x2000     | p        | 0x1000
           |          |
           p -------> x
```

---

### 2. Объявление и инициализация

```c
// Объявление
int *ptr;          // Указатель на int (неинициализирован!)

// Инициализация
int value = 100;
ptr = &value;      // ptr теперь указывает на value

// Можно сразу
int *ptr2 = &value;

// Нулевой указатель (никуда не указывает)
int *null_ptr = NULL;

// ❌ ОПАСНО: неинициализированный указатель!
int *wild_ptr;     // Указывает на случайный адрес!
*wild_ptr = 5;     // КРАШ!
```

---

### 3. Разыменование (Dereferencing)

```c
int x = 10;
int *p = &x;

// Чтение через указатель
int y = *p;        // y = 10

// Изменение через указатель
*p = 20;           // x теперь равен 20!

printf("%d\n", x); // 20
```

**Важно**: `*p` в **объявлении** и в **разыменовании** — разные вещи!

```c
int *p;     // Объявление: p — это указатель
*p = 5;     // Разыменование: записываем 5 по адресу в p
```

---

### 4. Указатели и функции

**Передача по значению (копия)**:
```c
void increment(int x) {
    x = x + 1;  // Изменяет только локальную копию!
}

int main() {
    int num = 5;
    increment(num);
    printf("%d\n", num);  // Всё ещё 5!
}
```

**Передача по указателю (изменяет оригинал)**:
```c
void increment(int *x) {
    *x = *x + 1;  // Изменяет значение по адресу
}

int main() {
    int num = 5;
    increment(&num);  // Передаём адрес
    printf("%d\n", num);  // 6!
}
```

---

### 5. Указатели и массивы

**Массив — это по сути указатель на первый элемент!**

```c
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;  // Эквивалентно &arr[0]

// Три способа обращения к элементам:
printf("%d\n", arr[2]);   // 30
printf("%d\n", *(arr+2)); // 30
printf("%d\n", p[2]);     // 30
printf("%d\n", *(p+2));   // 30
```

**Арифметика указателей**:
```c
int *p = arr;

p++;        // Сдвиг на sizeof(int) байт вперёд
printf("%d\n", *p);  // 20 (второй элемент)

p += 2;     // Сдвиг на 2 элемента
printf("%d\n", *p);  // 40
```

**Визуализация**:
```
arr:  [10] [20] [30] [40] [50]
       ↑
       p (изначально)

p++:  [10] [20] [30] [40] [50]
            ↑
            p (теперь)
```

---

### 6. Типы указателей

Указатель "знает" размер типа, на который указывает:

```c
char   *pc;    // +1 сдвигает на 1 байт
int    *pi;    // +1 сдвигает на 4 байта (обычно)
double *pd;    // +1 сдвигает на 8 байт

int arr[3] = {10, 20, 30};
int *p = arr;

printf("%p\n", p);      // Например, 0x1000
printf("%p\n", p+1);    // 0x1004 (сдвиг на 4 байта!)
```

---

### 7. Указатели на указатели

```c
int x = 42;
int *p = &x;       // Указатель на x
int **pp = &p;     // Указатель на указатель

printf("%d\n", x);     // 42
printf("%d\n", *p);    // 42
printf("%d\n", **pp);  // 42!
```

**Визуализация**:
```
x (42) <--- p <--- pp
```

Используется для:
- Массивов массивов (матриц)
- Изменения указателя внутри функции
- Динамических структур данных

---

### 8. Распространённые ошибки

```c
// ❌ Разыменование NULL
int *p = NULL;
*p = 5;  // КРАШ!

// ❌ Разыменование неинициализированного
int *p;
*p = 5;  // КРАШ!

// ❌ Возврат указателя на локальную переменную
int* bad_function() {
    int x = 10;
    return &x;  // x умирает после выхода из функции!
}

// ❌ Выход за границы через указатель
int arr[5] = {1, 2, 3, 4, 5};
int *p = arr;
*(p + 10) = 99;  // Запись за пределами массива!

// ✅ Правильно: всегда проверяйте NULL
int *p = get_pointer();
if (p != NULL) {
    *p = 5;
}
```

---

## 🎯 Миссия: "Навигатор указателей"

### Легенда

V. оставил новый файл — `pointers.dat`. Это не простые данные, а **схема памяти** с цепочкой указателей.

Каждая запись содержит:
- **Offset** — смещение относительно начала файла
- **Value** — значение (число или ASCII-код)
- **Next** — смещение следующей записи (0 = конец цепочки)

Файл зашифрован так, что **порядок чтения определяется указателями**, а не последовательностью в файле!

**Ваша задача**: создать `pointer_navigator` — программу, которая:
1. Загружает схему памяти
2. Следует по цепочке указателей
3. Извлекает секретное сообщение
4. Реконструирует порядок доступа к данным

---

### Формат файла `pointers.dat`

**Бинарная структура записи** (12 байт):
```c
struct Entry {
    uint32_t offset;   // Смещение текущей записи (4 байта)
    uint32_t value;    // Значение (4 байта)
    uint32_t next;     // Смещение следующей записи (4 байта, 0 = конец)
};
```

**Особенности**:
- Записи могут идти в **произвольном** порядке в файле
- Порядок чтения определяется полем `next`
- Первая запись имеет offset=0
- Последняя запись имеет next=0

**Пример цепочки** (визуально):
```
Файл:
[Entry at 0x24: offset=0x24, value='W', next=0x00]  <-- последний
[Entry at 0x12: offset=0x12, value='o', next=0x24]
[Entry at 0x00: offset=0x00, value='H', next=0x12]  <-- первый

Чтение: 0x00 → 0x12 → 0x24
Результат: "How"
```

---

### Технические требования

**Входные данные**: файл `pointers.dat` (бинарный)

**Выходные данные**:
```
=== POINTER NAVIGATOR ===
Starting at offset: 0x0000

[0x0000] Value: 0x48 ('H') → Next: 0x0012
[0x0012] Value: 0x6F ('o') → Next: 0x0024
[0x0024] Value: 0x77 ('w') → Next: 0x0000 (END)

Decoded message: "How"
Total entries traversed: 3
```

**Функционал**:
1. Прочитать весь файл в память
2. Найти начальную запись (offset=0)
3. Следовать по цепочке `next`
4. Извлечь значения (можно декодировать как ASCII, если printable)
5. Вывести полный путь и декодированное сообщение

---

## 📝 Задание

### Starter Code

Файл `starter.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_ENTRIES 256

// Entry structure (12 bytes)
typedef struct {
    uint32_t offset;
    uint32_t value;
    uint32_t next;
} Entry;

// TODO: Implement functions

// Find entry by offset
Entry* find_entry(Entry *entries, int count, uint32_t offset) {
    // ваш код
}

// Check if character is printable ASCII
int is_printable(uint32_t value) {
    // ваш код
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pointers.dat>\n", argv[0]);
        return 1;
    }

    // TODO:
    // 1. Open file
    // 2. Read all entries into array
    // 3. Start from offset=0
    // 4. Follow the chain (next pointers)
    // 5. Print path and decode message

    return 0;
}
```

---

### Подсказки

1. **Чтение структур из файла**:
```c
FILE *f = fopen(filename, "rb");
Entry entries[MAX_ENTRIES];
int count = fread(entries, sizeof(Entry), MAX_ENTRIES, f);
```

2. **Поиск записи по offset**:
```c
for (int i = 0; i < count; i++) {
    if (entries[i].offset == target_offset) {
        return &entries[i];
    }
}
```

3. **Проверка на зацикливание**:
```c
int visited[MAX_ENTRIES] = {0};
// При посещении записи i:
if (visited[i]) {
    printf("ERROR: Cycle detected!\n");
    break;
}
visited[i] = 1;
```

4. **Декодирование ASCII**:
```c
if (value >= 32 && value < 127) {
    printf("('%c')", (char)value);
}
```

---

## ✅ Проверка

```bash
# Сборка
make

# Тестирование
make test

# Или вручную
./pointer_navigator pointers.dat
```

**Критерии успеха**:
- Все записи прочитаны корректно
- Цепочка указателей пройдена полностью
- Сообщение декодировано правильно
- Обнаруживаются циклы (если есть)
- Обрабатываются некорректные offset'ы

---

## 🎁 Артефакт

После выполнения миссии вы получите:
- **Навык**: мастерское владение указателями
- **Инструмент**: `pointer_navigator` — обходчик связанных структур
- **Понимание**: как работают linked lists, trees, graphs

Эти знания критичны для Episode 07 (динамическая память)!

---

## 🔗 Навигация

- [← Episode 05: Memory Map](../episode-05-memory-map/README.md)
- [→ Episode 07: Deep Dive](../episode-07-deep-dive/README.md)
- [📋 Season 2: Memory & Pointers](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"Master pointers, and you master the machine itself."* — V.
