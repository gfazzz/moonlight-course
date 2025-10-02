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

## 📚 THEORY BLOCK 1: XOR Encryption (23:47 - 04:12)

### 💡 Основы криптографии

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

## 🎭 04:12 — Звонок Eva Zimmerman (UBS Zürich)

**04:00** — XOR теория изучена. Вы начинаете брутфорс message1.enc...

**04:12** — Телефон звонит. Швейцарский номер (+41).

```
📞 Eva Zimmerman (UBS Zürich, Cryptographer)

"Морфеус? Я Ева Циммерман, криптограф UBS Zürich. 
Viktor дал мне ваш номер.

XOR брутфорс 0x20-0x7E займёт секунды - 95 вариантов.
Но помните known-plaintext attack: если знаете начало 
('URGENT:'), ключ находится за одну операцию!

Caesar cipher далее? Frequency analysis - ваш друг.
'E' = 12.7% в английском. Найдите самую частую букву.

Звоните если нужна помощь. Удачи!"
```

**🔓 ПРОФИЛЬ:** Eva Zimmerman — Senior Cryptographer UBS, PhD ETH Zürich, эксперт classical & modern crypto. **Главный криптограф операции MOONLIGHT.**

---

## 📚 THEORY BLOCK 2: Caesar Cipher (04:12 - 06:30)

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

---

#### 🎨 Визуальная метафора: Шифровальный диск Цезаря

```
╔════════════════════════════════════════════════════════════════╗
║  CAESAR CIPHER = ШИФРОВАЛЬНЫЙ ДИСК ЦЕЗАРЯ                      ║
╠════════════════════════════════════════════════════════════════╣
║                                                                ║
║  Представьте два диска, вложенных друг в друга:                ║
║                                                                ║
║         🔵 ВНЕШНИЙ ДИСК (Plaintext alphabet):                  ║
║         ╭───────────────────────────────────╮                  ║
║     Z →│  A B C D E F G H I J K L M N O P  │← P                ║
║    Y  ↗│                                   │↖  Q                ║
║   X   │     [ЦЕНТР]     [ПОВОРОТ]         │   R                ║
║  W    │                                   │    S                ║
║   V   │  Z Y X W V U T S R Q              │   T                ║
║     U →│                                   │← U                ║
║         ╰───────────────────────────────────╯                  ║
║                                                                ║
║         🔴 ВНУТРЕННИЙ ДИСК (Ciphertext, shift=3):              ║
║         ╭───────────────────────────────────╮                  ║
║     C →│  D E F G H I J K L M N O P Q R S  │← S                ║
║    B  ↗│                                   │↖  T                ║
║   A   │                                   │   U                ║
║  Z    │                                   │    V                ║
║   Y   │  C B A Z Y X W V U T              │   W                ║
║     X →│                                   │← X                ║
║         ╰───────────────────────────────────╯                  ║
║                                                                ║
║  📍 ШИФРОВАНИЕ (shift=3):                                      ║
║     Внешний A → Внутренний D  (сдвиг на 3 вправо)             ║
║     Внешний H → Внутренний K                                  ║
║     Внешний E → Внутренний H                                  ║
║                                                                ║
║     "HELLO" → "KHOOR"                                          ║
║                                                                ║
║  📍 ДЕШИФРОВАНИЕ:                                              ║
║     Повернуть диск обратно на 3 позиции                       ║
║     (или на 26-3=23 вперёд — тот же эффект!)                  ║
║                                                                ║
║  💡 КАК ЦЕЗАРЬ ИСПОЛЬЗОВАЛ ЭТО:                                ║
║     Римский полководец носил два кольца:                      ║
║     - Одно с plaintext алфавитом                              ║
║     - Другое с сдвинутым (shift=3)                            ║
║                                                                ║
║     Повернул → зашифровал письмо → отправил курьеру           ║
║     Получатель: повернул свой диск → расшифровал              ║
║                                                                ║
║  ⚠️ УЯЗВИМОСТЬ:                                                ║
║     Всего 25 возможных положений диска → брутфорс легко!      ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝
```

**Метафора работает потому что:**
- 🔄 **Поворот диска** = математический сдвиг `(c + shift) % 26`
- 🔒 **Секрет** = угол поворота (shift)
- 🔓 **Взлом** = перебрать все 25 углов (брутфорс)

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

