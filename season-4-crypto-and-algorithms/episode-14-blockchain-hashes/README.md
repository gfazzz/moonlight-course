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

#### 🎨 Визуальная метафора: Blockchain = Цепь с замками

```
╔════════════════════════════════════════════════════════════════╗
║  BLOCKCHAIN = ЦЕПЬ С САМОЗАПИРАЮЩИМИСЯ ЗАМКАМИ                 ║
╠════════════════════════════════════════════════════════════════╣
║                                                                ║
║  Представьте хранилище с цепочкой сейфов:                      ║
║                                                                ║
║   СЕЙФ #0        СЕЙФ #1        СЕЙФ #2        СЕЙФ #3        ║
║  ╔════════╗     ╔════════╗     ╔════════╗     ╔════════╗      ║
║  ║ DATA:  ║     ║ DATA:  ║     ║ DATA:  ║     ║ DATA:  ║      ║
║  ║ "INIT" ║     ║ "$250K"║     ║ "GPS"  ║     ║ "$1.5M"║      ║
║  ║        ║     ║        ║     ║        ║     ║        ║      ║
║  ║ 🔒 5381║────►║ 🔑 5381║────►║ 🔑 7892║────►║ 🔑 9034║      ║
║  ║ ЗАМОК  ║     ║ 🔒 7892║     ║ 🔒 9034║     ║ 🔒 4521║      ║
║  ╚════════╝     ╚════════╝     ╚════════╝     ╚════════╝      ║
║                                                                ║
║  📍 КАК ЭТО РАБОТАЕТ:                                          ║
║                                                                ║
║  1️⃣ СОЗДАНИЕ СЕЙФА:                                            ║
║     - Кладёте данные внутрь                                   ║
║     - Берёте КЛЮЧ от предыдущего сейфа (prev_hash)            ║
║     - Закрываете на НОВЫЙ ЗАМОК (hash текущего сейфа)         ║
║                                                                ║
║  2️⃣ ПРОВЕРКА:                                                  ║
║     - ЗАМОК сейфа #0 = 5381                                   ║
║     - КЛЮЧ сейфа #1 должен быть = 5381 ✅                     ║
║     - Если не совпадает → цепь разорвана!                     ║
║                                                                ║
║  3️⃣ ПОПЫТКА ПОДДЕЛКИ:                                          ║
║                                                                ║
║     Злоумышленник меняет данные в сейфе #1:                   ║
║     "$250K" → "$1M"                                            ║
║                                                                ║
║     Что происходит:                                           ║
║     ┌─────────────────────────────────────────┐               ║
║     │ СЕЙФ #1 (изменён!)                      │               ║
║     │ DATA: "$1M" (вместо "$250K")            │               ║
║     │ 🔑 5381 (правильный ключ от сейфа #0)  │               ║
║     │ 🔒 9F2A (НОВЫЙ замок! был 7892)        │← ИЗМЕНИЛСЯ!   ║
║     └─────────────────────────────────────────┘               ║
║                                                                ║
║     Теперь сейф #2:                                           ║
║     ┌─────────────────────────────────────────┐               ║
║     │ СЕЙФ #2                                 │               ║
║     │ 🔑 7892 (ожидает старый замок)         │               ║
║     │ НО! Замок сейфа #1 теперь 9F2A!        │               ║
║     │ 7892 ≠ 9F2A  ❌ КЛЮЧ НЕ ПОДХОДИТ!      │               ║
║     └─────────────────────────────────────────┘               ║
║                                                                ║
║     🚨 ЦЕПЬ РАЗОРВАНА! Подделка обнаружена!                   ║
║                                                                ║
║  4️⃣ КАСКАДНЫЙ ЭФФЕКТ:                                          ║
║                                                                ║
║     Изменили сейф #1 → замок изменился → сейф #2 невалиден   ║
║                       → замок #2 изменился → сейф #3 невалиден ║
║                       → ... → ВСЯ ЦЕПЬ после #1 невалидна!    ║
║                                                                ║
║  💡 ПОЧЕМУ ЭТО ГЕНИАЛЬНО:                                      ║
║                                                                ║
║     ✅ Изменить ОДИН блок = невозможно                        ║
║        (следующий блок обнаружит подделку)                    ║
║                                                                ║
║     ✅ Чтобы подделать данные нужно:                          ║
║        1. Изменить блок #1                                    ║
║        2. Пересчитать хеш блока #1                            ║
║        3. Изменить prev_hash в блоке #2                       ║
║        4. Пересчитать хеш блока #2                            ║
║        5. ... до конца цепи!                                  ║
║                                                                ║
║     ✅ Для 15,000 блоков → практически невозможно!            ║
║        (особенно с Proof-of-Work)                             ║
║                                                                ║
║  📊 В РЕАЛЬНОЙ ЖИЗНИ (Bitcoin):                                ║
║                                                                ║
║     - 1 блок = ~10 минут майнинга                             ║
║     - Подделать 1000 блоков = ~7 дней работы                  ║
║     - НО! Пока вы считаете, сеть добавила новые блоки        ║
║     - Невозможно догнать без 51% computing power!             ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝
```

**Метафора работает потому что:**
- 🔒 **Замок** = hash блока (уникальный идентификатор)
- 🔑 **Ключ** = prev_hash (ссылка на предыдущий блок)
- 🔗 **Цепь** = последовательность замок→ключ→замок→ключ
- 🚨 **Разрыв** = несовпадение ключа и замка (invalid chain)

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

