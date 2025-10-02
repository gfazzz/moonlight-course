# 🎯 Season 1 Final Project: moonlight_decoder

> *"Собери свой первый инструмент из компонентов миссий"*

---

## 📖 Предыстория

Ты прошёл через четыре эпизода. Расшифровал сообщения, классифицировал данные, нашёл паттерны, создал модульные инструменты. Теперь время собрать всё воедино.

**20:15** — Ты сидишь за ноутбуком Виктора. На экране — финальная задача:

> "Morpheus, если ты это читаешь — значит, ты прошёл все испытания.  
> Теперь собери **moonlight_decoder** — инструмент, который понадобится тебе в Season 2.  
> 
> Извлеки компоненты из своего кода. Интегрируй их. Создай единую систему.  
> Это не просто упражнение. Это твоё **оружие**.
> 
> — V."

---

## 🎯 Миссия: Собрать moonlight_decoder

### Что это?
Универсальный инструмент для работы с зашифрованными данными, который:
- Читает файлы с зашифрованными данными
- Определяет тип шифрования автоматически
- Расшифровывает XOR, Caesar, и другие алгоритмы
- Выводит результат в читаемом виде
- Валидирует данные перед обработкой

### Почему это важно?
В **Season 2** ты будешь работать с архивами Виктора. Без decoder'а ты не сможешь их прочитать.

---

## 📦 Компоненты проекта

Извлеки код из своих решений эпизодов:

### 1. Episode 01: Byte Operations
**Файлы:** `byte_ops.c` и `byte_ops.h`

**Что извлечь:**
```c
// byte_ops.h
void print_bytes_hex(const unsigned char *data, size_t len);
void print_bytes_as_chars(const unsigned char *data, size_t len);
int is_printable(unsigned char byte);
```

**Из какого кода:**
- Твоя программа `byte_printer` из Episode 01
- Функции для вывода байтов в hex и как символов

---

### 2. Episode 02: Data Validation
**Интегрируется в:** `utils.c` и `utils.h`

**Что извлечь:**
```c
// utils.h
int validate_file(const char *filename);
size_t read_file_to_buffer(const char *filename, unsigned char **buffer);
int is_valid_range(int value, int min, int max);
```

**Из какого кода:**
- Валидация данных из `data_classifier`
- Проверка диапазонов широты/долготы

---

### 3. Episode 03: Pattern Analysis
**Интегрируется в:** `decoder.c` и `decoder.h`

**Что извлечь:**
```c
// decoder.h
int detect_encryption_type(const unsigned char *data, size_t len);
unsigned char find_most_frequent_byte(const unsigned char *data, size_t len);
```

**Из какого кода:**
- Анализ паттернов из `pattern_finder`
- Поиск наиболее частых байтов (для frequency analysis)

---

### 4. Episode 04: Cryptography
**Файлы:** `crypto.c` и `crypto.h` (уже есть!)

**Что использовать:**
```c
// crypto.h
void xor_decrypt(unsigned char *data, size_t len, unsigned char key);
void caesar_decrypt(char *text, int shift);
unsigned char crack_xor_key(const unsigned char *data, size_t len);
```

**Из какого кода:**
- Твой модуль `crypto` из Episode 04
- Уже модульный, просто переиспользуй!

---

## 🏗️ Структура проекта

Создай следующую структуру в корне `season-1-foundations/`:

```
season-1-foundations/
├── byte_ops.c          ← Создай из Episode 01
├── byte_ops.h          ← Создай из Episode 01
├── crypto.c            ← Копируй из episode-04/solution/
├── crypto.h            ← Копируй из episode-04/solution/
├── utils.c             ← Создай (validation из Episode 02)
├── utils.h             ← Создай
├── decoder.c           ← Создай (pattern analysis из Episode 03)
├── decoder.h           ← Создай (главный API)
├── season_project.c    ← Main файл (используй starter!)
└── Makefile            ← Обнови (раскомментируй строки)
```

---

## 🔧 Пошаговая инструкция

### Шаг 1: Создай byte_ops модуль
```bash
cd ~/moonlight-course/season-1-foundations
```

Создай `byte_ops.h`:
```c
#ifndef BYTE_OPS_H
#define BYTE_OPS_H

#include <stddef.h>

// Печать байтов в hex формате
void print_bytes_hex(const unsigned char *data, size_t len);

// Печать байтов как символы
void print_bytes_as_chars(const unsigned char *data, size_t len);

// Проверка, является ли байт печатным символом
int is_printable(unsigned char byte);

#endif
```

