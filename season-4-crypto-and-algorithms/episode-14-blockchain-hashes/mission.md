# 🔗 Mission Brief: Blockchain & Hash Functions

**CLASSIFIED - MOONLIGHT OPERATION**  
**Episode 14 - Blockchain Verification**

---

## 📋 Situation Report

**Date:** 16 декабря, 14:23  
**Location:** Безопасный дом MOONLIGHT  
**Contact:** V. (handler)

Расшифрованные в Episode 13 сообщения привели к архиву операции MOONLIGHT. Данные хранятся в blockchain-системе для защиты от подделки. Разведка сообщает о попытках взлома и модификации блоков.

**Критичность:** КРИТИЧЕСКАЯ  
**Дедлайн:** 24 часа

---

## 🎯 Mission Objectives

### Primary Objectives

1. **Implement Hash Function**
   - Реализовать криптографическую хеш-функцию (djb2 или FNV-1a)
   - Протестировать на лавинный эффект
   - Убедиться в детерминированности

2. **Build Blockchain Verifier**
   - Создать структуру Block
   - Реализовать вычисление хеша блока
   - Проверить целостность всей цепи

3. **Detect Tampering**
   - Найти поврежденные/подделанные блоки
   - Идентифицировать точки взлома
   - Сформировать отчет о нарушениях

4. **Analyze Financial Transactions**
   - Извлечь транзакции из блоков
   - Вычислить баланс счета MOONLIGHT
   - Найти подозрительные переводы

### Secondary Objectives

- Реализовать Proof-of-Work (mining)
- Создать audit trail для финансовых операций
- Сохранить все артефакты для расследования

---

## 📦 Provided Assets

### Blockchain Data File

```
moonlight_chain.dat - binary file with blockchain data
```

**Формат блока:**
```c
typedef struct Block {
    int index;              // 0, 1, 2, ...
    time_t timestamp;       // Unix timestamp
    char data[256];         // Transaction data
    unsigned long prev_hash; // Hash of previous block
    unsigned long hash;      // Hash of this block
    int nonce;              // Proof-of-work nonce
} Block;
```

### Transaction Format

Данные в блоке (block.data):
```
"TRANSFER:FROM:TO:AMOUNT"
```

Примеры:
```
TRANSFER:MOONLIGHT:V:50000.00
TRANSFER:ASSET_A:MOONLIGHT:125000.50
TRANSFER:MOONLIGHT:OPERATION:75000.00
```

### Starter Code

```c
// starter.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    unsigned long prev_hash;
    unsigned long hash;
    int nonce;
} Block;

// TODO: Implement hash function
unsigned long hash_djb2(const unsigned char *str);

// TODO: Calculate block hash
unsigned long calculate_block_hash(Block *block);

// TODO: Verify blockchain integrity
bool verify_blockchain(Block *chain, int length);

// TODO: Analyze transactions
double calculate_balance(Block *chain, int length, const char *address);
```

---

## 🔍 Intelligence

### Known Information

**Hash Function Properties:**
- Детерминированность: одинаковый вход → одинаковый хеш
- Лавинный эффект: 1 бит изменения → ~50% бит хеша меняются
- Односторонность: невозможно восстановить данные из хеша

**Blockchain Structure:**
```
Block 0 (Genesis) → Block 1 → Block 2 → ... → Block N
    ↑                  ↑          ↑
prev_hash=0       prev_hash   prev_hash
                  =hash[0]    =hash[1]
```

**Verification Checklist:**
1. Hash integrity: `block.hash == calculate_block_hash(block)`
2. Chain linkage: `block[i].prev_hash == block[i-1].hash`
3. Index sequence: `block[i].index == i`
4. Genesis block: `block[0].prev_hash == 0`

**Financial Analysis:**
- Счет MOONLIGHT должен иметь положительный баланс
- Все транзакции должны быть валидны
- Сумма входящих - исходящих = баланс

---

## 📝 Operational Guidelines

### Step 1: Implement Hash Function

**djb2 algorithm:**
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

**Test:**
```c
// "MOONLIGHT" должен давать одинаковый хеш всегда
unsigned long h1 = hash_djb2("MOONLIGHT");
unsigned long h2 = hash_djb2("MOONLIGHT");
assert(h1 == h2);

// Малое изменение → другой хеш
unsigned long h3 = hash_djb2("MOONLIGH");  // одна буква
assert(h1 != h3);
```

### Step 2: Calculate Block Hash

```c
unsigned long calculate_block_hash(Block *block) {
    char buffer[512];
    
    // Конкатенация всех полей
    snprintf(buffer, sizeof(buffer), "%d%ld%s%lu%d",
             block->index,
             (long)block->timestamp,
             block->data,
             block->prev_hash,
             block->nonce);
    
    return hash_djb2((unsigned char*)buffer);
}
```

### Step 3: Verify Blockchain

```c
bool verify_blockchain(Block *chain, int length) {
    for (int i = 0; i < length; i++) {
        // Check 1: Hash integrity
        unsigned long calculated = calculate_block_hash(&chain[i]);
        if (chain[i].hash != calculated) {
            printf("❌ Block %d: hash mismatch!\n", i);
            printf("   Expected: %lu\n", chain[i].hash);
            printf("   Got:      %lu\n", calculated);
            return false;
        }
        
        // Check 2: Chain linkage
        if (i > 0) {
            if (chain[i].prev_hash != chain[i-1].hash) {
                printf("❌ Block %d: broken chain link!\n", i);
                return false;
            }
        } else {
            // Genesis block
            if (chain[0].prev_hash != 0) {
                printf("❌ Genesis block invalid!\n");
                return false;
            }
        }
        
        // Check 3: Index
        if (chain[i].index != i) {
            printf("❌ Block %d: wrong index!\n", i);
            return false;
        }
    }
    
    return true;
}
```

