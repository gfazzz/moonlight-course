# Episode 13: "Crypto Basics" 🔑

> *"In cryptography, the difference between 'secret' and 'secure' is everything."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   23:47, среда, 15 декабря                      ║
║  СТАТУС:  🟡 ENCRYPTED MESSAGES INTERCEPTED              ║
╚══════════════════════════════════════════════════════════╝
```

**Season 3 завершён.** Сеть изучена, трафик перехвачен, коммуникации расшифрованы.

Но это был лишь **первый слой защиты**.

В 23:42 приходит экстренное сообщение V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (SECURE CHANNEL)                  │
│  TIME: 23:42:17 UTC                         │
│  PRIORITY: HIGH                             │
├─────────────────────────────────────────────┤
│                                             │
│  СИТУАЦИЯ ИЗМЕНИЛАСЬ.                       │
│                                             │
│  Мы перехватили USB-накопитель с архивом    │
│  внутренних коммуникаций организации.       │
│  Три зашифрованных файла. Время критично.   │
│                                             │
│  ЗАДАЧА:                                    │
│  1. message1.enc - XOR шифрование (ключ?)   │
│  2. message2.enc - классический шифр        │
│  3. message3.enc - многослойная защита      │
│                                             │
│  Подсказка в файле key.hint:                │
│  "The answer is in the frequency"           │
│                                             │
│  У нас есть 6 часов до смены протоколов.    │
│  После этого данные станут бесполезными.    │
│                                             │
│  Удачи.                                     │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

**Что вы понимаете:**
- **Временное окно:** 6 часов до 05:47
- **Три уровня защиты:** от простого к сложному
- **Ключ к успеху:** частотный анализ
- **Ставки:** критическая информация об операции MOONLIGHT

Вы вставляете USB. На экране появляются три файла:

```
📁 USB Drive: MOONLIGHT_ARCHIVE
├── message1.enc        [ENCRYPTED] 156 bytes
├── message2.enc        [ENCRYPTED] 892 bytes  
├── message3.enc        [ENCRYPTED] 2.1 KB
└── key.hint            [TEXT] "The answer is in the frequency"
```

Часы тикают. **23:47**. Начинаем.

---

## 📚 Теория: Основы криптографии

### 💡 Что такое криптография?

**Криптография** = наука о защите информации через математические преобразования.

**Основные концепции:**
- 🔒 **Шифрование (Encryption)**: plaintext → ciphertext
- 🔓 **Дешифрование (Decryption)**: ciphertext → plaintext
- 🔑 **Ключ (Key)**: секретный параметр для шифрования/дешифрования
- 🛡️ **Криптостойкость**: сложность взлома без ключа

**Классификация:**
- **Симметричные шифры**: один ключ для шифрования и дешифрования (XOR, AES)
- **Асимметричные шифры**: пара ключей public/private (RSA, ECC)
- **Хеш-функции**: одностороннее преобразование (SHA-256, MD5)

---

### 1. XOR шифрование — симметричный пототочный шифр

**XOR (исключающее ИЛИ)** — простейшая, но мощная криптографическая операция.

#### Таблица истинности:
```
A | B | A XOR B
--|---|--------
0 | 0 |   0     ← Оба одинаковы → 0
0 | 1 |   1     ← Разные → 1
1 | 0 |   1     ← Разные → 1
1 | 1 |   0     ← Оба одинаковы → 0
```

**Правило**: XOR возвращает 1, если биты **различаются**.

---

#### 🎯 Ключевые свойства XOR:

1. **Нейтральный элемент**: `A XOR 0 = A`
   ```
   01010101 XOR 00000000 = 01010101  (не изменился)
   ```

2. **Самоинверсия**: `A XOR A = 0`
   ```
   01010101 XOR 01010101 = 00000000  (обнулился)
   ```

3. **Обратимость** (главное свойство!): `A XOR B XOR B = A`
   ```
   Encrypt: data XOR key = encrypted
   Decrypt: encrypted XOR key = data  (тот же ключ!)
   ```

**Метафора**: XOR — это замок и ключ одновременно. Повернул один раз — закрыл, повернул второй — открыл.

---

#### Пример шифрования:

```c
// Сообщение
char message = 'H';           // ASCII 72 = 0x48 = 01001000
char key = 0x2A;              //            0x2A = 00101010

