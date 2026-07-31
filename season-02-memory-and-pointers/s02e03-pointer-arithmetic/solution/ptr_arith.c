/*
 * OPERATION MOONLIGHT — s02e03 "Указатели и массивы"
 * Эталонное решение: ptr_arith.c
 *
 * Концепт серии: арифметика указателей. Имя массива «распадается» в указатель
 *                на первый элемент; p+i указывает на i-й элемент; *(p+i) == p[i];
 *                разность указателей считается в элементах.
 * Задача: пройти массив только через указатели (без индексов).
 */
#include <stdio.h>

int main(void) {
    int arr[6] = {5, 10, 15, 20, 25, 30};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    int *p = arr;   /* массив -> указатель на первый элемент */

    printf("=== POINTER ARITHMETIC ===\n");
    printf("arr[0]=%d, *p=%d, *(p+2)=%d, p[2]=%d\n", arr[0], *p, *(p + 2), p[2]);

    /* Сумма обходом указателем: q идёт от начала до arr+n (за последний элемент). */
    long sum = 0;
    for (int *q = arr; q < arr + n; q++) sum += *q;
    printf("sum via pointer walk = %ld\n", sum);

    /* Разность указателей — в ЭЛЕМЕНТАХ, а не в байтах. */
    printf("arr+n - arr = %ld elements\n", (arr + n) - arr);

    /* Максимум и его индекс через вычитание указателей. */
    int *max = arr;
    for (int *q = arr + 1; q < arr + n; q++) if (*q > *max) max = q;
    printf("max = %d at index %ld\n", *max, max - arr);
    return 0;
}
