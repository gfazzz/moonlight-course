/*
 * OPERATION MOONLIGHT — s02e01 "Карта памяти"
 * Эталонное решение: memmap.c
 *
 * Концепт серии: модель памяти — у каждой переменной есть адрес (&),
 *                типы имеют размер (sizeof), элементы массива лежат подряд.
 * Задача: показать размеры типов и что массив занимает непрерывный блок.
 *
 * Вывод детерминирован на x86-64 (Linux) — платформе автотеста.
 */
#include <stdio.h>

int main(void) {
    printf("=== MEMORY MAP ===\n");
    printf("sizeof: char=%zu int=%zu long=%zu double=%zu ptr=%zu\n",
           sizeof(char), sizeof(int), sizeof(long), sizeof(double), sizeof(void *));

    int arr[5] = {10, 20, 30, 40, 50};
    printf("\nint arr[5] occupies %zu bytes\n", sizeof(arr));

    /* Разница адресов соседних элементов = размер элемента (непрерывность). */
    long gap = (char *)&arr[1] - (char *)&arr[0];
    printf("gap between &arr[i] and &arr[i+1] = %ld bytes (== sizeof(int) = %zu)\n",
           gap, sizeof(int));

    int contiguous = 1;
    for (int i = 0; i < 4; i++) {
        if ((char *)&arr[i + 1] - (char *)&arr[i] != (long)sizeof(int)) contiguous = 0;
    }
    printf("all elements contiguous: %s\n", contiguous ? "yes" : "no");

    /* Адрес переменной (&) и чтение по нему через указатель (*). */
    int x = 42;
    int *p = &x;             /* p хранит адрес x */
    printf("\nint x = 42; int *p = &x;  =>  *p reads %d\n", *p);
    return 0;
}
