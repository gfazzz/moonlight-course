# Episode 14: Blockchain & Hash Functions 🔗

> "The blockchain is an incorruptible digital ledger of economic transactions that can be programmed to record not just financial transactions but virtually everything of value." — Don & Alex Tapscott

## 📋 Описание

После успешной расшифровки сообщений в Episode 13, **V.** передает вам флешку с архивом операции MOONLIGHT. Данные хранятся в простой blockchain-системе для защиты от подделки. Ваша задача — понять принципы blockchain, реализовать верификацию и найти поврежденные блоки.

**В этом эпизоде вы научитесь:**
- Реализовывать криптографические хеш-функции
- Создавать простую blockchain-систему
- Проверять целостность данных через цепочку хешей
- Понимать Proof-of-Work (mining)

---

## 🎯 Цели обучения

### Теория
- **Хеш-функции:** свойства, коллизии, криптостойкость, лавинный эффект
- **Blockchain:** структура блока, цепочка хешей, immutability
- **Proof-of-Work:** mining, difficulty adjustment
- **Integrity checking:** верификация через хеши, Merkle trees (концепт)

### Практика
- Реализация хеш-функции (djb2)
- Создание blockchain структуры данных
- Верификация целостности цепи
- Простой mining (Proof-of-Work)

---

## 📖 Теоретическая часть

### 1. Криптографические хеш-функции

**Хеш-функция** преобразует данные произвольной длины в фиксированную строку (digest).

**Свойства криптографических хешей:**
1. **Детерминированность:** одинаковый вход → одинаковый хеш
2. **Быстрое вычисление:** O(n) по размеру данных
3. **Лавинный эффект:** малое изменение входа → совершенно другой хеш
4. **Односторонность:** невозможно восстановить данные из хеша
5. **Стойкость к коллизиям:** сложно найти два входа с одним хешем

**Пример: SHA-256**
```
Input:  "MOONLIGHT"
SHA256: 8f4e3c0... (64 hex chars, 256 bits)

Input:  "MOONLIGH"  (одна буква удалена)
SHA256: 2a9d1f7... (совершенно другой хеш!)
```

**Простая хеш-функция (djb2):**
```c
unsigned long hash_djb2(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}
```

---

### 2. Blockchain структура

**Блок содержит:**
```c
typedef struct Block {
    int index;              // Номер блока
    time_t timestamp;       // Время создания
    char data[256];         // Полезные данные
    unsigned long prev_hash; // Хеш предыдущего блока
    unsigned long hash;      // Хеш текущего блока
    int nonce;              // Proof-of-work
} Block;
```

**Как вычисляется хеш блока:**
```c
unsigned long calculate_block_hash(Block *block) {
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%d%ld%s%lu%d",
             block->index,
             block->timestamp,
             block->data,
             block->prev_hash,
             block->nonce);
    
    return hash_djb2((unsigned char*)buffer);
}
```

**Цепочка блоков:**
```
Block 0 (Genesis)         Block 1                Block 2
┌─────────────────┐      ┌─────────────────┐    ┌─────────────────┐
│ index: 0        │      │ index: 1        │    │ index: 2        │
│ data: "..."     │      │ data: "..."     │    │ data: "..."     │
│ prev_hash: 0    │◄─────│ prev_hash: 5381 │◄───│ prev_hash: 7892 │
│ hash: 5381      │      │ hash: 7892      │    │ hash: 9034      │
└─────────────────┘      └─────────────────┘    └─────────────────┘
```

---

### 3. Proof of Work (Mining)

**Концепция:** найти nonce, чтобы хеш блока удовлетворял условию (например, начинался с N нулей).

```c
void mine_block(Block *block, int difficulty) {
    char target[65] = {0};
    for (int i = 0; i < difficulty; i++) {
        target[i] = '0';
    }
    
    block->nonce = 0;
    while (1) {
        block->hash = calculate_block_hash(block);
        
        char hash_str[65];
        sprintf(hash_str, "%016lx", block->hash);
        
        if (strncmp(hash_str, target, difficulty) == 0) {
            printf("Block mined! Nonce: %d\n", block->nonce);
            break;
        }
        
        block->nonce++;
    }
}
```

**Сложность mining:**
- Difficulty 1: ~16 попыток
- Difficulty 2: ~256 попыток
- Difficulty 4: ~65536 попыток
- Bitcoin: ~20 ведущих нулей (2^80 попыток!)

---

### 4. Верификация блокчейна

**Проверки:**
1. **Целостность блока:** `hash == calculate_block_hash(block)`
2. **Связь с предыдущим:** `block[i].prev_hash == block[i-1].hash`
3. **Proof-of-work:** хеш удовлетворяет difficulty
4. **Последовательность индексов:** `block[i].index == i`

```c
bool verify_blockchain(Block *chain, int length, int difficulty) {
    for (int i = 0; i < length; i++) {
        // Проверка 1: хеш блока
        unsigned long calculated_hash = calculate_block_hash(&chain[i]);
        if (chain[i].hash != calculated_hash) {
            printf("Block %d: hash mismatch!\n", i);
            return false;
        }
        
        // Проверка 2: связь с предыдущим
        if (i > 0 && chain[i].prev_hash != chain[i-1].hash) {
            printf("Block %d: chain broken!\n", i);
            return false;
        }
        
        // Проверка 3: proof-of-work
        // (проверка difficulty)
    }
    
    return true;
}
```

---

## 🎬 Сюжет

**Дата:** 16 декабря, 14:23  
**Локация:** Безопасный дом MOONLIGHT

После расшифровки сообщений вы получили доступ к архиву операции. Данные хранятся в blockchain-системе — каждый документ, каждый артефакт записан в неизменяемый блок.

