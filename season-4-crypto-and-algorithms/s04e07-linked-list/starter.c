/*
 * OPERATION MOONLIGHT — s04e07 "Связный список"
 * Каркас. Скопируй в artifacts/list.c и закрой TODO.
 *
 *   cp starter.c artifacts/list.c
 *   make test
 *
 * Концепт: O(1) вставка в голову, O(n) поиск; разворот тремя указателями;
 *          поиск цикла алгоритмом Флойда (медленный/быстрый).
 * ВАЖНО: перед освобождением цикл надо разорвать, иначе free зациклится.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

/* TODO 1: push(&head, value) — выделить узел, n->next = *head, *head = n. Вернуть -1 при ошибке. */

static void print_list(const Node *head) {
    for (const Node *p = head; p != NULL; p = p->next) printf(" %d", p->value);
    printf("\n");
}

/* TODO 2: length(head) — посчитать узлы. */
/* TODO 3: find(head, value) — вернуть позицию (0-based) или -1. */

/* TODO 4: reverse(head) — три указателя:
     prev = NULL; пока head: next = head->next; head->next = prev; prev = head; head = next;
     вернуть prev. */

/* TODO 5: detect_cycle(head) — Флойд: slow = slow->next, fast = fast->next->next;
     если slow == fast — вернуть точку встречи; если fast упёрся в NULL — вернуть NULL. */

/* TODO 6: free_list(head) — освободить, сохраняя next ДО free. */

int main(void) {
    Node *head = NULL;

    printf("=== LINKED LIST ===\n");

    /* TODO 7: push значений 10,20,30,40,50; печать "built (head insert):" + список,
       "length: %d", поиск 30 и 99, разворот и печать "reversed:". */

    printf("\n--- Floyd's cycle detection ---\n");
    /* TODO 8: печать "cycle in a normal list: yes|no".
       Затем: найди хвост, найди третий узел (head->next->next), замкни tail->next = third;
       вызови detect_cycle и напечатай
         "after linking tail -> node[2]: cycle detected (meeting at value %d)".
       Разорви цикл (tail->next = NULL), освободи список,
       напечатай "cycle broken; list freed". */

    (void)head;
    return 0;
}
