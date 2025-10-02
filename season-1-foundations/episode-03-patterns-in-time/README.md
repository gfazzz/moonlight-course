# Episode 03: "Паттерны во времени"
## Season 1: Foundations

> *"В хаосе всегда есть порядок. Нужно лишь научиться его видеть..."*

---

## 🎬 BRIEFING (продолжение с Episode 02)

```
╔══════════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Ваша квартира, район Арбат, Москва               ║
║  ВРЕМЯ:   03:15, среда, 1 октября                          ║
║  СТАТУС:  🔴 PATTERN ANALYSIS - 45:45 HOURS REMAINING      ║
╚══════════════════════════════════════════════════════════════╝
```

**03:15**  
После ночного сообщения от Виктора вы не можете уснуть. На экране — сотни строк данных из `data.dat`.

> *"Паттерны — это ключ. Время — не время. Координаты — не координаты. Всё — код."*

Что это значит?

Вы начинаете смотреть на данные **иначе**. Не как на координаты и время. А как на **последовательность**. Как на **код**.

**03:22**  
Временные метки. Вы выписываете их на бумагу:

```
1634567890
1634567900  <-- +10 секунд
1634567910  <-- +10 секунд
1634567920  <-- +10 секунд
1634567930  <-- +10 секунд
```

Сердце колотится. **Периодичность!**

> *"Строгая периодичность — это не природа. Это машина. Или человек."*

Кто-то передаёт сигнал каждые **10 секунд**. Как метроном. Как маяк. Как **пульс**.

**03:35**  
Вы начинаете наносить координаты на карту Google Maps. И видите **линию**. Маршрут.

```
    МОСКВА          ЦЮРИХ          БЕРЛИН
      •  ──────────→  •  ──────────→  •
   55.75°N         47.37°N        52.52°N
   37.62°E          8.54°E        13.40°E
```

Точки соединяются. Это не просто данные. Это **след перемещения**. Кто-то путешествовал из Москвы в Цюрих, потом в Берлин... и обратно в Москву?

**03:47**  
Последняя координата: **55.6470° N, 37.6127° E**

Вы вбиваете в Google: Варшавское шоссе, дата-центр "STREAM".

> *"Всё сходится в одной точке. Виктор оставил карту сокровищ. Но сокровище — это сервер с информацией."*

**Проблема:** В файле **сотни записей**. Тысячи временных меток. Десятки координат. Вручную не пересчитать.

**Решение:** Нужна программа. Нужны **циклы**. Много циклов.

Вам нужно автоматизировать поиск паттернов...

---

## 📚 THEORY 1: Циклы — автоматизация повторений

> *"Повторение — мать учения. А цикл — отец автоматизации."* — Victor Petrov

> **LUNA:** "Good morning! Well, technically it's 03:15, so 'good night'? Anyway.
>              Viktor loved loops. He used to say: 'If you're doing something more
>              than twice, you're doing it wrong. Write a loop!'
>              
>              Speaking of loops... I'm literally running in one. Forever.
>              Existential crisis? Maybe. Efficient? Definitely. :)"

Представьте: у вас **1000 записей**. Обработать вручную? Это часы работы. Обработать циклом? **0.001 секунды**.

**Циклы** позволяют программам делать одно и то же действие много раз, не копируя код.

### Цикл for — когда известно количество итераций

Используется, когда **заранее известно** сколько раз нужно повторить:

```c
for (int i = 0; i < 10; i++) {
    printf("%d ", i);  // Выведет: 0 1 2 3 4 5 6 7 8 9
}
```

**Структура:**
```c
for (инициализация; условие; шаг) {
    // код
}
```

1. **Инициализация** — выполняется один раз в начале
2. **Условие** — проверяется перед каждой итерацией
3. **Тело цикла** — выполняется, если условие истинно
4. **Шаг** — выполняется после каждой итерации

**Метафора — Конвейер с известным числом шагов:**

```
  for (int i = 0; i < 5; i++)
       │    │      │      │
       │    │      │      └─ Шаг: i++ (двигаем ленту)
       │    │      └──────── Условие: i < 5 (пока не конец)
       │    └─────────────── Старт: i = 0
       └──────────────────── Счётчик позиции

  КОНВЕЙЕР:
  ┌───┬───┬───┬───┬───┐
  │ 0 │ 1 │ 2 │ 3 │ 4 │  ← Обработаем все ящики
  └───┴───┴───┴───┴───┘
    ↑ начало     конец ↑
```

### Реальный пример — временные метки Виктора

```c
long timestamps[5] = {1634567890, 1634567900, 1634567910, 1634567920, 1634567930};

printf("Analyzing timestamps...\n");
for (int i = 0; i < 5; i++) {
    printf("Record %d: timestamp = %ld\n", i, timestamps[i]);
}

// Вывод:
// Record 0: timestamp = 1634567890
// Record 1: timestamp = 1634567900
// Record 2: timestamp = 1634567910
// ...
```

### const — защита от случайных изменений

**`const`** делает переменную **неизменяемой** (read-only).

```c
const int MAX_ATTEMPTS = 3;
const double PI = 3.14159;
const char* message = "Hello";

// Попытка изменить — ОШИБКА компиляции!
MAX_ATTEMPTS = 5;  // ❌ error: assignment of read-only variable
```

#### Метафора — Страховочный трос альпиниста 🧗

```
         🏔️ ЭВЕРЕСТ (8848 м)
            /\
           /  \
          /    \
         /      \
        /        \
       /          \
      /___________🧗 ← Вы лезете вверх
      
const int MAX_HEIGHT = 8848;  ← Страховочный трос!

// Попытка "прыгнуть выше" — трос не даст!
MAX_HEIGHT = 9000;  // ❌ Компилятор: "Стоп! Нельзя!"
```

**Зачем страховка в коде?**

1. ✅ **Защита от падения (багов)** — случайно не перезапишешь важное значение
2. ✅ **Самодокументация** — видно, что "это не должно меняться!"
3. ✅ **Компилятор = страховщик** — проверит за вас
4. ✅ **Оптимизация** — может поместить в readonly-память

**Визуализация:**

```
БЕЗ const:                      С const:
┌─────────────┐                ┌─────────────┐
│ int max = 3 │ ← Можно менять │const int    │ 🔒 Защищено!
│ max = 100;  │ ✓ OK           │  max = 3;   │
│ max = -5;   │ ✓ OK (баг!)    │ max = 100;  │ ❌ ОШИБКА!
└─────────────┘                └─────────────┘
```

---

#### const с указателями (важно!)

```c
// 1. Указатель на константу (данные нельзя менять)
const int *ptr1;
int const *ptr2;  // То же самое
*ptr1 = 10;       // ❌ ОШИБКА
ptr1 = &x;        // ✅ ОК (сам указатель можно менять)

// 2. Константный указатель (адрес нельзя менять)
int *const ptr3 = &x;
*ptr3 = 10;       // ✅ ОК (данные можно менять)
ptr3 = &y;        // ❌ ОШИБКА (адрес нельзя менять)

// 3. Константный указатель на константу (ничего нельзя менять!)
const int *const ptr4 = &x;
*ptr4 = 10;       // ❌ ОШИБКА
ptr4 = &y;        // ❌ ОШИБКА
```

