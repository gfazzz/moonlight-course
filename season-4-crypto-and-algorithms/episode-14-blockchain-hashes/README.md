# Episode 14: "Blockchain & Hash Functions" 🔗

> *"Trust, but verify. The blockchain lets you verify."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Безопасный дом MOONLIGHT, Москва               ║
║  ВРЕМЯ:   05:27, четверг, 16 декабря                    ║
║  СТАТУС:  🟡 BLOCKCHAIN VERIFICATION REQUIRED            ║
╚══════════════════════════════════════════════════════════╝
```

**Episode 13 завершён.** Три сообщения расшифрованы. Координаты находятся в blockchain ledger.

В 05:27 V. отправляет критический файл:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (SECURE CHANNEL)                  │
│  TIME: 05:27:33 UTC                         │
│  PRIORITY: CRITICAL                         │
├─────────────────────────────────────────────┤
│                                             │
│  BLOCKCHAIN LEDGER ПОЛУЧЕН.                 │
│                                             │
│  Файл: moonlight_ledger.dat                 │
│  Размер: 15,000 блоков                      │
│  Период: Jan 2020 - Dec 2020                │
│                                             │
│  ПРОБЛЕМА:                                  │
│  Разведка сообщает о попытках подделки.     │
│  Кто-то пытался изменить финансовые записи. │
│                                             │
│  ЗАДАЧА:                                    │
│  1. Понять структуру blockchain             │
│  2. Верифицировать целостность цепи         │
│  3. Найти поврежденные блоки                │
│  4. Извлечь координаты из валидных данных   │
│                                             │
│  Blockchain - это НЕИЗМЕНЯЕМАЯ цепь.        │
│  Если хоть один блок изменён -              │
│  вся цепь после него станет невалидной.     │
│                                             │
│  Принцип: изменил блок N → все блоки        │
│  N+1, N+2... получат другие хеши.           │
│                                             │
│  У тебя 4 часа до рассвета.                 │
│  Координаты должны быть чистыми.            │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

**Что вы понимаете:**
- 📊 **15,000 блоков** — год финансовых транзакций
- 🔗 **Blockchain** — каждый блок связан с предыдущим через хеш
- 🛡️ **Immutability** — изменение одного блока ломает всю цепь
- 🎯 **Задача** — найти подделки и извлечь чистые данные

Вы загружаете `moonlight_ledger.dat`:

```
╔═══ BLOCKCHAIN LEDGER ═══════════════════════════════════╗
║  Total blocks: 15,000                                   ║
║  Genesis block: 0x00000000 (Jan 1, 2020 00:00:00)      ║
║  Latest block: 0x0000EA5F (Dec 31, 2020 23:59:59)      ║
║                                                         ║
║  First 3 blocks preview:                                ║
║                                                         ║
║  [Block 0] GENESIS                                      ║
║    Hash: 0x5FDB8A3C                                     ║
║    Data: "Operation MOONLIGHT initialized"              ║
║                                                         ║
║  [Block 1] Transfer                                     ║
║    Prev: 0x5FDB8A3C                                     ║
║    Hash: 0x7A2B9C1D                                     ║
║    Data: "Transfer $250K to account 0x4A7F..."          ║
║                                                         ║
║  [Block 2] Location                                     ║
║    Prev: 0x7A2B9C1D                                     ║
║    Hash: 0x9E3D4F2A                                     ║
║    Data: "Coordinates: 55.7558, 37.6173"                ║
╚═════════════════════════════════════════════════════════╝
```

Но что-то не так... Блок 2 показывает координаты, но хеш выглядит подозрительно.

**05:30. Начинаем верификацию.** ⏰

---

## 📚 Теория: Blockchain & Hash Functions

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

**Визуализация blockchain (упрощенная):**

```
                BLOCKCHAIN CHAIN
                
Block 0 (GENESIS)                Block 1                     Block 2
┌──────────────────┐            ┌──────────────────┐         ┌──────────────────┐
│ Index: 0         │            │ Index: 1         │         │ Index: 2         │
│ Timestamp: ...   │            │ Timestamp: ...   │         │ Timestamp: ...   │
│ Data: "GENESIS"  │            │ Data: "TX #1"    │         │ Data: "TX #2"    │
│ Prev: 0x00000000 │            │ Prev: 0x5FDB8A3C │───┐     │ Prev: 0x7A2B9C1D │───┐
│ Hash: 0x5FDB8A3C │←───────────┼──────────────────┘   │     │ Hash: 0x9E3D4F2A │   │
└──────────────────┘            └──────────────────┘         └──────────────────┘   │
                                                                                      │
                                                             Block 3                  │
                                                            ┌──────────────────┐      │
                                                            │ Index: 3         │      │
                                                            │ Timestamp: ...   │      │
                                                            │ Data: "TX #3"    │      │
                                                            │ Prev: 0x9E3D4F2A │←─────┘
                                                            │ Hash: 0x4C1A5E8B │
                                                            └──────────────────┘

