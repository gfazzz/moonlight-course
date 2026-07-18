/*
 * OPERATION MOONLIGHT — s04e06 "Двоичный поиск и Big O"
 * Каркас. Скопируй в artifacts/search.c и закрой TODO.
 *
 *   cp starter.c artifacts/search.c
 *   make test
 *
 * Концепт: в отсортированных данных поиск делит диапазон пополам -> ~log2(n) шагов.
 */
#include <stdio.h>
#include <stdlib.h>

/* TODO 1: linear_search(a, n, target, &steps) — идти подряд, считая проверки;
   вернуть индекс или -1. */

/* TODO 2: binary_search(a, n, target, &steps):
     lo = 0, hi = n-1;
     while (lo <= hi) { steps++;
       mid = lo + (hi - lo)/2;        // так не переполнится
       если a[mid] == target -> вернуть mid;
       если a[mid] < target  -> lo = mid + 1;  иначе hi = mid - 1; }
     вернуть -1. */

int main(void) {
    printf("=== SEARCH: O(n) vs O(log n) ===\n");

    int demo[16];
    for (int i = 0; i < 16; i++) demo[i] = i * 3;

    printf("\ndemo (16 sorted values):");
    for (int i = 0; i < 16; i++) printf(" %d", demo[i]);
    printf("\n");

    /* TODO 3: двоичный поиск 27 -> "binary search 27 -> index %d in %ld steps\n"
       и 28 (нет в массиве) -> "binary search 28 -> not found (%ld steps)\n" */

    printf("\n--- worst case: searching the LAST element ---\n");
    printf("%9s %14s %14s\n", "n", "linear steps", "binary steps");

    int sizes[] = {1024, 65536, 1048576};
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        /* TODO 4: выдели массив на n элементов, заполни a[i] = i*3;
           ищи последний элемент обоими способами, напечатай "%9d %14ld %14ld";
           освободи память. */
        (void)n;
    }

    printf("\nbinary halves the range each step: ~log2(n) steps instead of n.\n");
    (void)demo;
    return 0;
}
