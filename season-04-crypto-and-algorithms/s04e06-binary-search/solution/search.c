/*
 * OPERATION MOONLIGHT — s04e06 "Двоичный поиск и Big O"
 * Эталонное решение: search.c
 *
 * Концепт серии: в отсортированных данных поиск не обязан смотреть все элементы.
 *                Двоичный поиск делит диапазон пополам: ~log2(n) шагов вместо n.
 * Задача: реализовать оба поиска со счётчиками и увидеть рост log n на практике.
 */
#include <stdio.h>
#include <stdlib.h>

/* Линейный поиск: идём подряд. Возвращает индекс или -1; steps — число проверок. */
static int linear_search(const int *a, int n, int target, long *steps) {
    *steps = 0;
    for (int i = 0; i < n; i++) {
        (*steps)++;
        if (a[i] == target) return i;
    }
    return -1;
}

/* Двоичный поиск: на каждом шаге отбрасываем половину диапазона. */
static int binary_search(const int *a, int n, int target, long *steps) {
    *steps = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        (*steps)++;
        int mid = lo + (hi - lo) / 2;        /* так не переполнится, в отличие от (lo+hi)/2 */
        if (a[mid] == target) return mid;
        if (a[mid] < target) lo = mid + 1;   /* цель правее */
        else                 hi = mid - 1;   /* цель левее */
    }
    return -1;
}

int main(void) {
    printf("=== SEARCH: O(n) vs O(log n) ===\n");

    /* Демонстрация на маленьком отсортированном массиве: значения i*3. */
    int demo[16];
    for (int i = 0; i < 16; i++) demo[i] = i * 3;

    printf("\ndemo (16 sorted values):");
    for (int i = 0; i < 16; i++) printf(" %d", demo[i]);
    printf("\n");

    long st = 0;
    int idx = binary_search(demo, 16, 27, &st);
    printf("binary search 27 -> index %d in %ld steps\n", idx, st);
    idx = binary_search(demo, 16, 28, &st);
    printf("binary search 28 -> %s (%ld steps)\n", idx < 0 ? "not found" : "found", st);

    /* Худший случай для линейного поиска: последний элемент. */
    printf("\n--- worst case: searching the LAST element ---\n");
    printf("%9s %14s %14s\n", "n", "linear steps", "binary steps");

    int sizes[] = {1024, 65536, 1048576};
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        int *a = malloc((size_t)n * sizeof(int));
        if (a == NULL) { perror("malloc"); return 1; }
        for (int i = 0; i < n; i++) a[i] = i * 3;

        int target = (n - 1) * 3;
        long sl = 0, sb = 0;
        linear_search(a, n, target, &sl);
        binary_search(a, n, target, &sb);
        printf("%9d %14ld %14ld\n", n, sl, sb);
        free(a);
    }

    printf("\nbinary halves the range each step: ~log2(n) steps instead of n.\n");
    return 0;
}
