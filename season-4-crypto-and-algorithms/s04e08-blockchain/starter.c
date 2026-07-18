/*
 * OPERATION MOONLIGHT — s04e08 "Блокчейн" (финал Season 4)
 * Каркас. Скопируй в artifacts/blockchain.c и закрой TODO.
 *
 *   cp starter.c artifacts/blockchain.c
 *   make test
 *
 * Концепт: каждый блок хранит хеш предыдущего. Правка байта -> хеш меняется ->
 *          рвётся связь -> подделка видна. PoW делает подделку ещё и дорогой.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NBLOCKS    5
#define DIFFICULTY 20      /* hash >> DIFFICULTY == 0  (старшие 12 бит нулевые) */

typedef struct {
    int      index;
    char     data[64];
    uint32_t prev_hash;
    uint32_t nonce;
    uint32_t hash;
} Block;

static uint32_t mix(uint32_t h, uint32_t v) { h ^= v; h *= 16777619u; return h; }

/* TODO 1: block_hash(index, data, prev, nonce) — начать с 2166136261u и
   подмешать (mix) index, каждый байт data, prev и nonce. Вернуть хеш. */

/* TODO 2: mine(b) — перебирать nonce от 0, пока (hash >> DIFFICULTY) != 0;
   записать найденные b->nonce и b->hash. */

/* TODO 3: verify(chain, n) — для каждого блока:
     пересчитать хеш и сравнить с сохранённым;
     проверить условие PoW;
     при i > 0 сверить chain[i].prev_hash с chain[i-1].hash.
   Вернуть индекс первого битого блока или -1. */

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

    /* TODO 4: собери цепочку: index=i, data=payload[i],
       prev_hash = (i==0) ? 0 : chain[i-1].hash, затем mine();
       печатай строку "%5d %-24s 0x%08X %10u 0x%08X". */

    /* TODO 5: verify -> "\nverify: chain valid" или "BROKEN". */

    printf("\n--- tampering: block 2 amount 1.8M -> 8.1M ---\n");
    /* TODO 6: подмени chain[2].data на "TX 8.1M EUR -> LONDON";
       снова verify -> "verify: BROKEN at block %d";
       пересчитай хеш блока 2 и покажи расхождение:
       "block 2 stored hash 0x%08X, recomputed 0x%08X — mismatch\n" */

    printf("\nto hide this, the forger must re-mine block 2 AND every block after it.\n");
    (void)payload; (void)chain;
    return 0;
}
