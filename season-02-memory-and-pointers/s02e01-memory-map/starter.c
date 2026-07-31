/*
 * OPERATION MOONLIGHT — s02e01 "Карта памяти"
 * Каркас. Скопируй в artifacts/memmap.c и закрой TODO.
 *
 *   cp starter.c artifacts/memmap.c
 *   make test
 *
 * Концепт: у каждой переменной есть адрес (&), типы имеют размер (sizeof),
 *          элементы массива лежат в памяти подряд.
 */
#include <stdio.h>

int main(void) {
    printf("=== MEMORY MAP ===\n");

    /* TODO 1: напечатай размеры типов через sizeof (спецификатор %zu):
       printf("sizeof: char=%zu int=%zu long=%zu double=%zu ptr=%zu\n",
              sizeof(char), sizeof(int), sizeof(long), sizeof(double), sizeof(void*)); */

    int arr[5] = {10, 20, 30, 40, 50};
    /* TODO 2: напечатай "int arr[5] occupies %zu bytes\n" — sizeof(arr). */

    /* TODO 3: посчитай разрыв адресов соседних элементов:
       long gap = (char*)&arr[1] - (char*)&arr[0];
       напечатай его и сравни с sizeof(int). */

    /* TODO 4: проверь, что ВСЕ соседние элементы отстоят на sizeof(int)
       (цикл), и напечатай "all elements contiguous: yes/no". */

    /* TODO 5: int x = 42; int *p = &x; напечатай, что *p читает 42:
       printf("\nint x = 42; int *p = &x;  =>  *p reads %d\n", *p); */

    (void)arr;
    return 0;
}