### Step 4: Analyze Transactions

```c
typedef struct {
    char from[64];
    char to[64];
    double amount;
} Transaction;

Transaction parse_transaction(const char *data) {
    Transaction tx = {0};
    sscanf(data, "TRANSFER:%63[^:]:%63[^:]:%lf", 
           tx.from, tx.to, &tx.amount);
    return tx;
}

double calculate_balance(Block *chain, int length, const char *address) {
    double balance = 0.0;
    
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) != 0) {
            continue;
        }
        
        Transaction tx = parse_transaction(chain[i].data);
        
        if (strcmp(tx.from, address) == 0) {
            balance -= tx.amount;  // Outgoing
        }
        if (strcmp(tx.to, address) == 0) {
            balance += tx.amount;  // Incoming
        }
    }
    
    return balance;
}
```

---

## ✅ Success Criteria

### Mission Complete When:

- ✅ Hash function implemented and tested
- ✅ Blockchain verification works correctly
- ✅ All corrupted blocks identified (if any)
- ✅ Financial analysis complete
- ✅ MOONLIGHT balance calculated
- ✅ All tests pass (`make test`)

### Expected Output

```
$ ./blockchain_verifier moonlight_chain.dat

MOONLIGHT Blockchain Verifier
═══════════════════════════════════════════════

Loading blockchain: moonlight_chain.dat
Blocks loaded: 10

Verifying blockchain...
✓ Block 0: Genesis (hash: 5381)
✓ Block 1: valid (hash: 2934187)
✓ Block 2: valid (hash: 8273645)
...
✓ Block 9: valid (hash: 1928374)

═══════════════════════════════════════════════
VERIFICATION RESULT: ✓ BLOCKCHAIN VALID
═══════════════════════════════════════════════

Financial Analysis:
───────────────────────────────────────────────
Total transactions: 8
MOONLIGHT balance: $1,234,567.89

Transaction log saved to: artifacts/audit_trail.txt
```

---

## 🎯 Deliverables

1. **blockchain_verifier executable**
   - Hash function implementation
   - Blockchain verification
   - Transaction analysis

2. **Audit Report** (artifacts/audit_trail.txt)
   - Все транзакции
   - Балансы счетов
   - Найденные аномалии

3. **Analysis Report** (artifacts/analysis.txt)
   - Статус верификации
   - Список поврежденных блоков (если есть)
   - Рекомендации по безопасности

---

## ⚠️ Security Notes

**Hash Function Selection:**
- djb2/FNV-1a: учебные цели, НЕ криптостойкие
- SHA-256: реальные проекты
- BLAKE2/SHA-3: современные альтернативы

**Blockchain Security:**
- 51% attack: контроль >50% вычислительной мощности
- Double-spend: попытка дважды потратить одни средства
- Sybil attack: создание множества фейковых узлов

**Best Practices:**
```c
// ❌ Небезопасное сравнение хешей (timing attack)
if (hash1 == hash2) { ... }

// ✅ Constant-time comparison
int constant_time_compare(unsigned long a, unsigned long b) {
    unsigned long diff = a ^ b;
    return diff == 0;
}
```

---

## 🐛 Troubleshooting

### Problem: "Hash mismatch on all blocks"

Проверьте порядок полей в `snprintf`:
```c
// Правильный порядок:
snprintf(buffer, size, "%d%ld%s%lu%d",
         block->index,
         block->timestamp,
         block->data,
         block->prev_hash,
         block->nonce);
```

### Problem: "Chain broken at block 1"

Genesis block должен иметь `prev_hash = 0`:
```c
if (i == 0) {
    // Genesis block
    chain[0].prev_hash = 0;
}
```

### Problem: "Balance calculation wrong"

Учитывайте направление транзакций:
```c
if (strcmp(tx.from, address) == 0) balance -= tx.amount;  // Outgoing
if (strcmp(tx.to, address) == 0)   balance += tx.amount;  // Incoming
```

---

## 🏆 Bonus Challenges

### Bonus 1: Proof-of-Work Mining
Реализуйте mining с difficulty (хеш должен начинаться с N нулей).

### Bonus 2: Merkle Tree
Создайте Merkle Tree для эффективной верификации транзакций.

### Bonus 3: Block Explorer
Создайте ASCII-визуализацию blockchain (как в браузере блоков).

---

## ⏭️ Next Steps

После успешного выполнения миссии:

1. **Сохраните все отчеты** в `artifacts/`
2. **Запустите тесты:** `make test`
3. **Доложите V.** о результатах верификации
4. **Перейдите к Episode 15:** Trading Algorithms

Анализ blockchain выявил подозрительные финансовые транзакции. Следующий шаг — исследование торговых алгоритмов организации...

---

<div align="center">

**MOONLIGHT OPERATION - CLASSIFIED**  
*"In hash we trust"*

[← Episode 13](../episode-13-crypto-basics/) | [Episode 15 →](../episode-15-trading-algorithms/)

</div>
