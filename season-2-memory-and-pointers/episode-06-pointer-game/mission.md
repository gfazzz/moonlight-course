# Миссия: "Навигатор указателей"

## 🎯 Цель

Создать `pointer_navigator` — утилиту для обхода связанных структур данных через указатели.

## 📋 Технические требования

### Входные данные

- Файл: `pointers.dat` (бинарный формат)
- Структура данных: массив записей Entry
- Порядок записей в файле — произвольный
- Порядок чтения определяется цепочкой указателей

### Формат данных

**Структура Entry** (12 байт):
```c
typedef struct {
    uint32_t offset;   // Смещение данной записи (4 байта)
    uint32_t value;    // Значение (4 байта)
    uint32_t next;     // Смещение следующей записи (4 байта, 0=конец)
} Entry;
```

**Правила**:
- Первая запись в цепочке имеет `offset = 0`
- Последняя запись имеет `next = 0`
- Записи могут быть в файле в любом порядке
- Чтение должно следовать по полю `next`

### Выходные данные

Формат вывода (stdout):
```
=== POINTER NAVIGATOR ===
Starting at offset: 0x0000

[0xXXXX] Value: 0xXX (optional_char) → Next: 0xYYYY
[0xYYYY] Value: 0xXX (optional_char) → Next: 0xZZZZ
...
[0xZZZZ] Value: 0xXX (optional_char) → Next: 0x0000 (END)

Decoded message: "extracted_text"
Total entries traversed: N

Path taken: 0x0000 → 0xYYYY → 0xZZZZ → END
```

### Функционал

1. **Загрузка данных**:
   - Открыть файл в бинарном режиме
   - Прочитать все записи Entry в массив
   - Определить количество записей

2. **Навигация по указателям**:
   - Начать с записи offset=0
   - Для каждой записи:
     * Вывести offset, value, next
     * Если value — printable ASCII (32-126), показать символ
     * Перейти к записи с offset=next
   - Остановиться когда next=0

3. **Декодирование сообщения**:
   - Собрать все value как символы (если printable)
   - Вывести итоговое сообщение

4. **Обнаружение ошибок**:
   - Циклы (запись указывает на уже посещенную)
   - Несуществующий offset в next
   - Отсутствие начальной записи (offset=0)

### Примеры

**Входной файл** (3 записи, перемешаны):
```
Entry 1: offset=0x0018, value=0x6F ('o'), next=0x0000
Entry 2: offset=0x0000, value=0x48 ('H'), next=0x000C
Entry 3: offset=0x000C, value=0x69 ('i'), next=0x0018
```

**Вывод**:
```
=== POINTER NAVIGATOR ===
Starting at offset: 0x0000

[0x0000] Value: 0x48 ('H') → Next: 0x000C
[0x000C] Value: 0x69 ('i') → Next: 0x0018
[0x0018] Value: 0x6F ('o') → Next: 0x0000 (END)

Decoded message: "Hio"
Total entries traversed: 3

Path taken: 0x0000 → 0x000C → 0x0018 → END
```

---

## 🔧 Реализация

### Обязательные функции

```c
// Поиск записи по offset
Entry* find_entry(Entry *entries, int count, uint32_t offset);

// Проверка printable ASCII
int is_printable(uint32_t value);

// Обнаружение цикла
int detect_cycle(int *visited, int index, int count);
```

### Структура main()

```c
int main(int argc, char *argv[]) {
    // 1. Проверка аргументов
    // 2. Открытие файла
    // 3. Чтение массива Entry
    // 4. Поиск начальной записи (offset=0)
    // 5. Цикл навигации:
    //    - Вывод текущей записи
    //    - Сохранение value (для декодирования)
    //    - Поиск следующей записи по next
    //    - Проверка на цикл
    // 6. Вывод декодированного сообщения
    // 7. Вывод статистики
    return 0;
}
```

---

## ✅ Критерии приёмки

- [ ] Программа компилируется без предупреждений
- [ ] Корректно читает структуры Entry из файла
- [ ] Находит начальную запись (offset=0)
- [ ] Следует по цепочке next указателей
- [ ] Декодирует printable ASCII в текст
- [ ] Обнаруживает и сообщает о циклах
- [ ] Обрабатывает некорректные offset'ы
- [ ] Выводит полный путь обхода
- [ ] `make test` проходит успешно

---

## 💡 Подсказки

<details>
<summary>Как читать массив структур?</summary>

```c
FILE *f = fopen(filename, "rb");
Entry entries[MAX_ENTRIES];
size_t count = fread(entries, sizeof(Entry), MAX_ENTRIES, f);
fclose(f);

printf("Read %zu entries\n", count);
```
</details>

<details>
<summary>Как искать запись по offset?</summary>

```c
Entry* find_entry(Entry *entries, int count, uint32_t offset) {
    for (int i = 0; i < count; i++) {
        if (entries[i].offset == offset) {
            return &entries[i];
        }
    }
    return NULL;  // Не найдено
}
```
</details>

<details>
<summary>Как обнаружить цикл?</summary>

```c
// Массив посещённых записей
int visited[MAX_ENTRIES] = {0};
int visit_count = 0;

Entry *current = start;
while (current != NULL && current->next != 0) {
    // Проверяем, не посещали ли
    for (int i = 0; i < visit_count; i++) {
        if (visited[i] == current->offset) {
            printf("CYCLE DETECTED!\n");
            return -1;
        }
    }
    
    visited[visit_count++] = current->offset;
    current = find_entry(entries, count, current->next);
}
```
</details>

<details>
<summary>Как собрать сообщение?</summary>

```c
char message[256] = {0};
int msg_len = 0;

Entry *current = start;
while (current != NULL && current->next != 0) {
    if (current->value >= 32 && current->value < 127) {
        message[msg_len++] = (char)current->value;
    }
    current = find_entry(entries, count, current->next);
}
message[msg_len] = '\0';

printf("Decoded message: \"%s\"\n", message);
```
</details>

---

**Удачи, агент!** 🕵️‍♂️
