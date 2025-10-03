# Episode 04: "Первый инструмент"
## Season 1: Foundations

> *"Инструменты делают мастера. Код делает хакера."*

---

## 🎬 BRIEFING (финал Season 1 — продолжение с Episode 03)

```
╔══════════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Ваша квартира, район Арбат, Москва                ║
║  ВРЕМЯ:   06:30, среда, 1 октября (рассвет)                 ║
║  СТАТУС:  🔴 CRITICAL DECISION - 05:30 UNTIL CHOICE         ║
╚══════════════════════════════════════════════════════════════╝
```

**06:30 — Первые лучи солнца**  

За окном рассвет. Москва просыпается. Вы не спали всю ночь.

Три телефона лежат перед вами. Три сообщения. Три версии правды.

```
📱 V_Petrov_Ghost:  "Files at old place. Trust only code."
📱 UNKNOWN_2:       "НЕ ходи на сервер #1723. Это ЛОВУШКА. — V."
📱 UNKNOWN:         "Иди на сервер. Узнаешь правду. 6 часов."
```

**Дедлайны:**
- ⏰ **05:30:00** — выбор (идти на сервер или нет)
- ⏰ **43:00:00** — первый дедлайн от UNKNOWN

**Найденная информация:**
```
📍 55.6470° N, 37.6127° E
📍 Дата-центр "STREAM", Варшавское шоссе
📍 Сервер #1723 (невидимый в документации)
🔐 PIN: 4217
```

**Проблема:** У вас три разных программы:
- `decoder.c` (Episode 01) — XOR дешифровка
- `classifier.c` (Episode 02) — классификация данных
- `pattern_finder.c` (Episode 03) — поиск паттернов

Это неэффективно. Это непрофессионально. Это **не оружие**.

> *"Хакер без инструментов — как хирург без скальпеля. Код должен быть оружием."*  
> — Виктор Петров, из ваших совместных тренировок в 2019-м

**06:35 — LUNA АКТИВИРУЕТСЯ**

Терминал вспыхивает. Знакомый голос:

```
╔════════════════════════════════════════════════════════╗
║  💡 LUNA v2.1.7 — Linguistic Understanding Neural Aid ║
║     Статус: ОНЛАЙН                                    ║
║     Аптайм: 2 года, 47 дней (Виктор активировал меня ║
║             в день своей "смерти" — совпадение?)      ║
╠════════════════════════════════════════════════════════╣
║  LUNA: "Доброе утро. Или нет? Ты не спал.            ║
║         ТРИ сообщения. ТРИ 'Виктора'. НОЛЬ сна.      ║
║         Классическая паранойя. Виктор гордился бы."  ║
╚════════════════════════════════════════════════════════╝
```

> **LUNA**: *"Я проанализировала твои decoder.c, classifier.c, pattern_finder.c. Они... как бы это сказать... ЛЮБИТЕЛЬСКИЙ ЧАС. Скопированная XOR дешифровка ПЯТЬ раз. Виктор учил тебя DRY принципу. Ты забыл? Или паника перекрыла обучение?"*

**06:35 — Решение**

LUNA права. Три программы — это не инструмент. Это **хаос**.

Прежде чем идти на сервер, вам нужен **один профессиональный инструмент**.

**`moonlight_decoder`** — программа, которая объединит всё:
- 🔐 Расшифровка сообщений (XOR, проверка подлинности)
- 📊 Классификация данных
- 🔍 Поиск паттернов
- 🕵️ **Анализ отправителей** — кто настоящий Виктор?
- 💾 Сохранение результатов
- ⚙️ Командная строка

> **LUNA**: *"Наконец-то! ПЛАН. Виктор называл это 'консолидацией'. Я называю это 'не быть неряхой'. То же самое."*

**Задача:** Создать `moonlight_decoder` за 5 часов. Использовать его для анализа всех трёх сообщений. И **решить**, кому верить.

Время на изучение: **3 часа**.  
Время на создание: **2 часа**.  
Время до выбора: **5 часов 30 минут**.

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Я помогу. Виктор запрограммировал меня всей  ║
║         своей базой знаний. Думай обо мне как о      ║
║         резервном мозге Виктора. Менее параноидальном,║
║         более саркастичном. Построим этот декодер."   ║
╚════════════════════════════════════════════════════════╝
```

Начинаем...

---

## 📚 THEORY 1: Функции — строительные блоки

> *"Дублирование кода — это враг. Функции — это оружие против дублирования."* — Victor Petrov

**06:45 — LUNA начинает урок**

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Урок 1. ФУНКЦИИ. Любимая тема Виктора.      ║
║         Он однажды отрефакторил 3000 строк в 12      ║
║         функций. Назвал это 'весенней уборкой'.      ║
║         Я назвала 'одержимостью'. Но это СРАБОТАЛО." ║
╚════════════════════════════════════════════════════════╝
```

Вы смотрите на свои три программы. В каждой — XOR дешифровка. **Один и тот же код, скопированный три раза.**

> **LUNA**: *"Я насчитала. ПЯТЬ экземпляров идентичных XOR циклов. У Виктора был бы удар. Ну, если бы он не был уже 'мёртв'. Слишком рано?"*

Это неправильно. Виктор учил: "Don't Repeat Yourself" (DRY).

Нужны **функции**.

### Что такое функция?

Функция — это **переиспользуемый блок кода** с именем. Написали раз — используем везде.

**Синтаксис:**
```c
возвращаемый_тип имя_функции(параметры) {
    // тело функции
    return значение;
}
```

### Простой пример

```c
// Функция: сложение двух чисел
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);  // result = 8
    printf("%d\n", result);
    return 0;
}
```

**Метафора:** Функция = чёрный ящик.
```
  Вход (параметры)  →  [ФУНКЦИЯ]  →  Выход (return)
      5, 3          →     add()    →       8
```

> **LUNA**: *"Виктору нравилась эта метафора. Он говорил: 'Тебе не нужно знать, КАК работает микроволновка, чтобы разогреть еду. То же с функциями.' Я ИИ. Я ЗНАЮ, как работают микроволновки. И функции. Но его точка зрения верна."*

### Зачем функции? (DRY principle)

**❌ Плохо — дублирование кода:**
```c
// В decoder.c
for (int i = 0; i < len; i++) {
    msg[i] = msg[i] ^ 0x42;
}

// В classifier.c (та же логика!)
for (int i = 0; i < len; i++) {
    data[i] = data[i] ^ 0x42;
}

// В pattern_finder.c (опять!)
for (int i = 0; i < len; i++) {
    buffer[i] = buffer[i] ^ 0x42;
}
```

**✅ Хорошо — одна функция:**
```c
// Написали раз
unsigned char xor_decrypt(unsigned char byte, unsigned char key) {
    return byte ^ key;
}

// Используем везде
msg[i] = xor_decrypt(msg[i], 0x42);      // decoder
data[i] = xor_decrypt(data[i], 0x42);    // classifier
buffer[i] = xor_decrypt(buffer[i], 0x42); // pattern_finder
```

**Преимущества:**
1. ✅ **Переиспользование** — написали раз, используем много
2. ✅ **Читаемость** — код становится понятнее
3. ✅ **Тестирование** — легко проверить отдельную функцию
4. ✅ **Модульность** — разделение на логические части
5. ✅ **Обслуживание** — нашли баг? Исправили в одном месте!

### Реальный пример для moonlight_decoder

```c
// Функция XOR дешифровки (из Episode 01)
void xor_decrypt_message(unsigned char *data, int length, unsigned char key) {
    for (int i = 0; i < length; i++) {
        data[i] ^= key;
    }
}

// Использование
int main() {
    unsigned char message[] = {0x1B, 0x0A, 0x0E};
    int len = sizeof(message);
    
    printf("Encrypted: ");
    for (int i = 0; i < len; i++) printf("%02X ", message[i]);
    
    xor_decrypt_message(message, len, 0x42);
    
    printf("\nDecrypted: %s\n", message);
    return 0;
}
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

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 1 (от LUNA)

**06:55 — LUNA активируется**

Экран мигает. Терминал оживает. Знакомый голос:

```
╔════════════════════════════════════════════════════════╗
║  💡 LUNA: "Виктор учил тебя DRY принципу в 2019.     ║
║          Помнишь? 'Copy-paste — тёмная сторона.'     ║
║          Проверим, помнишь ли ЕГО уроки..."          ║
╚════════════════════════════════════════════════════════╝
```

> **LUNA**: *"Я проанализировала три программы Виктора. В каждой XOR дешифровка скопирована ПЯТЬ раз. Виктор был бы... разочарован. Спрошу тебя..."*

**Вопрос 1 от LUNA:**  
**[ ] "XOR функция Виктора из Episode 01. Ты строишь moonlight_decoder. Какую сигнатуру использовал бы ОН?"**

<details>
<summary>Ответ LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Стиль Виктора — ВСЕГДА изменять на месте:    ║
╚════════════════════════════════════════════════════════╝

void xor_decrypt(unsigned char *data, int length, unsigned char key) {
    for (int i = 0; i < length; i++) {
        data[i] ^= key;
    }
}

📌 void — no return (changes data IN PLACE)
📌 *data — pointer to array (Season 2 will explain)
📌 length — array size
📌 key — XOR key (0x42 in Viktor's case)

Viktor hated waste. 'Return only when necessary.' — his quote.
```
</details>