### 🏋️ Промежуточная задача: Hash Avalanche Effect Tester ⭐⭐☆☆☆

**ПЕРЕД основной миссией потренируйтесь на лавинном эффекте!**

Создайте инструмент `hash_avalanche_tester` для проверки качества hash-функции djb2.

**Файл:** `artifacts/hash_avalanche_tester.c`

**Что реализовать:**
```c
// 1. Hash-функция djb2
unsigned long hash_djb2(const unsigned char *str);

// 2. Подсчёт различающихся бит между двумя хешами
int count_bit_differences(unsigned long h1, unsigned long h2);

// 3. Визуализация хеша в binary формате
void visualize_bits(unsigned long value, char *buffer);

// 4. Тест лавинного эффекта для пары строк
void test_avalanche_effect(const char *str1, const char *str2);
```

**Тестовые пары:**
- `"MOONLIGHT"` vs `"MOONLIGH"` (1 символ удалён)
- `"MOONLIGHT"` vs `"moonlight"` (case change)
- `"BLOCK_0"` vs `"BLOCK_1"` (1 цифра изменена)
- `"Transfer $15.7M to 0x4A7F"` vs `"Transfer $25.0M to 0x9B2C"`

**Ожидаемый вывод:**
```
═══ AVALANCHE EFFECT TEST ═══
Input 1: "MOONLIGHT"
Input 2: "MOONLIGH"

Hash 1:  0x7C8DF4A2  01111100100011011111010010100010
Hash 2:  0x2F91B6E5  00101111100100011011011011100101

Bit differences: 17 / 32 (53.1%)
✅ Good avalanche effect! (expected ~50%)
```

**Почему это важно для Episode 14?**
- Лавинный эффект = база blockchain integrity
- Изменение 1 байта в блоке → весь hash другой
- Следующий блок обнаруживает подделку (prev_hash не совпадёт)

**Компиляция и запуск:**
```bash
cd artifacts
gcc -Wall -Wextra -std=c11 hash_avalanche_tester.c -o hash_tester
./hash_tester
```

**Тест:**
```bash
cd tests
./test_avalanche.sh
```

**Решение:** `solution/hash_avalanche_tester_solution.c` (смотреть только если застряли!)

---

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

## 🤔 Контрольные вопросы (интегрированы в сюжет миссии)

**💡 Эти вопросы помогут проверить понимание материала через призму сюжета Episode 14.**

### Базовое понимание (5 вопросов):

<details>
<summary><strong>1. В 05:30 вы начали верификацию blockchain с 15,000 блоков. Почему изменение Block #7250 делает невалидными ВСЕ блоки после него?</strong></summary>

**Ответ:**

**Каскадный эффект (Cascade Effect):**

Каждый блок содержит `prev_hash` — хеш предыдущего блока:

```
Block 7250          Block 7251           Block 7252
┌──────────┐       ┌──────────┐        ┌──────────┐
│ data: X  │       │ data: Y  │        │ data: Z  │
│ hash: A2 │◄──────│ prev: A2 │◄───────│ prev: B3 │
└──────────┘       │ hash: B3 │        │ hash: C4 │
                   └──────────┘        └──────────┘
```

**Изменение Block #7250:**
1. `data` изменён → `hash` пересчитывается → теперь `0x9F` (вместо `0xA2`)
2. Block #7251 ожидает `prev_hash = 0xA2`
3. НО! Block #7250 теперь имеет `hash = 0x9F`
4. **0xA2 ≠ 0x9F** → Block #7251 невалиден!
5. Block #7251 невалиден → его `hash` неправильный → Block #7252 невалиден
6. И так далее до конца цепи!

**В коде:**
```c
for (int i = 7251; i < 15000; i++) {
    if (chain[i].prev_hash != chain[i-1].hash) {
        printf("Block %d INVALID: prev_hash mismatch!\n", i);
        // Все блоки после #7250 невалидны
    }
}
```

Вот почему blockchain называют **immutable** (неизменяемым)!

</details>

<details>
<summary><strong>2. В 07:12 обнаружен IP 185.220.101.47 в Block #7250. Это тот же IP из Season 2 Episode 07! Что это значит?</strong></summary>

**Ответ:**

**Связь между сезонами!**

**Season 2 Episode 07 (20:00):**
- Файл `archive.dat` растёт в реальном времени
- Удалённая атака от **185.220.101.47** (Tor exit node)
- Троянская программа от агентов Z.

**Season 4 Episode 14 (07:12):**
- Block #7250 содержит транзакцию от **185.220.101.47**
- Попытка подделки финансовых записей
- Та же группа атакующих!

**Что это значит:**
1. Агенты Z. используют **тот же Tor node** для атак
2. Blockchain ledger содержит **след** их активности
3. Координаты в blockchain — часть операции MOONLIGHT
4. V. отслеживает их через blockchain (публичный ledger!)

**Сквозная линия:**
```
S2 Ep07: Атака от 185.220.101.47 (память)
       ↓
S4 Ep14: Та же IP в blockchain (crypto-финансы)
       ↓
S5: Financial Markets? (продолжение следует...)
```

Easter Egg: проверьте 185.220.101.47 на реальном tor exit node tracker!

</details>