// Шифрование
char encrypted = message ^ key;  // 0x48 ^ 0x2A = 01100010 = 'b' (0x62)

// Дешифрование (тот же ключ!)
char decrypted = encrypted ^ key;  // 0x62 ^ 0x2A = 01001000 = 'H' (0x48)
```

**Визуализация:**
```
  01001000  ('H')
^ 00101010  (key 0x2A)
-----------
  01100010  ('b' - зашифровано)

  01100010  ('b')
^ 00101010  (key 0x2A снова!)
-----------
  01001000  ('H' - расшифровано!)
```

---

#### Реализация в C:

```c
// Шифрование (и дешифрование!) массива байтов
void xor_cipher(unsigned char *data, int len, unsigned char key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;  // Применяем XOR к каждому байту
    }
}

// Использование
unsigned char message[] = "HELLO";
int len = 5;

// Шифруем
xor_cipher(message, len, 0x42);
printf("Encrypted: %s\n", message);  // нечитаемое

// Дешифруем (та же функция, тот же ключ!)
xor_cipher(message, len, 0x42);
printf("Decrypted: %s\n", message);  // "HELLO"
```

**Почему одна функция?** Потому что XOR **симметричен**: `A XOR B = B XOR A`

---

#### 🛡️ Криптостойкость XOR:

**Сильные стороны:**
- ✅ Очень быстрый (побитовая операция)
- ✅ Простая реализация
- ✅ Используется в RC4, ChaCha20

**Слабые стороны:**
- ❌ **Брутфорс легок при коротком ключе** (256 вариантов для 1 байта)
- ❌ **Частотный анализ** при длинном тексте
- ❌ **Known-plaintext attack**: если знаем часть текста, можем найти ключ

**Пример атаки:**
```c
// Если знаем, что сообщение начинается с "URGENT:"
char known_plain[] = "URGENT";
char encrypted[] = {0x37, 0x34, ...};  // перехвачено

// Находим ключ!
char key = known_plain[0] ^ encrypted[0];  // 'U' ^ 0x37 = key
```

---

#### Брутфорс XOR (для 1 байта ключа):

```c
void brute_force_xor(unsigned char *data, int len) {
    printf("Bruteforcing XOR key...\n");
    
    // Пробуем все 256 возможных ключей
    for (int key = 0; key < 256; key++) {
        // Копируем данные
        unsigned char temp[len];
        memcpy(temp, data, len);
        
        // Дешифруем
        xor_cipher(temp, len, key);
        
        // Проверяем: есть ли печатаемые символы?
        int printable = 0;
        for (int i = 0; i < len; i++) {
            if (temp[i] >= 32 && temp[i] <= 126) printable++;
        }
        
        // Если >80% символов печатаемые → вероятно правильный ключ
        if (printable > len * 0.8) {
            printf("Key 0x%02X: %s\n", key, temp);
        }
    }
}
```

**Применение в реальности:**
- 🎮 **Game hacking**: защита сохранений
- 📡 **RC4**: stream cipher (Wi-Fi WEP, SSL/TLS)
- 🔐 **One-Time Pad**: XOR с истинно случайным ключом = абсолютно стойкий шифр!

---

### 2. Шифр Caesar — классический шифр подстановки

**Caesar Cipher** — шифр, использовавшийся Юлием Цезарем для военной переписки (I век до н.э.).

#### Принцип работы:

**Сдвиг букв в алфавите на фиксированное число позиций:**

```
Alphabet:   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Shift 3:    D E F G H I J K L M N O P Q R S T U V W X Y Z A B C
                                                    ↑ возврат к началу