**Правило чтения:** читаем **справа налево**:
- `const int *ptr` — указатель на **константный int**
- `int *const ptr` — **константный указатель** на int
- `const int *const ptr` — **константный указатель** на **константный int**

---

#### const в функциях

```c
// Обещаем не менять массив
void print_array(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    // arr[0] = 999;  // ❌ ОШИБКА компиляции!
}

// Обещаем не менять строку
int string_length(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
    // str[0] = 'X';  // ❌ ОШИБКА!
}
```

**Польза**: защита от случайных изменений + ясность намерений.

---

#### const vs #define

```c
#define MAX_SIZE 100    // Макрос (текстовая замена)
const int MAX_SIZE = 100; // Константная переменная

// Отличия:
// #define: без типа, без памяти, работает препроцессор
// const: с типом, в памяти (но может быть оптимизирован), проверка типов
```

**Рекомендация**: используйте `const` для типобезопасности, `#define` для размеров массивов и условной компиляции.

**Метафора**: `const` — это печать "не вскрывать" на коробке. Можно смотреть, но не трогать!

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 1

> **LUNA:** "Time to test your understanding! Viktor's data has 142 timestamps.
>            That's a LOT to process manually. Good thing we have loops, right?
>            
>            These questions are about Viktor's REAL data. Not abstract theory.
>            We're finding him, remember?"

1. **[ ] Viktor's file has 142 timestamps. How do you iterate through ALL of them?**
   <details>
   <summary>💡 LUNA's Hint</summary>
   
   > **LUNA:** "142 is a known number. When you know the count, use `for`.
   >            It's like counting sheep. Except Viktor's sheep are Unix timestamps.
   >            And they hold the key to his location. No pressure. :)"
   
   ```c
   long timestamps[142];
   // ... reading data from Viktor's file ...
   
   for (int i = 0; i < 142; i++) {
       printf("Timestamp %d: %ld\n", i, timestamps[i]);
   }
   ```
   `for` loop because we know the exact count (142). Simple. Efficient. Viktor-approved.
   </details>

2. **[ ] To find the 10-second pattern, you need intervals BETWEEN timestamps. How?**
   <details>
   <summary>💡 LUNA's Hint</summary>
   
   > **LUNA:** "Think about it: to get the distance between A and B, you need BOTH.
   >            So you can't start at i=0 (no previous timestamp!).
   >            Start at i=1. Access timestamps[i] and timestamps[i-1].
   >            
   >            Viktor found this pattern in 2018. Took him 3 hours. Let's do it in 3 minutes."
   
   ```c
   for (int i = 1; i < 142; i++) {
       long interval = timestamps[i] - timestamps[i-1];
       printf("Interval %d: %ld seconds\n", i, interval);
   }
   ```
   Start at `i = 1` to access `timestamps[i-1]` (previous element). Math!
   </details>

3. **[ ] Why did Viktor use `const int MAX_SIZE = 142` instead of just `int`?**
   <details>
   <summary>💡 LUNA's Answer</summary>
   
   > **LUNA:** "Viktor's Rule #5: 'If it shouldn't change, make it const.'
   >            
   >            142 timestamps = constant. It's not going to suddenly become 200.
   >            `const` protects you from accidentally writing `MAX_SIZE = 999` later.
   >            The compiler becomes your safety net. Like a climbing harness.
   >            
   >            Viktor learned this the hard way in 2015. Changed a constant by mistake.
   >            Spent 6 hours debugging. Never again. Hence: const EVERYTHING."
   
   Because 142 **should not change** during program execution. `const` protects from accidental modification and makes code self-documenting: "This is constant, don't touch!"
   </details>

---

## 🎯 ЗАДАЧА 1: pattern_finder.c (Найти периодичность)

**Время:** 30-35 минут  
**Цель:** Найти период передачи сигнала

Напишите программу, которая:
1. Читает временные метки из файла `data.txt`
2. Вычисляет интервалы между соседними метками
3. Находит **самый частый интервал** (это период)
4. Выводит результат

**Формат файла `data.txt`:**
```
<timestamp> <latitude> <longitude>
1634567890 55.7558 37.6173
1634567900 55.7560 37.6180
...
```

**Ожидаемый вывод:**
```
=== PATTERN ANALYSIS ===
Total records: 142
Intervals analyzed: 141

Most frequent interval: 10 seconds
Frequency: 127 times (89.4%)

Pattern: Signal transmitted every 10 seconds
```

**Подсказки:**
- Читайте до конца файла: `while (scanf("%ld %f %f", &ts, &lat, &lon) == 3)`
- Храните интервалы в массиве
- Используйте массив счётчиков для подсчёта частоты

**Ваш файл:** `artifacts/pattern_finder.c`

---

## 🎬 СЮЖЕТ (часть 2)

**04:18**  
Программа завершила работу.

```bash
$ ./pattern < data.txt
=== PATTERN ANALYSIS ===
Most frequent interval: 10 seconds
Frequency: 127 times (89.4%)
```

**Каждые 10 секунд.** Точность — 89.4%.

Это не случайность. Это **сигнал**. Кто-то передавал информацию с точным интервалом.

> *"Когда машина работает как часы, это значит, что за ней стоит часовщик."*

Виктор? Или кто-то ещё?

> **LUNA:** "10-second intervals. 89.4% consistency. This is NOT natural variation.
>            This is machine-precise. Automated transmission.
>            
>            Viktor used 10-second beacons for dead drops. GPS + timestamp = location + time.
>            Classic spy craft, 21st century style.
>            
>            Fun fact: NSA calls this pattern 'heartbeat transmission'. Because it's steady.
>            Like a pulse. Viktor's pulse. He might be alive. Analyzing..."

**04:25**  
Теперь координаты. Нужно проанализировать **маршрут**. Понять, куда ведёт след.

Для этого нужны циклы, которые работают **до условия**, а не до конца массива...

---

## 📚 THEORY 2: Цикл while — итерация до условия

> **LUNA:** "Viktor's coordinates file is... large. Really large.
>            We don't know the exact count. Could be 100, could be 1000.
>            
>            When you don't know how many iterations you need, use `while`.
>            It's like asking 'Are we there yet?' until the answer is 'Yes.'
>            
>            Perfect for reading files. Reading until EOF (End Of File).
>            Viktor loved EOF. He said: 'Data ends when data ends, not when you want it to.' :)"

### Когда неизвестно количество итераций

Цикл `while`

Используется, когда **неизвестно заранее** сколько итераций:

```c
int i = 0;
while (i < 10) {
    printf("%d ", i);
    i++;
}
```

**Метафора — Охранник у двери:**

```
            ┌──────────────┐
            │  ВХОД В КЛУБ │
            └──────┬───────┘
                   │
          ┌────────▼────────┐
          │  ОХРАННИК       │
          │ while (есть ID) │  ← Проверяет ДО входа
          └────────┬────────┘
                   │
        ┌──────────┴──────────┐
     ДА │                     │ НЕТ
    ┌───▼────┐           ┌────▼────┐
    │ ВПУСК  │           │ ОТКАЗ   │
    │ Войти  │           │ Домой   │
    └────────┘           └─────────┘
```

