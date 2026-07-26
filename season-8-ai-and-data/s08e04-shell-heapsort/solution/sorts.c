/*
 * OPERATION MOONLIGHT — s08e04 "Shell sort и heapsort"
 * Эталонное решение: sorts.c
 *
 * Концепт серии: два алгоритма, перенесённые сюда из Season 4.
 *   - Shell sort: insertion sort с убывающим шагом. Дальние обмены убирают
 *     «инверсии на расстоянии», которые insertion sort двигает по одной.
 *     Скорость критически зависит от последовательности шагов.
 *   - Heapsort: строит кучу и извлекает максимумы. Гарантирует O(n log n)
 *     в ХУДШЕМ случае и не требует дополнительной памяти — в отличие от
 *     merge sort (память) и quicksort (худший случай O(n²)).
 * Задача: реализовать оба, посчитать реальные операции и сравнить с insertion.
 *
 * Замеры честные: считаем сравнения и обмены, а не время (оно не воспроизводимо).
 */
#include <stdio.h>
#include <string.h>

#define N 4000

static long cmps, swaps;

static void swap_i(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; swaps++; }

/* Детерминированные данные (LCG). */
static void fill(int *a, int n) {
    unsigned long s = 20260726UL;
    for (int i = 0; i < n; i++) {
        s = s * 1103515245UL + 12345UL;
        a[i] = (int)((s >> 16) & 0xFFFF);
    }
}

static int is_sorted(const int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}

/* --- Базовый ориентир: сортировка вставками --- */
static void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && (cmps++, a[j] > key)) { a[j + 1] = a[j]; swaps++; j--; }
        a[j + 1] = key;
    }
}

/* --- Shell sort с последовательностью Кнута: 1, 4, 13, 40, 121, ... (3h+1) --- */
static void shell_sort(int *a, int n) {
    int h = 1;
    while (h < n / 3) h = 3 * h + 1;

    for (; h >= 1; h /= 3) {
        for (int i = h; i < n; i++) {
            int key = a[i], j = i - h;
            while (j >= 0 && (cmps++, a[j] > key)) { a[j + h] = a[j]; swaps++; j -= h; }
            a[j + h] = key;
        }
    }
}

/* --- Heapsort: просеивание вниз + извлечение максимума --- */
static void sift_down(int *a, int start, int end) {
    int root = start;
    while (2 * root + 1 <= end) {
        int child = 2 * root + 1;
        if (child + 1 <= end && (cmps++, a[child] < a[child + 1])) child++;
        if ((cmps++, a[root] < a[child])) { swap_i(a, root, child); root = child; }
        else return;
    }
}

static void heap_sort(int *a, int n) {
    for (int start = n / 2 - 1; start >= 0; start--) sift_down(a, start, n - 1);  /* построение */
    for (int end = n - 1; end > 0; end--) {                                       /* извлечение */
        swap_i(a, 0, end);
        sift_down(a, 0, end - 1);
    }
}

static void run(const char *name, void (*fn)(int *, int), int n) {
    int a[N];
    fill(a, n);
    cmps = swaps = 0;
    fn(a, n);
    printf("%-16s сравнений %9ld, обменов %8ld, отсортирован: %s\n",
           name, cmps, swaps, is_sorted(a, n) ? "да" : "НЕТ");
}

int main(void) {
    printf("=== Shell sort и heapsort (перенос из Season 4) ===\n");
    printf("элементов: %d, данные детерминированные\n\n", N);

    run("insertion", insertion_sort, N);
    run("shell (Knuth)", shell_sort, N);
    run("heapsort", heap_sort, N);

    /* Худший случай для insertion: обратный порядок. Heapsort к нему нечувствителен. */
    printf("\n--- худший случай: массив по убыванию ---\n");
    int a[N], b[N];
    for (int i = 0; i < N; i++) a[i] = b[i] = N - i;

    cmps = swaps = 0; insertion_sort(a, N);
    long ins_worst = cmps;
    printf("insertion        сравнений %9ld\n", ins_worst);

    cmps = swaps = 0; heap_sort(b, N);
    long heap_worst = cmps;
    printf("heapsort         сравнений %9ld\n", heap_worst);
    printf("insertion хуже в %.1f раза — у heapsort худшего случая нет\n",
           (double)ins_worst / (double)heap_worst);

    return 0;
}