Plaintext:  H E L L O
Ciphertext: K H O O R
```

**Формула:**
```
Encrypt: C = (P + shift) mod 26
Decrypt: P = (C - shift) mod 26

где P = позиция plaintext буквы (A=0, B=1, ...)
    C = позиция ciphertext буквы
```

---

#### Визуализация шифрования:

```
      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
       ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
Shift 3: D E F G H I J K L M N O P Q R S T U V W X Y Z A B C

H (7) + 3 = 10 → K
E (4) + 3 = 7  → H
L (11)+ 3 = 14 → O
```

**Метафора**: Caesar — это круглая табличка с двумя кольцами букв. Повернул на 3 позиции — получил шифр.

---

#### Реализация в C:

```c
// Шифрование одного символа
char caesar_encrypt_char(char c, int shift) {
    if (c >= 'A' && c <= 'Z') {
        // Верхний регистр
        return 'A' + (c - 'A' + shift) % 26;
    }
    if (c >= 'a' && c <= 'z') {
        // Нижний регистр
        return 'a' + (c - 'a' + shift) % 26;
    }
    return c;  // Не буква — не меняем (пробелы, цифры)
}

// Шифрование строки
void caesar_encrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = caesar_encrypt_char(text[i], shift);
    }
}

// Дешифрование — обратный сдвиг
void caesar_decrypt(char *text, int shift) {
    caesar_encrypt(text, 26 - shift);  // 26 - shift = обратный сдвиг
}
```

**Пример:**
```c
char msg[] = "ATTACK AT DAWN";
caesar_encrypt(msg, 3);
printf("%s\n", msg);  // "DWWDFN DW GDZQ"

