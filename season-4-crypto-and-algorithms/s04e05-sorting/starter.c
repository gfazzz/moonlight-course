/*
 * OPERATION MOONLIGHT — s04e05 "Сортировки"
 * Каркас. Скопируй в artifacts/sorting.c и закрой TODO.
 *
 *   cp starter.c artifacts/sorting.c
 *   make test
 *
 * Концепт: измерить РЕАЛЬНУЮ разницу между O(n^2) и O(n log n) счётчиком сравнений.
 * (Shell sort, heapsort и перемешивание Кнута — в Season 8.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long cmp_ins, cmp_mrg;

/* Детерминированный генератор — не меняй сид, иначе тест не сойдётся. */
static unsigned long lcg_state;
static int lcg_next(void) {
    lcg_state = lcg_state * 1103515245UL + 12345UL;
    return (int)((lcg_state >> 16) & 0x7FFF);
}

/* TODO 1: insertion_sort(a, n) — вставками; каждое сравнение a[j] > key
   считай в cmp_ins (подсказка: while (j >= 0 && (cmp_ins++, a[j] > key))). */

/* TODO 2: merge_sort(a, n, tmp) — рекурсивно делим пополам, затем слияние;
   каждое сравнение a[i] <= a[j] при слиянии считай в cmp_mrg;
   после слияния memcpy обратно в a. */

static int is_sorted(const int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}

int main(void) {
    printf("=== SORTING: O(n^2) vs O(n log n) ===\n");

    int demo[10] = {42, 7, 19, 3, 88, 15, 61, 4, 23, 50};
    /* TODO 3: печать "before: ", сортировка вставками, печать "after: ",
       затем "sorted: yes/no (insertion, %llu comparisons)". */

    printf("\n--- comparisons vs input size ---\n");
    printf("%8s %14s %14s %8s\n", "n", "insertion", "merge", "ratio");

    int sizes[] = {64, 256, 1024};
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        /* TODO 4: выдели a, b, tmp; lcg_state = 2024; заполни a[i]=lcg_next(), b[i]=a[i];
           обнули счётчики, отсортируй a вставками, b слиянием;
           напечатай "%8d %14llu %14llu %7.1fx" (n, cmp_ins, cmp_mrg, отношение);
           освободи память. */
        (void)n;
    }

    printf("\ninsertion grows ~n^2, merge ~n log n: the gap widens with n.\n");
    (void)demo; (void)is_sorted;
    return 0;
}
