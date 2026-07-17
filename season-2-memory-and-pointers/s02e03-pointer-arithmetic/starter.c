/*
 * OPERATION MOONLIGHT — s02e03 "Указатели и массивы"
 * Каркас. Скопируй в artifacts/ptr_arith.c и закрой TODO.
 *
 *   cp starter.c artifacts/ptr_arith.c
 *   make test
 *
 * Концепт: p+i указывает на i-й элемент; *(p+i) == p[i]; разность указателей —
 *          в элементах. Пройди массив только через указатели.
 */
#include <stdio.h>

int main(void) {
    int arr[6] = {5, 10, 15, 20, 25, 30};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    int *p = arr;   /* имя массива распадается в указатель на первый элемент */

    printf("=== POINTER ARITHMETIC ===\n");
    /* TODO 1: покажи эквивалентности:
       printf("arr[0]=%d, *p=%d, *(p+2)=%d, p[2]=%d\n", arr[0], *p, *(p+2), p[2]); */

    /* TODO 2: сумма обходом указателем (без индексов):
       for (int *q = arr; q < arr + n; q++) sum += *q;  затем печать. */
    long sum = 0;

    /* TODO 3: разность указателей в элементах:
       printf("arr+n - arr = %ld elements\n", (arr + n) - arr); */

    /* TODO 4: максимум и его индекс через max-arr. */

    (void)p; (void)n; (void)sum;
    return 0;
}
