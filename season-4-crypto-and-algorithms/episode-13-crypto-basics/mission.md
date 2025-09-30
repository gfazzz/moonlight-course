# 🔐 Mission Brief: Crypto Basics

**CLASSIFIED - MOONLIGHT OPERATION**  
**Episode 13 - Crypto Basics**

---

## 📋 Situation Report

**Date:** 15 декабря, 23:47  
**Location:** Заброшенный склад, промзона  
**Contact:** V. (handler)

Наша команда перехватила зашифрованные сообщения между агентами организации. Анализ показал использование примитивных криптографических методов — вероятно, XOR и Caesar cipher.

**Критичность:** ВЫСОКАЯ  
**Дедлайн:** 48 часов

---

## 🎯 Mission Objectives

### Primary Objectives

1. **Decrypt XOR-encrypted message** (message1.enc)
   - Определить ключ шифрования (single byte XOR)
   - Извлечь plaintext
   - Подтвердить расшифровку (должно начинаться с "URGENT:")

2. **Break Caesar Cipher** (message2.enc)
   - Использовать частотный анализ или брутфорс
   - Найти shift value
   - Извлечь plaintext

3. **Multi-layer Decryption** (message3.enc)
   - Расшифровать комбинированное шифрование (Caesar + XOR)
   - Определить последовательность применения шифров
   - Извлечь финальное сообщение

### Secondary Objectives

- Создать универсальный инструмент `crypto_tool` для будущих операций
- Документировать найденные ключи и методы
- Сохранить все артефакты для дальнейшего анализа

---

## 📦 Provided Assets

### Files on USB Drive

```
episode-13-crypto-basics/
├── message1.enc          # XOR-encrypted (binary)
├── message2.enc          # Caesar cipher (text)
├── message3.enc          # Multi-layer encryption
└── key.hint              # Подсказка: "The answer is in the frequency"
```

### Starter Code

```c
// starter.c - ваша точка входа
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TODO: Реализовать XOR decryption
void xor_decrypt_single_byte(unsigned char *data, int len, unsigned char key) {
    // YOUR CODE HERE
}

// TODO: Брутфорс XOR ключа
void brute_force_xor(unsigned char *data, int len) {
    // YOUR CODE HERE
}

// TODO: Caesar cipher decryption
void caesar_decrypt(char *text, int shift) {
    // YOUR CODE HERE
}

// TODO: Автоматическое определение Caesar shift
int detect_caesar_shift(const char *ciphertext) {
    // YOUR CODE HERE
}

// TODO: Multi-layer decryption
void multi_layer_decrypt(unsigned char *data, int len, int caesar_shift, unsigned char xor_key) {
    // YOUR CODE HERE
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command> <file>\n", argv[0]);
        fprintf(stderr, "Commands: xor, caesar, multi\n");
        return 1;
    }
    
    // TODO: Реализовать command-line interface
    
    return 0;
}
```

---

## 🔍 Intelligence

### Known Information

**XOR Encryption:**
- Ключ: single byte (0x20 - 0x7E ASCII printable range)
- Длина сообщения: ~200 bytes
- Подпись: начинается с "URGENT:"

**Caesar Cipher:**
- Shift: 0-25 (стандартный диапазон)
- Язык: английский
- Сохранены пробелы и знаки препинания

**Multi-layer:**
- Hint: "First rotate, then XOR with 0x5A"
- Применены последовательно: сначала Caesar, затем XOR
- Расшифровка в обратном порядке!

### Frequency Analysis Reference

Самые частые буквы в английском языке:
```
E - 12.7%    T - 9.1%     A - 8.2%
O - 7.5%     I - 7.0%     N - 6.7%
```

---

## 📝 Operational Guidelines

### Step 1: Reconnaissance

```bash
# Проверьте файлы
ls -lh *.enc

# Посмотрите содержимое (осторожно, может быть бинарное!)
file message1.enc
file message2.enc
file message3.enc

# Прочитайте hint
cat key.hint
```

### Step 2: XOR Decryption

```c
// Алгоритм брутфорса
for (unsigned char key = 0x20; key <= 0x7E; key++) {
    // 1. Расшифровать с текущим ключом
    // 2. Проверить на "читаемость" (printable chars)
    // 3. Проверить сигнатуру "URGENT:"
    // 4. Если совпало — вывести результат
}
```

### Step 3: Caesar Cipher

```c
// Метод 1: Брутфорс
for (int shift = 0; shift < 26; shift++) {
    // Расшифровать и вывести
}

// Метод 2: Частотный анализ
// 1. Подсчитать частоты букв
// 2. Найти самую частую букву
// 3. Предположить, что это 'E'
// 4. Вычислить shift
```

