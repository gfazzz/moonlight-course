/*
 * OPERATION MOONLIGHT — s02e08 "Охота на утечки"
 * УЧЕБНЫЙ ФАЙЛ (не для сдачи): логгер, который ТЕЧЁТ.
 *
 * Он работает и печатает правильно — но не освобождает память.
 * Твоя задача: найти все утечки и написать корректную версию в
 * artifacts/fixed_logger.c. Доказательство — чистый прогон под санитайзером.
 *
 * Посмотреть утечки:
 *   gcc -fsanitize=address -g buggy_logger.c -o buggy && ./buggy
 *   # или (Linux): valgrind --leak-check=full ./buggy
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LogEntry {
    char *message;
    struct LogEntry *next;
} LogEntry;

typedef struct {
    LogEntry *head;
    LogEntry *tail;
    int count;
} LogBuffer;

static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static int add_log(LogBuffer *b, const char *msg) {
    LogEntry *e = malloc(sizeof(LogEntry));       /* LEAK: узел не освобождается */
    e->message = dup_str(msg);                    /* LEAK: строка не освобождается */
    e->next = NULL;
    if (b->tail) b->tail->next = e; else b->head = e;
    b->tail = e;
    b->count++;
    return 0;
}

/* БАГ: функции освобождения нет вовсе. Весь список утекает. */

int main(void) {
    LogBuffer buf = {0};
    const char *msgs[] = {
        "agent online",
        "dump analyzed",
        "trojan neutralized",
        "rendezvous: 55.7558N 37.6173E"
    };
    for (int i = 0; i < 4; i++) add_log(&buf, msgs[i]);

    for (LogEntry *e = buf.head; e != NULL; e = e->next)
        printf("[log] %s\n", e->message);

    printf("--- %d entries flushed ---\n", buf.count);
    /* БАГ: нет free_all(&buf) — 4 узла + 4 строки утекли. */
    return 0;
}
