/*
 * OPERATION MOONLIGHT — s02e02 "Указатели"
 * Каркас. Скопируй в artifacts/swap.c и закрой TODO.
 *
 *   cp starter.c artifacts/swap.c
 *   make test
 *
 * Концепт: указатель хранит адрес. *p — читать/писать данные по адресу.
 * Задача: swap двух int через указатели (то, что нельзя без них).
 */
#include <stdio.h>

/* TODO 1: реализуй swap — поменять местами значения по адресам a и b.
 * void swap(int *a, int *b) {
 *     int tmp = *a; *a = *b; *b = tmp;
 * }
 */

int main(void) {
    int x = 42, y = 99;

    printf("=== POINTERS ===\n");

    int *p = &x;   /* p хранит адрес x */
    /* TODO 2: напечатай "x=%d, *p=%d (p -> x)\n", x, *p */

    /* TODO 3: *p = 7;  затем напечатай "after *p=7: x=%d\n", x  (должно быть 7) */

    printf("\nbefore swap: x=%d y=%d\n", x, y);
    /* TODO 4: вызови swap(&x, &y); напечатай "after swap:  x=%d y=%d\n", x, y */

    int *np = NULL;
    /* TODO 5: напечатай "\nNULL pointer is safe to test: np is %s\n",
       np == NULL ? "NULL" : "set" */

    (void)p; (void)np;
    return 0;
}
