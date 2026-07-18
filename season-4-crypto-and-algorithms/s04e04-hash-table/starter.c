/*
 * OPERATION MOONLIGHT — s04e04 "Хеш-таблица"
 * Каркас. Скопируй в artifacts/hashtable.c и закрой TODO.
 *
 *   cp starter.c artifacts/hashtable.c
 *   make test
 *
 * Концепт: массив корзин + цепочки для коллизий. Собирает S2 (malloc/списки)
 *          и s04e03 (хеш). Всё выделенное должно быть освобождено (проверяет ASan).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NBUCKETS 8

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *buckets[NBUCKETS];
    int count;
} Table;

static uint32_t djb2(const char *s) {
    uint32_t h = 5381;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        h = ((h << 5) + h) + *p;
    return h;
}

static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* TODO 1: ht_put(t, key, value) — idx = djb2(key) % NBUCKETS;
   выдели Node, скопируй ключ (dup_str), вставь в ГОЛОВУ цепочки, t->count++.
   При неудаче malloc — освободи частично выделенное и верни -1. */

/* TODO 2: ht_get(t, key, &out) — та же корзина, пройди цепочку, сравни strcmp,
   верни 1 и запиши значение, иначе 0. */

/* TODO 3: ht_free(t) — по всем корзинам: free(n->key) и free(n),
   сохраняя n->next ДО free (иначе use-after-free). */

int main(void) {
    Table t = {{0}, 0};
    const char *keys[] = {"viktor", "eva", "ghost", "anna", "krylov",
                          "weiss", "marcus", "luna", "architect", "dmitry"};
    int n = 10;

    printf("=== HASH TABLE (chaining, %d buckets) ===\n", NBUCKETS);

    /* TODO 4: вставь все ключи со значениями (i+1)*10. */

    printf("\n--- bucket distribution ---\n");
    /* TODO 5: для каждой корзины: длина цепочки и перечисление ключей:
       "bucket %d (%d):" затем " %s" на каждый ключ, затем перевод строки. */

    printf("\n--- lookups ---\n");
    /* TODO 6: найди "eva", "architect", "unknown":
       "%-10s -> %d\n" или "%-10s -> not found\n" */

    /* TODO 7: "\nentries=%d buckets=%d load factor=%.2f\n" (count/NBUCKETS) */
    /* TODO 8: ht_free(&t); печать "table freed\n" */

    (void)keys; (void)n; (void)t;
    return 0;
}