## 🎭 06:30 — SMS от Dr. Leonard Weiss (ETH Zürich)

**06:15** — message1.enc взломан (XOR key 0x42)! Caesar теория изучена...

**06:30** — SMS с швейцарского номера.

```
📱 Dr. Leonard Weiss (ETH Zürich, Professor)

"Eva передала ваш номер. Леонард Вайс, профессор 
криптографии ETH.

Chi-squared test для автоопределения shift:

χ² = Σ (observed - expected)² / expected

Для каждого shift (0-25):
  1. Расшифруйте
  2. Подсчитайте частоты
  3. Вычислите χ²
  4. Минимальный χ² = правильный shift!

Ближе к английскому = меньше χ².

— Dr. Weiss"
```

**🔓 ПРОФИЛЬ:** Dr. Leonard Weiss — Professor ETH Zürich, PhD Stanford, наставник Eva, автор "Modern Cryptanalysis" (2018).

---

## 📚 THEORY BLOCK 3: Frequency Analysis (06:30 - 09:00)

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

## 🎭 07:45 — Звонок Dmitry Orlov + 09:00 Email "The Architect"

**07:30** — message2.enc взломан (ROT13)! Frequency analysis изучен...

**07:45** — Звонок от Dmitry Orlov (старый друг, embedded engineer).

```
📞 Dmitry Orlov (Moscow, Embedded Security)

"Братан! XOR в embedded везде - авионика, IoT, военные.
Одна инструкция процессора!

Known-plaintext attack:
  key = known_plaintext XOR ciphertext

Это взломало WEP в Wi-Fi (2001).

Multi-layer шифрование? Расшифровка в ОБРАТНОМ порядке!
Caesar → XOR зашифровано = XOR → Caesar расшифровка.
Как матрёшка: снимаешь с внешнего слоя.

Удачи!"
```

**09:00** — Подозрительный email с .onion адреса.

```
📧 The Architect (Tor hidden service) ⚠️

"Слишком быстро, Морфеус.

Мы наблюдаем:
  04:12 Eva, 06:30 Dr. Weiss, 07:45 Dmitry

GENESIS не единственный кто знает о вашей операции.

Blockchain - только начало.
Координаты приведут туда, куда МЫ хотим.

Viktor думает что контролирует ситуацию.
Он ошибается.

Добро пожаловать в Новую Эру.
— The Architect"
```

**🚨 НОВАЯ УГРОЗА:** "The Architect" — представитель "Новой Эры", следит за операцией, знает всё. Связь с Крыловым? (TBD в Ep15-16)

**Вы сообщаете V. о новой угрозе и продолжаете работу...**

---

## 🎯 МИССИЯ

### 🏋️ Промежуточная задача: Frequency Analyzer ⭐⭐☆☆☆

**ПЕРЕД основной миссией потренируйтесь на частотном анализе!**

Создайте инструмент `frequency_analyzer` для подсчёта частот букв и автоматического определения Caesar shift.

**Файл:** `artifacts/frequency_analyzer.c`

**Что реализовать:**
```c
// 1. Подсчёт частот букв A-Z (игнорировать регистр)
void count_frequency(const char *text, int freq[26], int *total);

// 2. Печать ASCII-гистограммы
void print_histogram(int freq[26], int total);

// 3. Поиск самой частой буквы
char find_most_frequent(int freq[26]);

// 4. Автоопределение Caesar shift (предполагаем E — самая частая)
int detect_caesar_shift(int freq[26]);
```

**Тестовые файлы:**
- `tests/sample_english.txt` — обычный английский текст
- `tests/sample_caesar.txt` — зашифровано Caesar shift=7

**Ожидаемый вывод:**
```
═══ FREQUENCY ANALYSIS ═══
Total letters: 623

E: ██████████████ 12.7% (79)
T: ██████████     9.5%  (59)
A: █████████      8.0%  (50)
...

Most frequent letter: E
Detected Caesar shift: 0
```

**Для `sample_caesar.txt` (shift=7):**
```
Most frequent letter: L  (E+7)
Detected Caesar shift: 7
💡 Hint: Try decrypting with shift=7
```

**Компиляция и запуск:**
```bash
cd artifacts
gcc -Wall -Wextra -std=c11 frequency_analyzer.c -o frequency_analyzer
./frequency_analyzer ../tests/sample_english.txt
./frequency_analyzer ../tests/sample_caesar.txt
```

