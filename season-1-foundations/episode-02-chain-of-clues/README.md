# Episode 02: "Цепочка зацепок"
## Season 1: Foundations

> *"Данные говорят, но нужно научиться их слушать..."*

---

## 🎬 BRIEFING (продолжение с Episode 01)

```
╔════════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Ваша квартира, район Арбат, Москва              ║
║  ВРЕМЯ:   00:50, среда, 1 октября                         ║
║  СТАТУС:  🔴 THREAT DETECTED - 47:10 HOURS REMAINING     ║
╚════════════════════════════════════════════════════════════╝
```

**00:50**  
После того SMS вы не можете сидеть спокойно. 

> *"Мы знаем, что ты расшифровал сообщение. У тебя есть 48 часов."*

Кто это? Как они узнали? Канал был защищён!

Адреналин. Пальцы на клавиатуре. Мозг в режиме гиперфокуса — знакомое состояние после трёх лет в полевых операциях.

Вы смотрите на часы: **00:50, среда**. Дедлайн: **00:50, пятница**. Чуть меньше **47 часов**, чтобы понять, что происходит.

> *"Нет времени на панику. Есть время на работу."* — Виктор всегда говорил это перед операциями.

**00:52**  
Сообщение Виктора. Вы читаете его снова:

> *"MOONLIGHT activated. They know. Files at old place. Trust only code."*

"Старое место" — вы внезапно вспоминаете. Это не физический адрес. Это **протокол скрытой передачи файлов**, который вы с Виктором разработали три года назад в подвале ФСБ. Dead drop в цифровом мире.

Протокол использует стеганографию, множественные прокси и шифрование слоями. Если Виктор пишет об этом, значит всё серьёзно.

**00:55**  
Вы достаёте старый ноутбук из-под кровати. Пыль. Забытый код. Запускаете скрипт.

```bash
$ python old_protocol.py --retrieve
[***] Connecting through 7 proxies...
[***] Decrypting layers... 1/3... 2/3... 3/3...
[***] File found: data.dat (142 KB)
[***] Download complete.
```

Внезапно терминал мигает. Знакомый интерфейс.

```
╔════════════════════════════════════════════════════════╗
║  🌙 LUNA v2.7 — Linguistic Understanding & Neural Aid ║
║  Status: ONLINE                                        ║
╚════════════════════════════════════════════════════════╝

> LUNA: "The 'old place' protocol. Viktor and I designed the
>        steganography layers together. 7 proxies, 3 encryption
>        stages, dead drop in digital space.
>        
>        I hope you remember the proxy sequence... :)
>        
>        File downloaded. Let's see what Viktor left for you."
```

**01:02**  
Загрузка завершена. Открываете файл. Внутри — **хаос**:

```
C 55.7558 37.6173 1634567890
T 14 30 1634567891  
I 42 0 1634567892
C -33.8688 151.2093 1634567900
T 23 47 1634567905
I 17 1 1634567910
C 52.5200 13.4050 1634567920
...
(142 KB данных)
```

Координаты. Временные метки. Идентификаторы. Сотни строк. Без порядка. Без структуры.

Это похоже на симфонию без дирижёра. Или на разбросанные пазлы детективного дела — каждая деталь важна, но картина не складывается.

```
> LUNA: "142 KB of structured chaos. Classic Viktor.
>        
>        Three types: C (Coordinates), T (Time), I (ID).
>        Hundreds of records. No apparent order.
>        
>        Viktor used to say: 'Data doesn't speak. You make it speak.'
>        
>        Your task: Classify. Validate. Find the pattern.
>        But first... you need to remember how to make DECISIONS in code."
```

> *"Данные не говорят сами. Их нужно заставить говорить."* — учил вас Виктор.

**Ваша задача:** Классифицировать данные. Найти смысл в хаосе. Понять, что пытается сказать Виктор.

Но сначала нужно вспомнить, как **принимать решения** в коде...

---

## 📚 THEORY 1: Условные операторы (if/else)

```
> LUNA: "Viktor used to say: 'Code without conditionals is like a
>        detective without questions. You just walk in circles.'
>        
>        Speaking from experience. Literally. I'm in a loop.
>        
>        Let's learn how programs make DECISIONS. Like you're about
>        to decide: trust Viktor's data, or question everything?"
```

> *"Код без условий — это дорога без развилок. Скучная и бесполезная."* — Victor Petrov

Программы должны **принимать решения** на основе условий, как детектив, анализирующий улики.

### Базовый синтаксис

```c
int temperature = 25;

if (temperature > 30) {
    printf("Жарко!\n");
} else if (temperature > 20) {
    printf("Комфортно\n");
} else {
    printf("Холодно\n");
}
```

### Метафора — Детектив на перекрёстке

```
             [ПРОВЕРКА УСЛОВИЯ]
                    |
            if (clue > 30)
                    |
        ┌───────────┴───────────┐
        │                       │
     ДА (true)              НЕТ (false)
        │                       │
   "Жарко!"                else if...
   [действие 1]               или
                           "Холодно!"
                           [действие 2]
```

Как детектив на развилке: изучаешь улику → принимаешь решение → идёшь по нужному пути.

### Операторы сравнения

```c
a == b  // равно
a != b  // не равно
a > b   // больше
a < b   // меньше
a >= b  // больше или равно
a <= b  // меньше или равно
```

⚠️ **СМЕРТЕЛЬНАЯ ошибка новичка:**
```c
// НЕПРАВИЛЬНО — присваивание вместо сравнения!
if (x = 5) {  
    printf("Всегда выполнится!\n");  // БАГ! x = 5 всегда истина
}

// ПРАВИЛЬНО — сравнение
if (x == 5) {  
    printf("Выполнится только если x равен 5\n");
}
```