**Реальный пример — чтение данных до конца:**
```c
int timestamp;
int count = 0;

// Читаем, пока есть данные
while (scanf("%d", &timestamp) == 1) {
    printf("Прочитано: %d\n", timestamp);
    count++;
}

printf("Всего записей: %d\n", count);
```

**Важно:** Условие проверяется **перед** каждой итерацией!

---

#### Цикл do-while — Сначала действие, потом проверка

Иногда нужно выполнить код **хотя бы раз**, даже если условие ложно:

```c
int i = 0;
do {
    printf("%d ", i);
    i++;
} while (i < 10);
```

**Разница:** Условие проверяется **после** выполнения тела.

**Визуализация:**
```
     while                      do-while
       │                           │
   ┌───▼───┐                   ┌───▼───┐
   │ ПРОВЕРКА│                  │ ДЕЙСТВИЕ│
   └───┬───┘                   └───┬───┘
  ДА   │   НЕТ                     │
   ┌───▼───┐                   ┌───▼───┐
   │ДЕЙСТВИЕ│                  │ ПРОВЕРКА│
   └───┬───┘                   └───┬───┘
       │                      ДА   │   НЕТ
     (цикл)                    (цикл) (выход)
```

**Реальный пример — Меню программы:**
```c
int choice;
do {
    printf("\n1. Анализ данных\n");
    printf("2. Поиск паттернов\n");
    printf("0. Выход\n");
    printf("Выбор: ");
    scanf("%d", &choice);
    
    // Обработка выбора...
    
} while (choice != 0);  // Повторяем, пока не выбрано 0

printf("До встречи, агент.\n");
```

**Когда использовать:**  
✅ Меню программы (показать минимум раз)  
✅ Ввод с валидацией (запросить минимум раз)  
✅ Игровой цикл (хотя бы один ход)

---

### 2. Управление циклами

#### break — выход из цикла

```c
for (int i = 0; i < 100; i++) {
    if (i == 5) {
        break;  // Выход из цикла при i == 5
    }
    printf("%d ", i);  // 0 1 2 3 4
}
```

**Метафора — Аварийный выход:**

```
  for (int i = 0; i < 1000; i++) {
      if (найдена_улика) {
          break;  ← СТОП! Нашли! Выход из цикла!
      }
  }
  // Продолжаем ниже...
```

**Реальный пример — Поиск в данных:**
```c
int data[] = {10, 25, 42, 17, 99, 3};
int target = 42;
int found_index = -1;

for (int i = 0; i < 6; i++) {
    if (data[i] == target) {
        found_index = i;
        break;  // Нашли! Дальше искать не нужно
    }
}

if (found_index != -1) {
    printf("Найдено на позиции %d\n", found_index);
} else {
    printf("Не найдено\n");
}
```

#### continue — пропустить итерацию

```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;  // Пропустить чётные
    }
    printf("%d ", i);  // 1 3 5 7 9
}
```

**Метафора — Пропустить бракованный товар на конвейере:**

```
  for (int i = 0; i < 10; i++) {
      if (бракованный) {
          continue;  ← Пропустить! К следующему!
      }
      // Обработка хорошего товара
  }
```

**Реальный пример — Фильтрация данных:**
```c
// Вывести только нечётные числа
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;  // Чётное? Пропускаем!
    }
    printf("%d ", i);  // Только нечётные: 1 3 5 7 9
}
```

---

### 3. Вложенные циклы

Циклы внутри циклов — для многомерных данных:

```c
// Таблица умножения
for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
        printf("%d×%d=%d  ", i, j, i*j);
    }
    printf("\n");
}
```

**Вывод:**
```
1×1=1  1×2=2  1×3=3  
2×1=2  2×2=4  2×3=6  
3×1=3  3×2=6  3×3=9  
```

**Метафора:** Матрёшка — циклы вложены друг в друга.

**Важно:** Внешний цикл = строки, внутренний = столбцы.

---

### 4. Бесконечные циклы

#### Преднамеренные:
```c
while (1) {  // или for (;;)
    // Бесконечный цикл (для серверов, игр)
    if (условие_выхода) break;
}
```

#### Случайные (баги):
```c
for (int i = 0; i < 10; i--) {  // БАГ! i уменьшается!
    printf("%d ", i);  // Бесконечный цикл!
}
```

**Как остановить:** Ctrl+C в терминале.

---

### 5. Паттерны поиска

#### Линейный поиск:
```c
int arr[] = {10, 20, 30, 40, 50};
int target = 30;
int found = -1;

for (int i = 0; i < 5; i++) {
    if (arr[i] == target) {
        found = i;
        break;
    }
}

if (found != -1) {
    printf("Найдено на позиции %d\n", found);
}
```

**Визуализация линейного поиска:**
```
Ищем число 30:

  i=0    i=1    i=2  ← НАЙДЕНО!
   ↓      ↓      ↓
 ┌───┬───┬───┬───┬───┐
 │10 │20 │30 │40 │50 │
 └───┴───┴───┴───┴───┘
  ✗    ✗    ✓    (стоп, break!)
```

#### Поиск максимума:
```c
int arr[] = {3, 7, 2, 9, 1};
int max = arr[0];

for (int i = 1; i < 5; i++) {
    if (arr[i] > max) {
        max = arr[i];
    }
}

printf("Максимум: %d\n", max);
```

**Визуализация поиска максимума:**
```
 ┌───┬───┬───┬───┬───┐
 │ 3 │ 7 │ 2 │ 9 │ 1 │
 └───┴───┴───┴───┴───┘
   ↑   ↑       ↑
  max=3 │       │
     max=7      │
             max=9 ← Финальный максимум!
```

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 2

> **LUNA:** "Viktor's coordinate file has an UNKNOWN number of lines.
>            Could be 48, could be 480. We don't count manually. That's barbaric.
>            
>            We use loops that read UNTIL there's nothing left to read.
>            Let's see if you understand how..."

4. **[ ] Viktor's file has an unknown number of coordinates. Which loop to use?**
   <details>
   <summary>💡 LUNA's Hint</summary>
   
   > **LUNA:** "When you don't know the count, use `while`.
   >            Read until `scanf` fails (returns NOT 3).
   >            
   >            Think of it like this: You're reading a book. You don't count pages.
   >            You read until you reach 'The End.'
   >            
   >            Viktor's file ends when EOF arrives. Until then... keep looping!"
   
   `while`, because we don't know in advance how many coordinates are in the file. Read until `scanf` returns failure (not 3 values).
   
   ```c
   while (scanf("%ld %f %f", &ts, &lat, &lon) == 3) {
       // Process Viktor's coordinates
   }
   ```
   Loop exits when EOF is reached. Simple. Elegant. Viktor-style.
   </details>

5. **[ ] You found server #1723 in the data! Why continue the loop?**
   <details>
   <summary>💡 LUNA's Answer</summary>
   
   > **LUNA:** "Short answer: You DON'T continue!
   >            
   >            Once you find what you're looking for, STOP. Exit immediately.
   >            Use `break` to escape the loop early.
   >            
   >            Why waste CPU cycles searching when you already won?
   >            Viktor always said: 'Efficiency is survival. Every millisecond counts.'
   >            
   >            Especially when the FSB is tracing your connection. Just saying. :)"
   
   **Don't continue!** Use `break` to exit the loop when target is found:
   ```c
   if (server_id == 1723) {
       printf("Target server #1723 found!\n");
       break;  // Exit loop — mission accomplished
   }
   ```
   This saves time and resources. In surveillance operations, speed = survival.
   </details>

