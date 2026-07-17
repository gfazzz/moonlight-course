/*
 * OPERATION MOONLIGHT — s02e05 "Куча"
 * Каркас. Скопируй в artifacts/heap.c и закрой TODO.
 *
 *   cp starter.c artifacts/heap.c
 *   make test
 *
 * Концепт: malloc берёт блок в куче (размер в рантайме), free возвращает его.
 * Задача: выделить n int, заполнить (i*i), посчитать сумму, освободить.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 10;

    printf("=== HEAP: malloc/free ===\n");

    /* TODO 1: int *buf = malloc(n * sizeof(int));  — размер в БАЙТАХ.
       TODO 2: проверь buf на NULL: if (buf == NULL) { perror("malloc"); return 1; } */
    int *buf = NULL;

    /* TODO 3: заполни buf[i] = i*i; посчитай sum. */
    long sum = 0;

    printf("allocated %d ints on heap (%zu bytes)\n", n, (size_t)n * sizeof(int));
    /* TODO 4: напечатай "buf[0]=%d buf[%d]=%d sum=%ld\n", buf[0], n-1, buf[n-1], sum */

    /* TODO 5: free(buf); buf = NULL; напечатай
       "freed; buf set to NULL: %s\n", buf == NULL ? "yes" : "no" */

    (void)buf; (void)sum;
    return 0;
}
