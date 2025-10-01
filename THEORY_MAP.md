# 🗺️ Карта теории курса MOONLIGHT

**Последнее обновление:** 1 октября 2025  
**Версия курса:** v0.3.0  
**Статус:** 🟢 Season 1-4 @ 80% (готовы к прохождению) | 🟡 Season 5-10 @ 20%

## 📋 Распределение тем по сезонам

### **Season 1: Foundations** — Базовый C 🟢 80%

| Тема                             | Episode | Детали сюжета                          |
|----------------------------------|---------|----------------------------------------|
| Переменные, типы данных          | 01      | Сообщение от "мёртвого" Виктора        |
| Массивы и строки                 | 01      | Зашифрованный текст                    |
| **sizeof** оператор              | 01      | Размер массива для декодера            |
| Ввод/вывод (printf, scanf)       | 01      | Интерактивный декодер                  |
| XOR операция                     | 01      | Шифр от V. с визуализацией             |
| Промежуточная задача             | 01      | **byte_printer.c**                     |
| Условные операторы (if/else)     | 02      | Классификация данных                   |
| Логические операторы             | 02      | Фильтрация 142 записей                 |
| **#define и макросы**            | 02      | Константы классификатора               |
| **scanf с пробелом**             | 02      | Важная деталь буфера                   |
| Switch statement                 | 02      | Маршрутизация типов данных             |
| Deadline                         | 02      | SMS "48 часов" от UNKNOWN              |
| Циклы (for, while, do-while)     | 03      | Поиск паттернов (период 10 сек)        |
| **const константы**              | 03      | Метафора: страховочный трос            |
| Сюжетный твист                   | 03      | Два "Виктора" — кому верить?           |
| Функции                          | 04      | Модульная архитектура                  |
| Модульность (.h/.c)              | 04      | 3 модуля: crypto, utils, decoder       |
| **struct структуры**             | 04      | Метафора: анкета ФСБ                   |
| **typedef псевдонимы**           | 04      | Упрощение типов                        |
| Работа с файлами (fopen/fread)   | 04      | Чтение зашифрованных файлов            |
| ФИНАЛЬНЫЙ ВЫБОР                  | 04      | Влияет на начало Season 2!             |

---

### **Season 2: Memory & Pointers** — Управление памятью 🟢 80%

| Тема                      | Episode | Зависит от        | Детали                              |
|---------------------------|---------|-------------------|-------------------------------------|
| Массивы и строки          | 05      | S1E01 (массивы)   | GPS координаты, deadline 6ч         |
| Адресная арифметика       | 05      | S1E01             | sizeof, границы массивов            |
| Указатели (основы)        | 06      | S1E04 (функции)   | Операторы & и *                     |
| Указатели и массивы       | 06      | S2E05, S1E01      | Floyd's cycle detection             |
| Linked Lists (intro)      | 06      | S1E04 (struct)    | Две цепочки (V. vs Z.)              |
| malloc/calloc             | 07      | S2E05-06          | Heap vs Stack                       |
| realloc                   | 07      | malloc            | Файл растёт в реальном времени      |
| free                      | 07      | malloc            | Освобождение памяти                 |
| Memory leaks (5 типов)    | 08      | S2E07             | Троян от Z., DoS через утечки       |
| Valgrind/leaks            | 08      | S2E07             | Профилирование памяти               |
| Memory pools              | 08      | S2E07             | Оптимизация                         |

---

### **Season 3: Networks** — Сетевое программирование 🟢 80%

