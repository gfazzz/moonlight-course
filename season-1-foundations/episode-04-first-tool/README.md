# Episode 04: "Первый инструмент"
## Season 1: Foundations

> *"Инструменты делают мастера. Код делает хакера."*

---

## 🎬 BRIEFING

**Локация:** Ваша квартира, Москва  
**Время:** 06:15, рассвет  
**Статус:** 🟢 Breakthrough

Паттерн найден. Период — 10 секунд. Маршрут построен. Финальная точка — дата-центр на Варшавском шоссе, сервер № 1723.

Но просто знать адрес недостаточно. Нужен **инструмент** — программа, которая:
- Расшифровывает перехваченные сообщения
- Классифицирует данные
- Находит паттерны
- Сохраняет результаты

Всё, что вы написали в Episodes 01-03, нужно объединить в один мощный дешифровщик. Не просто скрипт, а **полноценный инструмент**.

Пришло время изучить **функции**, **модули** и работу с **файлами**.

**Ваша задача:** Создать `moonlight-decoder` — ваш первый профессиональный инструмент.

---

## 📚 THEORY

### 1. Функции — переиспользование кода

Функция — это независимый блок кода, который можно вызывать многократно.

#### Объявление функции:

```c
возвращаемый_тип имя_функции(параметры) {
    // тело функции
    return значение;
}
```

**Пример:**
```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);  // result = 8
    printf("%d\n", result);
    return 0;
}
```

**Метафора:** Функция = чёрный ящик. Вход → обработка → выход.

---

#### Зачем функции?

1. **Переиспользование:** Написали раз, используем много
2. **Читаемость:** Код становится понятнее
3. **Тестирование:** Легко проверить отдельную функцию
4. **Модульность:** Разделение на логические части

**Плохо:**
```c
// Дублирование кода
int sum1 = a + b;
int sum2 = c + d;
int sum3 = e + f;
```

**Хорошо:**
```c
int add(int x, int y) { return x + y; }

int sum1 = add(a, b);
int sum2 = add(c, d);
int sum3 = add(e, f);
```

---

### 2. Параметры и возвращаемые значения

#### Передача по значению:
```c
void modify(int x) {
    x = 100;  // Изменяет только локальную копию
}

int main() {
    int num = 5;
    modify(num);
    printf("%d\n", num);  // Всё ещё 5!
}
```

#### Передача по указателю (будет в Season 2):
```c
void modify(int *x) {
    *x = 100;  // Изменяет оригинал
}
```

#### Возврат значения:
```c
int square(int x) {
    return x * x;
}

float divide(float a, float b) {
    if (b == 0) {
        printf("Error: division by zero\n");
        return 0.0;
    }
    return a / b;
}
```

#### Функция без возврата (void):
```c
void print_hello() {
    printf("Hello!\n");
    // Нет return
}
```

---

### 3. Заголовочные файлы (.h)

Для больших программ код разделяется на файлы:

**crypto.h** (объявления):
```c
#ifndef CRYPTO_H
#define CRYPTO_H

// Объявления функций
unsigned char xor_byte(unsigned char byte, unsigned char key);
void decrypt_message(unsigned char *data, int length, unsigned char key);

#endif
```

**crypto.c** (реализация):
```c
#include "crypto.h"

unsigned char xor_byte(unsigned char byte, unsigned char key) {
    return byte ^ key;
}

void decrypt_message(unsigned char *data, int length, unsigned char key) {
    for (int i = 0; i < length; i++) {
        data[i] = xor_byte(data[i], key);
    }
}
```

**main.c** (использование):
```c
#include <stdio.h>
#include "crypto.h"

int main() {
    unsigned char msg[] = {0x1B, 0x0A, 0x0E};
    decrypt_message(msg, 3, 0x42);
    printf("%s\n", msg);
    return 0;
}
```

**Компиляция:**
```bash
gcc -o program main.c crypto.c
```

**Метафора:** `.h` файл = оглавление книги. `.c` файл = содержание.

---

### 4. Работа с файлами

#### Открытие файла:
```c
FILE *fp = fopen("data.txt", "r");  // "r" = read
if (fp == NULL) {
    printf("Error opening file\n");
    return 1;
}
```

**Режимы:**
- `"r"` — чтение
- `"w"` — запись (перезаписывает)
- `"a"` — добавление
- `"rb"`, `"wb"` — бинарный режим

#### Чтение:
```c
char line[256];
while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
}
```

#### Запись:
```c
fprintf(fp, "Result: %d\n", result);
```

#### Закрытие:
```c
fclose(fp);  // Всегда закрывайте файлы!
```