**Запомните:** `=` — присваивание (засунуть в коробку), `==` — сравнение (проверить содержимое)

### Логические операторы

Комбинируйте условия:

```c
int age = 25;
int has_license = 1;  // 1 = true, 0 = false

// AND (&&) - оба условия должны быть истинны
if (age >= 18 && has_license) {
    printf("Можете водить\n");
}

// OR (||) - хотя бы одно условие истинно
if (age < 18 || !has_license) {
    printf("Нельзя водить\n");
}

// NOT (!) - инвертирует условие
if (!has_license) {
    printf("Нет прав\n");
}
```

**Таблицы истинности:**
```
AND (&&):           OR (||):          NOT (!):
0 && 0 = 0          0 || 0 = 0        !0 = 1
0 && 1 = 0          0 || 1 = 1        !1 = 0
1 && 0 = 0          1 || 0 = 1
1 && 1 = 1          1 || 1 = 1
```

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 1

```
> LUNA: "Viktor's file has data. Lots of data. Let's make sure you
>        can READ it before you ANALYZE it. Three quick questions."
```

**1. [ ] В файле Viktor'а первая запись: `C 55.7558 37.6173 1634567890`. Как проверить, что тип — 'C'?**

```
> LUNA: "Question: How do you check if the type character is 'C'?
>        
>        Hint: You need to COMPARE, not ASSIGN. Viktor made this
>        mistake once in 2017. Just once. Never again. I was there
>        to laugh at him. Well, I wasn't born yet, but his memory
>        is my memory. Weird flex."
```

<details>
<summary>💡 Решение от LUNA</summary>

```c
char type;
scanf(" %c", &type);  // Прочитать тип (пробел перед %c!)
if (type == 'C') {
    printf("Это координаты!\n");
}
```

```
> LUNA: "Correct! Notice: type == 'C' (compare), not type = 'C' (assign).
>        
>        == asks: 'Is this equal?'
>        =  says: 'Make it equal!'
>        
>        Big difference. Viktor learned it the hard way."
```
</details>

---

**2. [ ] Координата 55.7558 — это Москва. Как проверить диапазон широты для Москвы?**

```
> LUNA: "Coordinate 55.7558° appears MULTIPLE TIMES in Viktor's data.
>        That's Moscow. Red Square, specifically. Same place as Episode 01!
>        
>        Question: What's the valid latitude range to identify Moscow?
>        A) 54.0 - 56.0 (very rough)
>        B) 55.0 - 56.0 (rough)
>        C) -90.0 - 90.0 (global, not helpful)
>        
>        Hint: Depends on your validation level. Paranoid = C. Practical = B."
```

<details>
<summary>💡 Решение от LUNA</summary>

```c
float lat = 55.7558;

// Проверка: Москва? (грубо)
if (lat >= 55.0 && lat <= 56.0) {
    printf("Москва!\n");
}

// Проверка: вообще валидная широта? (планета Земля)
if (lat >= -90.0 && lat <= 90.0) {
    printf("Valid latitude\n");
}
```

```
> LUNA: "Operator && means AND. Both conditions must be TRUE.
>        
>        lat >= 55.0  AND  lat <= 56.0
>           ✓              ✓
>        Both true? Print 'Москва!'
>        
>        Viktor's Rule #1: 'Always validate globally first (±90),
>        then narrow down locally (Moscow).'"
```
</details>

---

**3. [ ] ОПАСНОСТЬ: Что произойдёт, если написать `if (type = 'C')` вместо `if (type == 'C')`?**

```
> LUNA: "This is the CLASSIC bug. The one that haunts developers
>        at 3 AM. The one Viktor debugged for 4 hours in 2018.
>        
>        Question: What happens if you write:
>        if (type = 'C')  // ОДИНАРНЫЙ =
>        
>        Hint: Think ASSIGNMENT vs COMPARISON."
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "⚠️ BUG DETECTED!
>        
>        if (type = 'C')  ← This is ASSIGNMENT, not comparison!
>        
>        What happens:
>        1. Variable 'type' gets value 'C' (assigned)
>        2. Expression 'type = 'C'' returns 'C' (67 in ASCII)
>        3. Any non-zero value = TRUE in C
>        4. Condition ALWAYS true, regardless of original type!
>        
>        CORRECT:
>        if (type == 'C')  ← Double == for comparison
>        
>        Viktor's debugging log (2018-06-14):
>        'Spent 4 hours debugging. It was a single =. FML.'
>        
>        Learn from his pain. Use ==."
```
</details>

---

## 🎯 ЗАДАЧА 1: type_classifier.c (Разминка)

**Время:** 20-25 минут  
**Цель:** Научиться различать типы записей

Напишите программу, которая:
1. Читает одну строку из файла: `<type> <val1> <val2> <timestamp>`
2. Определяет тип (`C`, `T`, или `I`)
3. Выводит человекопонятное описание

**Типы:**
- `C` — Coordinate (широта, долгота)
- `T` — Time (часы, минуты)
- `I` — ID (числовой идентификатор, unused)

**Пример входных данных:**
```
C 55.7558 37.6173 1634567890
```

**Ожидаемый вывод:**
```
Type: Coordinate
Latitude: 55.76
Longitude: 37.62
Timestamp: 1634567890
```

**Подсказки:**
- Используйте `if/else if/else` для различения типов
- Формат: `scanf(" %c %f %f %ld", &type, &val1, &val2, &timestamp);`
- **Важно:** пробел перед `%c` игнорирует whitespace!

**Ваш файл:** `artifacts/type_classifier.c`

---