Если изменить Block 1 → его hash изменится → Block 2 будет невалидным (prev_hash не совпадет)!
```

**Примеры криптографических хеш-функций:**

| Функция | Размер вывода | Применение |
|---------|---------------|------------|
| **MD5** | 128 бит | ❌ Устарела (коллизии найдены) |
| **SHA-1** | 160 бит | ❌ Устарела (коллизии с 2017) |
| **SHA-256** | 256 бит | ✅ Bitcoin, TLS, подписи |
| **SHA-3** | Переменный | ✅ Новый стандарт (Keccak) |
| **BLAKE2** | Переменный | ✅ Быстрая альтернатива SHA |

**Свойство: Лавинный эффект (Avalanche Effect)**

```c
// Даже ОДНА буква → совершенно другой хеш!
hash("MOONLIGHT") = 0x8F4E3C0A12D5B789
hash("MOONLIGH")  = 0x2A9D1F7C45E3A601  // Последняя буква удалена

// Изменение одного бита меняет ~50% бит в выходе!
```

---

### 3. Proof-of-Work (Mining)

**Задача mining:** найти `nonce`, чтобы хеш блока удовлетворял условию (например, начинался с N нулей).

**Пример:**
```
Target: хеш должен начинаться с "00" (difficulty=2)

Попытка 1: nonce=0 → hash=0x7A2B9C1D ❌
Попытка 2: nonce=1 → hash=0x8E3D4F2A ❌
Попытка 3: nonce=2 → hash=0x9F1A5B3C ❌
...
Попытка 14289: nonce=14288 → hash=0x004A7F8E ✅ (начинается с "00")
```

**Реализация mining:**

```c
void mine_block(Block *block, int difficulty) {
    char target[65] = {0};
    for (int i = 0; i < difficulty; i++) {
        target[i] = '0';
    }
    
    char hash_str[65];
    block->nonce = 0;
    
    while (1) {
        block->hash = calculate_block_hash(block);
        
        // Преобразуем хеш в hex-строку для проверки
        sprintf(hash_str, "%016lx", block->hash);
        
        if (strncmp(hash_str, target, difficulty) == 0) {
            printf("✅ Block mined! Nonce: %d, Hash: 0x%016lx\n", 
                   block->nonce, block->hash);
            break;
        }
        
        block->nonce++;
    }
}

// Проверка difficulty
bool check_difficulty(unsigned long hash, int difficulty) {
    char hash_str[65];
    sprintf(hash_str, "%016lx", hash);
    
    for (int i = 0; i < difficulty; i++) {
        if (hash_str[i] != '0') return false;
    }
    return true;
}
```

**Bitcoin Mining:**
- Bitcoin использует SHA-256 (дважды!)
- Difficulty автоматически корректируется каждые 2016 блоков
- Цель: новый блок каждые ~10 минут
- В 2020: difficulty ≈ 10²² (нужно 10²² попыток в среднем!)

---

### 4. Immutability (Неизменяемость)

**Почему blockchain неизменяем?**

```
Исходная цепь:
Block 0 → Block 1 → Block 2 → Block 3
(hash A)  (hash B)  (hash C)  (hash D)