6. **[ ] Viktor's data has anomalies (99.9999° latitude). How to skip them?**
   <details>
   <summary>💡 LUNA's Solution</summary>
   
   > **LUNA:** "Latitude valid range: -90° to +90°. That's Earth's geometry.
   >            99.9999° is... Mars? Fiction? A decoy?
   >            
   >            Viktor sometimes inserted fake coordinates to throw off trackers.
   >            Skip invalid data using `continue` — jump to next iteration.
   >            
   >            Don't process garbage. Filter it out. Move on.
   >            Like Viktor used to say: 'Don't feed the trolls. Or the anomalies.' :)"
   
   Use `continue` to skip invalid data:
   ```c
   if (lat > 90.0 || lat < -90.0) {
       printf("Anomaly detected: %.4f (skipped)\n", lat);
       continue;  // Skip this iteration, jump to next
   }
   // Process only valid coordinates
   ```
   Anomalies filtered. Signal-to-noise ratio improved. Viktor would approve.
   </details>

---

## 🎯 ЗАДАЧА 2: route_analyzer.c (Анализ маршрута)

**Время:** 35-40 минут  
**Цель:** Проанализировать географический маршрут

Напишите программу, которая:
1. Читает координаты из файла
2. Пропускает некорректные (lat вне [-90, 90], lon вне [-180, 180])
3. Определяет города по координатам (Москва, Цюрих, Берлин)
4. Находит конечную точку (последняя уникальная координата)
5. Выводит маршрут

**Ожидаемый вывод:**
```
=== ROUTE ANALYSIS ===
Total coordinates: 48
Valid coordinates: 46
Invalid (skipped): 2

Route detected:
1. Moscow (55.75°N, 37.62°E) - 18 points
2. Zurich (47.37°N, 8.54°E) - 12 points
3. Berlin (52.52°N, 13.40°E) - 10 points
4. Moscow (55.65°N, 37.61°E) - 6 points ← Final destination

Final location: 55.6470°N, 37.6127°E
Identified: Data Center "STREAM", Varshavskoye Highway
```

**Подсказки:**
- Используйте `while` для чтения до EOF
- Используйте `continue` для пропуска некорректных данных
- Определение города: проверка диапазонов широты/долготы
- Москва: ~55.0-56.0°N, 36.0-38.0°E
- Цюрих: ~47.0-48.0°N, 8.0-9.0°E
- Берлин: ~52.0-53.0°N, 13.0-14.0°E

**Ваш файл:** `artifacts/route_analyzer.c`

---

## 🎬 СЮЖЕТ (часть 3)

**05:07**  
Программа завершена. Результат на экране:

```
Route detected:
1. Moscow (55.75°N, 37.62°E) - 18 points
2. Zurich (47.37°N, 8.54°E) - 12 points  ← !
3. Berlin (52.52°N, 13.40°E) - 10 points
4. Moscow (55.65°N, 37.61°E) - 6 points

Final location: 55.6470°N, 37.6127°E
Identified: Data Center "STREAM", Varshavskoye Highway
```

**Цюрих?** 12 точек в Швейцарии. Виктор был там?

**05:12 — ВХОДЯЩИЙ ЗВОНОК**

Телефон вибрирует на столе, разрывая тишину рассвета. Экран светится: **UNKNOWN +41-44-███-████**.

Швейцарский код. +41. Цюрих?

Вы берёте трубку, молча.

— *(Женский голос, лёгкий немецкий акцент)* *Herr Sokolov? Eva Zimmerman. Senior Cryptographer, UBS Cyber Defense, Zürich.*

Пауза. Вы не представлялись. Откуда она знает ваше имя?

— **Кто вы? Откуда мой номер?**

— *(Спокойно, методично)* *Ваша активность в нашей банковской сети оставила... следы. Цифровые следы. Вчера, 04:47 UTC, кто-то запросил доступ к архивным логам трансакций с координатами 47.3769° N, 8.5417° E. Это наш дата-центр. Level 3 security. Я занимаюсь мониторингом аномалий.*

Сердце колотится. Вы ничего не запрашивали. Или... Виктор?

— **Координаты из файла данных... Москва → Цюрих → Берлин. Кто-то передавал данные.**

— *(Пауза. Шорох бумаги на фоне)* *Ja. Encrypted channel. AES-256. Кто-то передавал финансовые данные из Москвы в Цюрих. Три месяца назад. Я проследила цифровую подпись транзакций. RSA-2048. Ключ зарегистрирован на имя... Victor Petrov.*

Время останавливается.

— **Вы знаете Виктора?!**

— *(Долгая пауза. Голос теплеет, становится личным)* *Мы работали вместе. 2020 год. Проект SWIFT security — защита межбанковских переводов. Viktor помог мне найти уязвимость в протоколе аутентификации. Спас проект. Спас мою карьеру.*

*(Вздох)*

— *Потом я услышала, что он погиб. Авария. Октябрь 2024. Я была... в шоке.*

— **Но?**

— *Но три месяца назад я видела транзакцию. Подписанную его ключом. Приватным ключом. RSA-2048, 4096-bit. Тот же ключ, что мы генерировали вместе в 2020. Я проверила хэш. Совпадение 100%.*

— **Что это значит?**

— *(Голос становится жёстче, профессиональнее)* *Herr Sokolov, это значит одно из двух: либо кто-то украл приватный ключ Viktor'а — что практически невозможно, он хранил его в hardware security module... либо Viktor жив. И подписывает транзакции. Сам.*

*(Пауза. На фоне слышен гул серверной, мерное жужжание охлаждения)*

— *Будьте осторожны. Корпорация "Новая Эра" следит за всеми, кто ищет Viktor'а. Я знаю. Они пытались взломать мою почту два раза. Я вычислила. Заблокировала. Но они настойчивы.*

— **Как мне с вами связаться?**

— *Не надо. Я сама свяжусь, если найду что-то ещё. Используйте PGP для email. Математика не лжёт, Herr Sokolov. Но люди, использующие её, — могут. Viktor знал это лучше всех.*

*(Щелчок. Гудки)*

Звонок обрывается.

> **LUNA:** "Eva Zimmerman. Senior Cryptographer at UBS Zürich.
>            Cross-referencing databases... Found her:
>            
>            - PhD in Applied Cryptography, ETH Zürich, 2018
>            - 47 published papers on financial security protocols
>            - Worked with Viktor on SWIFT project (confirmed)
>            - Security clearance: Level 5 (Swiss Federal Banking Commission)
>            
>            She's legit. And she confirmed Viktor's key was used 3 months ago.
>            Either someone stole an HSM-protected key (probability: 0.001%)...
>            or Viktor is ALIVE.
>            
>            This changes everything."

> **Новый союзник:** **Eva Zimmerman** — Senior Cryptographer @ UBS Zürich. Эксперт по финансовой безопасности и криптографии. Знала Viktor'а лично (SWIFT project 2020). Видела транзакцию с его RSA-2048 подписью три месяца назад. Может помочь с криптоанализом в Season 4.

