/*
 * OPERATION MOONLIGHT — s09e04 "Хранилище: страницы и B-дерево"
 * Каркас. Скопируй в artifacts/btree.c и реализуй.
 *
 *   cp starter.c artifacts/btree.c
 *   make test
 *
 * Алгоритм и требования — в mission.md, формат вывода — в tests/expected.txt.
 *
 * Схема вставки (классическая, CLRS гл. 18):
 *   - идём от корня вниз;
 *   - если потомок, в который предстоит спуститься, ПОЛОН — расщепляем его
 *     ЗАРАНЕЕ (превентивно), и только потом спускаемся;
 *   - тогда рекурсия никогда не возвращает «переполнение» наверх.
 */
#include <stdio.h>
#include <string.h>

#define T 3                      /* минимальная степень */
#define MAXK (2 * T - 1)
#define MAXC (2 * T)
#define MAXNODES 256

typedef struct {
    int  nkeys;
    int  leaf;
    int  key[MAXK];
    long val[MAXK];
    int  child[MAXC];
} Node;

static Node pool[MAXNODES];
static int  used = 0;
static int  root = -1;
static long page_reads = 0;              /* считай КАЖДОЕ чтение узла при поиске */

/* Реализуй: */
static void split_child(int parent_id, int i);
static void bt_insert(int k, long v);
static int  bt_search(int id, int k, long *out, int *depth);
static void bt_inorder(int id, int *count, int limit);
static int  tree_height(int id);

static int linear_scan(const int *keys, int n, int k, long *reads) {
    for (int i = 0; i < n; i++) { (*reads)++; if (keys[i] == k) return 1; }
    return 0;
}

int main(void) {
    printf("=== B-дерево: индекс над страницами ===\n");
    printf("минимальная степень T = %d (в узле от %d до %d ключей)\n\n", T, T - 1, MAXK);

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

    printf("--- обход по порядку (первые 15 ключей) ---\n  ");
    int count = 0;
    bt_inorder(root, &count, 15);
    printf("\n  всего ключей в дереве: %d\n\n", count);

    printf("низкое дерево = мало чтений с диска. Это и есть смысл B-дерева.\n");
    return 0;
}

/* --- Твоя реализация --- */
static void split_child(int parent_id, int i) { (void)parent_id; (void)i; }
static void bt_insert(int k, long v) { (void)k; (void)v; }
static int  bt_search(int id, int k, long *out, int *depth) {
    (void)id; (void)k; (void)out; (void)depth; return 0;
}
static void bt_inorder(int id, int *count, int limit) { (void)id; (void)count; (void)limit; }
static int  tree_height(int id) { (void)id; return 0; }
