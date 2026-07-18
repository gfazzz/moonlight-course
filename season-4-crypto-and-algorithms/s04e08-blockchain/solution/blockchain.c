/*
 * OPERATION MOONLIGHT — s04e08 "Блокчейн" (финал Season 4)
 * Эталонное решение: blockchain.c
 *
 * Концепт серии: цепочка блоков, где каждый хранит хеш предыдущего. Правка
 *                любого байта меняет хеш блока -> рвётся связь со следующим,
 *                и подделка видна. Proof-of-Work делает подделку ещё и дорогой.
 * Задача: собрать цепочку с PoW, проверить её, подделать блок и найти разрыв.
 *
 * Собирает сезон: хеш (s04e03), цепочка ссылок (s04e07), проверка целостности.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NBLOCKS   5
#define DIFFICULTY 20      /* требуем hash >> DIFFICULTY == 0 (старшие 12 бит нулевые) */

typedef struct {
    int      index;
    char     data[64];
    uint32_t prev_hash;
    uint32_t nonce;
    uint32_t hash;
} Block;

/* Перемешивание в духе FNV-1a: XOR + умножение на простое. */
static uint32_t mix(uint32_t h, uint32_t v) {
    h ^= v;
    h *= 16777619u;
    return h;
}

/* Хеш блока считается по ВСЕМ его полям, кроме самого поля hash. */
static uint32_t block_hash(int index, const char *data, uint32_t prev, uint32_t nonce) {
    uint32_t h = 2166136261u;
    h = mix(h, (uint32_t)index);
    for (const unsigned char *p = (const unsigned char *)data; *p; p++) h = mix(h, *p);
    h = mix(h, prev);
    h = mix(h, nonce);
    return h;
}

/* Proof-of-Work: перебираем nonce, пока хеш не станет достаточно «малым». */
static uint32_t mine(Block *b) {
    uint32_t nonce = 0;
    for (;;) {
        uint32_t h = block_hash(b->index, b->data, b->prev_hash, nonce);
        if ((h >> DIFFICULTY) == 0) { b->nonce = nonce; b->hash = h; return nonce; }
        nonce++;
    }
}

/* Проверка: пересчитать хеш, сверить связь с предыдущим и условие PoW.
   Возвращает индекс первого битого блока или -1, если цепочка цела. */
static int verify(const Block *chain, int n) {
    for (int i = 0; i < n; i++) {
        uint32_t h = block_hash(chain[i].index, chain[i].data,
                                chain[i].prev_hash, chain[i].nonce);
        if (h != chain[i].hash) return i;                       /* данные правлены */
        if ((h >> DIFFICULTY) != 0) return i;                   /* PoW не выполнен */
        if (i > 0 && chain[i].prev_hash != chain[i - 1].hash) return i;  /* связь порвана */
    }
    return -1;
}

int main(void) {
    const char *payload[NBLOCKS] = {
        "GENESIS",
        "TX 4.2M EUR -> ZURICH",
        "TX 1.8M EUR -> LONDON",
        "TX 9.6M EUR -> CAYMAN",
        "AUDIT EVA ZIMMERMAN"
    };

    Block chain[NBLOCKS];

    printf("=== MOONLIGHT LEDGER (blockchain) ===\n");
    printf("difficulty: top %d bits must be zero\n\n", 32 - DIFFICULTY);
    printf("%5s %-24s %10s %10s %10s\n", "index", "data", "prev", "nonce", "hash");

    for (int i = 0; i < NBLOCKS; i++) {
        chain[i].index = i;
        snprintf(chain[i].data, sizeof chain[i].data, "%s", payload[i]);
        chain[i].prev_hash = (i == 0) ? 0u : chain[i - 1].hash;
        mine(&chain[i]);
        printf("%5d %-24s 0x%08X %10u 0x%08X\n",
               chain[i].index, chain[i].data, chain[i].prev_hash,
               chain[i].nonce, chain[i].hash);
    }

    int bad = verify(chain, NBLOCKS);
    printf("\nverify: %s\n", (bad < 0) ? "chain valid" : "BROKEN");

    /* Подделка: меняем сумму в блоке 2, всё остальное оставляем как есть. */
    printf("\n--- tampering: block 2 amount 1.8M -> 8.1M ---\n");
    snprintf(chain[2].data, sizeof chain[2].data, "TX 8.1M EUR -> LONDON");

    bad = verify(chain, NBLOCKS);
    printf("verify: %s", (bad < 0) ? "chain valid" : "BROKEN");
    if (bad >= 0) printf(" at block %d", bad);
    printf("\n");

    uint32_t recomputed = block_hash(chain[2].index, chain[2].data,
                                     chain[2].prev_hash, chain[2].nonce);
    printf("block 2 stored hash 0x%08X, recomputed 0x%08X — mismatch\n",
           chain[2].hash, recomputed);
    printf("\nto hide this, the forger must re-mine block 2 AND every block after it.\n");
    return 0;
}