## 🎬 СЮЖЕТ (часть 2)

**01:28**  
Программа работает. Вы видите структуру:

```bash
$ ./type_classifier
C 55.7558 37.6173 1634567890

Type: Coordinate
Latitude: 55.76
Longitude: 37.62
```

Это **Москва**. Красная площадь. Те же координаты, что упоминались в конце Episode 01!

```
> LUNA: "55.7558°N, 37.6173°E — Red Square, Moscow.
>        
>        Viktor mentioned these coordinates before. Twice.
>        Once in Episode 01. Now here again.
>        
>        This is NOT random. Nothing Viktor does is random.
>        Not the XOR key (0x42 = Answer to Everything).
>        Not the coordinates (Red Square = Meeting point?).
>        Not even the timestamp (1634567890 = Oct 18, 2021, 10:38 AM).
>        
>        Everything is a message. We just need to READ it."
```

Вы смотрите на экран. Координаты Москвы... Снова. Паттерн формируется.

Но в файле сотни записей. Нужна автоматизация. Нужен **switch** для классификации всех записей...

---

## 📚 THEORY 2: Switch statement — диспетчер решений

```
> LUNA: "Think of switch as a train dispatcher. One variable arrives,
>        multiple destinations. Fast, clean, efficient.
>        
>        Unlike nested if-else, which is... well, a mess.
>        
>        Viktor preferred switch for classification tasks. Three types?
>        Perfect for switch."
```

Когда вариантов много, `switch` работает как **диспетчер на вокзале**.

### Синтаксис

```c
char type = 'C';

switch (type) {
    case 'C':
        printf("Coordinate\n");
        break;
    case 'T':
        printf("Time\n");
        break;
    case 'I':
        printf("ID\n");
        break;
    default:
        printf("Unknown type\n");
}
```

**Важно:** `break` обязателен! Иначе код **провалится** в следующий case (fall-through).

### Метафора — Диспетчер на вокзале

```
        ПОЕЗД (переменная type)
               |
         [ДИСПЕТЧЕР]
               |
    ┌──────────┼──────────┐
    │          │          │
  case 'C'   case 'T'   case 'I'  ...  default
    │          │          │              │
Coordinate   Time       ID          Unknown
```

Диспетчер смотрит на тип записи и направляет её в нужный блок обработки.

### Switch vs if/else

**Когда использовать switch:**
- ✅ Много вариантов (5+)
- ✅ Проверка на равенство
- ✅ Целые типы или char

**Когда использовать if/else:**
- ✅ Диапазоны (`if (x > 10 && x < 20)`)
- ✅ Сложные условия (`if (a && b || c)`)
- ✅ Float/double значения

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 2

```
> LUNA: "Switch is powerful. But with great power... well, you know.
>        Let's make sure you don't shoot yourself in the foot."
```

**4. [ ] Что произойдёт, если забыть `break` в case?**

```
> LUNA: "The infamous fall-through bug. Viktor fell for this in 2019.
>        Classified 'Coordinates' as 'Coordinates Time ID Unknown'.
>        
>        Question: What happens if you forget 'break'?"
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "Without 'break', code FALLS THROUGH to the next case!
>        
>        Example:
>        switch (type) {
>            case 'C':
>                printf("Coordinate\n");
>                // ❌ Forgot break! Code continues...
>            case 'T':
>                printf("Time\n");
>                break;
>        }
>        
>        Input: type = 'C'
>        Output:
>        Coordinate
>        Time
>        
>        Why? No break → code falls into next case.
>        
>        Fix: ALWAYS use break (unless you know what you're doing).
>        
>        Viktor's Rule #5: 'Never trust yourself to remember break.'"
```
</details>

---

**5. [ ] Можно ли использовать switch для float значений (координат)?**

```
> LUNA: "Viktor's data has float coordinates (55.7558).
>        Can you use switch for them?
>        
>        Hint: Switch is picky about types."
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "❌ NO! Switch only works with INTEGER types.
>        
>        Allowed:
>        ✅ int, char, long, short, enum
>        
>        NOT allowed:
>        ❌ float, double, string
>        
>        Why? Switch uses jump tables. Floats are imprecise.
>        Can't hash 55.7558 reliably.
>        
>        For floats, use if/else:
>        if (lat > 55.0 && lat < 56.0) { ... }
>        
>        Viktor used switch for TYPE ('C', 'T', 'I') — char.
>        But if/else for VALUES (55.7558) — float."
```
</details>

---

**6. [ ] Зачем нужен `default` в switch? Что он делает?**

```
> LUNA: "Viktor's file could have corrupted data. Transmission errors.
>        Or... something worse. Intentional fakes.
>        
>        Question: What if type is not 'C', 'T', or 'I'?
>        What if it's 'X', '?', or garbage?"
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "default catches EVERYTHING that doesn't match a case.
>        
>        switch (type) {
>            case 'C': ...
>            case 'T': ...
>            case 'I': ...
>            default:  ← Catches 'X', '?', '\0', garbage, etc.
>                printf("Unknown type!\n");
>        }
>        
>        Why is this critical?
>        
>        Viktor's file could have:
>        - Transmission errors (bit flips)
>        - Intentional fake records (enemy sabotage)
>        - End-of-file garbage
>        
>        default = Error handling. Security. Paranoia.
>        
>        Viktor's Rule #3: 'Never trust data. Always have a default.'"
```
</details>

---

## 🎯 ЗАДАЧА 2: data_classifier.c (Основная)

**Время:** 30-35 минут  
**Цель:** Классифицировать ВСЕ записи из файла

Напишите программу, которая:
1. Читает все строки из файла `data.dat`
2. Использует **switch** для классификации типов
3. Считает количество каждого типа
4. Выводит статистику