> **V.:** "Блокчейн защищает наши данные от подделки. Но разведка докладывает о попытках взлома. Нужно проверить целостность всей цепи. Если найдешь поврежденные блоки — сообщи немедленно."

Вы получаете файл `moonlight_chain.dat` с последовательностью блоков. Необходимо:
1. Понять структуру blockchain
2. Реализовать хеш-функцию для блоков
3. Верифицировать целостность цепи
4. Найти поврежденные блоки (если есть)

---

## 💻 Задания

### Задание 1: Hash Function Implementation ⭐⭐☆☆☆

Реализуйте простую хеш-функцию и тесты.

```c
// TODO: Реализовать
unsigned long hash_djb2(const unsigned char *str);

// TODO: Тест лавинного эффекта
void test_avalanche_effect(void);
```

**Критерии:**
- Одинаковый вход → одинаковый хеш
- Малое изменение входа → большое изменение хеша

---

### Задание 2: Blockchain Structure ⭐⭐⭐☆☆

Создайте структуру блока и функции для работы с цепью.

```c
typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    unsigned long prev_hash;
    unsigned long hash;
    int nonce;
} Block;

// TODO: Реализовать
void create_genesis_block(Block *block);
void add_block(Block *chain, int index, const char *data);
unsigned long calculate_block_hash(Block *block);
```

---

### Задание 3: Blockchain Verification ⭐⭐⭐⭐☆

Верифицируйте целостность blockchain.

```c
// TODO: Реализовать
bool verify_blockchain(Block *chain, int length);
int find_corrupted_blocks(Block *chain, int length);
```

**Проверки:**
- Целостность хешей
- Связность цепи
- Корректность индексов

---

### Задание 4: Proof-of-Work (Mining) ⭐⭐⭐⭐⭐

Реализуйте простой mining с difficulty.

```c
// TODO: Реализовать
void mine_block(Block *block, int difficulty);
bool check_difficulty(unsigned long hash, int difficulty);
```

**Концепция:** Найти nonce, чтобы хеш блока начинался с N нулей (в hex представлении).

**Пример:**
- difficulty=1: хеш должен начинаться с "0..."
- difficulty=2: хеш должен начинаться с "00..."
- difficulty=3: хеш должен начинаться с "000..."

---

## 🔍 Hints

<details>
<summary>Подсказка 1: djb2 hash function</summary>

```c
unsigned long hash_djb2(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}
```

</details>

<details>
<summary>Подсказка 2: Block hash calculation</summary>

Хеш блока = хеш от конкатенации всех полей:
```c
char buffer[512];
snprintf(buffer, sizeof(buffer), "%d%ld%s%lu%d",
         block->index,
         block->timestamp,
         block->data,
         block->prev_hash,
         block->nonce);

return hash_djb2((unsigned char*)buffer);
```

</details>

<details>
<summary>Подсказка 3: Chain verification</summary>

Для каждого блока (кроме genesis):
1. Пересчитать хеш → сравнить с block->hash
2. Проверить block->prev_hash == chain[i-1].hash
3. Если не совпадает → блок поврежден

</details>

---

## ✅ Проверка

```bash
cd episode-14-blockchain-hashes
make test
```

**Ожидаемый результат:**
```
Testing hash function...              [PASS]
Testing blockchain creation...        [PASS]
Testing verification...               [PASS]
Testing proof-of-work...              [PASS]

Blockchain status: VALID ✓
Total blocks: 10
All blocks properly mined ✓
```

---

## 🎓 Дополнительные материалы

### Blockchain
- [Bitcoin Whitepaper](https://bitcoin.org/bitcoin.pdf) — Satoshi Nakamoto
- [Blockchain Demo](https://andersbrownworth.com/blockchain/) — визуализация
- [Mastering Bitcoin](https://github.com/bitcoinbook/bitcoinbook) — Andreas Antonopoulos

### Hash Functions
- [SHA-256 Algorithm](https://en.wikipedia.org/wiki/SHA-2)
- [Hash Function Security](https://csrc.nist.gov/projects/hash-functions)

### Financial Applications
- [DeFi](https://ethereum.org/en/defi/) — Decentralized Finance
- [Smart Contracts](https://ethereum.org/en/developers/docs/smart-contracts/)

---

## 🏆 Бонусные задания

### Bonus 1: Merkle Tree ⭐⭐⭐⭐⭐
Реализуйте Merkle Tree для эффективной верификации транзакций.

### Bonus 2: SHA-256 Implementation ⭐⭐⭐⭐⭐
Реализуйте настоящий SHA-256 (сложно, но полезно!)

### Bonus 3: Merkle Tree ⭐⭐⭐⭐⭐
Реализуйте упрощенный Merkle Tree для хранения нескольких данных в одном блоке.

---

## 📊 Что вы узнали

После Episode 14 вы умеете:

- ✅ Реализовывать криптографические хеш-функции
- ✅ Создавать blockchain структуры данных
- ✅ Верифицировать целостность через цепочку хешей
- ✅ Понимать Proof-of-Work (mining)
- ✅ Детектировать подделку данных через immutability

---

## 🔜 Следующий эпизод

В **Episode 15: Sorting & Search Algorithms** вы научитесь:
- Реализовывать классические алгоритмы сортировки
- Понимать временную сложность (Big O)
- Применять бинарный поиск
- Оптимизировать производительность кода

Верифицированный blockchain содержит миллионы записей логов. Чтобы найти нужную информацию, понадобятся эффективные алгоритмы...

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 14 of 32*

[← Episode 13](../episode-13-crypto-basics/) | [Season 4](../README.md) | [Episode 15 →](../episode-15-trading-algorithms/)

</div>
