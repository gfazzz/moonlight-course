/*
 * OPERATION MOONLIGHT — s02e08 "Охота на утечки" (финал Season 2)
 * Каркас. Скопируй в artifacts/fixed_logger.c и закрой TODO.
 *
 *   cp starter.c artifacts/fixed_logger.c
 *   make test
 *
 * Изучи buggy_logger.c (он течёт), затем почини здесь.
 * Концепт: найти все утечки и устранить; доказать чистоту санитайзером.
 * Собирает сезон: связный список + malloc + дисциплина free.
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
    LogEntry *e = malloc(sizeof(LogEntry));
    if (e == NULL) return -1;
    e->message = dup_str(msg);
    if (e->message == NULL) { free(e); return -1; }
    e->next = NULL;
    if (b->tail) b->tail->next = e; else b->head = e;
    b->tail = e;
    b->count++;
    return 0;
}

/* TODO 1: free_all(LogBuffer *b) — пройти список и освободить у КАЖДОГО узла
   и строку (e->message), и сам узел (e). Сохраняй e->next ДО free(e)! */

int main(void) {
    LogBuffer buf = {0};
    const char *msgs[] = {
        "agent online",
        "dump analyzed",
        "trojan neutralized",
        "rendezvous: 55.7558N 37.6173E"
    };
    int n = 4;

    printf("=== MOONLIGHT LOGGER ===\n");
    for (int i = 0; i < n; i++)
        if (add_log(&buf, msgs[i]) != 0) { perror("malloc"); return 1; }

    for (LogEntry *e = buf.head; e != NULL; e = e->next)
        printf("[log] %s\n", e->message);

    printf("--- %d entries flushed ---\n", buf.count);

    /* TODO 2: free_all(&buf); */
    printf("all entries freed\n");
    return 0;
}