| Тема                         | Episode | Зависит от    | Детали сюжета                      |
|------------------------------|---------|---------------|------------------------------------|
| TCP/IP модель                | 09      | —             | Метафора "Город Интернет"          |
| IP адресация                 | 09      | S1: массивы   | **IP → GPS декодирование!**        |
| Порты и протоколы            | 09      | —             | Port 31337 (leet)                  |
| Byte order (htons/ntohs)     | 09      | —             | Network vs host order              |
| Промежуточная задача         | 09      | S1E01         | **ip_converter.c**                 |
| Socket programming           | 10      | S3E09         | Подключение к серверу V.           |
| TCP/UDP сокеты               | 10      | S3E09         | Challenge-response                 |
| **DDoS-атака в 10:16**       | 10      | S3E09         | Deadline 5 часов!                  |
| Промежуточная задача         | 10      | S3E09         | **simple_echo_client.c**           |
| Raw sockets (требует sudo)   | 11      | S3E09-10      | SOCK_RAW, IPPROTO_TCP              |
| Packet sniffing              | 11      | S3E09-10      | Перехват трафика Z.                |
| **Counter-surveillance**     | 11      | S3E09-10      | Z. тоже используют sniffing!       |
| IP/TCP headers парсинг       | 11      | S1: struct    | struct iphdr, tcphdr               |
| Промежуточная задача         | 11      | S1E04         | **header_parser.c**                |
| TCP session reconstruction   | 12      | S3E09-11      | 8 пакетов → поток                  |
| qsort по SEQ numbers         | 12      | —             | Библиотечная функция               |
| **XOR дешифровка**           | 12      | **S1E01**     | Callback к Season 1!               |
| ФИНАЛ                        | 12      | —             | Встреча с Viktor, USB Season 4     |

**✅ ПРОВЕРКА**: Season 3 НЕ использует темы из Season 4!

- ❌ Не использует алгоритмы сортировки (только `qsort` из stdlib)
- ❌ Не использует linked lists, trees, hash tables
- ❌ Не использует хеш-функции (кроме простейшей в E10)
- ✅ Только анонсирует Season 4 в конце

---

### **Season 4: Crypto & Algorithms** — Криптография и алгоритмы 🟢 80%

| Тема                            | Ep  | Зависит от           | Новые концепции                    |
|---------------------------------|-----|----------------------|------------------------------------|
| Caesar cipher                   | 13  | S1E01                | Частотный анализ                   |
| XOR cipher (углублённо)         | 13  | S1E01                | Криптоанализ                       |
| **Chi-squared test**            | 13  | S1E02                | Автоопределение shift              |
| **Known-plaintext attack**      | 13  | S1E01                | Криптоанализ                       |
| Хеш-функции (djb2, SHA-256)     | 14  | S1E02                | **Avalanche effect**               |
| Blockchain                      | 14  | S1E04                | Immutability, PoW                  |
| **Merkle Trees (concept)**      | 14  | S1E04                | Корневой хеш                       |
| Bubble/Selection/Insertion      | 15  | S1E03                | Big O notation                     |
| **Shell Sort** (Knuth)          | 15  | S1E03                | O(n^3/2), gap sequence             |
| **Heapsort**                    | 15  | S1E03                | O(n log n) worst case              |
| Merge/Quick Sort                | 15  | S1E04                | Divide & Conquer                   |
| **Knuth Shuffle**               | 15  | S1E03                | Fisher-Yates                       |
| **Sentinel optimization**       | 15  | S1E03                | Loop optimization                  |
| Binary Search                   | 15  | S1E03                | O(log n), loop invariants          |
| **Amortized analysis**          | 15  | S1E02                | Average performance                |
| Linked Lists (single, double)   | 16  | S2E07                | Динамические структуры             |
| Stacks & Queues                 | 16  | S2E07                | LIFO/FIFO                          |
| Binary Search Tree (BST)        | 16  | S2E07                | O(log n) average                   |
| **AVL Trees**                   | 16  | S2E07, S4E16         | O(log n) guaranteed, rotations     |
| **Hash Tables** (advanced)      | 16  | S4E14, S2E07         | Chaining, Open addressing          |
| **Load factor & Rehashing**     | 16  | S4E14                | Performance optimization           |
| **Database Engine**             | 16  | S1-S4                | System integration                 |

**✨ Новое в v2.0 Enhanced (Season 4):**
- ✅ TAOCP концепции: Shell Sort (Knuth), Heapsort, Knuth Shuffle, Sentinel optimization
- ✅ Advanced structures: AVL Trees с автобалансировкой (4 типа rotations)
- ✅ Hash Tables: chaining, open addressing, double hashing, load factor
- ✅ Database Engine: интеграция Hash Table + AVL Tree + Linked List + Stack/Queue
- ✅ Mathematical analysis: Big O, amortized analysis, loop invariants
- ✅ Криптоанализ: frequency analysis, Chi-squared, known-plaintext attack
- ✅ Blockchain: avalanche effect, cascade validation, Merkle Trees

