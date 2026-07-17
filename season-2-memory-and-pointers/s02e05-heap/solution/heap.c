/*
 * OPERATION MOONLIGHT — s02e05 "Куча"
 * Эталонное решение: heap.c
 *
 * Концепт серии: динамическая память. malloc берёт блок в куче (heap),
 *                размер — в рантайме; free возвращает его. Проверяй malloc на NULL,
 *                каждый malloc — свой free, после free обнуляй указатель.
 * Задача: выделить массив на n элементов, заполнить, посчитать, освободить.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 10;   /* в реальности приходит из рантайма (файла/аргумента/сети) */

    printf("=== HEAP: malloc/free ===\n");

    int *buf = malloc((size_t)n * sizeof(int));   /* размер в БАЙТАХ */
    if (buf == NULL) { perror("malloc"); return 1; }   /* malloc может вернуть NULL */

    for (int i = 0; i < n; i++) buf[i] = i * i;

    long sum = 0;
    for (int i = 0; i < n; i++) sum += buf[i];

    printf("allocated %d ints on heap (%zu bytes)\n", n, (size_t)n * sizeof(int));
    printf("buf[0]=%d buf[%d]=%d sum=%ld\n", buf[0], n - 1, buf[n - 1], sum);

    free(buf);        /* вернули память */
    buf = NULL;       /* чтобы случайно не воспользоваться освобождённым (dangling) */
    printf("freed; buf set to NULL: %s\n", buf == NULL ? "yes" : "no");
    return 0;
}