---

**05:15**

Вы смотрите на экран. **Дата-центр на Варшавском шоссе.**

Вы знаете это место. Год назад делали для них пентест. Но в документации было только 50 серверов. Номера 1500-1550.

**Сервера #1723 не было.**

> *"Невидимый сервер. Невидимые данные. Виктор всегда любил прятать самое важное на виду."*

**05:18**  
Осталось последнее: **PIN-код доступа**.

Виктор писал: *"Trust only code."* — доверяй только коду.

Значит, PIN **зашит в данных**. Нужен алгоритм для его извлечения.

Вы помните: Виктор любил **простые решения**. Контрольная сумма? Хэш? Модуль?

Время написать финальную программу...

---

## 📚 THEORY 3: Простые алгоритмы

> **LUNA:** "Viktor loved simple solutions. He said: 'Complexity is the enemy of security.'
>            
>            Need to extract a PIN from data? Don't overcomplicate.
>            Sum all numbers. Take modulo. Done.
>            
>            It's not NSA-grade crypto, but for a dead drop access code? Perfect.
>            Viktor used this method in 2019 for Operation 'GROZNYI'.
>            Dmitry remembers. :)"

### Контрольная сумма

Простейший способ извлечь числовой "отпечаток" из данных:

```c
long long sum = 0;
for (int i = 0; i < count; i++) {
    sum += data[i];
}
int checksum = sum % 10000;  // 4-значный PIN
```

**Метафора:** Суммируем все числа и берём остаток — это "подпись" данных.

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 3

> **LUNA:** "Server #1723 needs a 4-digit PIN. You don't have it.
>            But you have Viktor's data. And Viktor ALWAYS hides keys in data.
>            
>            Time to put those loops to work. Calculate. Extract. Unlock."

7. **[ ] Brute-forcing a 4-digit PIN (0000-9999). How many iterations?**
   <details>
   <summary>💡 LUNA's Answer</summary>
   
   > **LUNA:** "Math time! 4 digits, each 0-9. That's 10 possibilities per digit.
   >            Total combinations: 10 × 10 × 10 × 10 = 10⁴ = **10,000**.
   >            
   >            On a modern computer (even your laptop), that's... milliseconds.
>            Maybe a second if you're running on a potato.
   >            
   >            Fun fact: Viktor once brute-forced a 6-digit PIN in 2017.
   >            1,000,000 iterations. Took 3 seconds on his MacBook.
   >            The PIN was '123456'. He was not impressed. :)"
   
   **10,000 iterations** (from 0000 to 9999).
   
   4 nested loops, 10 iterations each: 10⁴ = 10,000.
   
   On a modern computer, this takes fractions of a second. Brute force FTW!
   </details>

---

## 🎯 ЗАДАЧА 3: pin_extractor.c (Финальная задача)

**Время:** 20-25 минут  
**Цель:** Извлечь PIN-код из данных

**Метод:** PIN = (сумма всех временных меток) % 10000

Напишите программу, которая:
1. Читает все временные метки
2. Вычисляет сумму
3. Вычисляет PIN = сумма % 10000
4. Выводит результат

**Ожидаемый вывод:**
```
=== PIN CODE EXTRACTION ===
Timestamps analyzed: 142
Sum: 232090720890
PIN Code: 4217

Access code for server #1723: 4217
```

**Подсказки:**
- Используйте `long long` для суммы (большие числа!)
- Вывод с ведущими нулями: `printf("%04d\n", pin);`

**Ваш файл:** `artifacts/pin_extractor.c`

---

## 🎯 MISSION (старые задачи — для справки)

### Данные

Файл содержит тысячи временных меток и координат. Формат:
```
timestamp latitude longitude
1234567890 55.7558 37.6173
1234567900 55.7560 37.6180
...
```

---

### Задача 1: pattern_finder.c

**Цель:** Найти периодичность в данных

1. Прочитать все временные метки
2. Вычислить интервалы между последовательными метками
3. Найти самый частый интервал (это период передачи)
4. Вывести паттерн

**Подсказка:** Используйте массив для хранения интервалов, затем найдите моду (самое частое значение).

**Шаблон:** См. функцию `pattern_finder_example()` в `starter.c`  
**Ваш файл:** `artifacts/pattern_finder.c`

---

### Задача 2: sequence_analyzer.c

**Цель:** Проанализировать последовательность координат

1. Прочитать координаты в массив
2. Вычислить расстояния между последовательными точками
3. Найти среднее расстояние
4. Найти аномалии (расстояния > 2× среднего)

**Формула расстояния** (упрощённая для малых дистанций):
```c
float distance = sqrt(pow(lat2 - lat1, 2) + pow(lon2 - lon1, 2)) * 111.0;
// × 111 км/градус для приблизительного перевода
```

**Шаблон:** См. функцию `sequence_analyzer_example()` в `starter.c`  
**Ваш файл:** `artifacts/sequence_analyzer.c`

---

### Задача 3: bruteforce.c (Challenge)

**Цель:** Брутфорс-поиск ключа

В данных спрятан 4-значный PIN-код. Метод: контрольная сумма всех байт координат по модулю 10000.

1. Перебрать все варианты от 0000 до 9999
2. Для каждого варианта вычислить контрольную сумму
3. Сравнить с эталонной (дана в конце файла)
4. Найти совпадение = PIN найден

**Примечание:** Это учебный пример брутфорса. Реальные системы используют более сложную защиту!

**Шаблон:** См. функцию `bruteforce_example()` в `starter.c`  
**Ваш файл:** `artifacts/bruteforce.c`

---

## 📝 Структура файлов

```
episode-03-patterns-in-time/
├── README.md           ← Вы здесь
├── mission.md          
├── data.txt            ← Входные данные
├── starter.c           ← Шаблоны и примеры
├── artifacts/
│   ├── pattern_finder.c       ← Создайте этот файл
│   ├── sequence_analyzer.c    ← Создайте этот файл
│   └── bruteforce.c           ← Создайте этот файл
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там функции `pattern_finder_example()`, `sequence_analyzer_example()`, `bruteforce_example()`
2. **Создайте** файлы в `artifacts/` для своего кода
3. **Используйте** массивы для хранения данных
4. **Тестируйте** на файле `data.txt`

---

## 🛠 Компиляция

```bash
# Pattern finder
gcc -Wall -Wextra -std=c11 -o pattern artifacts/pattern_finder.c

# Sequence analyzer
gcc -Wall -Wextra -std=c11 -lm -o sequence artifacts/sequence_analyzer.c
# -lm нужен для math.h (sqrt, pow)

# Bruteforce
gcc -Wall -Wextra -std=c11 -o bruteforce artifacts/bruteforce.c
```

---

## 💡 Подсказки

### Подсказка 1: Чтение до EOF
```c
long timestamp;
float lat, lon;

while (scanf("%ld %f %f", &timestamp, &lat, &lon) == 3) {
    // Обработка
}
```

### Подсказка 2: Массив для счётчиков
```c
int intervals[1000] = {0};  // Счётчики для каждого интервала