---

## 🔗 Критические зависимости

### Season 3 ← Season 1:
```
S3E12 (Encrypted Communications)
  └─→ S1E01 (XOR шифрование)  ✅ Объяснено в S1!
  └─→ S1E04 (struct)           ✅ Объяснено в S1!
  └─→ S1E04 (typedef)          ✅ Объяснено в S1!
```

### Season 3 ← Season 2:
```
S3E10-12 (Сокеты и перехват)
  └─→ S2E05 (указатели)        ✅ Объяснено в S2!
  └─→ S2E06 (массивы и ptr)    ✅ Объяснено в S2!
```

### Season 4 ← Season 1-3:
```
S4E16 (Data Structures)
  └─→ S2E07 (malloc/free)      ✅ Объяснено в S2!
  └─→ S1E04 (struct)           ✅ Объяснено в S1!
  
S4E14 (Blockchain)
  └─→ S1E04 (struct)           ✅ Объяснено в S1!
  └─→ S4E13 (хеш-функции)      ✅ Будет объяснено в S4E13!
```

---

## ✅ Проверка целостности курса

### Все ли темы объяснены до использования?

| Использование | Где используется    | Где объяснено                  | Статус |
|---------------|---------------------|--------------------------------|--------|
| sizeof        | S1E01 (decoder)     | S1E01                          | ✅     |
| #define       | S1E02, S1E03        | S1E02                          | ✅     |
| const         | S1E03, S1E04        | S1E03                          | ✅     |
| struct        | S1E04, S3E09-12     | S1E04                          | ✅     |
| typedef       | S1E04, S3E09-12     | S1E04                          | ✅     |
| XOR           | S1E01, S3E12        | S1E01                          | ✅     |
| qsort         | S3E12               | stdlib (не требует теории)     | ✅     |
| malloc/free   | S2E07, S4E16        | S2E07                          | ✅     |

**Вывод**: ✅ Все темы объяснены ДО первого использования!

---

## 🎯 Рекомендованный порядок изучения

```
Season 1 (Foundations)
   ↓
Season 2 (Memory)
   ↓
Season 3 (Networks)  ← использует S1 (XOR, struct) + S2 (pointers)
   ↓
Season 4 (Crypto)    ← использует S1-2 (struct, malloc) + анонсируется в S3
```

**ВАЖНО**: Нельзя пропускать сезоны! Каждый строится на предыдущих.

---

## 🔍 Часто задаваемые вопросы

**Q: Можно ли начать с Season 3?**  
A: ❌ Нет! Нужны знания struct, typedef (S1) и указателей (S2).

**Q: В Season 3 используются алгоритмы сортировки?**  
A: Только `qsort()` из stdlib — библиотечная функция, не требует знания алгоритмов.

**Q: Где изучать typedef?**  
A: Season 1, Episode 04, секция 5.

**Q: Где изучать const?**  
A: Season 1, Episode 03, секция 2.

**Q: Где изучать sizeof?**  
A: Season 1, Episode 01, после секции про массивы.

---

---

## 📚 Статистика покрытия теории

### Season 1-4 (v2.0 Enhanced):
- **Общее количество концепций**: 60+
- **Из них TAOCP (Donald Knuth)**: 12
- **Визуальных метафор**: 12
- **ASCII-диаграмм**: 30+
- **Контрольных вопросов**: 150+ (интегрированы в сюжет)
- **Промежуточных задач**: 10 (с автотестами)

### Покрытие тем C (Season 1-4):
```
✅ Базовый синтаксис:        100% (S1)
✅ Память и указатели:        100% (S2)
✅ Сетевое программирование:  100% (S3)
✅ Алгоритмы и структуры:     100% (S4)
🟡 Embedded/IoT:               20% (S6)
🟡 System programming:         20% (S7)
🟡 AI/ML:                      20% (S8)
🟡 Advanced topics:            20% (S9-10)
```

---

*Последнее обновление: 1 октября 2025 (v0.3.0)*