**Тест:**
```bash
cd tests
./test_frequency.sh
```

**Решение:** `solution/frequency_analyzer_solution.c` (смотреть только если застряли!)

---

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

## 🤔 Контрольные вопросы (интегрированы в сюжет миссии)

**💡 Эти вопросы помогут проверить понимание материала через призму сюжета Episode 13.**

### Базовое понимание (5 вопросов):

<details>
<summary><strong>1. В 04:47 вы использовали known-plaintext attack для message1.enc. Почему знание начала сообщения ("URGENT:") позволяет найти ключ XOR?</strong></summary>

**Ответ:**

XOR шифрование симметрично: `ciphertext XOR key = plaintext`

Если мы знаем:
- `plaintext` = "URGENT:" (первые 7 байтов)
- `ciphertext` = первые 7 байтов файла message1.enc

То можем найти ключ:
```
key = plaintext XOR ciphertext
```

Пример:
```
plaintext[0] = 'U' = 0x55
ciphertext[0] = 0x17 (из файла)
key = 0x55 XOR 0x17 = 0x42
```

Проверка: `0x17 XOR 0x42 = 0x55` = 'U' ✅

Это называется **known-plaintext attack** — один из самых эффективных методов взлома симметричных шифров.

</details>

<details>
<summary><strong>2. Ключ 0x42 для XOR — это "Answer to Everything" (Hitchhiker's Guide). Почему XOR симметричен? (encrypt = decrypt)</strong></summary>

**Ответ:**

XOR обладает уникальным свойством **самоинверсии**:

```
A XOR B XOR B = A
```

Доказательство:
```
data = 'H' = 01001000
key  = 0x42 = 01000010

Шифрование:
encrypted = data XOR key = 01001000 XOR 01000010 = 00001010

Дешифрование (тот же ключ!):
decrypted = encrypted XOR key = 00001010 XOR 01000010 = 01001000 = 'H'
```

Поэтому одна функция `xor_cipher()` работает и для шифрования, и для дешифрования!

**0x42 = 66 = ASCII 'B'** — отсылка к числу 42 из "Hitchhiker's Guide to the Galaxy" (Answer to the Ultimate Question).

</details>

<details>
<summary><strong>3. В 05:45 frequency analysis показал, что 'K' — самая частая буква в message2.enc. Вы предположили 'E', но это была ошибка. Почему 'T' оказался правильным?</strong></summary>

**Ответ:**

В английском языке частоты букв:
1. **E** — 12.7% (самая частая)
2. **T** — 9.1% (вторая по частоте)
3. **A** — 8.2%

Но message2.enc оказался зашифрован **ROT13** (shift=13), где:
- 'A' → 'N'
- 'E' → 'R'
- **'T' → 'G'** (но частота сохраняется!)

Если в шифротексте 'K' самая частая, это может быть:
- Вариант 1: E + shift = K → shift = 6
- Вариант 2: T + shift = K → shift = 17
- **Вариант 3: ROT13** где второй по частоте 'T' → 'G', но из-за особенностей текста 'K' оказалась чаще

Мораль: **частотный анализ — это эвристика**, не всегда работает с первого раза. Нужно пробовать топ-3 варианта!

</details>

<details>
<summary><strong>4. ROT13 (shift=13) симметричен: encrypt = decrypt. Почему это работает только для shift=13?</strong></summary>

**Ответ:**

Alphabet имеет 26 букв. ROT13 сдвигает на **ровно половину** алфавита:

```
A B C D E F G H I J K L M | N O P Q R S T U V W X Y Z
↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓   ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
N O P Q R S T U V W X Y Z | A B C D E F G H I J K L M
```

Шифрование: A + 13 = N  
Дешифрование: N + 13 = A (wrap around!)

Математика:
```
(x + 13) mod 26 + 13 mod 26 = (x + 26) mod 26 = x
```

Работает **только** для shift=13, потому что 13 = 26/2.

Для других shift нужны разные функции encrypt/decrypt.

</details>

<details>
<summary><strong>5. В message3.enc использовано multi-layer шифрование (Caesar + XOR). В каком порядке нужно расшифровывать? Почему в обратном?</strong></summary>

**Ответ:**

**Шифрование (прямой порядок):**
```
plaintext → Caesar(shift=7) → XOR(0x5A) → ciphertext
```

**Дешифрование (обратный порядок):**
```
ciphertext → XOR(0x5A) → Caesar_decrypt(shift=7) → plaintext
```

Почему обратный?

Представьте матрёшку:
1. Кладём сообщение (plaintext)
2. Оборачиваем в Caesar (первый слой)
3. Оборачиваем в XOR (второй слой)

Чтобы достать сообщение:
1. Снимаем XOR (внешний слой)
2. Снимаем Caesar (внутренний слой)

В коде:
```c
// Шифрование
caesar_encrypt(data, 7);
xor_cipher(data, len, 0x5A);

// Дешифрование (обратный порядок!)
xor_cipher(data, len, 0x5A);  // сначала XOR
caesar_decrypt(data, 7);       // потом Caesar
```

</details>

---

### Практическое применение (5 вопросов):

<details>
<summary><strong>6. Вы брутфорсили XOR ключ в диапазоне 0x20-0x7E. Сколько это вариантов? Почему не 0x00-0xFF?</strong></summary>

**Ответ:**

Диапазон `0x20-0x7E`:
```
0x7E - 0x20 + 1 = 126 - 32 + 1 = 95 вариантов
```

Почему не `0x00-0xFF` (256 вариантов)?

Диапазон `0x20-0x7E` = **ASCII printable characters**:
- 0x20 = пробел ' '
- 0x7E = тильда '~'
- Включает: буквы, цифры, знаки препинания

Ключи вне этого диапазона дадут **нечитаемый** plaintext с control characters (0x00-0x1F) и extended ASCII (0x80-0xFF).

Для сообщений типа "URGENT: ..." используют printable keys для удобства (можно ввести с клавиатуры).

Брутфорс 95 вариантов = мгновенно (микросекунды)!

</details>

<details>
<summary><strong>7. Chi-squared test для автоопределения Caesar shift. Как он работает? Что сравнивается?</strong></summary>

**Ответ:**

**Chi-squared (χ²) test** сравнивает **ожидаемые** и **наблюдаемые** частоты букв.

Алгоритм:
```
Для каждого shift от 0 до 25:
  1. Расшифровать текст с этим shift
  2. Подсчитать частоты букв
  3. Вычислить χ² = Σ (observed - expected)² / expected
  4. Shift с минимальным χ² → правильный!
```

Пример:
```c
double chi_squared(int freq[26], int total) {
    // Ожидаемые частоты английского языка (%)
    double expected[26] = {
        8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.2,
        0.8, 4.0, 2.4, 6.7,  7.5, 1.9, 0.1, 6.0, 6.3, 9.1,
        2.8, 1.0, 2.4, 0.2,  2.0, 0.1
    };
    
    double chi2 = 0.0;
    for (int i = 0; i < 26; i++) {
        double observed = 100.0 * freq[i] / total;
        double exp = expected[i];
        chi2 += (observed - exp) * (observed - exp) / exp;
    }
    return chi2;
}
```

Меньше χ² → ближе к английскому → правильный shift!

</details>

<details>
<summary><strong>8. Частота 'E' в английском = 12.7%. Если в шифротексте 'K' = 12.5%, какой сдвиг?</strong></summary>

**Ответ:**

```
Если E (4) сдвинулась в K (10):
shift = K - E = 10 - 4 = 6
```

Проверка:
```
E (4) + 6 = 10 = K ✅
T (19) + 6 = 25 = Z
A (0) + 6 = 6 = G
```

**Caesar shift = 6**

В коде:
```c
int detect_shift(int freq[26]) {
    char most_freq = find_most_frequent(freq);
    return (most_freq - 'E' + 26) % 26;
}
```

`+ 26` нужен для отрицательных значений (если most_freq < 'E').

</details>

<details>
<summary><strong>9. Vigenère cipher упомянут как улучшение Caesar. В чём разница? Почему сложнее взломать?</strong></summary>

**Ответ:**

**Caesar cipher:** один shift для всего текста
```
Key: 3
"HELLO" → shift each by 3 → "KHOOR"
```

**Vigenère cipher:** многобуквенный ключ, каждая позиция свой shift
```
Key: "ABC" (shifts: 0, 1, 2, 0, 1, 2, ...)

H + 0 = H
E + 1 = F
L + 2 = N
L + 0 = L
O + 1 = P

"HELLO" → "HFNLP"
```

Почему сложнее взломать?

1. **Частотный анализ не работает** напрямую — одна буква шифруется по-разному:
   ```
   "EEE" with key "ABC" → "EFG"  (разные буквы!)
   ```

2. Нужно сначала **определить длину ключа** (Kasiski examination, Index of Coincidence)

3. Затем разбить на подстроки и взломать каждую как Caesar

Для длинного ключа (≈ длина текста) = практически невзламываем без криптоанализа!

</details>

<details>
<summary><strong>10. Known-plaintext attack: если знаем первые 10 байт plaintext и ciphertext, как найти ключ XOR?</strong></summary>

**Ответ:**

```c
// Дано:
unsigned char plaintext[10] = "URGENT: Me";
unsigned char ciphertext[10] = {0x17, 0x37, ...}; // из файла

// Найти ключ:
unsigned char key = plaintext[0] ^ ciphertext[0];

// Проверить на остальных байтах:
for (int i = 1; i < 10; i++) {
    if ((plaintext[i] ^ key) != ciphertext[i]) {
        // Ключ неправильный или шифр сложнее (многобайтовый XOR)
        printf("Key mismatch at position %d\n", i);
    }
}

// Если все совпало → используем key для расшифровки всего файла
```

Этот метод работает **только для single-byte XOR**. 

Для multi-byte XOR (key = "SECRET") нужно знать больше plaintext или использовать другие методы (повторы ключа, статистический анализ).

</details>

---

### Анализ кода (3 вопроса):

<details>
<summary><strong>11. В коде `xor_cipher()` одна функция для encrypt и decrypt. Почему это работает?</strong></summary>

**Ответ:**

```c
void xor_cipher(unsigned char *data, int len, unsigned char key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;  // Одна операция для encrypt и decrypt!
    }
}
```

Работает из-за свойства XOR: **A ⊕ B ⊕ B = A**

```
Encrypt: data ⊕ key = encrypted
Decrypt: encrypted ⊕ key = (data ⊕ key) ⊕ key = data ⊕ (key ⊕ key) = data ⊕ 0 = data
```

Bonus: XOR коммутативен и ассоциативен!

Поэтому можно использовать одну функцию:
```c
xor_cipher(message, len, 0x42);  // encrypt
xor_cipher(message, len, 0x42);  // decrypt (same function!)
```

</details>

<details>
<summary><strong>12. Функция `brute_force_caesar()` проверяет "читаемость" текста (printable >80%). Почему не 100%?</strong></summary>

**Ответ:**

В реальных текстах могут быть:
- **Знаки препинания:** `. , ! ? ; :`
- **Цифры:** `0-9`
- **Спецсимволы:** `@ # $ %`
- **Переводы строк:** `\n \r`

Которые НЕ шифруются Caesar cipher (остаются как есть).

Пример:
```
"HELLO, WORLD! 123" → Caesar shift=3 → "KHOOR, ZRUOG! 123"
                                          ^^^^^       ^^^^
                                       не меняются
```

Поэтому `printable >= 80%` — разумный порог:
- Если >80% printable → вероятно правильный shift
- Если <80% → много мусора → неправильный shift

В коде:
```c
int printable_count = 0;
for (int i = 0; i < len; i++) {
    if (decrypted[i] >= 32 && decrypted[i] <= 126) {
        printable_count++;
    }
}

if (printable_count >= len * 0.8) {  // 80%
    printf("Possible plaintext: %s\n", decrypted);
}
```

</details>

<details>
<summary><strong>13. Почему `hash("MOONLIGHT") != hash("MOONLIGH")` (лавинный эффект) важен для криптографии?</strong></summary>

**Ответ:**

**Лавинный эффект (Avalanche Effect):** малое изменение входа → большое изменение выхода.

Пример с djb2:
```c
hash("MOONLIGHT") = 0x7C8DF4A2
hash("MOONLIGH")  = 0x2F91B6E5  // ~50% бит изменились!
```

Почему важно?

1. **Обнаружение изменений:**
   ```
   File before: hash = 0x7C8DF4A2
   File after:  hash = 0x2F91B6E5  → файл изменён!
   ```

2. **Blockchain integrity (Episode 14):**
   ```
   Block data изменён на 1 байт → hash блока полностью другой
   → следующий блок обнаруживает несовпадение prev_hash
   → цепь разорвана!
   ```

3. **Password hashing:**
   ```
   password1: "Secret123" → hash1
   password2: "Secret124" → hash2 (очень отличается!)
   ```
   Злоумышленник не может угадать пароль по похожести хешей.

Без лавинного эффекта: `hash("password") ≈ hash("passwore")` → можно подобрать пароль!

</details>

---

### Сюжет и контекст (2 вопроса):

<details>
<summary><strong>14. В 09:23 вы обнаружили имя "GENESIS" в message3.enc. Кто это? (Hint: Genesis Block)</strong></summary>

**Ответ:**

**GENESIS** — кодовое имя лидера организации, упомянутое в расшифрованном сообщении:

```
"CRITICAL: Operation MOONLIGHT phase 2 begins Dec 20.
 All agents converge on primary location.
 Blockchain ledger contains coordinates.
 — GENESIS"
```

**Отсылка к криптовалютам:**

**Genesis Block** = первый блок в blockchain:
- Bitcoin Genesis Block: January 3, 2009
- Создан Satoshi Nakamoto
- Содержит сообщение: "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"

Кодовое имя "GENESIS" намекает что:
1. Это **основатель** операции MOONLIGHT
2. Возможно связан с **blockchain** (Episode 14!)
3. Начал операцию (genesis = начало)

В Episode 14 вы увидите blockchain с Genesis Block и узнаете больше...

</details>

<details>
<summary><strong>15. Financial records $15.7M упомянуты в message2.enc. Почему именно эта сумма? (Hint: Bitcoin Pizza Day)</strong></summary>

**Ответ:**

**$15.7M** — отсылка к **Bitcoin Pizza Day** (22 мая 2010).

История:
- Laszlo Hanyecz купил 2 пиццы Papa John's за **10,000 BTC**
- На тот момент (2010): $41 (цена 2 пиццы)
- В декабре 2020 (время Episode 13): 10,000 BTC ≈ **$15.7 million**!

Самая дорогая пицца в истории! 🍕💰

Это **первая документированная покупка** за Bitcoin в реальном мире → символ перехода криптовалюты из теории в практику.

В контексте Episode 13:
- Сумма $15.7M в blockchain → связь с криптовалютами
- Episode 14 будет про blockchain verification
- Операция MOONLIGHT использует crypto-финансы

**Easter Egg:** Каждый год 22 мая криптосообщество празднует Bitcoin Pizza Day!

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

**Но что-то не даёт покоя...**

---

## ⚠️ CLIFFHANGER

```
╔══════════════════════════════════════════════════════════╗
║  🚨 ТРЕВОЖНЫЕ ВОПРОСЫ                                    ║
╠══════════════════════════════════════════════════════════╣
║                                                          ║
║  Email от "The Architect" в 09:00:                      ║
║  "Мы наблюдаем за вашим прогрессом..."                  ║
║                                                          ║
║  ❓ КТО такой "The Architect"?                          ║
║  ❓ Что за "Новая Эра" (организация)?                   ║
║  ❓ Связь с Крыловым? С агентами Z.?                    ║
║  ❓ Как они знают о ВСЕХ ваших контактах?               ║
║  ❓ Почему "координаты приведут туда, куда МЫ хотим"?   ║
║                                                          ║
║  🎯 GENESIS упомянут в message3.enc                      ║
║     → Кто это? Лидер какой организации?                 ║
║                                                          ║
║  💰 $15.7M в blockchain                                  ║
║     → Отсылка к Bitcoin Pizza Day                       ║
║     → Crypto-финансы критичны для операции              ║
║                                                          ║
║  📅 Dec 18-20, 2020                                      ║
║     → Координаты на Красной площади?                    ║
║     → Viktor появится ЛИЧНО? (первая встреча!)          ║
║                                                          ║
║  ⏰ Episode 13 завершён                                  ║
║     НО опасность только нарастает...                    ║
║                                                          ║
║  🔜 Episode 14: Blockchain & Hashes                      ║
║     Верификация ledger + поиск координат                ║
║     Eva поможет с blockchain expertise                  ║
║     IP 185.220.101.47 появится снова! (Season 2 link!)  ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

**Впереди — самая сложная проверка. Blockchain не прощает ошибок...**

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