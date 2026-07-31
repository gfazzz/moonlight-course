/*
 * OPERATION MOONLIGHT — s02e08 "Охота на утечки" (финал Season 2)
 * Эталонное решение: fixed_logger.c
 *
 * Концепт серии: найти и устранить утечки в реальном коде; доказать чистоту
 *                инструментом (valgrind / AddressSanitizer). Собирает всё:
 *                связный список (e04) + malloc (e05) + дисциплина free (e07).
 * Задача: логгер копит записи в связном списке и КОРРЕКТНО освобождает их.
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

/* Добавить запись: узел + копия строки. Обе аллокации — под ответственность free_all. */
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

/* Освободить ВЕСЬ список: у каждого узла — и строку, и сам узел. Порядок важен:
   сохраняем next ДО free(e), иначе use-after-free. */
static void free_all(LogBuffer *b) {
    LogEntry *e = b->head;
    while (e != NULL) {
        LogEntry *nx = e->next;   /* сохранили next до освобождения узла */
        free(e->message);
        free(e);
        e = nx;
    }
    b->head = b->tail = NULL;
    b->count = 0;
}

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
    for (int i = 0; i < n; i++) {
        if (add_log(&buf, msgs[i]) != 0) { perror("malloc"); free_all(&buf); return 1; }
    }

    for (LogEntry *e = buf.head; e != NULL; e = e->next)
        printf("[log] %s\n", e->message);

    printf("--- %d entries flushed ---\n", buf.count);

    free_all(&buf);            /* без этого — утечка всех узлов и строк */
    printf("all entries freed\n");
    return 0;
}
