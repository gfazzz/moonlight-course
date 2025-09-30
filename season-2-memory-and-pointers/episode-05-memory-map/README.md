# Episode 05: "Memory Map" 🗺️

> *"Arrays are the foundation. Master them, and memory becomes your ally."*

## 📚 Теория: Массивы и строки в C

### 1. Массивы — упорядоченные данные

**Массив** — это последовательность элементов одного типа, расположенных в памяти подряд.

```c
// Объявление и инициализация
int numbers[5] = {10, 20, 30, 40, 50};

// Доступ к элементам (индексация с 0)
int first = numbers[0];   // 10
int last = numbers[4];    // 50

// Размер массива
int size = sizeof(numbers) / sizeof(numbers[0]);  // 5
```

**Важно**:
- Индексация начинается с 0
- Размер массива фиксирован при объявлении
- Выход за границы массива — undefined behavior (UB)!

---

### 2. Строки — массивы символов

В C строка — это массив `char`, заканчивающийся нулевым символом `'\0'`.

```c
// Три способа создания строки
char str1[] = "Hello";                    // Автоматический размер (6 байт с '\0')
char str2[10] = "World";                  // Явный размер (остальное заполнено '\0')
char str3[] = {'H', 'i', '\0'};          // Посимвольно

// ОШИБКА: нет завершающего '\0'
char bad[] = {'B', 'a', 'd'};            // Не строка!
```

**Визуализация в памяти**:
```
str1:  'H' 'e' 'l' 'l' 'o' '\0'
       [0] [1] [2] [3] [4] [5]
```

---

### 3. Стандартная библиотека `string.h`

```c
#include <string.h>

// Длина строки (без '\0')
size_t len = strlen("Hello");  // 5

// Копирование
char dest[20];
strcpy(dest, "Copy this");

// Сравнение (0 = равны, <0 или >0 = различны)
if (strcmp(str1, str2) == 0) {
    printf("Strings are equal\n");
}

// Конкатенация (добавление в конец)
strcat(dest, " more text");
```

**⚠️ Опасность**: `strcpy` и `strcat` не проверяют размер буфера → переполнение!

**Безопасные версии**:
```c
strncpy(dest, source, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';  // Гарантируем завершение

strncat(dest, source, sizeof(dest) - strlen(dest) - 1);
```

---

### 4. Многомерные массивы

```c
// Двумерный массив (матрица)
int matrix[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

// Доступ к элементу
int value = matrix[1][2];  // 7

// Обход всех элементов
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
        printf("%d ", matrix[i][j]);
    }
    printf("\n");
}
```

---

### 5. Массивы и функции

```c
// Массив в функцию передаётся по указателю (без копирования!)
void process_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;  // Изменения видны снаружи!
    }
}

int main() {
    int data[5] = {1, 2, 3, 4, 5};
    process_array(data, 5);  // data изменится!
    // data теперь: {2, 4, 6, 8, 10}
}
```

---

### 6. Распространённые ошибки

```c
// ❌ Выход за границы
int arr[5];
arr[5] = 100;  // Ошибка! Индексы: 0-4

// ❌ Неинициализированный массив
int data[100];
printf("%d", data[0]);  // Мусор в памяти!

// ❌ Переполнение буфера
char small[5];
strcpy(small, "This is too long");  // Переполнение!

// ✅ Правильно
char small[20];
strncpy(small, "Safe copy", sizeof(small) - 1);
small[sizeof(small) - 1] = '\0';
```

---

## 🎯 Миссия: "Карта памяти"

### Легенда

После успешной расшифровки сообщения V., вы обнаружили вложенный файл — `memory_dump.dat`. Это дамп памяти с закодированной информацией.

Анализ показывает:
- Файл содержит **структуры данных** переменной длины
- Каждая структура начинается с **заголовка** (тип и размер)
- Данные могут быть: координаты, имена файлов, временные метки
- Между структурами есть "мусор" — случайные байты для маскировки

**Ваша задача**: создать `memory_mapper` — программу, которая:
1. Читает дамп памяти
2. Находит все валидные структуры данных
3. Извлекает и выводит информацию в читаемом виде
4. Строит "карту памяти" — визуализацию расположения данных

---

### Формат данных

**Структура записи**:
```
[TYPE (1 байт)] [SIZE (1 байт)] [DATA (SIZE байт)] [CHECKSUM (1 байт)]
```