**Формат файла:**
```
<type> <val1> <val2> <timestamp>
```

**Ожидаемый вывод:**
```
=== DATA CLASSIFICATION ===
Coordinates (C): 48
Time records (T): 34
ID records (I): 60
Unknown: 0
---
Total: 142 records
```

**Подсказки:**
- Читайте до конца файла: `while (scanf(...) == 4) { ... }`
- Используйте счётчики: `int coord_count = 0;`
- Формат: `scanf(" %c %f %f %ld", &type, &val1, &val2, &timestamp);`
- **Критически важно:** пробел перед `%c`!

**Ваш файл:** `artifacts/data_classifier.c`

**Компиляция и запуск:**
```bash
gcc -Wall -Wextra -std=c11 -o classifier artifacts/data_classifier.c
./classifier < data.dat
```

---

## 🎬 СЮЖЕТ (часть 3)

**01:55**  
Компиляция завершена. Программа анализирует данные...

```bash
$ ./classifier < data.dat
=== DATA CLASSIFICATION ===
Coordinates (C): 48
Time records (T): 34
ID records (I): 60
---
Total: 142 records
```

**48 координат.** Три города? Или больше?

Вы смотрите на экран. Данные структурированы. Но этого недостаточно.

**01:56 — ВХОДЯЩЕЕ СООБЩЕНИЕ**

Внезапно терминал вспыхивает. Новое сообщение. **Неизвестный отправитель.**

```
╔════════════════════════════════════════════════════════╗
║  📨 ENCRYPTED MESSAGE (AES-256)                       ║
║  FROM: UNKNOWN                                         ║
║  IP: 127.0.0.1 (localhost?! Кто-то в вашей сети?)     ║
╚════════════════════════════════════════════════════════╝

> "48 координат — это неплохо.
   Но 12 из них — фальшивки.
   Широта 99.99 — маркер подделки.
   Валидируй данные. Всегда.
   
   P.S. Виктор научил тебя этому. Не забывай.
   
   :)"
```

**Кто это?!**

Вы пытаетесь отследить IP. Соединение обрывается через 2 секунды. Никаких следов.

Только смайлик: **:)**

LUNA мгновенно реагирует:

```
> LUNA: "⚠️ ALERT: Unidentified sender breached our secure channel!
>        
>        IP trace: 127.0.0.1 (localhost?!)
>        Encryption: AES-256 (military grade)
>        Connection duration: 2.3 seconds
>        Trace result: FAILED
>        
>        Analysis: Someone is IN YOUR NETWORK. Or spoofing localhost.
>        Message content: Helpful (mentions Viktor, validation).
>        Signature: ':)' — playful, confident.
>        
>        Threat level: UNKNOWN
>        
>        Viktor never mentioned anyone else. No third party in the protocol.
>        This person knows:
>        - You're analyzing Viktor's data
>        - The file structure (48 coordinates)
>        - Viktor's teaching style (validation)
>        
>        Recommendation: Proceed with caution. But... they're helping.
>        For now.
>        
>        I'll keep monitoring. If they appear again, I'll try to trace.
>        
>        Friendly or enemy? I don't know. Stay sharp."
```

> **Новый контакт:** "Призрак" — анонимный хакер. Мотивы неясны. Но пока помогает. Кто он? Как он взломал ваш защищённый канал?

---

**01:58**

Слова "Призрака" эхом в голове. **Валидация данных.** Координаты 99.99 — маркер фальшивок.

Виктор всегда проверял данные дважды. Он учил вас: *"Никогда не доверяй данным. Проверяй всё."*

Нужна **валидация**. Прямо сейчас.

---

## 📚 THEORY 3: Валидация данных

```
> LUNA: "Viktor's Rule #3 (yes, he had numbered rules):
>        
>        'Never trust data. Not from friends, not from enemies,
>         not even from yourself.'
>        
>        Paranoid? Maybe. Alive? Definitely.
>        
>        That 'Ghost' person just confirmed: 12 fake records in the file.
>        Latitude 99.99 = marker. Viktor's sabotage detection.
>        
>        Let's learn VALIDATION. It might save your life. Literally."
```

> *"Данные без валидации — это мина. Рано или поздно взорвётся."* — Виктор Петров

Всегда проверяйте входные данные! В реальном мире данные могут быть:
- ❌ Некорректными (широта 999°)
- ❌ Вредоносными (SQL injection, buffer overflow)
- ❌ Случайно повреждёнными (ошибки передачи)
- ❌ Намеренно подделанными (враг подсунул ложные координаты)

### Валидация координат

```c
float latitude, longitude;
scanf("%f %f", &latitude, &longitude);

// Валидация широты: [-90, 90]
if (latitude < -90.0 || latitude > 90.0) {
    printf("ERROR: Invalid latitude %.2f\n", latitude);
    return 1;
}

// Валидация долготы: [-180, 180]
if (longitude < -180.0 || longitude > 180.0) {
    printf("ERROR: Invalid longitude %.2f\n", longitude);
    return 1;
}

printf("Valid coordinates: %.2f, %.2f\n", latitude, longitude);
```

### Валидация времени

```c
int hours, minutes;
scanf("%d %d", &hours, &minutes);

// Часы: [0, 23]
if (hours < 0 || hours > 23) {
    printf("ERROR: Invalid hours %d\n", hours);
    return 1;
}

// Минуты: [0, 59]
if (minutes < 0 || minutes > 59) {
    printf("ERROR: Invalid minutes %d\n", minutes);
    return 1;
}

printf("Valid time: %02d:%02d\n", hours, minutes);
```