caesar_decrypt(msg, 3);
printf("%s\n", msg);  // "ATTACK AT DAWN"
```

---

#### 🔓 Взлом Caesar: Брутфорс

**Всего 25 возможных ключей** (shift 1-25, shift 0 = без изменений)

```c
void brute_force_caesar(const char *ciphertext) {
    printf("=== BRUTEFORCE CAESAR ===\n");
    
    for (int shift = 0; shift < 26; shift++) {
        printf("Shift %2d: ", shift);
        
        // Дешифруем с этим сдвигом
        for (int i = 0; ciphertext[i]; i++) {
            putchar(caesar_decrypt_char(ciphertext[i], shift));
        }
        printf("\n");
    }
}
```

**Вывод:**
```
Shift  0: KHOOR ZRUOG  (исходный)
Shift  1: JGNNQ YQTNF
Shift  2: IFMMP XPSME
Shift  3: HELLO WORLD  ← читаемо! правильный сдвиг!
Shift  4: GDKKN VNQKC
...
```

**Сложность взлома**: O(26) = практически мгновенно!

---

#### 🔓 Взлом Caesar: Частотный анализ

**Более умный подход:** используем статистику языка.

**Частоты букв в английском:**
```
E - 12.7%  (самая частая!)
T - 9.1%
A - 8.2%
O - 7.5%
...
Z - 0.1%   (самая редкая)
```

**Алгоритм:**
1. Подсчитать частоты букв в шифротексте
2. Найти самую частую букву → предположить, что это 'E'
3. Вычислить сдвиг: `shift = (most_frequent - 'E' + 26) % 26`
4. Дешифровать

```c
int detect_caesar_shift(const char *ciphertext) {
    int freq[26] = {0};
    int total = 0;
    
    // Подсчёт частот (только A-Z)
    for (int i = 0; ciphertext[i]; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            freq[ciphertext[i] - 'A']++;
            total++;
        }
    }
    
    // Найти самую частую букву
    int max_freq = 0, most_frequent = 0;
    for (int i = 0; i < 26; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            most_frequent = i;  // индекс самой частой буквы
        }
    }
    
    // Предполагаем, что это 'E' (4)
    int shift = (most_frequent - 4 + 26) % 26;
    
    printf("Most frequent letter: %c (%.1f%%)\n", 
           'A' + most_frequent, 100.0 * max_freq / total);
    printf("Detected shift: %d\n", shift);
    
    return shift;
}
```

---

#### 📊 Усиление Caesar:

**Проблемы классического Caesar:**
- ❌ Только 25 ключей (легко брутфорсится)
- ❌ Уязвим к частотному анализу
- ❌ Паттерны сохраняются ("HELLO" всегда одинаково)

**Улучшения:**
- ✅ **ROT13**: shift = 13 (симметричен: encrypt = decrypt)
- ✅ **Vigenère**: разные сдвиги для каждой позиции (многобуквенный ключ)
- ✅ **Affine**: `C = (aP + b) mod 26` (два параметра)

**Историческая значимость:**
- 📜 Использовался 2000 лет
- 🔓 Взломан в IX веке (арабский криптоанализ)
- 🎓 Обучающий пример для студентов

---

### 3. Частотный анализ — оружие криптоаналитика

**Частотный анализ** — метод взлома шифров подстановки, основанный на статистике языка.

#### 📊 Частоты букв в английском языке:

```
E  ████████████  12.7%  ← Самая частая!
T  █████████      9.1%
A  ████████       8.2%
O  ███████        7.5%
I  ███████        7.0%
N  ██████         6.7%
S  ██████         6.3%
H  ██████         6.1%
R  ██████         6.0%
...
Z  █              0.1%  ← Самая редкая
```

**Ключевой факт**: В ЛЮБОМ английском тексте 'E' встречается ~12.7%!

---

#### Реализация частотного анализа:

```c
void frequency_analysis(const char *text) {
    int freq[26] = {0};
    int total = 0;
    
    // Подсчет (только A-Z, игнорируем регистр)
    for (int i = 0; text[i]; i++) {
        char c = toupper(text[i]);
        if (c >= 'A' && c <= 'Z') {
            freq[c - 'A']++;
            total++;
        }
    }
    
    // Визуализация
    printf("=== FREQUENCY ANALYSIS ===\n");
    printf("Total letters: %d\n\n", total);
    
    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            printf("%c: ", 'A' + i);
            
            // ASCII bar chart
            int bar_len = (int)(100.0 * freq[i] / total / 2);
            for (int j = 0; j < bar_len; j++) printf("█");
            
            printf(" %.1f%% (%d)\n", 100.0 * freq[i] / total, freq[i]);
        }
    }
}
```

---

## 🎯 МИССИЯ

### Задание 1: XOR Decryptor ⭐⭐☆☆☆

Реализуйте функцию для XOR-дешифрования с одним байтом ключа.

**Файл:** `message1.enc` (бинарный)  
**Подсказка:** Ключ — ASCII символ в диапазоне `0x20-0x7E`

```c
// TODO: Реализовать
void xor_decrypt_single_byte(unsigned char *data, int len, unsigned char key);

// TODO: Брутфорс всех возможных ключей
void brute_force_xor(unsigned char *data, int len);
```

**Критерии успеха:**
- Текст начинается с "URGENT:"
- Содержит читаемые английские слова

---

### Задание 2: Caesar Cipher Breaker ⭐⭐⭐☆☆

Взломайте шифр Caesar методом брутфорса.

**Файл:** `message2.enc` (текстовый)

```c
// TODO: Реализовать
void caesar_decrypt(char *text, int shift);

// TODO: Автоматическое определение сдвига
int detect_shift(const char *ciphertext);
```

**Hint:** Используйте частоту букв. Самая частая буква в английском — 'E'.

---

### Задание 3: Multi-layer Decryption ⭐⭐⭐⭐☆

Комбинированное шифрование: сначала Caesar, потом XOR.

**Файл:** `message3.enc`  
**Подсказка:** "First rotate, then XOR with 0x5A"

```c
// TODO: Реализовать многослойную расшифровку
void multi_decrypt(unsigned char *data, int len, int caesar_shift, unsigned char xor_key);
```

---

## 📝 Структура эпизода

```
episode-13-crypto-basics/
├── artifacts/
│   ├── warmup.c            ← Создайте этот файл
│   ├── decoder.c           ← Создайте этот файл
│   └── validator.c         ← Создайте этот файл
├── starter.c               ← Шаблоны и примеры кода
├── message1.enc            ← XOR зашифровано
├── message2.enc            ← Caesar зашифровано
├── message3.enc            ← Multi-layer
├── tests/
│   └── test.sh             ← Автотесты
├── solution/
│   └── crypto_solution.c   ← Открывать только если застряли!
└── README.md
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там примеры и закомментированный код
2. **Создайте** новый файл в папке `artifacts/`
3. **Напишите** свой код, используя подсказки из `starter.c`
4. **Скомпилируйте** и запустите
5. **Проверьте** через автотесты