### Step 4: Multi-layer

```c
// Порядок расшифровки (обратный применению):
// 1. Сначала XOR (последнее применённое)
xor_decrypt_single_byte(data, len, 0x5A);

// 2. Затем Caesar (первое применённое)
caesar_decrypt(data, detected_shift);
```

---

## ✅ Success Criteria

### Mission Complete When:

- ✅ All three messages decrypted successfully
- ✅ Plaintexts are readable English text
- ✅ Message 1 starts with "URGENT:"
- ✅ All tests pass (`make test`)

### Expected Output

```
$ ./crypto_tool xor message1.enc
Trying XOR key: 0x42 ('B')
[SUCCESS] Decrypted with key 0x42:

URGENT: Operation compromised. Extraction point changed to...
```

---

## 🎯 Deliverables

1. **crypto_tool executable**
   - XOR decryption
   - Caesar cipher breaker
   - Multi-layer support

2. **Decrypted messages** (save to `artifacts/`)
   - message1_decrypted.txt
   - message2_decrypted.txt
   - message3_decrypted.txt

3. **Report** (artifacts/analysis.txt)
   - Найденные ключи
   - Методы взлома
   - Содержание сообщений

---

## ⚠️ Security Notes

**IMPORTANT:**
- Эти методы УЧЕБНЫЕ. Не используйте в реальных проектах!
- XOR с одним байтом — крайне слабое шифрование
- Caesar cipher взламывается за секунды
- Для реальных задач используйте OpenSSL/libsodium

**Timing Attacks:**
```c
// ❌ VULNERABLE to timing attack
if (strcmp(password, correct_password) == 0) {
    // strcmp останавливается на первом отличии!
}

// ✅ SAFE (constant-time comparison)
int constant_time_compare(const char *a, const char *b, int len) {
    int diff = 0;
    for (int i = 0; i < len; i++) {
        diff |= a[i] ^ b[i];
    }
    return diff == 0;
}
```

---

## 🐛 Troubleshooting

### Problem: "Binary file not readable"

```bash
# Используйте xxd для просмотра hex
xxd message1.enc | head

# Или od
od -A x -t x1z message1.enc | head
```

### Problem: "Wrong decryption output"

```c
// Убедитесь, что используете unsigned char!
unsigned char data[256];  // ✅ RIGHT
char data[256];           // ❌ WRONG (sign extension issues)
```

### Problem: "Caesar shift detection fails"

```c
// Не забудьте нормализовать к верхнему регистру
char c_upper = toupper(c);

// Учитывайте только буквы, игнорируйте пробелы/знаки
if (!isalpha(c)) continue;
```

---

## 📚 Reference Materials

### XOR Truth Table
```
A  B  A^B
0  0   0
0  1   1
1  0   1
1  1   0
```

### Caesar Cipher Example
```
Plaintext:  HELLO WORLD
Shift: 3
Ciphertext: KHOOR ZRUOG
```

### English Letter Frequencies
```
E 12.7%  T 9.1%   A 8.2%   O 7.5%   I 7.0%
N 6.7%   S 6.3%   H 6.1%   R 6.0%   D 4.3%
```

---

## 🏆 Bonus Challenges

### Bonus 1: Visualization
Создайте ASCII-график частот букв:
```
A: ████████ (8.2%)
B: ██ (1.5%)
C: ███ (2.8%)
...
```

### Bonus 2: Vigenère Cipher
Попробуйте взломать Vigenère cipher (многобуквенный ключ).

### Bonus 3: Performance
Оптимизируйте брутфорс для работы с файлами >10 MB.

---

## 📞 Emergency Contact

**Handler:** V.  
**Secure Channel:** Encrypted IRC #moonlight-ops  
**Dead Drop:** Old warehouse, coordinates in Episode 01

**В случае компрометации:**
1. Уничтожить все артефакты
2. Использовать аварийный протокол EXODUS
3. Связаться через резервный канал

---

## ⏭️ Next Steps

После успешного выполнения миссии:

1. **Сохраните все артефакты** в `artifacts/`
2. **Запустите тесты:** `make test`
3. **Доложите V.** о результатах
4. **Перейдите к Episode 14:** Hash Functions

Расшифрованные сообщения укажут на архив с критически важными данными. Но некоторые файлы могут быть повреждены...

---

<div align="center">

**MOONLIGHT OPERATION - CLASSIFIED**  
*"In crypto we trust"*

[← Season 3](../../season-3-networks/episode-12-encrypted-communications/) | [Episode 14 →](../episode-14-hash-functions/)

</div>