Создай `byte_ops.c` — извлеки функции из Episode 01.

---

### Шаг 2: Создай utils модуль
Создай `utils.h` и `utils.c` с валидацией из Episode 02.

---

### Шаг 3: Создай decoder модуль
Создай `decoder.h`:
```c
#ifndef DECODER_H
#define DECODER_H

#include <stddef.h>

// Определение типа шифрования
#define ENCRYPTION_UNKNOWN 0
#define ENCRYPTION_XOR     1
#define ENCRYPTION_CAESAR  2

int detect_encryption_type(const unsigned char *data, size_t len);
int decrypt_auto(unsigned char *data, size_t len);

#endif
```

---

### Шаг 4: Используй crypto из Episode 04
```bash
# Crypto уже модульный, просто используй его!
```

---

### Шаг 5: Собери main файл
Используй `season_project_starter.c` как основу.

Раскомментируй `#include` для своих модулей.

Реализуй функции:
1. Чтение файла → `utils.c`
2. Определение типа шифрования → `decoder.c`
3. Расшифровка → `crypto.c`
4. Вывод → `byte_ops.c`

---

### Шаг 6: Обнови Makefile
Открой `Makefile` в корне сезона.

Раскомментируй строки с твоими `.c` файлами:
```makefile
PROJECT_SRC = season_project.c
PROJECT_SRC += byte_ops.c      # Раскомментируй
PROJECT_SRC += crypto.c utils.c decoder.c  # Раскомментируй
```

---

### Шаг 7: Собери проект
```bash
make season_project
```

Исправь ошибки компиляции, если есть.

---

### Шаг 8: Тестирование
```bash
# Создай тестовый зашифрованный файл
echo -n "Hello from Viktor!" > test.txt
# Зашифруй его (используй encoder из Episode 01 или создай вручную)

# Расшифруй с помощью decoder'а
./moonlight_decoder test.enc
```

---

## ✅ Критерии успеха

Твой `moonlight_decoder` должен:

- ✅ Компилироваться без ошибок
- ✅ Читать зашифрованный файл из аргумента командной строки
- ✅ Автоматически определять тип шифрования (XOR или Caesar)
- ✅ Расшифровывать данные
- ✅ Выводить результат в читаемом виде
- ✅ Обрабатывать ошибки (файл не найден, неверный формат)

**Бонус:**
- 🌟 Поддержка нескольких форматов вывода (hex, text, binary)
- 🌟 Статистика о расшифрованных данных
- 🌟 Сохранение результата в файл

---

## 🎓 Что ты узнаешь

1. **Модульность** — как разбивать код на переиспользуемые компоненты
2. **API Design** — как проектировать .h файлы
3. **Интеграция** — как собирать систему из частей
4. **Линковка** — как компилятор связывает .c файлы
5. **Рефакторинг** — как извлекать код из решений

---

## 🚀 Следующий шаг

После того как соберёшь decoder, сохрани его!

В **Season 2** ты будешь использовать этот код через:
```c
#include "../season-1-foundations/decoder.h"
#include "../season-1-foundations/crypto.h"
```

**Твой код станет твоим оружием в следующих сезонах!**

---

## 💡 Подсказки

**Застрял?**
1. Посмотри на свои решения в `episode-*/solution/`
2. Начни с простого — сначала один модуль
3. Тестируй каждый модуль отдельно
4. Используй `printf` для отладки
5. Используй AI (Cursor) для помощи с синтаксисом

**Ошибки компиляции?**
- Проверь header guards (`#ifndef`, `#define`, `#endif`)
- Проверь, что все `.h` подключены в `.c`
- Проверь, что все функции объявлены в `.h` перед использованием

---

## 📞 Финальное сообщение от Viktor

> "Morpheus, decoder — это основа. В Season 2 ты столкнёшься с архивом на 2GB.  
> Без правильной работы с памятью ты не справишься.  
> 
> Пусть твой код будет чистым. Он будет жить долго.
> 
> Увидимся на Красной площади.  
> 
> — V."

---

**Время:** 20:45  
**Deadline:** Нет. Это твой проект. Но Season 2 не начнётся, пока decoder не готов.

**Начинай! 🚀**