<details>
<summary><strong>3. Genesis Block создан 1 января 2020, 00:00:00. Почему это важно для верификации всей цепи?</strong></summary>

**Ответ:**

**Genesis Block = якорь всей цепи**

```c
Block 0 (GENESIS)
┌───────────────────────┐
│ index: 0              │
│ timestamp: 1577836800 │ ← Jan 1, 2020 00:00:00 UTC
│ data: "Operation MOONLIGHT initialized"
│ prev_hash: 0          │ ← НЕТ предыдущего блока!
│ hash: 0x5FDB8A3C      │ ← Первый хеш в цепи
└───────────────────────┘
```

**Почему важно:**

1. **Единственный блок без prev_hash:**
   ```c
   if (block[0].prev_hash != 0) {
       printf("Genesis block corrupted!\n");
       return false;
   }
   ```

2. **Неизменяемый anchor:**
   - Все участники сети знают хеш Genesis Block
   - Изменение Genesis → вся цепь невалидна
   - Невозможно подделать историю с начала

3. **Proof of origin:**
   - Timestamp = начало операции
   - "MOONLIGHT initialized" = подтверждение подлинности
   - Часто содержит важное сообщение (Bitcoin: газета The Times!)

4. **Validation starts here:**
   ```c
   unsigned long expected_hash = chain[0].hash; // 0x5FDB8A3C
   unsigned long calculated = calculate_block_hash(&chain[0]);
   
   if (expected_hash != calculated) {
       printf("Genesis block hash mismatch → entire chain invalid!\n");
   }
   ```

**Bitcoin Genesis Block (Jan 3, 2009):**
```
"The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"
```
Satoshi Nakamoto встроил заголовок газеты → доказательство что блок создан не раньше этой даты!

</details>

<details>
<summary><strong>4. Difficulty mining для Bitcoin = 20 ведущих нулей (2^80 попыток). Сколько это времени на вашем компьютере?</strong></summary>

**Ответ:**

**Расчёт:**

Допустим ваш компьютер вычисляет SHA-256:
- Средний CPU: ~1 миллион хешей/сек (1 MH/s)
- Хороший GPU: ~100 MH/s
- ASIC майнер: ~100 TH/s (терахеш!)

**Для 20 ведущих нулей:**
```
Попыток = 2^80 ≈ 1.2 × 10^24 (квинтиллион!)
```

**На вашем CPU (1 MH/s):**
```
Время = 1.2 × 10^24 / 1,000,000 = 1.2 × 10^18 секунд
      = 38 миллиардов лет!
      = 2.7× возраст Вселенной! 🤯
```

**На GPU (100 MH/s):**
```
Время = 380 миллионов лет (всё ещё безумно долго!)
```

**На ASIC (100 TH/s):**
```
Время = 3.8 месяца (но difficulty постоянно растёт!)
```

**Почему Bitcoin работает?**

1. **Вся сеть майнит вместе:**
   - ~200 exahash/s (EH/s) = 200,000,000 TH/s
   - Блок находится каждые ~10 минут

2. **Difficulty корректируется:**
   - Каждые 2016 блоков (~2 недели)
   - Если блоки находятся быстрее → difficulty растёт
   - Если медленнее → difficulty падает

3. **Proof-of-Work защищает от подделок:**
   - Чтобы изменить 1000 блоков назад:
     - Пересчитать PoW для всех 1000
     - Догнать текущую цепь
     - **Нужно >51% мощности сети!**

**В Episode 14 (difficulty=2):**
```
Попыток = 16^2 = 256 (в среднем)
Время на CPU = 0.0002 секунды 😊
```

</details>

<details>
<summary><strong>5. Лавинный эффект: изменение 1 байта → ~50% бит меняются. Почему именно 50%, а не 100% или 0%?</strong></summary>

**Ответ:**

**Статистическое распределение:**

Хорошая hash-функция должна быть **псевдослучайной**:

```
Изменили 1 бит входа → каждый бит выхода имеет 50% шанс измениться
```

**Почему 50%?**

1. **Аналогия с подбрасыванием монеты:**
   - 32-битный хеш = 32 монетки
   - Каждая может упасть орлом (0) или решкой (1) с вероятностью 50%
   - В среднем: 16 орлов, 16 решек

2. **Математика:**
   ```
   P(бит изменился) = 0.5  (для хорошей hash-функции)
   Ожидаемое количество изменений = 32 * 0.5 = 16 бит
   ```

3. **Почему НЕ 100%?**
   - Если ВСЕ биты всегда меняются → функция предсказуема!
   - Злоумышленник: "Ага, изменил 1 байт → все биты флипнулись"

4. **Почему НЕ 0%?**
   - 0% = хеш не изменился = нет лавинного эффекта
   - Можно подобрать похожие входы → collision attack

**Проверка в коде:**
```c
int diff = count_bit_differences(h1, h2);
double percent = 100.0 * diff / 32;

if (percent >= 40 && percent <= 60) {
    printf("✅ Good avalanche (close to 50%%)\n");
} else {
    printf("❌ Poor avalanche (too far from 50%%)\n");
}
```

**Реальный пример (SHA-256):**
```
hash("MOONLIGHT") = 8f4e3c0a...
hash("MOONLIGH")  = 2a9d1f7c...

Bit differences: 128 / 256 = 50.0% ✅
```

Идеальная hash-функция = неотличима от случайного оракула!