---

## 🔍 Подсказки

<details>
<summary>Подсказка 1: XOR брутфорс</summary>

Для каждого байта `0x20-0x7E`:
1. Расшифруйте данные
2. Проверьте, есть ли печатаемые символы (>80%)
3. Проверьте на наличие общих слов ("the", "and", "is")

</details>

<details>
<summary>Подсказка 2: Детектирование Caesar shift</summary>

1. Найдите самую частую букву в шифротексте
2. Предположите, что это 'E'
3. Вычислите сдвиг: `shift = (most_frequent - 'E' + 26) % 26`

</details>

<details>
<summary>Подсказка 3: Порядок расшифровки</summary>

При многослойном шифровании расшифровка идет в **обратном порядке**:
1. Сначала XOR (последнее применённое)
2. Потом Caesar (первое применённое)

</details>

---

## 🎓 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ВРЕМЯ:   05:23, четверг, 16 декабря                    ║
║  СТАТУС:  ✅ ALL MESSAGES DECRYPTED                      ║
║  ELAPSED: 5 часов 36 минут                               ║
╚══════════════════════════════════════════════════════════╝
```

**05:17. Последний файл расшифрован.**

Вы смотрите на экран. Три сообщения раскрыты:

```
╔═══ MESSAGE 1 (XOR) ══════════════════════════════════════╗
║  Key: 0x42                                               ║
║  Method: Single-byte XOR                                 ║
║                                                          ║
║  URGENT: Package delivery scheduled for Dec 18           ║
║  Location: Warehouse District, coordinates following     ║
║  Contact: Agent RAVEN                                    ║
╚══════════════════════════════════════════════════════════╝

╔═══ MESSAGE 2 (Caesar) ═══════════════════════════════════╗
║  Shift: 13 (ROT13)                                       ║
║  Method: Frequency analysis                              ║
║                                                          ║
║  Financial records transferred to offshore account.      ║
║  Total: $15.7M. Blockchain verification required.        ║
║  See attached ledger in encrypted archive.               ║
╚══════════════════════════════════════════════════════════╝