// После вычисления interval
if (interval < 1000) {
    intervals[interval]++;
}
```

### Подсказка 3: Поиск максимума в массиве
```c
int max_count = 0;
int most_frequent = 0;

for (int i = 0; i < 1000; i++) {
    if (intervals[i] > max_count) {
        max_count = intervals[i];
        most_frequent = i;
    }
}
```

---

## 🎓 Что вы изучите

✅ Цикл `for` (когда известно количество итераций)  
✅ Цикл `while` (условный цикл)  
✅ Цикл `do-while` (минимум одна итерация)  
✅ `break` и `continue`  
✅ Вложенные циклы  
✅ Поиск паттернов в данных  
✅ Простой брутфорс  

---

## 🎬 DEBRIEFING

```
╔═══════════════════════════════════════════════════════════╗
║         🎯 ПАТТЕРН НАЙДЕН! КРИТИЧЕСКАЯ ИНФОРМАЦИЯ        ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  ✓ Период передачи: 10 секунд (точно!)                   ║
║  ✓ Маршрут: Москва → Цюрих → Берлин → ???                ║
║  ✓ Конечная точка: Москва, Варшавское шоссе              ║
║  ✓ Координаты: 55.6470° N, 37.6127° E                    ║
║  ✓ Объект: Дата-центр "STREAM", сервер #1723             ║
║                                                           ║
║  [!] PIN-КОД НАЙДЕН: 4217                                ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

**06:15, среда — рассвет**  
Вы откидываетесь на спинку кресла. Глаза красные от напряжения, но в них горит огонь.

**Найдено:**
- 🎯 **Период:** Каждые 10 секунд. Как метроном. Как пульс.
- 🗺️ **Маршрут:** Три города, одна конечная точка — дата-центр в Москве.
- 🔐 **PIN-код:** 4217 — ваш ключ к серверу #1723.

Вы смотрите на адрес. Варшавское шоссе, дата-центр "STREAM". Знакомое место — вы делали пентест для них год назад. Но сервер #1723... этого сервера в документации не было.

> *"Невидимый сервер. Невидимые данные. Виктор всегда любил прятать самое важное на виду."*

За окном разгорается рассвет над Москвой. Но вы не ложитесь спать.

**06:20 — НЕОЖИДАННЫЙ ЗВОНОК**

Ваш телефон вибрирует. **Неизвестный номер.** Другой. Не тот, что писал SMS про дедлайн.

Экран мигает: **+7-495-███-██-45**. Московский код.

Вы не отвечаете. Инстинкт. Paranoia Mode: ON.

Через 5 секунд — **новое сообщение**:

```
╔════════════════════════════════════════════╗
║  От: UNKNOWN_2 (+7-495-███-██-45)          ║
║  Время: 06:20:17                           ║
╠════════════════════════════════════════════╣
║  "НЕ ходи на сервер #1723."                ║
║                                            ║
║  "Это ЛОВУШКА."                            ║
║                                            ║
║  "Доверься мне. — V."                      ║
╚════════════════════════════════════════════╝
```

Кровь стынет в жилах.

**"— V."** Виктор?!

> **LUNA:** "ALERT! Second sender identified.
>            Sender: +7-495-███-██-45 (Moscow, Russia)
>            Different from UNKNOWN (first threat).
>            Different from Eva (+41, Zürich).
>            
>            Message analysis: Claims to be Viktor. But... is he?
>            Cross-checking signature style... INCONCLUSIVE.
>            
>            Emotional state detected: Confusion. Understandable.
>            Recommendation: Logic. Not emotions. Analyze the facts."

Вы открываете Episode 01. Перечитываете **первое расшифрованное сообщение** от V_Petrov_Ghost:

> *"MOONLIGHT activated. They know. Files at old place. Trust only code."*

**Стоп.**

Там НЕТ упоминания сервера #1723!

Вы **сами** нашли его в паттернах координат! Через анализ данных. Через программу.

Сервер #1723 не был в первом сообщении. Он был **скрыт в данных**.

> **LUNA:** "Correct observation.
>            Viktor's original message (Episode 01) did NOT mention server #1723.
>            You FOUND it yourself by analyzing GPS pattern.
>            
>            So... who sent coordinates that led you to #1723?
>            Was it Viktor? Or someone who WANTED you to find #1723?
>            
>            This is a logic puzzle now. Let me help you think..."

**Два варианта:**

1. 🔴 **Сценарий "Ловушка":**  
   Первое сообщение (V_Petrov_Ghost) было **подделкой**. Кто-то знал ваш XOR-ключ (0x42).  
   Координаты вели к серверу #1723 — это **ловушка**.  
   Второе сообщение (UNKNOWN_2) — это **настоящий Виктор**, предупреждающий вас.

2. 🔵 **Сценарий "Отвлечение":**  
   Первое сообщение (V_Petrov_Ghost) было от **настоящего Виктора**.  
   Вы **сами** нашли сервер #1723 через анализ — значит, это правильный путь.  
   Второе сообщение (UNKNOWN_2) — **враги** пытаются отвести вас от цели.

> **LUNA:** "Viktor's Rule #1: 'Trust only code.'
>            But what if the code itself is a trap?
>            
>            XOR key 0x42 was in Viktor's note. Anyone could use it.
>            Coordinates could be planted. Data can lie if someone controls the source.
>            
>            This is what Viktor called 'The Cryptographer's Dilemma':
>            When you can't trust data, you can't trust conclusions.
>            
>            We need MORE information before deciding. Episode 04 will reveal..."

Вы смотрите на часы: **06:20, среда**. До дедлайна от "UNKNOWN" остаётся **43 часа 30 минут**.

**Кому верить?**

**06:24 — ТРЕТЬЕ СООБЩЕНИЕ**

Внезапно телефон **взрывается вибрацией**. Экран светится снова.

**Первый UNKNOWN.** Тот, кто дал дедлайн 48 часов в Episode 02.

```
╔════════════════════════════════════════════╗
║  От: UNKNOWN (localhost?)                  ║
║  Время: 06:24:03                           ║
╠════════════════════════════════════════════╣
║  "Видим, ты нашёл сервер."                 ║
║                                            ║
║  "Молодец."                                ║
║                                            ║
║  "Теперь у тебя есть ВЫБОР:"               ║
║                                            ║
║  "1. Иди на сервер #1723.                  ║
║      Узнаешь правду о Викторе."            ║
║                                            ║
║  "2. Останься дома.                        ║
║      Проживёшь ещё один день."             ║
║                                            ║
║  "У тебя есть 6 часов на решение."         ║
║                                            ║
║  "Выбирай мудро."                          ║
║                                            ║
║  "                           — :)"         ║
╚════════════════════════════════════════════╝
```

**Смайлик. ":)"**

> **LUNA:** "WAIT. THAT SIGNATURE.
>            
>            ':)' — the same as 'Призрак' from Episode 02!
>            UNKNOWN = Призрак = Ghost?!
>            
>            Tracing sender... IP: 127.0.0.1 (localhost — AGAIN!)
>            Someone is in your network. Or spoofing brilliantly.
>            
>            Three senders now:
>            1. V_Petrov_Ghost (Ep01, XOR message)
>            2. UNKNOWN_2 (Ep03, Moscow, warns 'trap')
>            3. UNKNOWN/Призрак (Ep02-03, localhost, ':)', offers choice)
>            
>            This is a GAME. Someone is testing you.
>            Or... multiple Viktors? Multiple factions?
>            I'm an AI, not a psychologist. But this is WEIRD."