</details>

---

### Практическое применение (5 вопросов):

<details>
<summary><strong>6. В 06:45 verify_blockchain() нашёл Block #7250 невалидным. Как определить был ли это случайный corruption или намеренная подделка?</strong></summary>

**Ответ:**

**Анализ типа невалидности:**

```c
// 1. Проверяем ВИД ошибки
if (chain[i].hash != calculate_block_hash(&chain[i])) {
    printf("Hash mismatch → возможно corruption\n");
    
    // Дополнительные проверки:
    
    // A. Битовые ошибки (случайные)
    int bit_diff = count_bit_differences(chain[i].hash, calculated);
    if (bit_diff == 1 || bit_diff == 2) {
        printf("→ Likely BIT FLIP (cosmic ray, disk error)\n");
        printf("→ Random corruption\n");
    }
    
    // B. Полное несовпадение (намеренное)
    else if (bit_diff > 10) {
        printf("→ Major hash change (>10 bits)\n");
        printf("→ Likely DELIBERATE tampering!\n");
        
        // Проверяем данные блока
        if (strstr(chain[i].data, "Transfer") != NULL) {
            printf("→ Financial transaction modified!\n");
            printf("→ SUSPICIOUS: possible fraud attempt\n");
        }
    }
}
```

**Признаки намеренной подделки:**

1. **Данные изменены логично:**
   ```
   Было: "Transfer $250K to 0x4A7F"
   Стало: "Transfer $1.5M to 0x4A7F"  ← сумма увеличена!
   ```

2. **IP адрес известного злоумышленника:**
   ```c
   if (strstr(chain[i].data, "185.220.101.47") != NULL) {
       printf("→ Known attacker IP from Season 2!\n");
   }
   ```

3. **Паттерн атаки:**
   ```
   Block #7250: сумма изменена
   Block #7251: валиден (не тронут)
   Block #7252: сумма изменена
   → Систематическая подделка!
   ```

4. **Timestamp подозрителен:**
   ```c
   if (chain[i].timestamp > chain[i+1].timestamp) {
       printf("→ Timestamp inconsistency!\n");
       printf("→ Block was backdated\n");
   }
   ```

**Случайный corruption:**
- 1-2 бита отличаются
- Нет логики в изменениях данных
- Единичный случай

**Намеренная подделка:**
- Много бит изменились (~50%)
- Данные логично изменены ($250K → $1.5M)
- Множественные блоки
- Следы атакующего (IP 185.220.101.47)

</details>

<details>
<summary><strong>7. Proof-of-Work для difficulty=2 занял 256 попыток. Сколько попыток для difficulty=4?</strong></summary>

**Ответ:**

**Экспоненциальный рост:**

```
Difficulty = N ведущих нулей в hex
```

**Hex digit может быть:** 0-9, A-F (16 вариантов)

**Вероятность:**
```
Difficulty 1: один ноль → 1/16 ≈ 6.25%
  → Попыток в среднем: 16

Difficulty 2: два нуля → 1/256 ≈ 0.39%
  → Попыток: 256

Difficulty 3: три нуля → 1/4096
  → Попыток: 4,096

Difficulty 4: четыре нуля → 1/65536
  → Попыток: 65,536
```

**Формула:**
```c
avg_attempts = pow(16, difficulty);

// Difficulty 2: 16^2 = 256 ✓
// Difficulty 4: 16^4 = 65,536
```

**Время на обычном CPU:**
```c
// Допустим: 1 million hashes/sec
double time_sec = avg_attempts / 1000000.0;

Difficulty 2: 256 / 1,000,000 = 0.0002 сек
Difficulty 4: 65,536 / 1,000,000 = 0.065 сек
Difficulty 6: 16,777,216 / 1,000,000 = 16.7 сек
Difficulty 8: ~1 час
Difficulty 10: ~7 дней
```

**Bitcoin (difficulty ≈ 20):**
```
16^20 = 1.2 × 10^24 попыток
На всей сети (200 EH/s): ~10 минут
```

**В Episode 14:**
```c
// Для быстрого майнинга используем difficulty=2
mine_block(&block, 2);  // ~256 попыток, мгновенно

// Для реалистичности можно использовать difficulty=4
mine_block(&block, 4);  // ~65,536 попыток, секунды
```

</details>

<details>
<summary><strong>8. Merkle Tree упомянут в теории. Как он ускоряет верификацию изменений в большом blockchain?</strong></summary>

**Ответ:**

**Merkle Tree = иерархическое хеширование**

```
        ROOT HASH
         /     \
       H12     H34
       / \     / \
      H1  H2  H3  H4
      |   |   |   |
    TX1  TX2 TX3 TX4
```

**Преимущества:**

1. **Быстрая проверка принадлежности:**
   
   Без Merkle Tree (проверка что TX2 в блоке):
   ```c
   // Нужно проверить ВСЕ транзакции
   for (int i = 0; i < 10000; i++) {
       if (transactions[i] == TX2) return true;
   }
   // O(n) операций
   ```

   С Merkle Tree:
   ```c
   // Нужно только 3 хеша! (log₂ 10000 ≈ 13)
   verify_path: TX2 → H2 → H12 → ROOT
   // O(log n) операций
   ```

2. **Компактное доказательство:**
   
   Blockchain с 10,000 транзакций:
   - Без Merkle: нужно передать все 10,000 TX (мегабайты!)
   - С Merkle: только ~13 хешей (килобайты)