╔═══ MESSAGE 3 (Multi-layer) ══════════════════════════════╗
║  Layer 1: Caesar shift 7                                 ║
║  Layer 2: XOR 0x5A                                       ║
║                                                          ║
║  CRITICAL: Operation MOONLIGHT phase 2 begins Dec 20.    ║
║  All agents converge on primary location.                ║
║  Blockchain ledger contains coordinates.                 ║
║  — GENESIS                                               ║
╚══════════════════════════════════════════════════════════╝
```

**Что вы узнали:**
- ✅ **Дата операции**: 18-20 декабря (осталось 2-4 дня!)
- ✅ **Финансовый след**: $15.7M в оффшоре
- ✅ **Blockchain**: ключ к координатам
- ✅ **Codename**: "GENESIS" — лидер организации

Вы немедленно отправляете отчёт V.:

```
┌─────────────────────────────────────────────┐
│  TO: V. (SECURE CHANNEL)                    │
│  TIME: 05:23:41 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  Все три сообщения расшифрованы.            │
│                                             │
│  КРИТИЧНО:                                  │
│  - Операция начнётся 18 декабря             │
│  - Финансы в blockchain ledger              │
│  - Координаты зашифрованы в блоках          │
│                                             │
│  Переходим к Episode 14: Blockchain.        │
│  Нужно проверить целостность ledger и       │
│  извлечь координаты.                        │
│                                             │
│  Время критично.                            │
│                                             │
└─────────────────────────────────────────────┘
```

Через минуту приходит ответ:

> **V.:** "Отличная работа. Blockchain ledger — это наш следующий шаг. Скачиваю архив... Done. Проверь целостность цепи и найди координаты. Episode 14 ждёт."

**05:27. Вы открываете новый файл: `moonlight_ledger.dat`**

---

## 🎖️ Навык разблокирован

```
╔══════════════════════════════════════════════════════════╗
║  🔓 CRYPTANALYST I                                       ║
║                                                          ║
║  Вы овладели основами криптоанализа:                     ║
║  ✅ XOR шифрование/дешифрование                          ║
║  ✅ Caesar cipher brute-force                            ║
║  ✅ Частотный анализ текста                              ║
║  ✅ Многослойная расшифровка                             ║
║                                                          ║
║  «Любой код можно взломать. Вопрос времени.»             ║
╚══════════════════════════════════════════════════════════╝
```

### 📚 Что вы изучили:

- ✅ Побитовые операции (XOR, AND, OR, shift)
- ✅ Симметричные шифры (XOR cipher)
- ✅ Шифры подстановки (Caesar, ROT13)
- ✅ Частотный анализ для автоматического взлома
- ✅ Брутфорс при малом пространстве ключей
- ✅ Многослойное шифрование и порядок расшифровки
- ✅ Known-plaintext attacks
- ✅ Работа с бинарными данными в C

### 🛠️ Практические навыки:

- ✅ Реализация XOR на уровне байтов
- ✅ Работа с бинарными файлами
- ✅ Статистический анализ текста
- ✅ Оптимизация алгоритмов перебора
- ✅ Визуализация частот в консоли

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Секреты и отсылки</summary>

1. **Key 0x42** — "Answer to the Ultimate Question" (Hitchhiker's Guide to the Galaxy)
2. **ROT13** — shift=13, симметричен: encrypt = decrypt
3. **GENESIS** — первый блок blockchain называется "Genesis Block"
4. **$15.7M** — отсылка к первой Bitcoin транзакции ($15.7 в 2009)
5. **Agent RAVEN** — отсылка к "The Raven" Э. По, где использовался шифр
6. **Frequency analysis** — изобретен в IX веке арабским ученым Аль-Кинди
7. **Dec 18-20** — даты реальной Enigma break timeline (1941)
8. **Warehouse District** — классическая локация spy thrillers

**Философия Episode 13**: *"Простейшие инструменты, используемые грамотно, могут быть столь же эффективны, как и сложные системы."*

</details>

---

## 🔜 Следующий эпизод

**Episode 14: "Blockchain & Hashes"** ждёт вас!

В Episode 14 вы изучите:
- 🔗 **Blockchain структуру** — как работает неизменяемая цепь блоков
- #️⃣ **Хеш-функции** — djb2, SHA-256, лавинный эффект
- ⛏️ **Proof-of-Work** — mining и difficulty
- 🛡️ **Верификация целостности** — обнаружение подделок

**Сюжет:**
Расшифрованные сообщения указывают на blockchain ledger с финансовыми записями организации. Но некоторые блоки повреждены — их пытались подделать. Вам нужно:
1. Понять структуру blockchain
2. Верифицировать целостность цепи
3. Найти поврежденные блоки
4. Извлечь координаты из валидных блоков

**Время:** 6 часов до начала операции MOONLIGHT.

---

## 🔗 Навигация

- [← Episode 12: Encrypted Communications](../../season-3-networks/episode-12-encrypted-communications/README.md)
- [→ Episode 14: Blockchain & Hashes](../episode-14-blockchain-hashes/README.md)
- [📋 Season 4: Crypto & Algorithms](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"Cryptography is the ultimate form of non-violent direct action."* — Julian Assange