### Почему это важно?

В файле Виктора могут быть:
- Ловушки от врагов (некорректные данные для провокации ошибок)
- Помехи при передаче (биты перевернулись)
- Специальные метки (координаты 99.99 = маркер?)

**Правило:** Никогда не доверяй данным. Проверяй всё.

---

## 📚 THEORY 4: scanf — чтение данных (важные детали!)

```
> LUNA: "The whitespace-before-%c trick. Viktor spent 4 hours debugging
>        this in 2018. I remember. I was there. 
>        
>        Well, I didn't exist yet, but I have his memories. Weird flex.
>        
>        Anyway. This bug is SUBTLE. It will haunt you at 3 AM.
>        Let's make sure you understand it NOW."
```

### ⚠️ КРИТИЧЕСКИ ВАЖНО: Пробел перед %c

При чтении символов (`char`) после чисел можно словить **баг**!

#### ❌ БАГ: Читает '\n' вместо символа

```c
int num;
char type;

printf("Enter number: ");
scanf("%d", &num);      // Вы вводите: 42 [Enter]
                        // scanf прочитал 42, но '\n' остался в буфере!

printf("Enter type: ");
scanf("%c", &type);     // ОШИБКА! Прочитает '\n', а не ваш символ!
```

#### ✅ РЕШЕНИЕ: Пробел перед %c

```c
int num;
char type;

printf("Enter number: ");
scanf("%d", &num);      // Вы вводите: 42 [Enter]

printf("Enter type: ");
scanf(" %c", &type);    // ✓ Пропустит '\n' и прочитает следующий символ
       ↑
    пробел!
```

### Визуализация проблемы

```
Буфер ввода:  [ 4 ][ 2 ][\n][  ]
                ↑    ↑    ↑
              scanf("%d") прочитал "42"
                          ↑
              scanf("%c") прочитает '\n' — БАГ!
              
              scanf(" %c") пропустит '\n' и будет ждать символ — ОК!
                     ↑
                  пробел игнорирует whitespace
```

### Правило для файла Виктора

```c
char type;
float val1, val2;
long timestamp;

// Правильный порядок:
while (scanf(" %c %f %f %ld", &type, &val1, &val2, &timestamp) == 4) {
               ↑
       пробел игнорирует \n, \t, пробелы!
    // Обработка...
}
```

**Возвращаемое значение scanf:**
- `scanf` возвращает **количество успешно прочитанных аргументов**
- `scanf("%d %d", &a, &b)` вернёт `2` при успехе
- Если ввод закончился (EOF), вернёт `EOF` (-1)

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 3

```
> LUNA: "Three final questions before the validator mission.
>        These are CRITICAL. Viktor's file has traps. Don't fall for them."
```

**7. [ ] Зачем нужен пробел перед %c в `scanf(" %c", &type)`?**

```
> LUNA: "The space-before-%c mystery. Viktor's 4-hour debugging session.
>        
>        Question: Why do we write scanf(' %c', &type) with a SPACE?
>                  (not scanf('%c', &type))
>        
>        Hint: What happens to the '\n' (Enter key) after you type a number?"
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "The space IGNORES all whitespace (spaces, tabs, newlines).
>        
>        Without space:
>        scanf('%d', &num);   // You type: 42 [Enter]
>        scanf('%c', &type);  // ❌ Reads '\n' instead of your character!
>        
>        With space:
>        scanf('%d', &num);   // You type: 42 [Enter]
>        scanf(' %c', &type); // ✅ Skips '\n', waits for next character!
>              ↑
>           space!
>        
>        Viktor's 2018 debugging log:
>        '23:47 — Why is type always '\n'?!'
>        '01:15 — Still debugging...'
>        '03:42 — FOUND IT. A SPACE. ONE SPACE. FML.'
>        
>        Learn from his suffering. Use the space."
```
</details>

---

**8. [ ] "Призрак" упомянул широту 99.99 как маркер подделки. Почему это некорректно?**

```
> LUNA: "The Ghost mentioned lat 99.99 as a fake marker.
>        Viktor probably planted these on purpose.
>        
>        Question: Why is latitude 99.99° impossible?
>        What's the valid range for latitude on Earth?"
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "Latitude range: -90.0 to +90.0 (South Pole to North Pole)
>        
>        99.99° is IMPOSSIBLE on Earth!
>        
>        Why?
>        - 90° = North Pole (literally the top of the planet)
>        - 99° = ... space? 900 km above North Pole?
>        
>        Viktor used impossible values as MARKERS:
>        - 99.99 = fake coordinate (sabotage detection)
>        - 25:30 time = fake timestamp
>        - -999 = error code
>        
>        Validation catches these. Always validate:
>        if (lat < -90.0 || lat > 90.0) {
>            printf('FAKE DETECTED!\n');
>        }
>        
>        Viktor's paranoia = survival."
```
</details>

---

**9. [ ] Как читать ВСЕ записи из файла Viktor'а до конца?**

```
> LUNA: "Viktor's file has 142 records. You can't read them one by one.
>        You need a LOOP that stops at EOF (End Of File).
>        
>        Question: How do you check if scanf successfully read data?
>        
>        Hint: scanf RETURNS something. What does it return?"
```

<details>
<summary>💡 Ответ от LUNA</summary>

```
> LUNA: "scanf returns the NUMBER OF SUCCESSFULLY READ ARGUMENTS.
>        
>        Example:
>        int result = scanf('%d %d', &a, &b);
>        // result = 2 (if both read)
>        // result = 1 (if only first read)
>        // result = EOF (if end of file)
>        
>        For Viktor's file:
>        while (scanf(' %c %f %f %ld', &type, &v1, &v2, &ts) == 4) {
>            // Successfully read 4 values
>            // Process record...
>        }
>        // Loop stops when scanf returns EOF (no more data)
>        
>        This is how you read ENTIRE files in C.
>        
>        Viktor used this pattern in 90% of his parsing code."
```
</details>