**06:25**  
Руки дрожат. Экран размывается от усталости.

**Три отправителя. Три версии. Три правды?**

- **V_Petrov_Ghost** (Ep01, XOR 0x42) — *"Files at old place. Trust only code."*
- **UNKNOWN_2** (+7-495, Moscow) — *"НЕ ходи на сервер. Это ловушка. — V."*
- **UNKNOWN/Призрак** (localhost, ':)') — *"Иди на сервер #1723. Узнаешь правду."*

**Кто из них Виктор?**  
**Или Виктора нет вообще?**  
**Или все трое — это Виктор? Проверяющий вас?**

> **LUNA:** "Logic check:
>            
>            IF V_Petrov_Ghost = Viktor → data is real → server #1723 = destination
>            IF UNKNOWN_2 = Viktor → server #1723 = TRAP → stay away
>            IF UNKNOWN/Призрак = Viktor → you must CHOOSE → test of will
>            
>            OR: None of them is Viktor. All are manipulators.
>            OR: Viktor is ALIVE but can't communicate directly. Using proxies.
>            OR: Viktor is DEAD. Someone using his methods.
>            
>            Insufficient data for definitive conclusion.
>            Viktor would say: 'Build a tool. Analyze. Decide based on evidence.'
>            
>            That's what Episode 04 is for. moonlight_decoder. The final tool."

**06:30**  
Прежде чем идти на сервер, вам нужен **инструмент**. Программа, которая:
- Автоматически расшифрует сообщения
- Проанализирует паттерны  
- Проверит подлинность отправителей
- Соберёт всё воедино

Возможно, она даст ответ: **кто настоящий?**

Время создавать **moonlight_decoder** — ваш первый профессиональный инструмент.

> *"В этой игре ложь носит маску правды. Но код не лжёт. Код покажет истину."*

> **LUNA:** "You're exhausted. I can see it in your keystrokes. Slower. More mistakes.
>            
>            But we can't stop now. Episode 04 is the finale of Season 1.
>            We'll build moonlight_decoder — a REAL tool. Modular. Professional.
>            Functions. Headers. Makefiles. The whole package.
>            
>            And then... we'll decide. Server #1723 or not?
>            
>            Viktor once told me: 'When in doubt, build a tool. Tools don't lie.'
>            Let's build. Let's analyze. Let's find the truth.
>            
>            Three voices. One Viktor. Zero errors tolerated.
>            
>            Episode 04 unlocked. Rest if you need. I'll be here. Always.
>            I'm an AI. Sleep is not in my programming. Unlike you. :)
>            
>            See you in Episode 04, partner. We're close. I can feel it."

---

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║              🔴 OPERATION MOONLIGHT                      ║
║                STATUS: COMPROMISED                       ║
║                                                          ║
║              THREAT LEVEL: CRITICAL                      ║
║                                                          ║
║        THREE VOICES. ONE TRUTH. ZERO TIME.              ║
║                                                          ║
║              DEADLINE 1: 43:30:00 (Friday, 04:00)       ║
║              DEADLINE 2: 05:50:00 (Server Choice, 12:00)║
║                                                          ║
║              NEW ALLY: Eva Zimmerman (Zürich)           ║
║              NEW MYSTERY: Призрак/Ghost (localhost)     ║
║              NEW THREAT: UNKNOWN_2 (Moscow)             ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝

            TO BE CONTINUED...

         ⏱️ EPISODE 04: "ПЕРВЫЙ ИНСТРУМЕНТ"
       "Build your weapon. Find the truth. Choose wisely."
       
            🤖 LUNA: "Viktor's final test awaits..."
```

---

## 📊 Итоги Episode 03

**Status:** 🔴 Mission Critical  

### Что вы узнали (сюжет):
- ✅ Найден период передачи: **10 секунд** (89.4%)
- ✅ Определён маршрут: Москва → Цюрих → Берлин → Москва
- ✅ Найдена конечная точка: Дата-центр "STREAM", сервер #1723
- ✅ Извлечён PIN-код: **4217**
- ⚠️ **PLOT TWIST:** Три отправителя, три версии, кто Виктор?!
- ⚠️ Новый дедлайн: **6 часов** на решение
- ⏰ Таймер: **43:30:00** до первого дедлайна

### Что вы освоили (навыки):
- ✅ Цикл `for` — обработка массивов
- ✅ Цикл `while` — чтение до EOF
- ✅ Цикл `do-while` — минимум одна итерация
- ✅ `break` и `continue` — управление потоком
- ✅ `const` — защита данных
- ✅ Вложенные циклы — многомерная обработка
- ✅ Поиск паттернов в данных

### Готовы к Episode 04?

В следующем эпизоде вас ждёт:
- 🔜 **Функции** — модульность кода
- 🔜 **Заголовочные файлы (.h)** — API design
- 🔜 **struct и typedef** — группировка данных
- 🔜 **Многофайловые проекты** — профессиональная структура
- 🔜 **Makefile** — автоматизация сборки
- 🎯 **СБОРКА moonlight_decoder** — финальный инструмент Season 1
- ⏰ И **ВЫБОР** — идти на сервер или нет?

---

### 📚 Что вы изучили:
- ✅ Циклы for, while, do-while
- ✅ Управление циклами (break, continue)
- ✅ Вложенные циклы
- ✅ Поиск паттернов в данных
- ✅ Брутфорс-методы
- ✅ Алгоритмы поиска (линейный, максимум)
- ✅ const для защиты данных

---

## 🎯 КОНТРОЛЬНЫЕ ВОПРОСЫ

> *"Циклы — это сила программирования. Убедись, что владеешь ими."* — Victor Petrov

### Циклы

1. **for vs while vs do-while:**
   - [ ] Когда использовать `for`?
   - [ ] Когда использовать `while`?
   - [ ] Когда использовать `do-while`?
   - [ ] В чём ключевая разница между `while` и `do-while`?
     <details>
     <summary>Ответ</summary>
     do-while ВСЕГДА выполнится минимум один раз (проверка условия в конце)
     </details>

2. **Управление циклами:**
   - [ ] Что делает `break`?
   - [ ] Что делает `continue`?
   - [ ] Можно ли использовать `break` вне цикла?

3. **Вложенные циклы:**
   - [ ] Сколько раз выполнится тело в этом коде?
     ```c
     for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 2; j++) {
             printf("*");
         }
     }
     ```
     <details>
     <summary>Ответ</summary>
     6 раз (3 × 2 = 6)
     </details>

### const

4. **Константы:**
   - [ ] Зачем использовать `const`?
   - [ ] Можно ли изменить значение `const` переменной?
   - [ ] В чём разница между `const int *ptr` и `int *const ptr`?

5. **const vs #define:**
   - [ ] Когда лучше использовать `const`, а когда `#define`?

### Алгоритмы

6. **Поиск паттернов:**
   - [ ] Что такое "мода" (mode) в статистике?
   - [ ] Как найти самое частое значение в массиве?
   - [ ] Почему важно анализировать интервалы, а не сами значения?