3. **Частичная верификация:**
   
   ```c
   // Легкий клиент (SPV - Simplified Payment Verification)
   // Не скачивает ВСЕ транзакции, только:
   - Merkle root (в заголовке блока)
   - Merkle proof для интересующих TX
   
   // Мобильный Bitcoin кошелёк = SPV клиент!
   ```

4. **Обнаружение изменений:**
   
   ```
   Изменили TX2 → H2 изменился → H12 изменился → ROOT изменился
   
   Проверка:
   if (block.merkle_root != calculated_root) {
       printf("Block modified!\n");
   }
   // Одна проверка вместо 10,000!
   ```

**В Episode 14 (упрощённо):**
```c
// Blockchain из 15,000 блоков
// Каждый блок: hash всех данных

// С Merkle Tree:
typedef struct Block {
    int index;
    time_t timestamp;
    unsigned long merkle_root;  // Root of TX Merkle Tree
    unsigned long prev_hash;
    unsigned long hash;         // Hash заголовка (+ merkle_root)
} Block;
```

**Bitcoin на практике:**
- Блок содержит ~2000 транзакций
- Merkle Tree глубина ≈ 11 уровней
- SPV proof = 11 хешей = ~352 байта (вместо мегабайт!)

</details>

<details>
<summary><strong>9. 51% Attack упомянут как угроза. Если злоумышленник контролирует 51% computing power, что он может сделать?</strong></summary>

**Ответ:**

**51% Attack = контроль большинства майнинговой мощности**

**Что МОЖЕТ злоумышленник:**

1. **Double-Spend (двойная трата):**
   
   ```
   Шаг 1: Отправить транзакцию жертве
   
   Honest Chain:        Attacker's Chain (секретная):
   [...] → Block N →    [...] → Block N' → Block N'+1 →
           TX: "Pay victim 10 BTC"         TX: "Pay myself"
   
   Шаг 2: Жертва ждёт 1-2 подтверждения
   
   Honest Chain:
   [...] → Block N → Block N+1 → Block N+2
           "Paid!"      ✓            ✓
   
   Шаг 3: Жертва отправляет товар 📦
   
   Шаг 4: Злоумышленник публикует СВОЮ цепь (длиннее!)
   
   Attacker's Chain (теперь публичная):
   [...] → Block N' → Block N'+1 → Block N'+2 → Block N'+3
                                    ↑ Эта цепь длиннее!
   
   Результат:
   - Сеть выбирает длинную цепь (правило консенсуса)
   - Транзакция жертве ОТМЕНЯЕТСЯ
   - Злоумышленник получил товар, но не заплатил! 💰
   ```

2. **Censorship (цензура транзакций):**
   ```c
   // Злоумышленник может игнорировать определённые TX
   if (transaction.from == "blacklisted_address") {
       skip_transaction();  // Не включать в блок
   }
   ```

3. **Rewrite Recent History (переписать недавнюю историю):**
   ```
   Можно откатить последние N блоков (если 51% мощности)
   НО! Чем дальше в прошлое → тем сложнее
   ```

**Что НЕ МОЖЕТ:**

❌ **Украсть чужие монеты** (нужна private key)
❌ **Изменить consensus rules** (хардфорк требует согласия всей сети)
❌ **Создать монеты из ничего** (блокирует протокол)
❌ **Переписать старую историю** (слишком дорого вычислительно)

**Защита:**

1. **Ждать больше подтверждений:**
   ```
   1 подтверждение: 51% attack возможен
   6 подтверждений: практически невозможен
   100 подтверждений: абсолютно безопасен
   ```

2. **Proof-of-Stake (альтернатива PoW):**
   - Вместо computing power → владение монетами
   - Атака на 51% = нужно купить 51% монет (дорого!)

3. **Diversified Mining:**
   - Множество майнинг-пулов
   - Ни один не имеет >50%

**В Episode 14:**
```c
// Moonlight blockchain (частный)
// Риск: один майнер контролирует все ноды
// Решение: distributed nodes, consensus algorithm
```

**Реальные случаи:**
- Ethereum Classic (2020): 51% attack, откат транзакций на $5.6M
- Bitcoin Gold (2018): 51% attack, double-spend $18M

</details>

<details>
<summary><strong>10. Blockchain ledger 15,000 блоков занимает сколько места на диске? (Estimate для простой структуры)</strong></summary>

**Ответ:**

**Расчёт размера:**

```c
typedef struct Block {
    int index;              // 4 байта
    time_t timestamp;       // 8 байт (на 64-bit системе)
    char data[256];         // 256 байт
    unsigned long prev_hash;// 8 байт
    unsigned long hash;     // 8 байт
    int nonce;              // 4 байта
} Block;
```

**Размер одного блока:**
```
4 + 8 + 256 + 8 + 8 + 4 = 288 байт
```

**15,000 блоков:**
```
288 * 15,000 = 4,320,000 байт
             = 4.32 MB
             = ~4.1 MiB
```

**С учётом выравнивания (struct padding):**
```c
sizeof(Block) ≈ 296 байт (padding до 8-byte boundary)
296 * 15,000 = 4.44 MB
```

**Сравнение с Bitcoin:**

Bitcoin блок содержит:
- ~2000 транзакций
- Средний размер TX: ~250 байт
- Заголовок блока: 80 байт