---

## 🎯 ЗАДАЧА 3: data_validator.c (С валидацией)

**Время:** 25-30 минут  
**Цель:** Найти некорректные записи

Напишите программу, которая:
1. Читает все записи из `data.dat`
2. **Валидирует** каждую запись по типу:
   - `C`: lat ∈ [-90, 90], lon ∈ [-180, 180]
   - `T`: hours ∈ [0, 23], minutes ∈ [0, 59]
   - `I`: любое положительное число
3. Считает корректные и некорректные записи
4. Выводит детали некорректных записей

**Ожидаемый вывод:**
```
=== DATA VALIDATION ===
Valid records: 140
Invalid records: 2

--- INVALID RECORDS ---
Line 23: C 99.9999 37.6173 1634567890 (latitude out of range)
Line 87: T 25 30 1634567905 (hours out of range)

--- STATISTICS ---
Coordinates (C): 47 valid, 1 invalid
Time (T): 33 valid, 1 invalid
ID (I): 60 valid, 0 invalid
```

**Подсказки:**
- Используйте switch для типов
- Внутри каждого case — валидация
- Считайте номер строки: `int line_number = 0;`

**Ваш файл:** `artifacts/data_validator.c`

---

## 🎬 СЮЖЕТ (часть 4)

**02:35**  
Валидация завершена.

```bash
$ ./validator < data.dat
=== DATA VALIDATION ===
Valid records: 140
Invalid records: 2
```

**Две некорректные записи!**

Вы смотрите на детали:
```
Line 23: C 99.9999 37.6173 1634567890 (latitude out of range)
Line 87: T 25 30 1634567905 (hours out of range)
```

Координата **99.9999** — это не планета Земля. Время **25:30** не существует.

Это **не ошибки передачи**. Это слишком специфично.

> *"Когда данные некорректны намеренно, это сигнал. Код в коде."* — учил Виктор.

**02:38**  
Вы смотрите на строки 23 и 87. Числа. **23 + 87 = 110**? Нет, не то.

**23 × 4 = 92**? Тоже нет.

Но вы вспоминаете: строка 23, строка 87... Это **индексы**. Может, они указывают на **другие записи**?

Нужно анализировать **связи** между данными. Нужны паттерны.

Но сейчас уже **02:40**. Вы работаете почти 2 часа без перерыва.

За окном — тишина. Слишком тихо.

---

## 🛠 Компиляция и запуск

### Компиляция:
```bash
gcc -Wall -Wextra -std=c11 -o classifier artifacts/data_classifier.c
gcc -Wall -Wextra -std=c11 -o validator artifacts/data_validator.c
```

### Запуск:
```bash
./classifier < data.dat
./validator < data.dat
```

### Автотесты:
```bash
cd tests
./test.sh
```

---

## 📝 Структура файлов

```
episode-02-chain-of-clues/
├── README.md
├── mission.md
├── data.dat              ← Входные данные от Виктора
├── starter.c             ← Шаблоны и примеры
├── artifacts/
│   ├── type_classifier.c    ← Создайте (Задача 1)
│   ├── data_classifier.c    ← Создайте (Задача 2) ⭐
│   └── data_validator.c     ← Создайте (Задача 3) ⭐
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там примеры функций
2. **Создайте** файлы в `artifacts/` для своего кода
3. **Используйте** шаблоны как подсказки
4. **Тестируйте** на файле `data.dat`

---

## 💡 Дополнительные подсказки

<details>
<summary>Hint 1: Чтение всех записей</summary>

```c
char type;
float val1, val2;
long timestamp;

while (scanf(" %c %f %f %ld", &type, &val1, &val2, &timestamp) == 4) {
    // Обработка записи
}
```
</details>

<details>
<summary>Hint 2: Switch для типов</summary>

```c
switch (type) {
    case 'C':
        // Обработка координат
        coord_count++;
        break;
    case 'T':
        // Обработка времени
        time_count++;
        break;
    case 'I':
        // Обработка ID
        id_count++;
        break;
    default:
        unknown_count++;
}
```
</details>

<details>
<summary>Hint 3: Валидация координат</summary>

```c
if (type == 'C') {
    if (val1 < -90.0 || val1 > 90.0) {
        printf("Invalid latitude: %.2f\n", val1);
        invalid_count++;
    } else if (val2 < -180.0 || val2 > 180.0) {
        printf("Invalid longitude: %.2f\n", val2);
        invalid_count++;
    } else {
        valid_count++;
    }
}
```
</details>

---

## 🎓 Что вы изучили

✅ Условные операторы (`if`, `else`, `else if`)  
✅ Switch statement для множественных вариантов  
✅ Логические операторы (`&&`, `||`, `!`)  
✅ Операторы сравнения (`==`, `!=`, `>`, `<`, `>=`, `<=`)  
✅ Валидацию входных данных  
✅ `scanf` с пробелом перед `%c` (критически важно!)  
✅ Чтение данных до EOF  
✅ Обработку некорректных данных  

---

## 🎬 CLIFFHANGER — ЧТО ЖЕ ПРОИСХОДИТ?!

**02:47, среда, 1 октября**  
Данные классифицированы. Валидация пройдена. Статистика собрана.

Вы откидываетесь на спинку кресла. Выдох.

```
╔═══════════════════════════════════════════════════════════╗
║          📊 РЕЗУЛЬТАТЫ АНАЛИЗА ДАННЫХ                     ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  ✓ Классифицировано: 142 записи                          ║
║  ✓ Координаты: 48 точек                                  ║
║  ✓ Временные метки: 34 записи                            ║
║  ✓ ID: 60 идентификаторов                                ║
║  ✓ Некорректные данные: 2 (строки 23, 87)                ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

