/*
 * OPERATION MOONLIGHT — s02e07 "Ошибки памяти"
 * Каркас. Скопируй в artifacts/safe.c и закрой TODO.
 *
 *   cp starter.c artifacts/safe.c
 *   make test
 *
 * Изучи buggy.c (четыре классические ошибки), затем напиши КОРРЕКТНУЮ версию.
 * Концепт: избежать утечки, use-after-free, double-free, выхода за границы.
 * Задача: продублировать строки в куче, использовать, освободить — без ошибок.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO 1: dup_str(const char *s) — malloc(strlen(s)+1), memcpy, вернуть копию
   (или NULL при неудаче). Не забудь +1 под '\0'. */

int main(void) {
    const char *names[] = {"viktor", "luna", "ghost"};
    int count = 3;

    printf("=== MEMORY: safe handling ===\n");

    for (int i = 0; i < count; i++) {
        /* TODO 2: char *c = dup_str(names[i]); проверь на NULL. */
        /* TODO 3: используй c (печать) ДО free:
           printf("copy %d: %s (len %zu)\n", i, c, strlen(c)); */
        /* TODO 4: free(c) ровно один раз, после использования; затем c = NULL. */
    }

    printf("all copies freed, no leaks\n");
    return 0;
}
