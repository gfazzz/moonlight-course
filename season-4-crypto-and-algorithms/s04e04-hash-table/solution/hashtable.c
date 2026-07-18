/*
 * OPERATION MOONLIGHT — s04e04 "Хеш-таблица"
 * Эталонное решение: hashtable.c
 *
 * Концепт серии: хеш-таблица = массив корзин + разрешение коллизий цепочками.
 *                put/get за O(1) в среднем; load factor показывает, когда пора расти.
 *                Собирает S2 (malloc/списки) и s04e03 (хеш).
 * Задача: построить таблицу с цепочками, вставить записи, найти, освободить.
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

/* Вставка в голову цепочки нужной корзины. */
static int ht_put(Table *t, const char *key, int value) {
    uint32_t idx = djb2(key) % NBUCKETS;
    Node *n = malloc(sizeof(Node));
    if (n == NULL) return -1;
    n->key = dup_str(key);
    if (n->key == NULL) { free(n); return -1; }
    n->value = value;
    n->next = t->buckets[idx];      /* старая голова становится следующей */
    t->buckets[idx] = n;
    t->count++;
    return 0;
}

/* Поиск: та же корзина, затем линейный проход по цепочке. */
static int ht_get(const Table *t, const char *key, int *out) {
    uint32_t idx = djb2(key) % NBUCKETS;
    for (Node *n = t->buckets[idx]; n != NULL; n = n->next)
        if (strcmp(n->key, key) == 0) { *out = n->value; return 1; }
    return 0;
}

static void ht_free(Table *t) {
    for (int i = 0; i < NBUCKETS; i++) {
        Node *n = t->buckets[i];
        while (n != NULL) {
            Node *nx = n->next;      /* сохранить до free (см. s02e08) */
            free(n->key);
            free(n);
            n = nx;
        }
        t->buckets[i] = NULL;
    }
    t->count = 0;
}

int main(void) {
    Table t = {{0}, 0};
    const char *keys[] = {"viktor", "eva", "ghost", "anna", "krylov",
                          "weiss", "marcus", "luna", "architect", "dmitry"};
    int n = 10;

    printf("=== HASH TABLE (chaining, %d buckets) ===\n", NBUCKETS);

    for (int i = 0; i < n; i++)
        if (ht_put(&t, keys[i], (i + 1) * 10) != 0) { perror("malloc"); ht_free(&t); return 1; }

    /* Распределение по корзинам: видно коллизии (цепочки длиннее 1). */
    printf("\n--- bucket distribution ---\n");
    for (int i = 0; i < NBUCKETS; i++) {
        int len = 0;
        for (Node *p = t.buckets[i]; p; p = p->next) len++;
        printf("bucket %d (%d):", i, len);
        for (Node *p = t.buckets[i]; p; p = p->next) printf(" %s", p->key);
        printf("\n");
    }

    printf("\n--- lookups ---\n");
    const char *probe[] = {"eva", "architect", "unknown"};
    for (int i = 0; i < 3; i++) {
        int v = 0;
        if (ht_get(&t, probe[i], &v)) printf("%-10s -> %d\n", probe[i], v);
        else                          printf("%-10s -> not found\n", probe[i]);
    }

    printf("\nentries=%d buckets=%d load factor=%.2f\n",
           t.count, NBUCKETS, (double)t.count / NBUCKETS);

    ht_free(&t);
    printf("table freed\n");
    return 0;
}