**02:48**  
Внезапно экран мигает. Новое уведомление.

Но не от неизвестного. От **старого протокола**.

```
[***] NEW FILE DETECTED: pattern.key
[***] Downloading...
[***] Download complete (1.2 KB)
```

**02:49**  
Вы открываете файл. Внутри — одна строка:

```
03:00 WINDOW CLOSES - SECOND MESSAGE INBOUND
```

Сердце пропускает удар. **03:00?** Это через **11 минут**!

**02:50**  
Вы смотрите на часы. **02:50**. Десять минут до чего-то.

Второе сообщение. Какое второе сообщение?!

**02:52**  
Внезапно вы понимаете: строки 23 и 87 в файле — это **не ошибки**.

23 и 87... 23:00 = 11 вечера. 87... 8-7 = 15? Или 8+7 = 15:00 = 3 часа дня?

Нет. Не то.

Вы смотрите на координаты:
```
Line 23: C 99.9999 37.6173 ...
```

**99.9999** — это не широта. Это **код**. Возможно, **частота**? **99.9 MHz** — радиостанция?

**02:55**  
Пять минут до 03:00.

Телефон вибрирует. Вы вздрагиваете.

SMS от UNKNOWN:

```
╔════════════════════════════════════╗
║  От: UNKNOWN                       ║
║  Время: 02:55                      ║
╠════════════════════════════════════╣
║  "Нашли паттерны?                  ║
║                                    ║
║   Хорошо.                          ║
║                                    ║
║   Продолжай копать.                ║
║                                    ║
║   Они уже знают, где ты."          ║
╚════════════════════════════════════╝
```

**02:56**  
Адреналин. Вы встаёте. Подходите к окну.

Внизу — машина. **Тёмная Audi A6**. Тонировка. Номера не видно.

Стоит уже **10 минут**. С момента, как вы скачали файл от Виктора.

**02:57**  
Они следят. Прямо сейчас.

Вам нужно действовать. Но как?

**02:58**  
Экран снова мигает:

```
[***] INCOMING TRANSMISSION - CHANNEL #7
[***] Decrypting...
```

Канал #7 — это **экстренный канал** с Виктором. Тот, который вы использовали только дважды: в Грозном 2019 и в Петербурге 2020.

**02:59**  
Расшифровка...

**03:00**  
Экран взрывается текстом:

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║     "ПАТТЕРНЫ — ЭТО КЛЮЧ.                                ║
║      ВРЕМЯ — НЕ ВРЕМЯ.                                   ║
║      КООРДИНАТЫ — НЕ КООРДИНАТЫ.                         ║
║      ВСЁ — КОД.                                          ║
║                                                          ║
║      EPISODE 03 РАСКРОЕТ ПРАВДУ.                         ║
║                                                          ║
║      ДОВЕРЯЙ ЦИКЛАМ. ОНИ ВИДЯТ ТО, ЧТО МЫ НЕ ВИДИМ.     ║
║                                                          ║
║                                          — V."           ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

**03:01**  
Сообщение исчезает. Экран гаснет.

Вы смотрите в окно. Машина уезжает. Медленно. Демонстративно.

Они знают, что вы знаете.

> *"Паттерны — это ключ. Время — не время. Координаты — не координаты. Всё — код."*

Что это значит?!

LUNA прерывает тишину:

```
> LUNA: "Viktor's message decoded.
>        
>        'PATTERNS ARE THE KEY' = Episode 03 is about loops.
>        'TIME IS NOT TIME' = Timestamps encode something else.
>        'COORDINATES ARE NOT COORDINATES' = GPS data is a cipher.
>        'EVERYTHING IS CODE' = Nothing is what it seems.
>        
>        Translation: You need to find PATTERNS in the data.
>        You need LOOPS. While loops. For loops. Nested loops.
>        You need to ITERATE. Compare. Detect repetition.
>        
>        Viktor's teaching you algorithms through a conspiracy.
>        Classic Viktor.
>        
>        And about that dark Audi A6 downstairs... I don't have eyes,
>        but I have your webcam feed. (Kidding. Or am I?)
>        
>        They watched you for 10 minutes. From 02:50 to 03:00.
>        Exactly during Channel #7 transmission.
>        They WANTED you to know they're watching.
>        
>        Psychological warfare. Intimidation.
>        
>        Don't let them win.
>        
>        Episode 03 unlocked. Let's find those patterns.
>        Stay safe, Morpheus. Viktor would be proud of you."
```

**03:02**  
Вам нужно найти паттерны. Вам нужны **циклы**. Много циклов.

Episode 03 ждёт.

Таймер тикает: **45:48:00** до дедлайна.

---

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║              🔴 OPERATION MOONLIGHT                      ║
║                STATUS: COMPROMISED                       ║
║                                                          ║
║              DEADLINE: 45:48:00                          ║
║                                                          ║
║        "They know where you are." — UNKNOWN             ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝

            TO BE CONTINUED...

         ⏱️ EPISODE 03: "ПАТТЕРНЫ ВО ВРЕМЕНИ"
           "Loops reveal what eyes cannot see."