---

### 5. Области видимости (scope)

```c
int global_var = 100;  // Глобальная — видна везде

void func() {
    int local_var = 50;  // Локальная — только в func()
    printf("%d\n", global_var);  // OK
    printf("%d\n", local_var);   // OK
}

int main() {
    printf("%d\n", global_var);  // OK
    // printf("%d\n", local_var);  // ОШИБКА! Не видна
    
    {
        int block_var = 25;  // Только в блоке {}
        printf("%d\n", block_var);  // OK
    }
    // printf("%d\n", block_var);  // ОШИБКА!
    
    return 0;
}
```

**Правило:** Используйте наименьший необходимый scope.

---

## 🎯 MISSION

### Задача: Создать moonlight-decoder

Объедините весь код из Episodes 01-03 в один инструмент.

#### Структура проекта:
```
moonlight-decoder/
├── decoder.c        # Главная программа
├── crypto.h         # Заголовок для криптофункций
├── crypto.c         # Реализация криптофункций
├── utils.h          # Утилиты
├── utils.c          # Реализация утилит
└── Makefile         # Автоматическая сборка
```

#### Функционал:

1. **Криптомодуль (crypto.c/h):**
   - `xor_decrypt()` — XOR дешифровка
   - `caesar_decrypt()` — Шифр Цезаря (bonus)
   - `validate_message()` — Проверка корректности

2. **Утилиты (utils.c/h):**
   - `read_file()` — Чтение файла
   - `write_file()` — Запись результата
   - `print_stats()` — Вывод статистики

3. **Главная программа (decoder.c):**
   - Парсинг аргументов командной строки
   - Вызов функций модулей
   - Обработка ошибок

---

## 📝 Пример использования

```bash
# Расшифровать файл
./decoder -i encrypted.dat -o decrypted.txt -k 0x42

# С выводом статистики
./decoder -i data.dat -o result.txt -k 0x42 -v

# Помощь
./decoder -h
```

---

## 🛠 Компиляция многофайлового проекта

```bash
# Вручную
gcc -c crypto.c -o crypto.o
gcc -c utils.c -o utils.o
gcc -c decoder.c -o decoder.o
gcc crypto.o utils.o decoder.o -o decoder

# С Makefile (проще!)
make
```

---

## 💡 Подсказки

### Подсказка 1: Header guards
```c
#ifndef CRYPTO_H
#define CRYPTO_H

// Объявления...

#endif
```

Предотвращает повторное включение файла.

### Подсказка 2: Аргументы командной строки
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    printf("File: %s\n", argv[1]);
    return 0;
}
```

### Подсказка 3: Проверка ошибок
```c
FILE *fp = fopen(filename, "r");
if (fp == NULL) {
    perror("Error opening file");  // Выводит системную ошибку
    return 1;
}
```

---

## 🎓 Что вы изучите

✅ Функции и их объявление  
✅ Параметры и возвращаемые значения  
✅ Заголовочные файлы (.h)  
✅ Работа с файлами (fopen, fgets, fprintf)  
✅ Области видимости  
✅ Модульная архитектура  
✅ Компиляция многофайловых проектов  
✅ Аргументы командной строки  

---

## 🎬 DEBRIEFING

После завершения у вас будет **moonlight-decoder** — полноценный инструмент для:
- Расшифровки сообщений
- Анализа данных
- Поиска паттернов

**Артефакт Season 1:** Готовый дешифровщик, который вы будете улучшать в следующих сезонах.

**Сюжет:** С инструментом в руках вы готовы к следующему шагу — взлому сервера. Но для этого нужно понять **память** и **указатели**...

**Status:** 🟢 Mission Ready  
**Next:** Season 2, Episode 05 - "Memory Dump"

---

## 🎯 Milestone: Season 1 Complete!

Поздравляем! Вы завершили **Season 1: Foundations**!

**Вы изучили:**
- ✅ Основы C (типы, операторы, массивы)
- ✅ Условные операторы (if/else/switch)
- ✅ Циклы (for/while/do-while)
- ✅ Функции и модули
- ✅ Работу с файлами

**Вы создали:**
- XOR дешифровщик
- Классификатор данных
- Анализатор паттернов
- Профессиональный инструмент

**Готовы к Season 2?** Там вас ждут указатели, динамическая память и реальная магия C! 🧙‍♂️

---

**Соберите свой инструмент, агент. Операция продолжается.**

```
// SEASON 1: COMPLETE
// MOONLIGHT DECODER: READY
// ADVANCING TO SEASON 2...
```