```
Bitcoin блок ≈ 2000 * 250 + 80 = 500 KB
Bitcoin blockchain (800,000 блоков): ~400 GB!
```

**Оптимизация:**

1. **Сжатие данных:**
   ```c
   // Вместо char data[256] использовать переменный размер
   typedef struct Block {
       int data_len;
       char *data;  // malloc только нужный размер
   } Block;
   
   // Экономия: ~50% если средний data < 128 байт
   ```

2. **Hash compression:**
   ```c
   // Вместо unsigned long (8 байт) использовать SHA-256 (32 байта)
   // НО! Больше размер, лучше безопасность
   ```

3. **Pruning (обрезка):**
   ```c
   // Старые блоки: удалить data, оставить только хеши
   // Для верификации chain integrity хватит хешей!
   ```

**В Episode 14:**
```bash
ls -lh moonlight_ledger.dat
-rw-r--r-- 1 user user 4.4M Dec 16 05:27 moonlight_ledger.dat
```

Компактно! Легко хранить и передавать.

</details>

---

### Анализ кода (3 вопроса):

<details>
<summary><strong>11. Функция calculate_block_hash() использует snprintf для конкатенации полей. Почему порядок полей важен?</strong></summary>

**Ответ:**

```c
unsigned long calculate_block_hash(Block *block) {
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%d%ld%s%lu%d",
             block->index,        // 1
             block->timestamp,    // 2
             block->data,         // 3
             block->prev_hash,    // 4
             block->nonce);       // 5
    
    return hash_djb2((unsigned char*)buffer);
}
```

**Почему порядок критичен:**

1. **Детерминизм:**
   ```c
   // Порядок: index, timestamp, data
   "0" "1577836800" "INIT" = "01577836800INIT"
   
   // Порядок: data, index, timestamp
   "INIT" "0" "1577836800" = "INIT01577836800"
   
   // РАЗНЫЕ строки → РАЗНЫЕ хеши!
   ```

2. **Consensus (согласованность):**
   ```c
   // Все узлы сети должны вычислять хеш ОДИНАКОВО
   // Иначе:
   Node A: hash = hash(index+data+timestamp)
   Node B: hash = hash(data+index+timestamp)
   
   // Node A и B получат РАЗНЫЕ хеши для ОДНОГО блока!
   // → Сеть разделится (fork)
   ```

3. **Validation:**
   ```c
   // При проверке используем ТОТ ЖЕ порядок
   unsigned long calculated = calculate_block_hash(&block);
   
   if (block.hash != calculated) {
       // Если порядок разный → всегда fail!
   }
   ```

4. **Malleability (податливость):**
   ```c
   // Если порядок не фиксирован, злоумышленник может:
   
   // Вариант 1: "123" + "ABC" = "123ABC" → hash1
   // Вариант 2: "ABC" + "123" = "ABC123" → hash2
   
   // Разные хеши для одних данных → атака!
   ```

**Лучшая практика:**

```c
// Использовать разделители
snprintf(buffer, sizeof(buffer), "%d|%ld|%s|%lu|%d",
         block->index, block->timestamp, block->data,
         block->prev_hash, block->nonce);

// "0|1577836800|INIT|0|0"
// Ясно видны границы полей
```

**Bitcoin использует:**
```
Version (4) + Prev Hash (32) + Merkle Root (32) + 
Timestamp (4) + Difficulty (4) + Nonce (4) = 80 байт
```
Строгий порядок, фиксированный размер!

</details>

<details>
<summary><strong>12. Почему mining loop может работать бесконечно? Что если nonce переполнится (overflow)?</strong></summary>

**Ответ:**

```c
void mine_block(Block *block, int difficulty) {
    block->nonce = 0;
    
    while (1) {  // ← Бесконечный цикл!
        block->hash = calculate_block_hash(block);
        
        if (check_difficulty(block->hash, difficulty)) {
            break;  // Нашли!
        }
        
        block->nonce++;  // ← Что если переполнится?
    }
}
```

**Проблема overflow:**

```c
int nonce; // 32-bit signed int

nonce = 2,147,483,647;  // INT_MAX
nonce++;                // Переполнение!
nonce = -2,147,483,648; // Стало отрицательным!
```

**Решения:**

1. **Unsigned int:**
   ```c
   unsigned int nonce; // 0 .. 4,294,967,295
   
   // Вдвое больше диапазон!
   ```

2. **64-bit nonce:**
   ```c
   unsigned long long nonce; // 0 .. 18,446,744,073,709,551,615
   
   // Практически невозможно перебрать все!
   ```

3. **Изменить другие поля:**
   ```c
   for (unsigned int extraNonce = 0; extraNonce < UINT_MAX; extraNonce++) {
       for (unsigned int nonce = 0; nonce < UINT_MAX; nonce++) {
           // Добавляем extraNonce в data
           sprintf(block->data + strlen(block->data), "_EN%u", extraNonce);
           
           block->nonce = nonce;
           block->hash = calculate_block_hash(block);
           
           if (check_difficulty(block->hash, difficulty)) return;
       }
   }
   
   // Всего комбинаций: UINT_MAX × UINT_MAX = 2^64
   ```

