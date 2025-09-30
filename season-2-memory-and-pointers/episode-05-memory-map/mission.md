# Миссия: "Карта памяти"

## 🎯 Цель

Создать `memory_mapper` — утилиту для анализа дампов памяти и извлечения структурированных данных.

## 📋 Технические требования

### Входные данные

- Файл: `memory_dump.dat` (бинарный формат)
- Размер: до 4096 байт
- Передаётся через аргумент командной строки

### Формат данных в дампе

**Структура записи**:
```
[TYPE] [SIZE] [DATA...] [CHECKSUM]
  1б     1б    SIZE байт    1б
```

**Типы (TYPE)**:
- `0x01` — STRING: текстовая строка (имя файла)
- `0x02` — COORDINATES: два float (широта, долгота)
- `0x03` — TIMESTAMP: uint32_t (UNIX timestamp)
- `0xFF` — GARBAGE: мусорные байты (игнорировать)

**Контрольная сумма (CHECKSUM)**:
- XOR всех байт в поле DATA
- Используется для проверки целостности

### Выходные данные

Формат вывода (stdout):
```
=== MEMORY MAP ===
[0x0000] GARBAGE (N bytes)
[0xXXXX] STRING (N bytes): "filename.txt"
[0xXXXX] COORDINATES (8 bytes): LAT° N/S, LON° E/W
[0xXXXX] TIMESTAMP (4 bytes): YYYY-MM-DD HH:MM:SS UTC
...

Total structures found: X
Total garbage bytes: Y
```

### Функционал

1. **Чтение файла**:
   - Открыть бинарный файл
   - Прочитать в буфер (uint8_t array)
   - Обработать ошибки открытия/чтения

2. **Парсинг структур**:
   - Пройти по буферу байт за байтом
   - Распознать тип структуры
   - Извлечь размер и данные
   - Проверить checksum

3. **Обработка типов данных**:
   - **STRING**: вывести как текст (с экранированием непечатных символов)
   - **COORDINATES**: преобразовать байты в два float, вывести в градусах
   - **TIMESTAMP**: преобразовать uint32_t в дату/время UTC
   - **GARBAGE**: подсчитать, но не выводить содержимое

4. **Вывод карты**:
   - Адрес начала структуры в hex (0xXXXX)
   - Тип структуры
   - Размер в байтах
   - Декодированное значение
   - Итоговая статистика

### Обработка ошибок

- Если checksum не совпадает — пропустить структуру, вывести `[CORRUPTED]`
- Если неожиданный конец файла — завершить парсинг
- Если неизвестный тип (не 0x01/0x02/0x03/0xFF) — считать garbage

### Примеры

**Входной дамп** (hex):
```
FF FF                           // 2 байта garbage
01 05 48 65 6C 6C 6F 48         // STRING "Hello" (checksum=0x48)
02 08 [4 байта float1] [4 байта float2] CS
03 04 [4 байта timestamp] CS
```

**Вывод**:
```
=== MEMORY MAP ===
[0x0000] GARBAGE (2 bytes)
[0x0002] STRING (5 bytes): "Hello"
[0x000A] COORDINATES (8 bytes): 55.7558° N, 37.6173° E
[0x0014] TIMESTAMP (4 bytes): 2024-03-15 18:30:00 UTC

Total structures found: 3
Total garbage bytes: 2
```

---

## 🔧 Реализация

### Обязательные функции

```c
// Вычисление XOR checksum
uint8_t calculate_checksum(uint8_t *data, int size);

// Парсинг типов данных
void parse_string(uint8_t *data, int size);
void parse_coordinates(uint8_t *data, int size);
void parse_timestamp(uint8_t *data, int size);

// Форматирование вывода
void format_timestamp(uint32_t timestamp, char *buffer);
const char* type_to_string(uint8_t type);
```

### Структура main()

```c
int main(int argc, char *argv[]) {
    // 1. Проверка аргументов
    // 2. Открытие файла
    // 3. Чтение в буфер
    // 4. Парсинг в цикле:
    //    - Прочитать TYPE
    //    - Если GARBAGE → пропустить байт
    //    - Иначе: прочитать SIZE, DATA, CHECKSUM
    //    - Проверить checksum
    //    - Вызвать parse_* для типа
    // 5. Вывод статистики
    // 6. Закрытие файла
    return 0;
}
```

---

## ✅ Критерии приёмки

- [ ] Программа компилируется без ошибок и предупреждений
- [ ] Все типы данных корректно распознаются
- [ ] Checksum проверяется для каждой структуры
- [ ] GARBAGE-байты пропускаются
- [ ] Координаты выводятся в формате градусов с N/S/E/W
- [ ] Временные метки конвертируются в UTC
- [ ] Выходной формат точно соответствует спецификации
- [ ] `make test` проходит успешно

---

## 💡 Подсказки

<details>
<summary>Как читать бинарный файл?</summary>

```c
FILE *f = fopen(filename, "rb");  // "rb" = read binary
if (!f) {
    perror("fopen");
    return 1;
}

uint8_t buffer[MAX_SIZE];
size_t bytes_read = fread(buffer, 1, MAX_SIZE, f);
fclose(f);
```
</details>

<details>
<summary>Как преобразовать 4 байта в float?</summary>

```c
float value;
memcpy(&value, &buffer[offset], sizeof(float));
// Теперь value содержит число
```
</details>

<details>
<summary>Как вычислить XOR checksum?</summary>

```c
uint8_t checksum = 0;
for (int i = 0; i < size; i++) {
    checksum ^= data[i];
}
return checksum;
```
</details>

<details>
<summary>Как конвертировать timestamp в дату?</summary>

```c
#include <time.h>

uint32_t ts = ...; // из файла
time_t t = (time_t)ts;
struct tm *tm_info = gmtime(&t);
char buffer[64];
strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", tm_info);
printf("%s\n", buffer);
```
</details>

---

**Удачи, агент!** 🕵️‍♂️