Попытка изменить Block 1:
Block 0 → Block 1* → Block 2 → Block 3
(hash A)  (hash B') (hash C)  (hash D)
              ↑         ↑
              └─────────┘
            Связь разорвана! prev_hash не совпадает!

Результат:
✅ Block 0 - валиден
❌ Block 1 - изменен (hash B → hash B')
❌ Block 2 - невалиден (prev_hash указывает на hash B, но Block 1 теперь hash B')
❌ Block 3 - невалиден (каскадный эффект)
```

**Верификация цепи:**

```c
bool verify_blockchain(Block *chain, int length) {
    for (int i = 0; i < length; i++) {
        // 1. Пересчитать хеш блока
        unsigned long calculated_hash = calculate_block_hash(&chain[i]);
        
        if (calculated_hash != chain[i].hash) {
            printf("❌ Block %d: hash mismatch!\n", i);
            printf("   Expected: 0x%016lx\n", chain[i].hash);
            printf("   Actual:   0x%016lx\n", calculated_hash);
            return false;
        }
        
        // 2. Проверить связь с предыдущим блоком
        if (i > 0 && chain[i].prev_hash != chain[i-1].hash) {
            printf("❌ Block %d: chain broken!\n", i);
            printf("   Prev hash in block: 0x%016lx\n", chain[i].prev_hash);
            printf("   Actual prev hash:   0x%016lx\n", chain[i-1].hash);
            return false;
        }
    }
    
    printf("✅ Blockchain is valid! All %d blocks verified.\n", length);
    return true;
}
```

---

### 5. Применение в реальном мире

**Blockchain используется для:**
- 💰 **Криптовалюты**: Bitcoin, Ethereum
- 📜 **Smart Contracts**: самоисполняемые контракты
- 🏥 **Медицинские записи**: неизменяемая история пациента
- 🗳️ **Голосование**: прозрачные выборы
- 📦 **Supply Chain**: отслеживание товаров

**Bitcoin Block Structure:**
```c
typedef struct BitcoinBlock {
    uint32_t version;
    uint8_t prev_block_hash[32];  // SHA-256
    uint8_t merkle_root[32];       // Merkle tree корень транзакций
    uint32_t timestamp;
    uint32_t difficulty_bits;
    uint32_t nonce;
    // + список транзакций
} BitcoinBlock;
```

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

## 📝 Структура эпизода

```
episode-14-blockchain-hashes/
├── README.md               ← Вы здесь
├── artifacts/              ← Ваши решения
│   ├── blockchain.c        ← Реализация blockchain
│   ├── hash_test.c         ← Тесты хеш-функции
│   └── verifier.c          ← Верификация цепи
├── data/
│   └── moonlight_ledger.dat ← Blockchain от V.
├── starter.c               ← Шаблоны кода
├── tests/
│   ├── test_hash.c
│   ├── test_blockchain.c
│   └── test_mining.c
└── Makefile
```

---

## 💡 Как работать

1. **Изучите теорию** выше (хеш-функции, blockchain, PoW)
2. **Откройте `starter.c`** — там шаблоны структур и функций
3. **Создайте файлы в `artifacts/`:**
   - `artifacts/blockchain.c` — реализация Block, хешей, верификации
   - `artifacts/hash_test.c` — тесты лавинного эффекта
   - `artifacts/verifier.c` — проверка `moonlight_ledger.dat`
4. **Компилируйте:** `make` (или `gcc artifacts/blockchain.c -o blockchain`)
5. **Тестируйте:** `make test` или запустите свои программы
6. **Верифицируйте ledger:** `./verifier data/moonlight_ledger.dat`

---

## 🏁 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Безопасный дом MOONLIGHT, Москва               ║
║  ВРЕМЯ:   09:18, четверг, 16 декабря                    ║
║  СТАТУС:  ✅ BLOCKCHAIN VERIFIED                         ║
╚══════════════════════════════════════════════════════════╝
```

**09:18. Верификация завершена.**

Вы запускаете финальную проверку:

```bash
$ ./verifier data/moonlight_ledger.dat

╔═══ BLOCKCHAIN VERIFICATION ═════════════════════════════╗
║  Loading blockchain: moonlight_ledger.dat               ║
║  Blocks loaded: 15,000                                  ║
║  Verifying integrity...                                 ║
╠═════════════════════════════════════════════════════════╣
║                                                         ║
║  [████████████████████████████████] 100%                ║
║                                                         ║
║  Block 0-7,249:      ✅ VALID                           ║
║  Block 7,250:        ❌ CORRUPTED                       ║
║  Block 7,251-7,289:  ❌ INVALID (cascade)               ║
║  Block 7,290-14,999: ✅ VALID (restored)                ║
║                                                         ║
║  CORRUPTION DETECTED:                                   ║
║    Block #7,250                                         ║
║    Original data: "Transfer $15.7M to 0x4A7F..."        ║
║    Tampered data: "Transfer $25.0M to 0x9B2C..."        ║
║    Modified: Dec 10, 2020 14:37:22                      ║
║                                                         ║
║  Evidence: Someone tried to increase the amount         ║
║  by $9.3M and change the recipient!                     ║
║                                                         ║
║  VALID BLOCKS: 14,960 / 15,000 (99.73%)                 ║
║  COORDINATES EXTRACTED:                                 ║
║    Block #8,142: 55.7558°N, 37.6173°E                   ║
║    Location: Red Square, Moscow                         ║
║    Timestamp: Dec 18, 2020 18:00:00                     ║
║                                                         ║
╚═════════════════════════════════════════════════════════╝
```

**КООРДИНАТЫ НАЙДЕНЫ!** 55.7558°N, 37.6173°E — Красная площадь, 18 декабря, 18:00.

Через 30 секунд приходит сообщение от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (SECURE CHANNEL)                  │
│  TIME: 09:19:07 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  ОТЛИЧНАЯ РАБОТА.                           │
│                                             │
│  Ты нашел подделку в Block #7,250.          │
│  Это была проверка на внимательность.       │
│                                             │
│  Blockchain - это TRUST через MATH,         │
│  а не через людей.                          │
│                                             │
│  Координаты чистые: Red Square, Dec 18.     │
│  Именно там будет финальная встреча.        │
│                                             │
│  Но перед этим тебе нужно научиться         │
│  работать с БОЛЬШИМИ данными.               │
│                                             │
│  У нас 15,000 блоков. Каждый содержит       │
│  сотни транзакций. Миллионы записей.        │
│                                             │
│  Чтобы найти нужную информацию,             │
│  понадобятся АЛГОРИТМЫ сортировки и поиска. │
│                                             │
│  Episode 15 ждет тебя.                      │
│                                             │
│  — V.                                       │
│                                             │
│  P.S. Изучи djb2 подробнее. Эта функция     │
│  проста, но элегантна. Daniel J. Bernstein  │
│  — гений криптографии.                      │
│                                             │
└─────────────────────────────────────────────┘
```

**ЧТО ВЫ УЗНАЛИ:**
- 🔗 **Blockchain** защищает данные через цепочку хешей
- 🛡️ **Immutability**: изменение одного блока ломает всю цепь
- 🔍 **Verification**: можно автоматически найти подделки
- ⛏️ **Mining (PoW)**: вычислительная работа защищает сеть
- 📍 **Координаты**: Red Square, Dec 18, 18:00 (финальная встреча!)

**МИССИЯ ЗАВЕРШЕНА!** ✅  
Episode 14 пройден. Blockchain верифицирован. Координаты получены.

---

## 🎖️ Навык разблокирован

```
╔════════════════════════════════════════════════════════╗
║  🏆 BLOCKCHAIN ENGINEER                                ║
╠════════════════════════════════════════════════════════╣
║  Вы освоили:                                           ║
║  • Криптографические хеш-функции                       ║
║  • Blockchain структуры и верификацию                  ║
║  • Proof-of-Work (mining)                              ║
║  • Immutability и каскадную валидацию                  ║
║                                                        ║
║  Этот навык применим в:                                ║
║  💰 Криптовалюты (Bitcoin, Ethereum)                   ║
║  📜 Smart Contracts                                    ║
║  🏥 Медицинские записи                                 ║
║  🗳️ Системы голосования                                ║
║  📦 Supply Chain Management                            ║
╚════════════════════════════════════════════════════════╝
```

---

## 🥚 Easter Eggs

1. **djb2 hash = 5381** → Magic number из comp.lang.c (Daniel J. Bernstein, 1991)
2. **Block #7,250 corrupted** → Именно 7250 = 50 × 145 (не случайность!)
3. **Red Square coordinates** → 55.7558, 37.6173 (Кремль, точный центр)
4. **Dec 18, 18:00** → 18/18 (двойное 18 = символическое значение)
5. **SHA-256 (дважды!)** → Bitcoin использует SHA256(SHA256(data)) для защиты от length-extension attack
6. **Merkle Root** → Назван в честь Ralph Merkle (изобретатель в 1979)
7. **Genesis Block** → Bitcoin Genesis: Jan 3, 2009 ("The Times 03/Jan/2009 Chancellor on brink of second bailout for banks")
8. **Difficulty = 10²²** → В 2020 сложность Bitcoin требовала ~150 exahashes/sec (вся энергия малой страны!)

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
Реализуйте Merkle Tree для эффективной верификации транзакций в одном блоке.

**Подсказка:**
```c
typedef struct MerkleNode {
    unsigned long hash;
    struct MerkleNode *left;
    struct MerkleNode *right;
} MerkleNode;

MerkleNode* build_merkle_tree(unsigned long *hashes, int count);
unsigned long get_merkle_root(MerkleNode *tree);
```

### Bonus 2: SHA-256 Implementation ⭐⭐⭐⭐⭐
Реализуйте настоящий SHA-256 (сложно, но очень полезно для понимания!)

### Bonus 3: Dynamic Difficulty Adjustment ⭐⭐⭐⭐☆
Реализуйте автоматическую корректировку сложности, как в Bitcoin (каждые 2016 блоков).

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
