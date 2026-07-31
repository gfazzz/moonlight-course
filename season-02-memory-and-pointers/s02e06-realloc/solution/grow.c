/*
 * OPERATION MOONLIGHT — s02e06 "realloc"
 * Эталонное решение: grow.c
 *
 * Концепт серии: растущие данные. realloc меняет размер блока в куче (обычно
 *                удваивают ёмкость). Безопасная идиома: результат realloc пишут
 *                во ВРЕМЕННЫЙ указатель — иначе при ошибке потеряешь старый блок.
 * Задача: динамический массив, растущий по мере добавления элементов.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int cap = 1, len = 0;
    int *a = malloc((size_t)cap * sizeof(int));
    if (a == NULL) { perror("malloc"); return 1; }

    printf("=== REALLOC: growing array ===\n");

    for (int i = 1; i <= 10; i++) {
        if (len == cap) {                       /* места нет — растём */
            cap *= 2;
            int *tmp = realloc(a, (size_t)cap * sizeof(int));
            if (tmp == NULL) { free(a); perror("realloc"); return 1; }
            a = tmp;                            /* только теперь переназначаем a */
            printf("grow -> capacity %d\n", cap);
        }
        a[len++] = i * i;
    }

    printf("len=%d cap=%d\n", len, cap);

    long sum = 0;
    printf("elements: ");
    for (int i = 0; i < len; i++) { printf("%d ", a[i]); sum += a[i]; }
    printf("\nsum=%ld\n", sum);

    free(a);
    return 0;
}
