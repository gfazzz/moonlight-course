# Episode 13: Crypto Basics 🔑

> "Криптография — это искусство писать или решать коды." — Дэвид Кан

## 📋 Описание

После успешного анализа сетевого трафика в Season 3, **V.** передает вам флешку с перехваченными сообщениями. Они зашифрованы простым, но эффективным методом. Ваша задача — написать дешифратор.

**В этом эпизоде вы научитесь:**
- Работать с побитовыми операциями (XOR, AND, OR, shift)
- Реализовывать шифр Caesar
- Проводить частотный анализ текста
- Брутфорсить простые шифры

---

## 🎯 Цели обучения

### Теория
- **XOR шифрование:** принцип работы, свойства XOR
- **Шифр Caesar:** сдвиг по алфавиту, ротация
- **Частотный анализ:** статистика букв в английском языке
- **Криптоанализ:** методы взлома простых шифров

### Практика
- Реализация XOR-шифрования/дешифрования
- Брутфорс Caesar cipher
- Детектирование языка по частотам
- Работа с бинарными данными

---

## 📖 Теоретическая часть

### 1. XOR шифрование

**XOR (исключающее ИЛИ)** — простейшая криптографическая операция.

**Таблица истинности:**
```
A | B | A XOR B
--|---|--------
0 | 0 |   0
0 | 1 |   1
1 | 0 |   1
1 | 1 |   0
```

**Свойства XOR:**
- `A XOR 0 = A` (нейтральный элемент)
- `A XOR A = 0` (самоинверсия)
- `A XOR B XOR B = A` (обратимость)

**Пример шифрования:**
```c
char message = 'A';      // 65 (0x41)
char key = 'K';          // 75 (0x4B)
char encrypted = message ^ key;  // 0x41 ^ 0x4B = 0x0A

char decrypted = encrypted ^ key;  // 0x0A ^ 0x4B = 0x41 ('A')
```

**Код:**
```c
void xor_encrypt(char *data, int len, char key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// Дешифрование — та же функция!
void xor_decrypt(char *data, int len, char key) {
    xor_encrypt(data, len, key);  // XOR обратим
}
```

---

### 2. Шифр Caesar

**Caesar Cipher** — шифр подстановки со сдвигом по алфавиту.

**Принцип:**
```
Plaintext:  HELLO
Shift: 3
Ciphertext: KHOOR

H -> K (H + 3)
E -> H (E + 3)
L -> O (L + 3)
...
```

**Реализация:**
```c
char caesar_encrypt(char c, int shift) {
    if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + shift) % 26;
    }
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + shift) % 26;
    }
    return c;  // не буква
}

char caesar_decrypt(char c, int shift) {
    return caesar_encrypt(c, 26 - shift);  // обратный сдвиг
}
```

**Брутфорс:**
```c
void brute_force_caesar(const char *ciphertext) {
    for (int shift = 0; shift < 26; shift++) {
        printf("Shift %2d: ", shift);
        for (int i = 0; ciphertext[i]; i++) {
            putchar(caesar_decrypt(ciphertext[i], shift));
        }
        printf("\n");
    }
}
```

---

### 3. Частотный анализ

Буквы в английском языке имеют разную частоту:

| Буква | Частота | Буква | Частота |
|-------|---------|-------|---------|
| E     | 12.7%   | T     | 9.1%    |
| A     | 8.2%    | O     | 7.5%    |
| I     | 7.0%    | N     | 6.7%    |

**Применение:**
1. Подсчитываем частоты букв в шифротексте
2. Сравниваем с эталонными частотами
3. Находим наиболее вероятный сдвиг

```c
void frequency_analysis(const char *text) {
    int freq[26] = {0};
    int total = 0;
    
    // Подсчет
    for (int i = 0; text[i]; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            freq[text[i] - 'A']++;
            total++;
        }
    }
    
    // Вывод
    for (int i = 0; i < 26; i++) {
        printf("%c: %.2f%%\n", 'A' + i, 
               100.0 * freq[i] / total);
    }
}
```

---

## 🎬 Сюжет

**Дата:** 15 декабря, 23:47  
**Локация:** Заброшенный склад на окраине города

