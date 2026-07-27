/*
 * OPERATION MOONLIGHT — s09e04 "Хранилище: страницы и B-дерево"
 * Эталонное решение: btree.c
 *
 * Концепт серии: база данных не читает файл целиком (s08e01) и не хранит записи
 *                списком. Она делит файл на СТРАНИЦЫ фиксированного размера и
 *                строит над ними индекс — B-дерево. Ключевое отличие от двоичного
 *                дерева (Season 4): узел B-дерева содержит МНОГО ключей, поэтому
 *                дерево очень низкое. Это важно потому, что стоимость операции
 *                измеряется не сравнениями, а ЧТЕНИЯМИ СТРАНИЦ с диска.
 * Задача: B-дерево с расщеплением узлов, поиск, обход по порядку, замер чтений.
 *
 * Реализация классическая (CLRS, гл. 18): вставка сверху вниз с превентивным
 * расщеплением полного потомка — тогда рекурсия никогда не возвращает «переполнение».
 */
#include <stdio.h>
#include <string.h>

#define T 3                      /* минимальная степень: в узле от T-1 до 2T-1 ключей */
#define MAXK (2 * T - 1)
#define MAXC (2 * T)
#define MAXNODES 256

typedef struct {
    int  nkeys;
    int  leaf;
    int  key[MAXK];
    long val[MAXK];
    int  child[MAXC];            /* индексы узлов в пуле = «номера страниц» */
} Node;

static Node pool[MAXNODES];
static int  used = 0;
static int  root = -1;

/* Счётчик обращений к страницам: главная метрика дисковых структур. */
static long page_reads = 0;
static Node *page(int id) { page_reads++; return &pool[id]; }

static int node_alloc(int leaf) {
    int id = used++;
    Node *n = &pool[id];
    n->nkeys = 0; n->leaf = leaf;
    return id;
}

/* Расщепление полного потомка: середина уходит наверх, узел делится надвое. */
static void split_child(int parent_id, int i) {
    Node *p = &pool[parent_id];
    int full_id = p->child[i];
    Node *full = &pool[full_id];

    int new_id = node_alloc(full->leaf);
    Node *nn = &pool[new_id];
    nn->nkeys = T - 1;

    for (int j = 0; j < T - 1; j++) {           /* правая половина -> новый узел */
        nn->key[j] = full->key[j + T];
        nn->val[j] = full->val[j + T];
    }
    if (!full->leaf)
        for (int j = 0; j < T; j++) nn->child[j] = full->child[j + T];

    full->nkeys = T - 1;

    for (int j = p->nkeys; j > i; j--) p->child[j + 1] = p->child[j];
    p->child[i + 1] = new_id;
    for (int j = p->nkeys - 1; j >= i; j--) {
        p->key[j + 1] = p->key[j];
        p->val[j + 1] = p->val[j];
    }
    p->key[i] = full->key[T - 1];               /* средний ключ поднимается к родителю */
    p->val[i] = full->val[T - 1];
    p->nkeys++;
}

static void insert_nonfull(int id, int k, long v) {
    Node *n = &pool[id];
    int i = n->nkeys - 1;

    if (n->leaf) {
        while (i >= 0 && k < n->key[i]) { n->key[i + 1] = n->key[i]; n->val[i + 1] = n->val[i]; i--; }
        n->key[i + 1] = k;
        n->val[i + 1] = v;
        n->nkeys++;
        return;
    }

    while (i >= 0 && k < n->key[i]) i--;
    i++;
    if (pool[n->child[i]].nkeys == MAXK) {      /* превентивное расщепление */
        split_child(id, i);
        if (k > n->key[i]) i++;
    }
    insert_nonfull(n->child[i], k, v);
}

static void bt_insert(int k, long v) {
    if (root < 0) {
        root = node_alloc(1);
        pool[root].key[0] = k;
        pool[root].val[0] = v;
        pool[root].nkeys = 1;
        return;
    }
    if (pool[root].nkeys == MAXK) {             /* корень полон -> дерево растёт вверх */
        int nr = node_alloc(0);
        pool[nr].child[0] = root;
        root = nr;
        split_child(nr, 0);
    }
    insert_nonfull(root, k, v);
}

