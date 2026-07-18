/*
 * OPERATION MOONLIGHT — s04e05 "Сортировки"
 * Эталонное решение: sorting.c
 *
 * Концепт серии: разница между O(n²) и O(n log n) — не теория, а часы работы.
 *                Считаем РЕАЛЬНОЕ число сравнений у insertion sort и merge sort
 *                на растущих массивах и смотрим, как расходятся кривые.
 * Задача: реализовать обе сортировки со счётчиками и сравнить.
 *
 * Примечание: Shell sort, heapsort и перемешивание Кнута — в Season 8
 * (там они нужны для больших данных); здесь база и её анализ.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long cmp_ins, cmp_mrg;

/* Детерминированный генератор (LCG) — чтобы результат воспроизводился. */
static unsigned long lcg_state;
static int lcg_next(void) {
    lcg_state = lcg_state * 1103515245UL + 12345UL;
    return (int)((lcg_state >> 16) & 0x7FFF);
}

static void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && (cmp_ins++, a[j] > key)) {   /* счётчик до сравнения */
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

static void merge_sort(int *a, int n, int *tmp) {
    if (n < 2) return;
    int mid = n / 2;
    merge_sort(a, mid, tmp);
    merge_sort(a + mid, n - mid, tmp);

    int i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        cmp_mrg++;
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else              tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < n)   tmp[k++] = a[j++];
    memcpy(a, tmp, (size_t)n * sizeof(int));
}

static int is_sorted(const int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}

int main(void) {
    printf("=== SORTING: O(n^2) vs O(n log n) ===\n");

    /* Маленький массив — покажем результат наглядно. */
    int demo[10] = {42, 7, 19, 3, 88, 15, 61, 4, 23, 50};
    int demo2[10];
    memcpy(demo2, demo, sizeof demo);

    printf("\nbefore: ");
    for (int i = 0; i < 10; i++) printf("%d ", demo[i]);
    cmp_ins = 0;
    insertion_sort(demo, 10);
    printf("\nafter:  ");
    for (int i = 0; i < 10; i++) printf("%d ", demo[i]);
    printf("\nsorted: %s (insertion, %llu comparisons)\n",
           is_sorted(demo, 10) ? "yes" : "no", cmp_ins);

    /* Как растёт число сравнений с размером массива. */
    printf("\n--- comparisons vs input size ---\n");
    printf("%8s %14s %14s %8s\n", "n", "insertion", "merge", "ratio");

    int sizes[] = {64, 256, 1024};
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        int *a = malloc((size_t)n * sizeof(int));
        int *b = malloc((size_t)n * sizeof(int));
        int *tmp = malloc((size_t)n * sizeof(int));
        if (!a || !b || !tmp) { perror("malloc"); free(a); free(b); free(tmp); return 1; }

        lcg_state = 2024;                       /* один и тот же сид -> воспроизводимо */
        for (int i = 0; i < n; i++) { a[i] = lcg_next(); b[i] = a[i]; }

        cmp_ins = cmp_mrg = 0;
        insertion_sort(a, n);
        merge_sort(b, n, tmp);

        printf("%8d %14llu %14llu %7.1fx\n", n, cmp_ins, cmp_mrg,
               (double)cmp_ins / (double)cmp_mrg);

        if (!is_sorted(a, n) || !is_sorted(b, n)) { printf("ERROR: not sorted\n"); }
        free(a); free(b); free(tmp);
    }

    printf("\ninsertion grows ~n^2, merge ~n log n: the gap widens with n.\n");
    return 0;
}
