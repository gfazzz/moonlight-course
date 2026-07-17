/*
 * OPERATION MOONLIGHT — s02e06 "realloc"
 * Каркас. Скопируй в artifacts/grow.c и закрой TODO.
 *
 *   cp starter.c artifacts/grow.c
 *   make test
 *
 * Концепт: realloc меняет размер блока (обычно удваивают ёмкость).
 *          Безопасно: результат realloc — во ВРЕМЕННЫЙ указатель.
 * Задача: динамический массив, растущий по мере добавления (1..10, значения i*i).
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int cap = 1, len = 0;
    int *a = malloc((size_t)cap * sizeof(int));
    if (a == NULL) { perror("malloc"); return 1; }

    printf("=== REALLOC: growing array ===\n");

    for (int i = 1; i <= 10; i++) {
        if (len == cap) {
            cap *= 2;
            /* TODO 1: int *tmp = realloc(a, cap * sizeof(int));
               if (tmp == NULL) { free(a); perror("realloc"); return 1; }
               a = tmp;
               напечатай "grow -> capacity %d\n", cap */
        }
        /* TODO 2: a[len++] = i * i; */
    }

    printf("len=%d cap=%d\n", len, cap);

    /* TODO 3: напечатай "elements: " + все элементы через "%d " + сумму "sum=%ld". */

    /* TODO 4: free(a); */
    (void)a;
    return 0;
}