Вы встречаетесь с **V.** в условленном месте. Он передает вам флешку:

> **V.:** "Наша команда перехватила сообщения между агентами организации. Они используют примитивное шифрование — вероятно, XOR или что-то подобное. Времени мало. Нужно узнать, что они планируют."

Вы подключаете флешку. На ней три файла:

1. `message1.enc` — короткое сообщение (XOR?)
2. `message2.enc` — длинный текст (Caesar?)
3. `key.hint` — подсказка: "The answer is in the frequency"

**Миссия:** Расшифровать все сообщения и доложить **V.** о планах организации.

---

## 💻 Задания

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

## 🔍 Hints

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

## ✅ Проверка

### Запуск тестов

```bash
cd episode-13-crypto-basics
make test
```

**Ожидаемый результат:**
```
Testing XOR decryption...         [PASS]
Testing Caesar cipher...           [PASS]
Testing multi-layer decryption...  [PASS]

All tests passed! ✓
```

### Финальная проверка

```bash
./crypto_tool decrypt message1.enc
./crypto_tool decrypt message2.enc
./crypto_tool decrypt message3.enc
```

---

## 🎓 Дополнительные материалы

### Статьи
- [XOR Cipher](https://en.wikipedia.org/wiki/XOR_cipher) — Wikipedia
- [Caesar Cipher](https://en.wikipedia.org/wiki/Caesar_cipher) — Wikipedia
- [Frequency Analysis](https://en.wikipedia.org/wiki/Frequency_analysis) — Wikipedia

### Инструменты
- [CyberChef](https://gchq.github.io/CyberChef/) — онлайн криптоанализ
- [dCode](https://www.dcode.fr/en) — инструменты для взлома шифров

### Книги
- **Applied Cryptography** — Bruce Schneier (глава 1)
- **The Code Book** — Simon Singh (история криптографии)

---

## 🏆 Бонусные задания

### Бонус 1: Vigenère Cipher ⭐⭐⭐⭐⭐

Реализуйте взлом шифра Vigenère (многобуквенный ключ).

**Подсказка:** Метод Kasiski, индекс совпадений.

### Бонус 2: Visual Crypto ⭐⭐⭐⭐☆

Создайте ASCII-визуализацию частотного анализа:

```
A: ████████ 8.2%
B: ██ 1.5%
C: ███ 2.8%
...
E: ████████████ 12.7%
```

### Бонус 3: Timing Attack ⭐⭐⭐⭐⭐

Реализуйте защиту от timing attack при сравнении паролей.

---

## 🐛 Типичные ошибки

### Ошибка 1: Неправильная работа с unsigned
```c
// ❌ WRONG
char data = 0xFF;
data ^= key;  // может дать неправильный результат

// ✅ RIGHT
unsigned char data = 0xFF;
data ^= key;
```

### Ошибка 2: Выход за границы алфавита
```c
// ❌ WRONG
char encrypted = 'Z' + 3;  // выходит за 'Z'

// ✅ RIGHT
char encrypted = 'A' + ('Z' - 'A' + 3) % 26;
```

### Ошибка 3: Игнорирование регистра
```c
// ❌ WRONG
if (c >= 'A' && c <= 'Z') {
    // только верхний регистр
}

// ✅ RIGHT
if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
    // оба регистра
}
```

---

## 📊 Что вы узнали

После прохождения Episode 13 вы умеете:

- ✅ Использовать побитовые операции для шифрования
- ✅ Реализовывать классические шифры (XOR, Caesar)
- ✅ Проводить частотный анализ текста
- ✅ Взламывать простые шифры методом брутфорса
- ✅ Работать с бинарными данными в C

---

## 🔜 Следующий эпизод

В **Episode 14: Hash Functions** вы научитесь:
- Реализовывать хеш-функции
- Проверять целостность данных
- Детектировать изменения в файлах
- Создавать цифровые подписи

Расшифрованные сообщения укажут на архив с критически важными данными. Но некоторые файлы повреждены...

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 13 of 32*

[← Episode 12](../../season-3-networks/episode-12-encrypted-communications/) | [Season 4](../README.md) | [Episode 14 →](../episode-14-hash-functions/)

</div>