```

---

## 📊 Итоги Episode 02

**Status:** 🟡 Mission In Progress  

### Что вы узнали (сюжет):
- ✅ Файл из "старого места" получен (data.dat, 142 KB)
- ✅ Данные классифицированы: 48 координат, 34 времени, 60 ID
- ⚠️ Найдены некорректные записи (строки 23, 87) — это **код**!
- ⚠️ Враг следит за вами (тёмная Audi A6)
- ⚠️ Второе сообщение от Виктора: "Паттерны — ключ. Всё — код."
- ⏰ Таймер: **45:48:00** до дедлайна

### Что вы освоили (навыки):
- ✅ Условные операторы (`if`/`else`)
- ✅ Switch statement
- ✅ Логические операторы (`&&`, `||`, `!`)
- ✅ Валидацию данных
- ✅ `scanf` с пробелом перед `%c`
- ✅ Чтение данных до EOF

### Готовы к Episode 03?

В следующем эпизоде вас ждёт:
- 🔜 Циклы `while` и `do-while`
- 🔜 `break` и `continue`
- 🔜 Поиск **паттернов** в данных
- 🔜 Расшифровка "кода в коде"
- ⏰ И таймер: **45 часов**... и враги на хвосте

---

## 🔗 Связь между эпизодами

**Из Episode 01 мы взяли:**
- ✅ Массивы → применили для хранения координат
- ✅ Циклы `for` → использовали для чтения данных
- ✅ XOR → научились думать о "коде в коде"

**Что нового в Episode 02:**
- ✅ Условные операторы → принятие решений
- ✅ Switch → классификация по типам
- ✅ Валидация → проверка корректности
- ✅ scanf → чтение структурированных данных

**Что будет в Episode 03:**
- 🔜 Циклы `while`/`do-while` → итерация до условия
- 🔜 `break`/`continue` → контроль потока
- 🔜 Поиск паттернов → связь между данными
- 🔜 Алгоритмы → "видеть то, что глаза не видят"

**Прогрессия:**
```
Ep01: ОБРАБОТКА (массивы, циклы, XOR, дешифровка)
  ↓
Ep02: КЛАССИФИКАЦИЯ (if/else, switch, валидация, типы)
  ↓
Ep03: АНАЛИЗ (while, паттерны, алгоритмы, связи)
  ↓
Ep04: ИНСТРУМЕНТ (функции, модули, moonlight_decoder)
```

> *"От байтов к решениям. От решений к паттернам. Расследование углубляется... И враг всё ближе."*

---

## 🎨 Easter Eggs

```
> LUNA: "Viktor loved Easter eggs. Hidden references. Layers of meaning.
>        Let me decode this episode for you. :)"
```

🔍 **Отсылки в этом эпизоде:**

- **Строки 23 и 87** — намеренно некорректные данные (код в коде)  
  ```
  > LUNA: "23 + 87 = 110? 23 × 4 = 92? No, no... Viktor's mind doesn't
  >        work that way. These are INDEX markers. Pointers to other
  >        records. Episode 03 will reveal..."
  ```

- **99.9999° широта** — возможно, радиочастота 99.9 MHz ("Эхо Москвы" до 2022)  
  ```
  > LUNA: "99.9 MHz was 'Echo of Moscow' radio station. Shut down in 2022.
  >        Viktor's way of saying: 'Echoes of the past'? Or just a marker?"
  ```

- **Тёмная Audi A6** — стандарт FSB/FSO для слежки в России  
  ```
  > LUNA: "Audi A6. Classic FSB choice. Blend in with business elite.
  >        Tinted windows. Diplomatic plates (probably fake).
  >        They're not hiding. They're SHOWING you they're watching."
  ```

- **Канал #7** — экстренный канал (отсылка к James Bond 007)  
  ```
  > LUNA: "Channel #7. Viktor's joke. Bond. 007. Get it?
  >        Used only twice: Grozny 2019, St. Petersburg 2020.
  >        Both near-death situations. Not a good sign."
  ```

- **03:00** — "witching hour" (час ведьм), классика шпионских триллеров  
  ```
  > LUNA: "3 AM. The hour when most people die in hospitals.
  >        When CIA does extraordinary renditions.
  >        When your brain is weakest. Psychological warfare 101."
  ```

- **"pattern.key"** — файл с подсказкой (key = ключ, буквально и метафорически)

- **"They know where you are"** — паранойя агента под прикрытием

- **Координаты 55.7558, 37.6173** = Красная площадь, Москва (сквозная линия Season 1-2)  
  ```
  > LUNA: "These coordinates appear THREE times now:
  >        - Episode 01: Final cliffhanger
  >        - Episode 02: Viktor's data (multiple times)
  >        - Season 2 finale: Meeting point (spoilers!)
  >        
  >        Red Square. The heart of Russia. The ultimate meeting point.
  >        Everything leads there. Always has."
  ```

- **"Призрак" (Ghost)** — появляется впервые, смайлик ':)'  
  ```
  > LUNA: "Who is this Ghost? How did they breach our channel?
  >        Why the smiley face? Are they friend or foe?
  >        
  >        Viktor never mentioned a third party. This is... unexpected.
  >        I'll keep analyzing. Something doesn't add up."
  ```

---

## 📚 Дополнительные ресурсы

- [CS50: Week 1 - C (Conditionals)](https://cs50.harvard.edu/x/)
- [C Programming: Switch Statement](https://en.cppreference.com/w/c/language/switch)
- [Input Validation in C](https://www.tutorialspoint.com/cprogramming/c_input_output.htm)

---

**Удачи, агент! Часы тикают... И враги близко.**

```
// SYSTEM STATUS: COMPROMISED
// THREAT LEVEL: CRITICAL
// TIME REMAINING: 45:48:00
// NEXT OBJECTIVE: EPISODE 03 - FIND PATTERNS
// ENEMY: WATCHING
```