**Типы данных**:
- `0x01` — строка (имя файла)
- `0x02` — координаты (широта, долгота — по 4 байта каждая, float)
- `0x03` — временная метка (UNIX timestamp — 4 байта, uint32_t)
- `0xFF` — маркер мусора (игнорировать до следующего валидного типа)

**Checksum** — простая контрольная сумма: XOR всех байт DATA.

---

### Пример дампа

```
FF FF 01 05 48 65 6C 6C 6F 48    // Мусор + строка "Hello"
FF FF FF 02 08 [float] [float] CS // Мусор + координаты
03 04 [timestamp] CS               // Временная метка
```

---

### Технические требования

**Входные данные**: файл `memory_dump.dat` (бинарный)

**Выходные данные** (stdout):
```
=== MEMORY MAP ===
[0x0000] GARBAGE (2 bytes)
[0x0002] STRING (5 bytes): "Hello"
[0x0009] GARBAGE (3 bytes)
[0x000C] COORDINATES (8 bytes): 55.7558° N, 37.6173° E
[0x0016] TIMESTAMP (4 bytes): 2024-03-15 18:30:00 UTC

Total structures found: 3
Total garbage bytes: 5
```

**Функционал**:
1. Чтение бинарного файла в массив байтов
2. Парсинг структур данных
3. Проверка checksum для валидности
4. Конвертация бинарных данных в читаемый формат
5. Вывод карты памяти с адресами

---

## 📝 Задание

### Starter Code

Файл `starter.c` содержит заготовку:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DUMP_SIZE 4096

// Типы структур
#define TYPE_STRING 0x01
#define TYPE_COORDS 0x02
#define TYPE_TIMESTAMP 0x03
#define TYPE_GARBAGE 0xFF

// TODO: Реализуйте функции

// Вычисление checksum (XOR всех байт)
uint8_t calculate_checksum(uint8_t *data, int size) {
    // ваш код здесь
}

// Парсинг строки
void parse_string(uint8_t *data, int size) {
    // ваш код здесь
}

// Парсинг координат (два float)
void parse_coordinates(uint8_t *data, int size) {
    // ваш код здесь
}

// Парсинг временной метки
void parse_timestamp(uint8_t *data, int size) {
    // ваш код здесь
}

// Конвертация UNIX timestamp в читаемый формат
void format_timestamp(uint32_t timestamp, char *buffer) {
    // ваш код здесь
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <memory_dump.dat>\n", argv[0]);
        return 1;
    }

    // TODO: 
    // 1. Открыть файл
    // 2. Прочитать в массив байтов
    // 3. Пройти по массиву, распознавая структуры
    // 4. Вывести карту памяти

    return 0;
}
```

---

### Подсказки

1. **Чтение бинарного файла**:
```c
FILE *f = fopen(filename, "rb");
uint8_t buffer[MAX_DUMP_SIZE];
size_t bytes_read = fread(buffer, 1, MAX_DUMP_SIZE, f);
```

2. **Конвертация байтов в float**:
```c
float value;
memcpy(&value, &buffer[offset], sizeof(float));
```

3. **Конвертация байтов в uint32_t**:
```c
uint32_t timestamp;
memcpy(&timestamp, &buffer[offset], sizeof(uint32_t));
```

4. **Форматирование времени**:
```c
#include <time.h>
time_t t = (time_t)timestamp;
struct tm *tm_info = gmtime(&t);
strftime(buffer, 26, "%Y-%m-%d %H:%M:%S UTC", tm_info);
```

---

## ✅ Проверка

```bash
# Сборка
make

# Тестирование
make test

# Или вручную
./memory_mapper memory_dump.dat
```

**Критерии успеха**:
- Все структуры данных распознаны правильно
- Checksum проверяется для каждой структуры
- Garbage-байты корректно пропускаются
- Выходной формат соответствует спецификации
- Программа не падает на некорректных данных

---

## 🎁 Артефакт

После выполнения миссии вы получите:
- **Навык**: работа с массивами и строками
- **Инструмент**: `memory_mapper` — парсер бинарных данных
- **Знание**: понимание layout данных в памяти

Этот инструмент понадобится в следующих эпизодах!

---

## 🔗 Навигация

- [← Episode 04: First Tool](../../season-1-foundations/episode-04-first-tool/README.md)
- [→ Episode 06: Pointer Game](../episode-06-pointer-game/README.md)
- [📋 Season 2: Memory & Pointers](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"Arrays and strings are the alphabet of systems programming. Learn to read memory like a book."* — V.