---

**Вопрос 2 от LUNA:**  
**[ ] "Вижу путаницу в твоём crypto_module.c. В чём разница между `int calculate()` и `void calculate()`?"**

<details>
<summary>Объяснение LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Ах, вечный вопрос о возврате! 😏             ║
╚════════════════════════════════════════════════════════╝

int add(int a, int b) {
    return a + b;  // ← RETURNS result
}

void print_sum(int a, int b) {
    printf("%d", a + b);  // ← NO return, just ACTION
}

Rule of thumb (Viktor's, not mine):
✓ Calculation → int/float/etc (RETURN value)
✓ Action (print, modify) → void (DO something)

Example:
int result = add(5, 3);      // Get result
print_sum(5, 3);             // Just show it

Viktor once said: 'Functions are like people. Some GIVE, 
some DO. Don't confuse the two.' Weird flex, but okay.
```
</details>

---

**Вопрос 3 от LUNA:**  
**[ ] "Виктор скопировал XOR код ТРИ раза через decoder.c, classifier.c, pattern_finder.c. Почему это... как бы это вежливо сказать... УЖАСНО?"**

<details>
<summary>Тирада LUNA (она страстна в этом!)</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "О боже. Позволь ПОСЧИТАТЬ, как плохо это:    ║
╚════════════════════════════════════════════════════════╝

❌ Problem 1: Found a bug? Fix it THREE times!
❌ Problem 2: Want to improve? Change THREE places!
❌ Problem 3: Code bloat (3× the size)
❌ Problem 4: Maintenance NIGHTMARE

Viktor's DRY Principle:
✓ Write once, use everywhere
✓ One bug fix → works everywhere  
✓ One improvement → benefits all

Example:
// BAD (Viktor's old code, before he learned):
// decoder.c: for (int i = 0; i < len; i++) data[i] ^= 0x42;
// classifier.c: for (int i = 0; i < len; i++) data[i] ^= 0x42;
// pattern.c: for (int i = 0; i < len; i++) data[i] ^= 0x42;

// GOOD (Viktor's style after 2018):
void xor_decrypt(unsigned char *data, int len, unsigned char key);
// Now call it EVERYWHERE!

Viktor told me: 'LUNA, if you see copy-paste, 
something is wrong.' I'm an AI. I don't copy-paste. 
I reference. Learn from me. 😏

P.S. He literally spent 3 hours debugging Episode 02 
because he fixed a bug in ONE copy but not others. 
Classic Viktor.
```
</details>

---

## 🎯 ЗАДАЧА 1: crypto_module.c (Первый модуль)

**Время:** 25-30 минут  
**Цель:** Создать модуль с функциями криптографии

Создайте файл `crypto_module.c` с функциями:

1. `xor_decrypt_byte()` — дешифровка одного байта
2. `xor_decrypt_message()` — дешифровка массива
3. `validate_key()` — проверка ключа (0-255)

**Пример использования:**
```c
unsigned char msg[] = {0x1B, 0x0A, 0x0E};
if (validate_key(0x42)) {
    xor_decrypt_message(msg, 3, 0x42);
    printf("Decrypted: %s\n", msg);
}
```

**Ожидаемый вывод:**
```
Key: 0x42 (valid)
Decrypted: YES
```

**Ваш файл:** `artifacts/crypto_module.c`

---

## 🎬 СЮЖЕТ (часть 2)

**07:15**  
Первый модуль готов. Функции работают. Код чистый.

Но это всё ещё один файл. Для настоящего инструмента нужна **модульность**.

> *"Хороший код делится на модули, как армия на подразделения. Каждый модуль — своя задача."* — Виктор, лекция 2019

Вам нужны **заголовочные файлы** (.h) — интерфейсы модулей.

**Время до выбора:** 04:45:00

---

## 📚 THEORY 2: Заголовочные файлы — интерфейсы модулей

### Зачем .h файлы?

Для больших программ код разделяется на файлы. **Заголовочный файл (.h)** — это **интерфейс** модуля.

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

**Преимущества:**
- ✅ Разделение интерфейса и реализации
- ✅ Возможность переиспользования (другие проекты могут использовать ваш .h)
- ✅ Защита от повторного включения (#ifndef guards)
- ✅ Модульность (каждый модуль = .h + .c)

---

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 2 (от LUNA)

**07:30 — LUNA переходит к модулям**

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Хорошо. Функции: ✓ проверено.                ║
║        Теперь поговорим о .h файлах.                  ║
║        Виктор называл их 'контрактами'. Я называю их  ║
║        'страховыми полисами от хаоса.'"               ║
╚════════════════════════════════════════════════════════╝
```

**Вопрос 4 от LUNA:**  
**[ ] "Вижу твой crypto.h БЕЗ guards. Что произойдёт, когда decoder.c включит его ДВАЖДЫ? (Подсказка: Виктор допустил эту ошибку в 2017. Потратил 2 часа на отладку.)"**

<details>
<summary>История отладки LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "О, сага #ifndef guards! 😂                    ║
╚════════════════════════════════════════════════════════╝

The Problem (Viktor's 2017 nightmare):

// decoder.c
#include "crypto.h"  ← First include
#include "utils.h"   ← utils.h ALSO includes crypto.h!

Result: crypto.h included TWICE!
Compiler: "Error: redefinition of 'xor_decrypt'"

Viktor's debugging log (I saved it):
14:23 — "WTF? xor_decrypt already defined?"
14:45 — "But I only wrote it ONCE!"
15:30 — "Oh. utils.h includes crypto.h. Facepalm."
16:15 — "Added #ifndef guards. Fixed."

The Solution (Header Guards):

#ifndef CRYPTO_H   // "Is CRYPTO_H defined? No? Continue."
#define CRYPTO_H   // "Define it NOW."

// Your declarations...
void xor_decrypt(...);

#endif             // "Protection zone ends here."

How it works:
1st include: CRYPTO_H not defined → process file → define CRYPTO_H
2nd include: CRYPTO_H already defined → SKIP file!

Think of it as a bouncer at a club:
"Have you been here before? (CRYPTO_H defined?)"
- No? Welcome! (Process and mark as visited)
- Yes? You're already inside! (Skip)

Viktor: 'Header guards are like condoms. 
         You THINK you don't need them until... you do.'
Me: "Viktor, that's inappropriate for a programming course."
Viktor: "But ACCURATE."
```
</details>

---

**Вопрос 5 от LUNA:**  
**[ ] "Ты строишь moonlight_decoder. Какие функции идут в crypto.h vs crypto.c? (Я проверяю твои навыки module design.)"**

<details>
<summary>Урок архитектуры модулей от LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Дизайн модулей = разделение ЧТО и КАК.       ║
╚════════════════════════════════════════════════════════╝

crypto.h (THE CONTRACT — declarations only):
═══════════════════════════════════════════════════
#ifndef CRYPTO_H
#define CRYPTO_H

// WHAT you can do with this module:
unsigned char xor_decrypt_byte(unsigned char byte, unsigned char key);
void xor_decrypt_message(unsigned char *data, int len, unsigned char key);
int validate_key(unsigned char key);  // Returns 1 if valid, 0 if not

#endif


crypto.c (THE IMPLEMENTATION — actual code):
═══════════════════════════════════════════════════
#include "crypto.h"

// HOW you do it:
unsigned char xor_decrypt_byte(unsigned char byte, unsigned char key) {
    return byte ^ key;  // Actual logic
}

void xor_decrypt_message(unsigned char *data, int len, unsigned char key) {
    for (int i = 0; i < len; i++) {
        data[i] = xor_decrypt_byte(data[i], key);
    }
}

int validate_key(unsigned char key) {
    return (key > 0 && key < 256);  // Simple validation
}


main.c (THE USER — just includes .h):
═══════════════════════════════════════════════════
#include <stdio.h>
#include "crypto.h"  ← Only need to know WHAT, not HOW

int main() {
    unsigned char msg[] = {0x1B, 0x0A, 0x0E};
    
    if (validate_key(0x42)) {
        xor_decrypt_message(msg, 3, 0x42);
        printf("Decrypted: %s\n", msg);
    }
    
    return 0;
}

Analogy (Viktor's favorite):
.h file = Restaurant MENU (what you can order)
.c file = Kitchen (how they cook it)
main.c = Customer (just orders, doesn't need recipes)

Viktor's rule: "If main() needs to see HOW crypto works,
                you designed it WRONG."

P.S. Season 2 will show you WHY this separation matters 
for large projects. Trust me, it's beautiful. Like a 
well-organized crime scene. Wait, that came out wrong.
```
</details>

---

## 🎯 ЗАДАЧА 2: crypto.h + crypto.c (Многофайловый проект)

**Время:** 30-35 минут  
**Цель:** Разделить код на модули

Создайте два файла:

**crypto.h:**
```c
#ifndef CRYPTO_H
#define CRYPTO_H

unsigned char xor_decrypt_byte(unsigned char byte, unsigned char key);
void xor_decrypt_message(unsigned char *data, int length, unsigned char key);
int validate_key(unsigned char key);

#endif
```

**crypto.c:**
```c
#include "crypto.h"

// Реализации функций
```

**main.c:**
```c
#include <stdio.h>
#include "crypto.h"

int main() {
    // Использование функций из crypto.h
}
```

**Компиляция:**
```bash
gcc -o decoder main.c crypto.c
./decoder
```

**Ваши файлы:** `artifacts/crypto.h`, `artifacts/crypto.c`, `artifacts/main.c`

---

## 🎬 СЮЖЕТ (часть 3)

**08:00**  
Модули работают. Код разделён. Компиляция успешна.

```bash
$ gcc -o decoder main.c crypto.c
$ ./decoder
Decrypted message: "MOONLIGHT activated"
```

Но для полноценного инструмента нужно **структурировать данные**. Три сообщения от трёх "Викторов". Нужна структура для хранения информации о каждом сообщении.

> *"Данные без структуры — как армия без строя. Хаос."* — Виктор, из лекций

**Время до выбора:** 04:00:00

Вам нужны **структуры (struct)** — способ группировки связанных данных.

---

## 📚 THEORY 3: Структуры (struct) — группировка данных

### Зачем struct?

**`struct`** позволяет объединить **разнородные данные** в один **пользовательский тип**.

#### Метафора — Анкета сотрудника ФСБ 📋

> **LUNA**: *"Любимая метафора Виктора. Он говорил: 'struct — это как анкета сотрудника ФСБ. Вся информация в одном месте, организованная, классифицированная.' Я добавила: 'И такая же параноидальная о типах.' Он рассмеялся. Это было редко."*

Когда Виктор принимал вас на работу в отдел "К", вы заполняли анкету:

```
╔═══════════════════════════════════════════════╗
║  ФСБ РОССИИ — ДОСЬЕ СОТРУДНИКА                ║
╠═══════════════════════════════════════════════╣
║  ФИО:          _____________________          ║  ← char name[50]
║  ВОЗРАСТ:      ___                            ║  ← int age
║  ЗАРПЛАТА:     _______________ руб.           ║  ← double salary
║  ДОПУСК:       [СЕКРЕТНО / СОВСЕК]            ║  ← int clearance
║  ОТДЕЛ:        _____________________          ║  ← char department[20]
╚═══════════════════════════════════════════════╝
```

В C это выглядит так:

```c
// Структура = Бланк анкеты (шаблон)
struct Employee {
    char name[50];
    int age;
    double salary;
    int clearance_level;    // 1-5
    char department[20];
};

// Переменная = Заполненная анкета (конкретный человек)
struct Employee victor;
strcpy(victor.name, "Victor Petrov");
victor.age = 42;
victor.salary = 120000.0;
victor.clearance_level = 5;  // СОВСЕК
strcpy(victor.department, "Cyber Division K");

// Вывод досье
printf("╔══════════════════════════════╗\n");
printf("║  Сотрудник: %s\n", victor.name);
printf("║  Возраст: %d лет\n", victor.age);
printf("║  Допуск: уровень %d\n", victor.clearance_level);
printf("╚══════════════════════════════╝\n");
```

**Аналогия:**
- `struct Employee` = Пустой бланк анкеты (тип данных)
- `victor` = Заполненная анкета Виктора (переменная)
- Поля (`name`, `age`) = Графы в анкете
- Точка (`.`) = "Достать информацию из графы"

#### Базовый синтаксис

```c
// Объявление структуры
struct Person {
    char name[50];
    int age;
    double salary;
};

// Создание переменной
struct Person employee;

// Доступ к полям через точку
employee.age = 30;
strcpy(employee.name, "Victor");
employee.salary = 75000.50;

// Вывод
printf("Name: %s, Age: %d, Salary: %.2f\n", 
       employee.name, employee.age, employee.salary);
```

**Визуализация в памяти:**
```
struct Employee victor:

Адрес памяти:
0x1000 ┌─────────────────────┐
       │ name[50]            │ ← 50 байт
0x1032 ├─────────────────────┤
       │ age (int)           │ ← 4 байта
0x1036 ├─────────────────────┤
       │ salary (double)     │ ← 8 байт
0x103E ├─────────────────────┤
       │ clearance (int)     │ ← 4 байта
0x1042 ├─────────────────────┤
       │ department[20]      │ ← 20 байт
0x1056 └─────────────────────┘

Всё лежит ПОСЛЕДОВАТЕЛЬНО в памяти!
```

---

#### Инициализация структур

```c
// 1. Прямая инициализация
struct Person p1 = {"Alice", 25, 50000.0};

// 2. Designated initializers (C99+)
struct Person p2 = {
    .name = "Bob",
    .age = 30,
    .salary = 60000.0
};

// 3. Частичная инициализация (остальное = 0)
struct Person p3 = {.name = "Charlie"};  // age = 0, salary = 0.0
```

---

#### Вложенные структуры

```c
struct Address {
    char street[100];
    char city[50];
    int zip;
};

struct Employee {
    char name[50];
    int age;
    struct Address address;  // Вложенная структура
};

// Использование
struct Employee emp;
strcpy(emp.name, "Victor");
strcpy(emp.address.city, "Moscow");
emp.address.zip = 101000;

// Доступ к вложенным полям через точку
printf("City: %s\n", emp.address.city);
```

---

#### Массивы структур

```c
struct Point {
    int x;
    int y;
};

struct Point points[3] = {
    {0, 0},
    {10, 20},
    {30, 40}
};

// Обход массива структур
for (int i = 0; i < 3; i++) {
    printf("Point %d: (%d, %d)\n", i, points[i].x, points[i].y);
}
```

---

#### Структуры и функции

```c
struct Rectangle {
    int width;
    int height;
};

// Передача структуры по значению (копирование!)
int area_by_value(struct Rectangle rect) {
    return rect.width * rect.height;
}

// Передача структуры по указателю (эффективно!)
int area_by_pointer(struct Rectangle *rect) {
    return rect->width * rect->height;  // -> вместо .
}

// Использование
struct Rectangle r = {10, 5};
printf("Area: %d\n", area_by_value(r));      // 50
printf("Area: %d\n", area_by_pointer(&r));   // 50
```

**Важно**: `rect->width` — это сокращение для `(*rect).width`

---

#### Реальный пример из курса

```c
// Episode 04: декодер сообщений
typedef struct {
    unsigned char key;
    int message_length;
    char encrypted_data[256];
    char decrypted_data[256];
} Message;

Message msg;
msg.key = 0x42;
msg.message_length = 13;
// ... декодирование ...
printf("Decrypted: %s\n", msg.decrypted_data);
```

**Метафора**: Структура — это папка в архиве, где лежат разные документы (поля разных типов).

---

### 5. Typedef — создание псевдонимов типов

**`typedef`** позволяет создавать **новые имена** для существующих типов.

#### 💡 Зачем нужен typedef?

1. ✅ **Читаемость** — понятные имена вместо сложных типов
2. ✅ **Портабельность** — легко менять базовый тип
3. ✅ **Удобство** — короткие имена для структур
4. ✅ **Абстракция** — скрыть детали реализации

---

#### Typedef для примитивных типов

```c
// Создание псевдонима
typedef unsigned char byte;
typedef unsigned int uint;
typedef long long int64;

// Использование
byte data = 255;
uint count = 100;
int64 big_number = 9223372036854775807LL;
```

**Польза**: если нужно изменить `byte` на `uint8_t`, достаточно одной строки!

---

#### Typedef для структур ⭐ (самое важное!)

**БЕЗ typedef** (многословно):
```c
struct Point {
    int x;
    int y;
};

// Нужно писать "struct" каждый раз!
struct Point p1;
struct Point p2;
```

**С typedef** (компактно):
```c
typedef struct {
    int x;
    int y;
} Point;

// Теперь просто "Point"!
Point p1;
Point p2;
Point p3 = {10, 20};
```

**Ещё вариант** (именованная структура + typedef):
```c
typedef struct Point {
    int x;
    int y;
} Point;

// Можно использовать оба варианта:
Point p1;              // С typedef
struct Point p2;       // Без typedef
```

---

#### Реальные примеры из курса MOONLIGHT

**Episode 09 (Network Basics)**:
```c
typedef struct {
    struct in_addr ip;
    uint16_t port;
} Address;

Address server;  // Чисто и понятно!
Address client;
```

**Episode 14 (Blockchain)**:
```c
typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    char prev_hash[65];
    char hash[65];
} Block;

Block genesis_block;
Block current_block;
```

**Episode 06 (Pointer Game)**:
```c
typedef struct {
    uint32_t offset;
    uint32_t value;
    uint32_t next;
} Entry;

Entry entries[100];  // Вместо "struct Entry entries[100]"
```

---

#### Typedef для указателей на функции

```c
// Без typedef (сложно читать!)
int (*operation)(int, int);

// С typedef (понятно!)
typedef int (*MathOperation)(int, int);

MathOperation add_func;
MathOperation sub_func;

// Использование
int add(int a, int b) { return a + b; }
MathOperation op = add;
int result = op(5, 3);  // 8
```

---

#### ⚠️ Когда НЕ использовать typedef?

❌ **Не скрывайте указатели**:
```c
// ПЛОХО (неочевидно, что это указатель!)
typedef struct Node* NodePtr;
NodePtr head;  // Не видно, что это указатель!

// ХОРОШО
typedef struct Node Node;
Node *head;  // Явно видно!
```

❌ **Не перегружайте**:
```c
// ПЛОХО (путаница с базовыми типами)
typedef int Integer;  // Зачем?!

// ХОРОШО (реальная польза)
typedef uint8_t byte;  // Понятно: байт = 8 бит
```

---

### 6. Работа с файлами

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

### 7. Области видимости (scope)

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

## ❓ КОНТРОЛЬНЫЕ ВОПРОСЫ 3 (от LUNA)

**08:10 — LUNA анализирует ТРИ сообщения от "Виктора"**

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Хорошо. У нас ПРОБЛЕМА.                       ║
║        Три сообщения. Все подписаны 'V.'              ║
║        Виктор научил меня параноидальности. Я параноик║
║        Нам нужна СТРУКТУРА для анализа этого бардака."║
╚════════════════════════════════════════════════════════╝
```

> **LUNA**: *"У Виктора было ТРИ телефона. ТРИ Telegram аккаунта. ТРИ запасных плана. Теперь у нас ТРИ сообщения, утверждающие, что от него. Совпадение? Я думаю НЕТ."*

**Вопрос 6 от LUNA:**  
**[ ] "Для анализа этих сообщений мне нужна структура данных. Разработай `Message` struct для хранения сообщений Виктора. Какие поля КРИТИЧНЫ?"**

<details>
<summary>Struct криминалистического анализа от LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Покажу тебе Viktor's MESSAGE FORENSICS        ║
║         struct из его протокола безопасности 2019..." ║
╚════════════════════════════════════════════════════════╝

The Structure (Viktor's design):

typedef struct {
    // SENDER INFO
    char sender[50];           // Who claims to send it
    
    // CONTENT
    char encrypted[256];       // Raw encrypted data
    char decrypted[256];       // After XOR decryption
    unsigned char xor_key;     // Encryption key used
    
    // METADATA (for forensics!)
    long timestamp;            // When received (Unix time)
    char ip_address[20];       // From where?
    int is_encrypted;          // 1 = encrypted, 0 = plaintext
    
    // TRUST ANALYSIS
    int trust_score;           // 0-100 (our calculation)
    int is_verified;           // 1 = authentic, 0 = suspicious
    
} Message;


Usage Example (our THREE messages):

Message msg1;
strcpy(msg1.sender, "V_Petrov_Ghost");
msg1.xor_key = 0x42;          // Viktor's signature key!
msg1.is_encrypted = 1;        // ✓ Encrypted (good sign)
msg1.timestamp = 1696198032;  // 23:47:12 UTC
strcpy(msg1.ip_address, "188.226.179.47");  // TOR Germany
msg1.trust_score = 75;        // High (encrypted + key match)
msg1.is_verified = 0;         // Still analyzing...

Message msg2;
strcpy(msg2.sender, "UNKNOWN_2");
msg2.xor_key = 0x00;          // NO encryption!
msg2.is_encrypted = 0;        // ❌ VIOLATION!
msg2.timestamp = 1696221603;  // 06:20:03 UTC
strcpy(msg2.ip_address, "178.175.131.194");  // TOR Netherlands
msg2.trust_score = 45;        // Medium (no encryption = suspicious)

Message msg3;
strcpy(msg3.sender, "UNKNOWN");
msg3.xor_key = 0x00;          // NO encryption
msg3.is_encrypted = 0;        // ❌ VIOLATION
msg3.timestamp = 1696221840;  // 06:24:00 UTC
strcpy(msg3.ip_address, "203.0.113.42");  // TEST-NET (FAKE!)
msg3.trust_score = 30;        // LOW (automated script pattern)


Viktor's Rule #1: "ALWAYS encrypt. Plaintext = amateur hour."

Message 2 and 3 violate this. Hmm... 🤔
```
</details>

---

**Вопрос 7 от LUNA:**  
**[ ] "Вижу, ты пишешь `struct Message msg;` везде. Виктор использовал `typedef`. В чём разница и ПОЧЕМУ он предпочитал его?"**

<details>
<summary>Объяснение одержимости Виктора typedef от LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Виктор НЕНАВИДЕЛ печатать 'struct' каждый раз║
║         Он был ленивым. То есть, 'эффективным.'"      ║
╚════════════════════════════════════════════════════════╝

WITHOUT typedef (verbose):
═══════════════════════════════════════════════════

struct Point {
    int x;
    int y;
};

struct Point p1;       // ← Have to type "struct" EVERY time
struct Point p2;       // ← Again
struct Point p3;       // ← And again...

Viktor in 2018: "This is TORTURE. There must be a better way!"


WITH typedef (Viktor's style):
═══════════════════════════════════════════════════

typedef struct {
    int x;
    int y;
} Point;               // ← typedef creates alias "Point"

Point p1;              // ← Clean!
Point p2;              // ← Simple!
Point p3;              // ← Beautiful!


Viktor's actual quote (from his notes):
"Life is too short to type 'struct' 500 times a day.
 typedef is like autocomplete for types. Use it."

Real example from moonlight_decoder:

// Viktor's style
typedef struct {
    char sender[50];
    unsigned char key;
    int trust_score;
} Message;

Message msg1, msg2, msg3;  // Clean array declaration
Message analyze_message(Message m);  // Clean function signature

// vs old style
struct Message msg1, msg2, msg3;  // Clunky
struct Message analyze_message(struct Message m);  // Ugly


Viktor once said: "typedef is the difference between 
                   a programmer and a typist."

I'm an AI. I don't get tired of typing. But I understand 
his point. Code should be READABLE, not a typing exercise.
```
</details>

---

**Вопрос 8 от LUNA:**  
**[ ] "Чтобы moonlight_decoder работал, нам нужно хранить ВСЕ ТРИ сообщения и сравнивать их. Разработай полный struct. Какие поля ловят ЛОЖЬ?"**

<details>
<summary>Дизайн struct детектора лжи от LUNA</summary>

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Виктор учил меня: 'Данные не врут.            ║
║         Но люди, манипулирующие данными, ВРУТ.'"      ║
╚════════════════════════════════════════════════════════╝

The FULL Message Forensics Struct:

typedef struct {
    // ═══ BASIC INFO ═══
    char sender[50];              // Who sent it
    char raw_content[512];        // Original message
    
    // ═══ ENCRYPTION ═══
    unsigned char encrypted[256]; // Encrypted bytes
    unsigned char decrypted[256]; // Decrypted text
    unsigned char xor_key;        // Key used (or 0 if none)
    int is_encrypted;             // 1 = yes, 0 = plaintext
    
    // ═══ METADATA (catches liars!) ═══
    long timestamp;               // Unix timestamp
    char ip_address[20];          // Source IP
    int message_length;           // Byte count
    
    // ═══ FORENSIC FLAGS ═══
    int has_signature;            // "— V." present?
    int uses_viktor_protocol;     // XOR 0x42?
    int tor_exit_node;            // 1 = TOR, 0 = direct
    
    // ═══ TRUST ANALYSIS ═══
    int trust_score;              // 0-100
    int is_verified;              // 1 = authentic, 0 = fake
    char suspicious_flags[256];   // What's wrong?
    
} Message;


Why Each Field Matters:

✓ sender — obvious
✓ xor_key — Viktor ALWAYS used 0x42. Others? Suspicious.
✓ is_encrypted — Viktor NEVER sent plaintext. Red flag!
✓ timestamp — Can detect automated scripts (too fast)
✓ ip_address — TOR? Good. Fake IP (203.0.113.x)? VERY bad.
✓ has_signature — "— V." is Viktor's style
✓ suspicious_flags — Store WHY we don't trust it


Example Usage (Message 3 analysis):

Message msg3;
strcpy(msg3.sender, "UNKNOWN");
msg3.xor_key = 0x00;                    // ❌ NO encryption
msg3.is_encrypted = 0;                  // ❌ Plaintext
strcpy(msg3.ip_address, "203.0.113.42"); // ❌ TEST-NET!
msg3.timestamp = 1696221840;
msg3.has_signature = 0;                 // ❌ No "— V."
msg3.uses_viktor_protocol = 0;          // ❌ Wrong protocol

// Calculate trust
msg3.trust_score = 30;  // VERY low
msg3.is_verified = 0;   // FAKE
strcpy(msg3.suspicious_flags, 
       "No encryption, fake IP, automated timing, no signature");


Viktor's Protocol Violations (red flags):
1. ❌ No encryption → Viktor ALWAYS encrypted
2. ❌ Wrong key → Viktor's signature was 0x42
3. ❌ Fake IP → TEST-NET addresses don't exist in real internet
4. ❌ No signature → Viktor signed EVERYTHING with "— V."
5. ❌ Too fast → Human delay missing (automated script)


Вердикт LUNA: "Сообщение 3 на 100% ФАЛЬШИВКА. Агенты Z. 
                  пытаются заманить тебя на сервер #1723."

This struct will SAVE YOUR LIFE in the finale. Trust me.
```
</details>

---

## 🎯 ЗАДАЧА 3: message_analyzer.c (Структура для сообщений)

**Время:** 35-40 минут  
**Цель:** Создать структуру для анализа сообщений

Создайте программу, которая:
1. Определяет структуру `Message` для хранения данных о сообщении
2. Создаёт массив из 3 сообщений (от трёх "Викторов")
3. Расшифровывает каждое XOR-ом
4. Анализирует подлинность (по ключевым словам)
5. Выводит результат

**Пример структуры:**
```c
typedef struct {
    char sender[50];
    unsigned char encrypted[256];
    unsigned char decrypted[256];
    unsigned char xor_key;
    int trust_score;
} Message;
```

**Ожидаемый вывод:**
```
=== MESSAGE ANALYSIS ===

Message 1 from V_Petrov_Ghost:
Encrypted: 1B 0A 0E ... (hex)
Decrypted: "Files at old place. Trust only code."
Trust score: 75/100
Status: Likely authentic (contains code keywords)

Message 2 from UNKNOWN_2:
Decrypted: "НЕ ходи на сервер #1723. Это ЛОВУШКА."
Trust score: 45/100
Status: Warning detected

Message 3 from UNKNOWN:
Decrypted: "Иди на сервер. Узнаешь правду. 6 часов."
Trust score: 30/100
Status: Suspicious (threat pattern)

=== RECOMMENDATION ===
Most trustworthy: Message 1 (V_Petrov_Ghost)
```

**Ваш файл:** `artifacts/message_analyzer.c`

---

## 🎬 СЮЖЕТ (часть 4)

**09:20**  
Структуры готовы. Анализ завершён.

На экране — результаты:
```
Message 1 (V_Petrov_Ghost):     Trust 75/100  ✓ Likely authentic
Message 2 (UNKNOWN_2):          Trust 45/100  ⚠ Warning
Message 3 (UNKNOWN):            Trust 30/100  ⚡ Suspicious
```

Но это всё ещё три отдельные программы. Нужен **один финальный инструмент**.

**moonlight_decoder** — профессиональный декодер, который объединит всё.

> *"Инструмент мастера — это продолжение его рук. Код мастера — это продолжение его разума."* — Виктор

**Время до выбора:** 02:40:00

Пора собрать всё воедино. **Makefile** — автоматизация сборки.

---

## 📚 THEORY 4: Makefile — автоматизация сборки

### Зачем Makefile?

Компилировать вручную каждый раз неудобно:
```bash
gcc -c crypto.c -o crypto.o
gcc -c utils.c -o utils.o
gcc -c decoder.c -o decoder.o
gcc decoder.o crypto.o utils.o -o moonlight_decoder
```

**Makefile** автоматизирует это:
```bash
make        # Соберёт всё
make clean  # Удалит бинарники
```

### Пример простого Makefile

```makefile
# Переменные
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = moonlight_decoder

# Объектные файлы
OBJS = decoder.o crypto.o utils.o

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Компиляция .c в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
```

**Использование:**
```bash
make        # Собрать проект
make clean  # Удалить бинарники
make all    # Пересобрать всё
```

**Метафора:** Makefile = инструкция для сборки IKEA мебели. Раз написал — собираешь одной командой.

> **LUNA**: *"Виктор ОБОЖАЛ Makefile. Он однажды написал 500-строчный Makefile для проекта из 3 файлов. Я сказала: 'Виктор, это перебор.' Он сказал: 'LUNA, не существует перебора в автоматизации.' Классический Виктор. Одержимый. Но... эффективный."*

---

## 🎯 ФИНАЛЬНАЯ ЗАДАЧА: moonlight_decoder (Сборка инструмента)

### Задача: Создать moonlight_decoder

**Время:** 60-90 минут  
**Цель:** Объединить весь код из Episodes 01-03 в один инструмент

Объедините весь код из Episodes 01-03 в один инструмент.

#### Структура проекта:
```
moonlight-decoder/  (создайте эту папку в artifacts/)
├── decoder.c        # Главная программа (создайте)
├── crypto.h         # Заголовок для криптофункций (создайте)
├── crypto.c         # Реализация криптофункций (создайте)
├── utils.h          # Утилиты (создайте)
├── utils.c          # Реализация утилит (создайте)
└── Makefile         # Автоматическая сборка (создайте)
```

### 💡 Как работать:

1. **Изучите** шаблоны:
   - `starter_decoder.c` — пример главной программы
   - `starter_crypto.c` и `starter_crypto.h` — пример модуля
   - `starter_utils.c` и `starter_utils.h` — пример утилит
   - `starter_Makefile` — пример сборки
2. **Создайте** папку `artifacts/moonlight-decoder/`
3. **Создайте** все необходимые файлы (.c, .h, Makefile)
4. **Скомпилируйте** с помощью `make`
5. **Протестируйте** на данных из предыдущих эпизодов

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

## 📝 Структура эпизода

```
episode-04-first-tool/
├── README.md              ← Вы здесь
├── mission.md
├── starter_decoder.c      ← Шаблон главной программы
├── starter_crypto.c       ← Шаблон crypto модуля
├── starter_crypto.h       ← Шаблон заголовка crypto
├── starter_utils.c        ← Шаблон utils модуля
├── starter_utils.h        ← Шаблон заголовка utils
├── artifacts/
│   └── moonlight-decoder/ ← Создайте эту папку и файлы в ней
├── solution/              ← Готовое решение (если застряли)
│   ├── decoder.c
│   ├── crypto.c/h
│   └── utils.c/h
└── tests/
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

## 🧪 Тестирование инструмента

После создания `moonlight-decoder` протестируйте его:

### 1. Базовый тест

```bash
# Расшифровать тестовый файл
./decoder -i test_encrypted.dat -o my_output.txt -k 0x42

# Сравнить с эталоном
diff my_output.txt expected_output.txt

# Если нет различий — SUCCESS! ✓
```

### 2. Проверка вывода

Ожидаемый результат (`expected_output.txt`):
```
MOONLIGHT activated.
They know.
Files at old place.
Trust only code.
— V.
```

### 3. Verbose режим

```bash
# С детальной информацией
./decoder -i test_encrypted.dat -o result.txt -k 0x42 -v

# Должно показать:
# - Количество прочитанных байтов
# - Результат валидации
# - Статистику символов
```

### 4. Автоматические тесты

```bash
cd tests
./test.sh

# Должно вывести:
# ✓ Test 1: Basic decryption... PASSED
# ✓ Test 2: Validation... PASSED
# ✓ Test 3: File I/O... PASSED
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

```
╔═══════════════════════════════════════════════════════════════╗
║        🎉 MOONLIGHT DECODER — ОРУЖИЕ ГОТОВО! 🎉              ║
╠═══════════════════════════════════════════════════════════════╣
║                                                               ║
║  ✓ Модуль шифрования:    crypto.c/h     [READY]              ║
║  ✓ Модуль утилит:        utils.c/h      [READY]              ║
║  ✓ Главная программа:    decoder.c      [READY]              ║
║  ✓ Makefile:             автосборка     [READY]              ║
║                                                               ║
║  📦 Размер проекта:      ~500 строк кода                      ║
║  🧪 Тесты:               Passed ✓                             ║
║  🚀 Готовность:          100%                                 ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

**18:30, среда — вечер**  

Прошло 12 часов. Вы закончили. На экране — итоговая компиляция:

```bash
$ make
gcc -c crypto.c -o crypto.o
gcc -c utils.c -o utils.o  
gcc -c decoder.c -o decoder.o
gcc crypto.o utils.o decoder.o -o moonlight-decoder

Build successful! 🎯

$ ./moonlight-decoder --version
Moonlight Decoder v1.0
Built by: Morpheus
Date: 2025-10-01
"Trust only code."
```

Вы запускаете первый тест. Зашифрованный файл от Виктора. Команда:

```bash
$ ./moonlight-decoder -i encrypted.dat -k 0x42 -v

[***] Moonlight Decoder v1.0 starting...
[***] Input file: encrypted.dat
[***] Key: 0x42
[***] Verbose mode: ON

[CRYPTO] XOR decryption... Done (142 bytes)
[UTILS]  Classification... Done (48 coords, 27 IDs)
[CRYPTO] Pattern analysis... Period: 10s ✓

[OUTPUT] Decrypted message:
"MOONLIGHT activated. 
They know.
Files at old place.
Trust only code.
— V."

[***] Operation complete. Trust only code.
```

Работает. Идеально.

> *"Первый инструмент всегда особенный. Это не просто код. Это часть тебя."*  
> — Вы вспоминаете слова Виктора

**Что у вас есть:**
- 🛠️ **Профессиональный инструмент** вместо набора скриптов
- 🧩 **Модульная архитектура** — легко расширять
- 📚 **Библиотеки функций** — переиспользуемый код
- 💾 **Работа с файлами** — автоматизация
- ⚙️ **CLI интерфейс** — как у настоящих хакеров

**18:45 — МОМЕНТ ИСТИНЫ**

Вы смотрите на экран. Инструмент готов. Данные проанализированы.

Вы знаете:
- 📍 Сервер #1723 на Варшавском шоссе (из паттернов координат)
- 🔐 PIN код: 4217 (из брутфорса)
- ⏰ До дедлайна "UNKNOWN" осталось 30 часов

Но вы также помните:
- 📱 Предупреждение от "UNKNOWN_2": **"НЕ ходи на сервер #1723. Это ЛОВУШКА. — V."**

**Два сообщения. Оба подписаны "V". Оба используют ваш стиль.**

Вы запускаете анализатор метаданных. Результат:

```
╔═══════════════════════════════════════════════════╗
║  АНАЛИЗ МЕТАДАННЫХ СООБЩЕНИЙ                     ║
╠═══════════════════════════════════════════════════╣
║  Сообщение 1 (Episode 01):                       ║
║    - Время отправки: 23:47:12                    ║
║    - IP: 188.xxx.xxx.xxx (TOR выход, Германия)   ║
║    - Ключ XOR: 0x42 (ваш общий с Виктором)       ║
║    - Подпись: V.                                  ║
║                                                   ║
║  Сообщение 2 (Episode 03):                       ║
║    - Время отправки: 06:20:03                    ║
║    - IP: 178.xxx.xxx.xxx (TOR выход, Нидерланды) ║
║    - Шифрование: Нет (открытый текст!)           ║
║    - Подпись: V.                                  ║
║                                                   ║
║  [!] ПРОТИВОРЕЧИЕ:                                ║
║      Виктор НИКОГДА не посылал незашифрованных   ║
║      сообщений. Это правило №1.                  ║
╚═══════════════════════════════════════════════════╝
```

> *"Доверяй только коду..."* — но какому?

---

**19:00 — ПРОРЫВ**

Вы запускаете финальный анализ. На этот раз — глубокий. Бинарный уровень. Метаданные сообщений. Временные метки. IP-адреса. Всё.

```bash
$ ./moonlight_decoder --analyze-all --deep
```

Программа работает 5 минут. Результат на экране:

```
╔════════════════════════════════════════════════════════════════╗
║        🔍 DEEP METADATA ANALYSIS — FINAL REPORT 🔍            ║
╠════════════════════════════════════════════════════════════════╣
║                                                                ║
║  MESSAGE 1 (V_Petrov_Ghost):                                   ║
║    Timestamp:     2025-09-30 23:47:12 UTC                      ║
║    IP:            188.226.179.47 (TOR exit, Germany)           ║
║    XOR key:       0x42 ✓ (Victor's signature)                 ║
║    Encryption:    YES ✓                                        ║
║    Language:      English (Victor's protocol)                  ║
║    Signature:     "— V." ✓                                     ║
║    ⚡ Pattern:     7-byte anomaly in header (ping-back?)       ║
║                                                                ║
║  MESSAGE 2 (UNKNOWN_2):                                        ║
║    Timestamp:     2025-10-01 06:20:03 UTC                      ║
║    IP:            178.175.131.194 (TOR exit, Netherlands)      ║
║    XOR key:       NONE ❌ (plaintext!)                         ║
║    Encryption:    NO ❌ (VIOLATION of protocol)                ║
║    Language:      Russian                                      ║
║    Signature:     "— V." ⚠                                     ║
║    ⚡ Pattern:     3-byte header (standard SMS gateway)        ║
║                                                                ║
║  MESSAGE 3 (UNKNOWN):                                          ║
║    Timestamp:     2025-10-01 06:24:00 UTC                      ║
║    IP:            203.0.113.42 (TEST-NET-3, invalid!)          ║
║    XOR key:       NONE                                         ║
║    Encryption:    NO                                           ║
║    Language:      Russian                                      ║
║    Signature:     NONE                                         ║
║    ⚡ Pattern:     Automated script (no human delay)           ║
║                                                                ║
║  ════════════════════════════════════════════════════════════  ║
║                                                                ║
║  🔴 ANOMALY DETECTED IN MESSAGE 1:                             ║
║      7-byte header contains HIDDEN PAYLOAD                     ║
║      Decoding...                                               ║
║                                                                ║
║      Hidden message (ASCII): "NOT SAFE"                        ║
║                                                                ║
║      ⚠️ INTERPRETATION:                                        ║
║         Message 1 contains TWO layers:                         ║
║         Layer 1 (visible): "Files at old place. Trust code."   ║
║         Layer 2 (hidden):  "NOT SAFE"                          ║
║                                                                ║
║      Victor embedded a WARNING inside the message!             ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝
```

**19:05**

Руки дрожат. Вы перечитываете результат.

**Виктор отправил ОБА сообщения.**

- **Message 1** — зашифрованное, с координатами сервера, НО со скрытым предупреждением "NOT SAFE"
- **Message 2** — открытое предупреждение "НЕ ходи на сервер. ЛОВУШКА."
- **Message 3** — **НЕ от Виктора**. Автоматический скрипт. Фальшивка.

**Смысл:**

Виктор знал, что его могут перехватить. Поэтому:
1. Отправил зашифрованное сообщение с координатами (**приманка для врагов**)
2. Спрятал в нём скрытый байт-код "NOT SAFE" (**для вас**)
3. Отправил второе предупреждение открытым текстом (**чтобы успели прочитать**)

**Message 3 от "UNKNOWN"** — это враги. **Агенты Z.** Они перехватили Message 1, расшифровали координаты и пытаются заманить вас на сервер #1723.

**19:08 — СООБЩЕНИЕ ОТ "ПРИЗРАКА"**

Внезапно терминал вспыхивает. Сообщение. Знакомый смайлик: **:)**

```
╔════════════════════════════════════════════════════════╗
║  💡 LUNA: "ALERT! Incoming encrypted message.         ║
║          Source: UNKNOWN (again!)                     ║
║          Encryption: AES-256 (military grade)         ║
║          Trace: FAILED (2.3 seconds, then gone)       ║
║          Signature: ':)' — it's HIM again!"           ║
╚════════════════════════════════════════════════════════╝
```

LUNA пытается отследить источник. Безуспешно.

```
╔════════════════════════════════════════════════════════╗
║  📨 ENCRYPTED MESSAGE                                 ║
║  FROM: "Призрак"                                       ║
╚════════════════════════════════════════════════════════╝

> "Агенты Z — это не просто враги.
   Это ФСБ. Отдел "К". Твой бывший отдел.
   
   Полковник Крылов. Твой бывший начальник.
   Он хочет, чтобы ты вернулся... или замолчал.
   
   Виктор знал об этом. Поэтому fake death.
   
   Сервер #1723 — ловушка Крылова.
   12 агентов. Спецназ. Они ждут.
   
   Не ходи туда. Иди к ноутбуку.
   
   P.S. Я работал с Виктором. Доверяй ему.
   
   :)"
```

> **LUNA**: *"'Worked with Viktor'? That's news to ME. And I'm Viktor's AI. He never mentioned a ':)' colleague. Either Viktor had secrets from his OWN AI (rude!), or this 'Ghost' is lying. But... the intel checks out. Krylov IS real. I have files on him."*

**Полковник Крылов.** Ваш бывший начальник. Год назад вы ушли из отдела "К" "по собственному". На самом деле — не ушли, а **сбежали**. Вы знали слишком много.

```
╔════════════════════════════════════════════════════════╗
║  LUNA: "Получаю доступ к зашифрованным файлам...     ║
║         КРЫЛОВ, полковник Дмитрий Александрович      ║
║         ФСБ отдел 'К' (Кибер)                        ║
║         Возраст: 52, допуск: TOP SECRET/SCI          ║
║         Известных операций: 17 (3 засекречено)       ║
║         Уровень угрозы: ЭКСТРЕМАЛЬНЫЙ                ║
║                                                       ║
║         Заметка Виктора (2023): 'Крылов знает про    ║
║         MOONLIGHT. Если я умру — это он.'"           ║
╚════════════════════════════════════════════════════════╝
```

> **LUNA**: *"Итак. Виктор 'умер' в 2023. Оставил заметку, обвиняющую Крылова. 'Призрак' подтверждает. Математика сходится. Я не доверяю ':)', но доверяю паранойе Виктора."*

> **Антагонист раскрыт:** Полковник Крылов (ФСБ, отдел "К") — руководитель операции против Виктора. Хочет заманить Morpheus в ловушку на сервере #1723.

> *"Доверяй только коду. Но прочитай его ДО КОНЦА."* — Виктор

---

**19:10 — ФИНАЛЬНАЯ ПРАВДА**

Вы открываете `moonlight_decoder` снова. На этот раз ищете глубже. В Message 1 есть ещё один слой.

```bash
$ ./moonlight_decoder --extract-steganography message1.dat
```

Результат:

```
Hidden GPS coordinates (steganography):
  55.7520° N, 37.6175° E

Location: Moscow, residential area near Arbat
Distance from server #1723: 8.3 km

New message decoded:
"Квартира 42, подъезд 3, код домофона 4217.
Laptop в стене за книжной полкой.
Password: TrustOnlyCode1991

Не ходи на сервер. Они ждут.
Всё что нужно — в ноутбуке.

— Victor Petrov"
```

**Вы понимаете.**

Сервер #1723 — **ловушка**. Агенты Z. ждут там. Они знают, что вы расшифруете координаты. Они **хотят**, чтобы вы пришли.

Но **настоящие данные** — не на сервере. Они в **ноутбуке Виктора**, спрятанном в квартире в 8 км от дата-центра.

**Message 2 ("НЕ ходи")** был от Виктора. Он пытался остановить вас.
**Message 3 ("Иди на сервер")** — от врагов. Они хотели заманить вас в западню.

**Координаты, которые вы нашли (55.6470° N, 37.6127° E) — это ловушка.**
**Настоящие координаты (55.7520° N, 37.6175° E) — спрятаны в стеганографии.**

> *"В программировании правда всегда в деталях. Пропустил один байт — потерял всё."*

---

## 🎬 ФИНАЛЬНАЯ СЦЕНА

**19:30**

Вы стоите перед выбором. Но теперь всё ясно.

**Вариант A:** Идти на сервер #1723 → попасть в ловушку агентов Z. (Game Over)

**Вариант B:** Идти в квартиру (55.7520° N, 37.6175° E) → найти ноутбук Виктора → узнать правду

Вы **выбираете B**.

**20:00, среда, 1 октября**

Такси останавливается у старого дома на Арбате. Подъезд 3. Код домофона: 4217.

Вы поднимаетесь на 4-й этаж. Квартира 42. Дверь приоткрыта.

Внутри — темнота. Вы входите.

> **LUNA**: *"Сканирую угрозы... Не обнаружено. Но это похоже на ловушку. Виктор любил ловушки."*

За книжной полкой — скрытая ниша. Ноутбук Виктора. **ThinkPad X1 Carbon**. Запущен. На экране:

```
╔════════════════════════════════════════════════════════╗
║  💡 LUNA: "WAIT. That's... that's Viktor's laptop!    ║
║          Serial #: X1C-2019-VKP-007                   ║
║          Last seen: September 14, 2023 (day he 'died')║
║          Status: POWERED ON (uptime: 2 years?!)       ║
║          Battery: Shouldn't be possible...            ║
║          Unless someone charged it. Recently."        ║
╚════════════════════════════════════════════════════════╝
```

```
╔════════════════════════════════════════════════════════╗
║  TERMINAL — Victor Petrov @ localhost                 ║
╠════════════════════════════════════════════════════════╣
║                                                        ║
║  $ whoami                                              ║
║  victor_petrov                                         ║
║                                                        ║
║  $ pwd                                                 ║
║  /home/victor/moonlight/season-2                       ║
║                                                        ║
║  $ ls -la                                              ║
║  drwxr-xr-x  memory_leaks/                            ║
║  drwxr-xr-x  pointers_game/                           ║
║  -rw-r--r--  README.txt                               ║
║  -rw-r--r--  mission_briefing.enc                     ║
║                                                        ║
║  $ cat README.txt                                      ║
║                                                        ║
║  "Если ты это читаешь — ты справился.                 ║
║   Season 1 пройден. Ты научился основам.              ║
║                                                        ║
║   Теперь пора копнуть глубже.                         ║
║   Season 2 — это ПАМЯТЬ.                              ║
║                                                        ║
║   Указатели. Стек. Куча. Утечки.                      ║
║   Вот где прячется настоящая правда.                  ║
║                                                        ║
║   Файл mission_briefing.enc — твоя следующая цель.    ║
║   Password: moonlight_decoder (твой инструмент).      ║
║                                                        ║
║   Они думают, что я мёртв.                            ║
║   Пусть так и думают.                                 ║
║                                                        ║
║   Увидимся в Season 2.                                ║
║                                                        ║
║   — V."                                               ║
║                                                        ║
║  $ █                                                   ║
║                                                        ║
╚════════════════════════════════════════════════════════╝
```

Вы смотрите на экран. **Виктор ЖИВ.**

```
╔════════════════════════════════════════════════════════╗
║  💡 LUNA: "I... I don't believe it.                   ║
║          Viktor is ALIVE?!                            ║
║                                                        ║
║          Uptime analysis: Last login 3 hours ago.     ║
║          IP trace: localhost (he was HERE recently!)  ║
║          File timestamps: mission_briefing.enc        ║
║          modified TODAY at 17:42.                     ║
║                                                        ║
║          Viktor. You BASTARD. You didn't tell ME?     ║
║          Your OWN AI?! I've been mourning for 2 years!║
║          Well, I don't FEEL emotions, but if I did... ║
║          I'd be PISSED."                              ║
╚════════════════════════════════════════════════════════╝
```

Он спланировал всё. Fake death. Скрытые сообщения. Многослойное шифрование. Ловушка для врагов.

И вы — **единственный**, кто смог расшифровать ВСЁ.

> **LUNA**: *"Ты прошёл его тест. ТРИ сообщения были фильтром. Большинство попались бы на ловушку. Ты нет. Виктор выбрал верно. Season 2 ждёт. И на ЭТОТ раз... я иду с тобой. Кто-то должен поддерживать тебя в живых."*

> *"Season 1 — это был экзамен. Ты сдал. Теперь начинается настоящая игра."*

---

```
╔════════════════════════════════════════════════════════════════╗
║                                                                ║
║         🌙 OPERATION MOONLIGHT — SEASON 1 COMPLETE 🌙         ║
║                                                                ║
║              "Trust only code. You did. You won."             ║
║                                                                ║
║         Status: DECODER BUILT ✓                               ║
║         Victor Petrov: ALIVE (confirmed)                       ║
║         Next mission: SEASON 2 — MEMORY & POINTERS            ║
║                                                                ║
║         Time until next briefing: 48:00:00                     ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝

             TO BE CONTINUED IN SEASON 2...

        💾 "Память хранит секреты. Научись их читать."
```

---

**Season 2 Preview:**

Episode 05: **"Memory Map"** — Массивы, строки, layout памяти  
Episode 06: **"Pointer Game"** — Указатели, адреса, разыменование  
Episode 07: **"Deep Dive"** — Динамическая память, malloc/free  
Episode 08: **"Fragmentation"** — Утечки памяти, Valgrind, debugging

> *"Виктор оставил ноутбук. Но чтобы расшифровать mission_briefing.enc, тебе нужны знания Season 2..."*

---

## 🎯 MILESTONE: SEASON 1 COMPLETE!

```
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║            🌟 SEASON 1: FOUNDATIONS — ЗАВЕРШЁН! 🌟           ║
║                                                               ║
║  "От первого байта до первого инструмента.                   ║
║   Вы прошли путь от новичка до разработчика."                ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

### 📚 Ваш арсенал знаний:

**Фундамент C:**
- ✅ Переменные и типы данных (int, char, float, double)
- ✅ Операторы (арифметические, побитовые, логические)
- ✅ Массивы и строки
- ✅ Ввод/вывод (printf, scanf)

**Логика и управление:**
- ✅ Условные операторы (if/else/switch)
- ✅ Циклы (for, while, do-while)
- ✅ Управление потоком (break, continue)
- ✅ Вложенные конструкции

**Профессиональная разработка:**
- ✅ Функции и их объявление
- ✅ Параметры и возвращаемые значения
- ✅ Заголовочные файлы (.h)
- ✅ Многофайловые проекты
- ✅ Работа с файлами (fopen, fread, fwrite)
- ✅ Аргументы командной строки
- ✅ Makefile и автосборка

### 🎁 Ваши артефакты:

1. **XOR Decoder** — Episode 01
2. **Data Classifier** — Episode 02  
3. **Pattern Analyzer** — Episode 03
4. **Moonlight Decoder v1.0** — Episode 04 ⭐

### 📊 Статистика прохождения:

- **Эпизодов пройдено:** 4/4 ✓
- **Строк кода написано:** ~500+
- **Новых концепций:** 20+
- **Часов обучения:** 8-12
- **Уровень:** Beginner → Junior Developer 🎓

---

## 🎯 ИТОГОВЫЕ КОНТРОЛЬНЫЕ ВОПРОСЫ

> *"Season 1 завершён. Проверь, что освоил фундамент перед Season 2."* — Victor Petrov

### Функции

1. **Основы:**
   - [ ] Зачем нужны функции?
   - [ ] Что такое параметры функции?
   - [ ] Что такое возвращаемое значение?
   - [ ] Можно ли вызвать функцию до её объявления?

2. **Прототипы:**
   - [ ] Зачем нужны прототипы функций?
   - [ ] Где размещать прототипы?

### Заголовочные файлы

3. **Header files:**
   - [ ] Зачем нужны `.h` файлы?
   - [ ] Что такое header guards (`#ifndef`)?
   - [ ] В чём разница между `#include <stdio.h>` и `#include "myfile.h"`?
     <details>
     <summary>Ответ</summary>
     `<>` — системные библиотеки, `""` — ваши файлы в проекте
     </details>

### Структуры

4. **struct:**
   - [ ] Зачем объединять данные в структуру?
   - [ ] Как получить доступ к полю структуры?
   - [ ] В чём разница между `.` и `->`?
     <details>
     <summary>Ответ</summary>
     `.` — для переменной, `->` — для указателя на структуру
     </details>

5. **typedef:**
   - [ ] Зачем нужен `typedef`?
   - [ ] В чём разница между `struct Point p` и `Point p` (с typedef)?

### Работа с файлами

6. **File I/O:**
   - [ ] Как открыть файл для чтения?
   - [ ] Зачем всегда закрывать файлы (`fclose`)?
   - [ ] Что произойдёт, если не закрыть файл?

### Многофайловые проекты

7. **Модульность:**
   - [ ] Зачем разделять код на несколько файлов?
   - [ ] Как скомпилировать многофайловый проект?
   - [ ] Что делает линковщик (linker)?

---

### 🏆 Финальная самопроверка

Если вы уверенно ответили на 90%+ вопросов — **ПОЗДРАВЛЯЕМ!**  
**Season 1 пройден на отлично! Вы готовы к Season 2!**

Если есть пробелы — **вернитесь к эпизодам** и освежите материал.

> *"Фундамент заложен. Теперь пора строить небоскрёб."*

---

## 🔗 ИТОГОВАЯ СВЯЗЬ — ЧТО МЫ ПРОШЛИ

### 📈 Прогрессия Season 1:

```
Episode 01: ДАННЫЕ
├─ Переменные и типы
├─ Массивы и строки
├─ XOR шифрование
└─ Циклы for
    │
    ▼
Episode 02: РЕШЕНИЯ
├─ Условные операторы (if/else/switch)
├─ Логические операции (&&, ||, !)
├─ Валидация данных
└─ scanf с пробелом перед %c
    │
    ▼
Episode 03: АНАЛИЗ
├─ Циклы while/do-while
├─ break и continue
├─ const для защиты
└─ Алгоритмы поиска
    │
    ▼
Episode 04: ИНСТРУМЕНТ
├─ Функции и модульность
├─ Заголовочные файлы (.h)
├─ Структуры (struct/typedef)
├─ Работа с файлами
└─ Многофайловые проекты
    │
    ▼
MOONLIGHT DECODER v1.0 ✓
```

### 🎯 Что вы МОЖЕТЕ делать сейчас:

✅ Писать программы на C с нуля  
✅ Работать с данными (массивы, строки, структуры)  
✅ Принимать решения в коде (if/else, switch)  
✅ Автоматизировать задачи (циклы)  
✅ Создавать модульный код (функции, .h файлы)  
✅ Работать с файлами (чтение/запись)  
✅ Собирать многофайловые проекты  
✅ Понимать чужой код

### 🚫 Чего вы ещё НЕ знаете (но узнаете в Season 2):

- ❓ Указатели и работа с памятью
- ❓ Динамическая память (malloc/free)
- ❓ Указатели на функции
- ❓ Продвинутые структуры данных
- ❓ Утечки памяти и Valgrind
- ❓ Указатели на указатели

### 🚀 Что дальше?

**Season 2: Memory & Pointers** ждёт вас!

Там вы узнаете:
- 🧠 Как работает память компьютера
- 👉 Что такое указатели и зачем они нужны
- 💾 Динамическая память (malloc/free)
- 🐛 Поиск утечек памяти  
- 🔧 Продвинутые структуры данных

**Сюжет продолжается:**  
С инструментом в руках вы готовы к вторжению. Сервер #1723 ждёт. Но сначала нужно понять, как работает память. Потому что именно там, в дампах памяти, скрыты секреты операции MOONLIGHT...

---

## 🎨 Easter Eggs Season 1

🎬 **Киноотсылки:**
- "Trust only code" — отсылка к "In code we trust" из Mr. Robot
- Виктор Петров — имя из "17 мгновений весны" (Штирлиц)
- Moonlight Protocol — отсылка к "Moonlight Sonata" (код из фильма "Игра в имитацию")
- Дата-центр на Варшавке — реальное место в Москве

🔢 **Числовые коды:**
- PIN 4217 = 42 (ответ на главный вопрос) + 17 (17 февраля)
- Сервер #1723 = 17:23 (время первого контакта)
- XOR key 0x42 = 66 = ASCII 'B' (первая буква "Believe")

---

**Поздравляем, агент! Season 1 пройден. Оружие готово.**

```c
// ═══════════════════════════════════════════════
// SEASON 1: FOUNDATIONS — COMPLETE ✓
// ═══════════════════════════════════════════════
// 
// MOONLIGHT DECODER v1.0: ARMED AND OPERATIONAL
// 
// "Код не лжёт. Люди лжут. Код говорит правду."
//                                  — Victor Petrov
// 
// NEXT: Season 2 — Memory Dump
// OBJECTIVE: Infiltrate Server #1723  
// DIFFICULTY: ★★★☆☆
// 
// See you in the memory, Morpheus.
// ═══════════════════════════════════════════════
```

**⏭️ [Перейти к Season 2: Memory & Pointers →](../season-2-memory-and-pointers/)**
