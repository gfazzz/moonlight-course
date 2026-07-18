/*
 * OPERATION MOONLIGHT — s04e07 "Связный список"
 * Эталонное решение: list.c
 *
 * Концепт серии: список платит O(n) за поиск, но даёт O(1) вставку в голову —
 *                там, где массив сдвигает всё. Плюс классика указателей:
 *                разворот списка и поиск цикла алгоритмом Флойда (заяц и черепаха).
 * Задача: построить список, найти, развернуть, обнаружить цикл и всё освободить.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

/* Вставка в голову — O(1): переставили два указателя. */
static int push(Node **head, int value) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) return -1;
    n->value = value;
    n->next = *head;
    *head = n;
    return 0;
}

static void print_list(const Node *head) {
    for (const Node *p = head; p != NULL; p = p->next) printf(" %d", p->value);
    printf("\n");
}

static int length(const Node *head) {
    int n = 0;
    for (const Node *p = head; p != NULL; p = p->next) n++;
    return n;
}

/* Поиск — O(n): другого пути в списке нет. Возвращает позицию или -1. */
static int find(const Node *head, int value) {
    int pos = 0;
    for (const Node *p = head; p != NULL; p = p->next, pos++)
        if (p->value == value) return pos;
    return -1;
}

/* Разворот на месте: классика трёх указателей. */
static Node *reverse(Node *head) {
    Node *prev = NULL;
    while (head != NULL) {
        Node *next = head->next;   /* сохранить до перезаписи */
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}

/* Флойд: медленный на шаг, быстрый на два. Встретились — есть цикл. */
static const Node *detect_cycle(const Node *head) {
    const Node *slow = head, *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return slow;      /* точка встречи внутри цикла */
    }
    return NULL;
}

static void free_list(Node *head) {
    while (head != NULL) {
        Node *nx = head->next;   /* сохранить до free */
        free(head);
        head = nx;
    }
}

int main(void) {
    Node *head = NULL;

    printf("=== LINKED LIST ===\n");

    for (int v = 10; v <= 50; v += 10)
        if (push(&head, v) != 0) { perror("malloc"); free_list(head); return 1; }

    printf("built (head insert):");
    print_list(head);
    printf("length: %d\n", length(head));

    printf("search 30 -> %d\n", find(head, 30));
    printf("search 99 -> %d (not found)\n", find(head, 99));

    head = reverse(head);
    printf("reversed:");
    print_list(head);

    printf("\n--- Floyd's cycle detection ---\n");
    printf("cycle in a normal list: %s\n", detect_cycle(head) ? "yes" : "no");

    /* Замыкаем хвост на третий узел — получаем цикл. */
    Node *tail = head, *third = head;
    while (tail->next != NULL) tail = tail->next;
    for (int i = 0; i < 2 && third->next != NULL; i++) third = third->next;
    tail->next = third;

    const Node *meet = detect_cycle(head);
    printf("after linking tail -> node[2]: %s", meet ? "cycle detected" : "no cycle");
    if (meet) printf(" (meeting at value %d)", meet->value);
    printf("\n");

    tail->next = NULL;          /* разрываем цикл, иначе free зациклится */
    free_list(head);
    printf("cycle broken; list freed\n");
    return 0;
}