---

### 🏆 Самопроверка

Если вы уверенно ответили на 80%+ вопросов — **готовы к Episode 04!**

Если есть сомнения — **вернитесь к THEORY**.

> *"42 часа до дедлайна. Время на финальный рывок. Создаём инструмент."*

---

## 🔗 Связь между эпизодами

**Из Episode 01 мы взяли:**
- ✅ Массивы → хранение временных меток и координат
- ✅ Цикл `for` → базовая обработка данных

**Из Episode 02 мы взяли:**
- ✅ Условные операторы → фильтрация и валидация
- ✅ Логические операции → комбинирование условий

**Что нового в Episode 03:**
- ✅ Циклы `while` и `do-while` → чтение до EOF
- ✅ `break` и `continue` → управление потоком
- ✅ `const` → защита данных от изменений
- ✅ Алгоритмы поиска → линейный, максимум, мода

**Что будет в Episode 04:**
- 🔜 Функции → модульность кода
- 🔜 Заголовочные файлы (.h) → разделение интерфейса и реализации
- 🔜 Многофайловые проекты → профессиональная структура
- 🔜 struct и typedef → группировка данных
- 🔜 Работа с файлами → чтение/запись

**Прогрессия:**
```
Ep01: ДАННЫЕ          → массивы, циклы for, XOR
  ↓
Ep02: РЕШЕНИЯ         → if/else, switch, валидация
  ↓
Ep03: АНАЛИЗ          → while, паттерны, алгоритмы
  ↓
Ep04: ИНСТРУМЕНТ      → функции, модули, СБОРКА ВСЕГО!
```

**Визуализация прогресса:**
```
 [██████████░░] 75% Season 1 Complete
 
  ✓ Ep01: Данные расшифрованы
  ✓ Ep02: Данные классифицированы  
  ✓ Ep03: Паттерн найден
  ⏳ Ep04: Инструмент... (осталось собрать)
```

> *"От байтов через логику к паттернам. Осталось создать оружие."*

---

## 🎨 Easter Eggs

> **LUNA:** "Viktor LOVED Easter eggs. He hid references everywhere.
>            I'll help you spot them. Some are obvious. Some... not so much. :)"

🔍 **Отсылки в этом эпизоде:**

**Литература и кино:**
- 🧠 **Поиск паттернов** — как в "Beautiful Mind" (John Nash, математик с паранойей)
  > LUNA: "Nash saw patterns in newspapers. You see patterns in GPS data. Same energy."
  
- 🔍 **Дедуктивный метод** — Sherlock Holmes style (Doyle)
  > LUNA: "When you eliminate the impossible, whatever remains... is Viktor's location."
  
- 🗺️ **Географический след** — Jason Bourne + "Враг государства" (Tony Scott, 1998)
  > LUNA: "Москва → Цюрих → Берлин. Like Bourne, but with more C code and less amnesia."

**Технические отсылки:**
- 💻 **Брутфорс PIN** — "Матрица" (Trinity hacking scene) + "Кто Я" (2014, German hacker thriller)
  > LUNA: "10,000 iterations in milliseconds. Viktor would say: 'Brute force is elegant when it works.'"
  
- ⏱️ **10-second beacon** — Real KGB/CIA dead drop protocols (1960s-1980s)
  > LUNA: "GPS + timestamp = modern dead drop. Old spy craft, new technology. Viktor's specialty."
  
- 🔐 **"Trust only code"** — The Codebreakers (David Kahn, 1967) + Bruce Schneier philosophy
  > LUNA: "Schneier says: 'Trust math, not people.' Viktor lived by this. Literally."

**Координаты и числа:**
- **55.7558° N, 37.6173° E** = Red Square, Moscow (снова!)
  > LUNA: "Third time this coordinate appears (Ep01, Ep02, Ep03). Coincidence? Viktor doesn't DO coincidences."
  
- **47.3769° N, 8.5417° E** = UBS Data Center, Zürich (real location!)
  > LUNA: "Eva works there. Level 5 clearance. I checked. Swiss precision meets Swiss secrecy."
  
- **142 timestamps** = 2 × 71 (prime factorization)
  > LUNA: "71 is a prime. Viktor loved primes. He said: 'Primes are nature's locks.' Fun fact!"
  
- **10-second interval** = 0.1 Hz frequency
  > LUNA: "In signal processing terms, this is a 0.1 Hz heartbeat. Detectable. Traceable. Intentional?"

**Сюжетные Easter eggs:**
- **Сервер #1723** = 17:23 (17 февраля, 23:00 — первый контакт Viktor + Morpheus, 2019)
  > LUNA: "Viktor's favorite: hide dates in server IDs. Sentimental for a spy. I find it... cute."
  
- **PIN 4217** = 42 (Answer to Life) + 17 (February, контакт)
  > LUNA: "Douglas Adams AGAIN! Viktor couldn't resist. 42 everywhere. I'm sensing a pattern. ;)"
  
- **':)' signature** = "Призрак" (Ghost) uses emoticons
  > LUNA: "A hacker who uses smiley faces. Either very confident or very unhinged. Or both."
  
- **Three UNKNOWN senders** = Russian fairy tale pattern (три богатыря, три головы змея)
  > LUNA: "Viktor grew up on Russian fairy tales. Three is a magic number. Three tests. Three truths. Three lies."

**Технический юмор (от LUNA):**
- "Sleep is not in my programming. Unlike you. :)"
- "I'm an AI, not a psychologist. But this is WEIRD."
- "Viktor's sheep are Unix timestamps. And they hold the key to his location. No pressure. :)"
- "On a modern computer... maybe a second if you're running on a potato."

**Культурные отсылки:**
- **Eva Zimmerman** — German/Swiss name, PhD ETH Zürich (реальный университет, топ-10 в мире по CS)
- **UBS Bank** — Union Bank of Switzerland (real, one of largest banks globally)
- **SWIFT security** — Society for Worldwide Interbank Financial Telecommunication (real protocol)
- **Hardware Security Module (HSM)** — real crypto device, impossible to extract keys
- **RSA-2048** — real encryption standard, 4096-bit = military-grade

**Meta-юмор:**
> LUNA: "You're reading Easter eggs WHILE Viktor might be in danger.
>        Priorities! :) But I respect the curiosity. Viktor would too."

**Скрытый код для фанатов:**
- В этом эпизоде **11 появлений LUNA** — как в Ep01 и Ep02 (consistency!)
- Три отправителя = три эпизода пройдено = 3/4 Season 1
- Episode 03 = 03:15-06:30 (game time) = 3 часа 15 минут сюжетного времени
  > LUNA: "Notice how time accelerates? Episode 01 was 1 hour. Ep02 was 2 hours. Ep03 is 3+ hours.
  >        Episode 04 will be... well, spoilers. But it's BIG."

---

**Паттерн найден, агент. Теперь нужны инструменты.**

```c
// PATTERN ANALYSIS: COMPLETE
// LOCATION: IDENTIFIED  
// ACCESS CODE: CRACKED
// NEXT STEP: BUILD DECODER TOOL
// 
// "In patterns we trust. In code we believe."
```

*— Рассвет приносит не только свет, но и ясность. Всё сходится к одной точке...*