4. **Timestamp increment:**
   ```c
   // Bitcoin: если nonce исчерпан, увеличить timestamp
   while (1) {
       for (block->nonce = 0; block->nonce < UINT_MAX; block->nonce++) {
           // ... mining ...
       }
       
       // Nonce исчерпан! Увеличиваем время
       block->timestamp++;
       
       // Новый timestamp → новый hash → начинаем сначала
   }
   ```

**Bitcoin на практике:**

```
Difficulty = 20 ведущих нулей
Попыток = 2^80

32-bit nonce = 2^32 = 4 миллиарда попыток
2^80 / 2^32 = 2^48 переполнений!

Решение: extraNonce в coinbase transaction
```

**В Episode 14 (difficulty=2):**
```c
// Попыток в среднем: 256
// int nonce вполне достаточно (до 2 миллиардов)
```

**Безопасная реализация:**
```c
void mine_block(Block *block, int difficulty) {
    block->nonce = 0;
    unsigned long attempts = 0;
    unsigned long max_attempts = ULONG_MAX;
    
    while (attempts < max_attempts) {
        block->hash = calculate_block_hash(block);
        
        if (check_difficulty(block->hash, difficulty)) {
            printf("Mined in %lu attempts\n", attempts);
            return;
        }
        
        block->nonce++;
        attempts++;
    }
    
    // Если дошли сюда → не нашли nonce!
    printf("Mining failed! Consider changing timestamp or data.\n");
}
```

</details>

<details>
<summary><strong>13. Cascade validation: если Block #5000 невалиден, нужно ли проверять Block #10000? Почему можно остановить проверку раньше?</strong></summary>

**Ответ:**

**Наивная верификация:**

```c
bool verify_blockchain_naive(Block *chain, int length) {
    bool all_valid = true;
    
    // Проверяем ВСЕ блоки
    for (int i = 0; i < length; i++) {
        if (!verify_single_block(&chain[i], &chain[i-1])) {
            printf("Block %d invalid\n", i);
            all_valid = false;  // НЕ прерываем!
        }
    }
    
    return all_valid;
}

// Для 15,000 блоков: 15,000 проверок
```

**Оптимизированная верификация:**

```c
bool verify_blockchain_optimized(Block *chain, int length) {
    for (int i = 0; i < length; i++) {
        // Проверка 1: hash integrity
        unsigned long calculated = calculate_block_hash(&chain[i]);
        if (chain[i].hash != calculated) {
            printf("Block %d: hash mismatch\n", i);
            printf("All blocks %d..%d are UNTRUSTED\n", i, length-1);
            return false;  // ← ПРЕРЫВАЕМ СРАЗУ!
        }
        
        // Проверка 2: chain linkage
        if (i > 0 && chain[i].prev_hash != chain[i-1].hash) {
            printf("Block %d: prev_hash mismatch\n", i);
            printf("Chain broken at block %d\n", i);
            return false;  // ← ПРЕРЫВАЕМ!
        }
    }
    
    return true;
}
```

**Почему можно остановиться:**

1. **Каскадная невалидность:**
   ```
   Block 5000: невалиден
           ↓
   Block 5001: prev_hash не совпадёт
           ↓
   Block 5002: prev_hash не совпадёт
           ↓
   ... ВСЕ последующие невалидны
   ```

2. **Нет смысла проверять:**
   ```c
   // Если #5000 невалиден, #10000 автоматически невалиден
   // Зачем тратить CPU?
   
   // Для 15,000 блоков:
   // Наивно: 15,000 проверок
   // Оптимизированно: 5,000 проверок (стоп на #5000)
   ```

3. **Fast fail:**
   ```c
   // Обнаружили первую проблему → сообщаем сразу
   // Пользователь может начать расследование
   
   printf("Corruption detected at block %d\n", first_invalid);
   printf("Need to restore from backup or resync\n");
   ```

**НО! Иногда нужна полная проверка:**

```c
int* find_all_corrupted_blocks(Block *chain, int length) {
    int *corrupted = malloc(length * sizeof(int));
    int count = 0;
    
    // Проверяем ВСЕ, не останавливаемся
    for (int i = 0; i < length; i++) {
        unsigned long calculated = calculate_block_hash(&chain[i]);
        
        if (chain[i].hash != calculated) {
            corrupted[count++] = i;
            // НЕ break! Продолжаем
        }
    }
    
    corrupted[count] = -1;  // Терминатор
    return corrupted;
}

// Результат: [5000, 7250, 12000, -1]
// Множественные точки corruption!
```

**Стратегия проверки:**

```c
// Быстрая проверка (для ежедневного use)
if (!verify_blockchain_optimized(chain, length)) {
    // Цепь сломана!
    
    // Глубокая проверка (для forensics)
    int *corrupted = find_all_corrupted_blocks(chain, length);
    
    printf("Corrupted blocks: ");
    for (int i = 0; corrupted[i] != -1; i++) {
        printf("%d ", corrupted[i]);
    }
    printf("\n");
    
    // Анализируем паттерн
    if (corrupted[1] - corrupted[0] == 1) {
        printf("Sequential corruption → disk failure?\n");
    } else {
        printf("Scattered corruption → deliberate attack?\n");
    }
}
```

**В Episode 14:**
```c
// 09:18 верификация завершена
// Найдено 3 невалидных блока: #7250, #7251, #7280
// Остановились на #7250 (первый)
// Глубокая проверка показала: всего 3 точки атаки
```

</details>

---