/* Поиск. Возвращает 1 и значение, попутно считая прочитанные страницы. */
static int bt_search(int id, int k, long *out, int *depth) {
    if (id < 0) return 0;
    Node *n = page(id);
    (*depth)++;
    int i = 0;
    while (i < n->nkeys && k > n->key[i]) i++;
    if (i < n->nkeys && k == n->key[i]) { *out = n->val[i]; return 1; }
    if (n->leaf) return 0;
    return bt_search(n->child[i], k, out, depth);
}

/* Обход по порядку: ключи выходят отсортированными — диапазонный запрос «бесплатен». */
static void bt_inorder(int id, int *count, int limit) {
    if (id < 0) return;
    Node *n = &pool[id];
    for (int i = 0; i < n->nkeys; i++) {
        if (!n->leaf) bt_inorder(n->child[i], count, limit);
        if (*count < limit) { printf("%d ", n->key[i]); }
        (*count)++;
    }
    if (!n->leaf) bt_inorder(n->child[n->nkeys], count, limit);
}

static int tree_height(int id) {
    if (id < 0) return 0;
    Node *n = &pool[id];
    return n->leaf ? 1 : 1 + tree_height(n->child[0]);
}

/* Для сравнения: линейный поиск по неупорядоченному «файлу» записей. */
static int linear_scan(const int *keys, int n, int k, long *reads) {
    for (int i = 0; i < n; i++) { (*reads)++; if (keys[i] == k) return 1; }
    return 0;
}

int main(void) {
    printf("=== B-дерево: индекс над страницами ===\n");
    printf("минимальная степень T = %d (в узле от %d до %d ключей)\n\n", T, T - 1, MAXK);

    /* Детерминированный набор ключей (LCG), значения — «смещения записей». */
    const int N = 200;
    int keys[200];
    unsigned long s = 20260726UL;
    for (int i = 0; i < N; i++) {
        s = s * 1103515245UL + 12345UL;
        keys[i] = (int)((s >> 16) % 1000);
        bt_insert(keys[i], 1000L + i);
    }

    printf("вставлено ключей: %d\n", N);
    printf("узлов (страниц) выделено: %d\n", used);
    printf("высота дерева: %d\n\n", tree_height(root));

    /* Поиск: сколько страниц прочитано. */
    printf("--- поиск ---\n");
    int probes[] = {keys[0], keys[77], keys[199], 1001};
    const char *label[] = {"первый ключ", "средний ключ", "последний ключ", "отсутствующий"};
    for (int i = 0; i < 4; i++) {
        long v = -1;
        int depth = 0;
        page_reads = 0;
        int found = bt_search(root, probes[i], &v, &depth);
        printf("  key=%4d (%s): %s, страниц прочитано: %ld\n",
               probes[i], label[i], found ? "найден" : "не найден", page_reads);
    }

    /* Сравнение с линейным сканированием того же объёма. */
    printf("\n--- B-дерево против линейного сканирования ---\n");
    long total_bt = 0, total_lin = 0;
    for (int i = 0; i < N; i++) {
        long v; int depth = 0;
        page_reads = 0;
        bt_search(root, keys[i], &v, &depth);
        total_bt += page_reads;

        long reads = 0;
        linear_scan(keys, N, keys[i], &reads);
        total_lin += reads;
    }
    printf("  %d поисков: B-дерево %ld чтений, линейно %ld чтений\n", N, total_bt, total_lin);
    printf("  выигрыш: %.1f раза\n\n", (double)total_lin / (double)total_bt);

    /* Обход по порядку — ключи уже отсортированы. */
    printf("--- обход по порядку (первые 15 ключей) ---\n  ");
    int count = 0;
    bt_inorder(root, &count, 15);
    printf("\n  всего ключей в дереве: %d\n\n", count);

    printf("низкое дерево = мало чтений с диска. Это и есть смысл B-дерева.\n");
    return 0;
}
