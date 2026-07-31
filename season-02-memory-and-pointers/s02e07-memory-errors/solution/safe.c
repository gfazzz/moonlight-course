/*
 * OPERATION MOONLIGHT — s02e07 "Ошибки памяти"
 * Эталонное решение: safe.c
 *
 * Концепт серии: анатомия ошибок памяти — утечка, use-after-free, double-free,
 *                выход за границы. Здесь — КОРРЕКТНАЯ версия задачи из buggy.c:
 *                каждый malloc освобождён ровно один раз, после последнего использования.
 * Задача: продублировать строки в куче, использовать, освободить — без ошибок.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Своя версия strdup: выделить в куче копию строки (вместе с '\0'). */
static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;      /* +1 под '\0' */
    char *p = malloc(n);
    if (p == NULL) return NULL;
    memcpy(p, s, n);
    return p;
}

int main(void) {
    const char *names[] = {"viktor", "luna", "ghost"};
    int count = 3;

    printf("=== MEMORY: safe handling ===\n");

    for (int i = 0; i < count; i++) {
        char *c = dup_str(names[i]);
        if (c == NULL) { perror("malloc"); return 1; }

        printf("copy %d: %s (len %zu)\n", i, c, strlen(c));  /* используем ДО free */

        free(c);           /* освобождаем ровно один раз, после последнего использования */
        c = NULL;          /* больше по нему не ходим */
    }

    printf("all copies freed, no leaks\n");
    return 0;
}