### Сюжет и контекст (2 вопроса):

<details>
<summary><strong>14. Block #7250 содержит координаты 55.7558°N, 37.6173°E. Это те же координаты из Season 2 и Season 3! Что происходит?</strong></summary>

**Ответ:**

**Сквозная сюжетная линия:**

**Season 2 Episode 08 (ФИНАЛ):**
```
18:47  Расшифровка coordinates.dat
       55.7558°N, 37.6173°E
       
       Это КРАСНАЯ ПЛОЩАДЬ, Москва!
       Место встречи с Viktor
```

**Season 3 Episode 09:**
```
03:15  GPS-твист
       IP 203.0.113.42 декодируется в координаты
       55.7558°N 37.6173°E
       
       SMS от UNKNOWN: "Ты слишком близко"
```

**Season 3 Episode 12 (ФИНАЛ):**
```
08:00  Встреча с Viktor на Красной площади
       У Спасской башни
       
       Viktor передаёт USB: "Season 4: Crypto & Algorithms"
```

**Season 4 Episode 14 (текущий):**
```
09:18  Block #7250 в blockchain
       "Location: 55.7558°N, 37.6173°E"
       
       ЭТО СНОВА КРАСНАЯ ПЛОЩАДЬ!
```

**Что это значит:**

1. **Центр операции MOONLIGHT:**
   - Координаты появляются 4 раза в разных сезонах
   - Красная площадь = точка сбора агентов
   - Возможно, там физический сервер или drop point

2. **Blockchain = логбук операции:**
   - Block #7250 записал координаты встречи
   - Финансовые транзакции связаны с этим местом
   - Blockchain хранит всю историю MOONLIGHT

3. **Подделка Block #7250:**
   - Кто-то пытался скрыть эти координаты
   - Или изменить место встречи
   - Агенты Z. пытаются сорвать операцию

4. **Подготовка к Season 5:**
   - Координаты ведут к финансовому хабу
   - Season 5: Financial Markets
   - Красная площадь → какой-то офис/сервер там?

**География:**
```
55.7558°N, 37.6173°E = Красная площадь, Москва
- Рядом: Кремль, Спасская башня
- Координаты с точностью ~10м
- Возможно: конкретное здание рядом
```

**Easter Egg:**
```bash
# Проверьте на Google Maps:
# 55.7558, 37.6173
# → Московский Кремль, Спасская башня!
```

</details>

<details>
<summary><strong>15. V. сказал "У тебя 4 часа до рассвета" (05:30 → 09:30). Но верификация завершена в 09:18. Успели на 12 минут! Почему deadline был критичен?</strong></summary>

**Ответ:**

**Временная линия:**

```
05:27  Получен blockchain ledger (15,000 блоков)
05:30  Начало верификации
       Deadline: 09:30 (рассвет в декабре в Москве)
       
06:45  Обнаружен Block #7250 невалиден
07:12  IP 185.220.101.47 → связь с Season 2!
08:30  Cascade validation завершён
09:18  Координаты извлечены → 55.7558°N, 37.6173°E
       
09:30  DEADLINE! ⏰
09:18  ✅ Успели на 12 минут!
```

**Почему deadline критичен:**

1. **Физическая встреча на рассвете:**
   ```
   V. в сообщении: "Координаты должны быть чистыми"
   
   → Встреча запланирована на 09:30 (рассвет)
   → Если не успеешь верифицировать → неправильное место
   → Агенты Z. могут подстеречь
   ```

2. **Blockchain синхронизация:**
   ```c
   // После 09:30 сеть добавит новые блоки
   // Координаты могут быть перезаписаны
   // Окно возможности: 4 часа
   ```

3. **Следы атаки исчезнут:**
   ```
   IP 185.220.101.47 виден в логах до 09:30
   После → логи ротируются, следы стираются
   Forensics должен быть быстрым!
   ```

4. **Сюжетное напряжение:**
   ```
   Deadline создаёт драматизм:
   - 3 часа: "Успеем?"
   - 1 час: "Осталось мало!"
   - 12 минут до deadline: "На волосок!"
   ```

**Сравнение с другими сезонами:**

```
Season 1 Ep02: 48 часов (SMS угроза)
Season 2 Ep05: 6 часов (эвакуация)
Season 2 Ep07: 3 часа (файл растёт)
Season 3 Ep10: 5 часов (DDoS deadline)
Season 4 Ep14: 4 часа (рассвет)
```

Deadlines — фирменный стиль MOONLIGHT! ⏱️

**Что если бы НЕ успели:**

```c
if (current_time > deadline) {
    printf("⏰ TOO LATE!\n");
    printf("Coordinates corrupted. Meeting compromised.\n");
    printf("Agents Z. intercepted the location.\n");
    printf("\n");
    printf("🚨 MISSION FAILED 🚨\n");
    printf("Return to Episode 14 beginning.\n");
}
```

**Успели на 12 минут:**
```
09:18  Координаты извлечены
09:20  Отправлено сообщение V.
09:25  Подготовка к выходу
09:30  Рассвет
09:45  Прибытие на Красную площадь
       
✅ MISSION SUCCESS
```

**Мотивация студента:**
- Deadline → нужно эффективно писать код
- Нет времени на перебор всех 15,000 блоков наивно
- Оптимизация важна! (cascade validation)
- Real-world skill: работа под давлением времени

</details>

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
