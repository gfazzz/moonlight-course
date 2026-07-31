/*
 * OPERATION MOONLIGHT — s08e04 "Shell sort и heapsort"
 * Каркас. Скопируй в artifacts/sorts.c и закрой TODO.
 *
 *   cp starter.c artifacts/sorts.c
 *   make test
 *
 * Считаем СРАВНЕНИЯ и ОБМЕНЫ (а не время — оно невоспроизводимо).
 * Приём подсчёта: (cmps++, a[j] > key) — оператор «запятая» считает и сравнивает.
 */
#include <stdio.h>
#include <string.h>

#define N 4000

static long cmps, swaps;

static void swap_i(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; swaps++; }

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

/* TODO 1: insertion_sort — классическая сортировка вставками со счётчиками. */
static void insertion_sort(int *a, int n);

/* TODO 2: shell_sort — insertion с шагом h.
     h = 1; while (h < n/3) h = 3*h + 1;         // шаги Кнута: 1, 4, 13, 40, ...
     for (; h >= 1; h /= 3)
         for (i = h; i < n; i++) { key = a[i]; j = i - h;
             while (j >= 0 && (cmps++, a[j] > key)) { a[j+h] = a[j]; swaps++; j -= h; }
             a[j+h] = key; } */
static void shell_sort(int *a, int n);

/* TODO 3: sift_down(a, start, end) — просеивание вниз:
     root = start;
     while (2*root+1 <= end) { child = 2*root+1;
        if (child+1 <= end && (cmps++, a[child] < a[child+1])) child++;
        if ((cmps++, a[root] < a[child])) { swap_i(a, root, child); root = child; }
        else return; } */
static void sift_down(int *a, int start, int end);

/* TODO 4: heap_sort — построение кучи (start от n/2-1 до 0) + извлечение
   (end от n-1 вниз: swap_i(a,0,end); sift_down(a,0,end-1)). */
static void heap_sort(int *a, int n);

/* TODO 5: run(name, fn, n) — заполнить, обнулить счётчики, вызвать, напечатать:
     "%-16s сравнений %9ld, обменов %8ld, отсортирован: да|НЕТ" */
static void run(const char *name, void (*fn)(int *, int), int n);

int main(void) {
    printf("=== Shell sort и heapsort (перенос из Season 4) ===\n");
    printf("элементов: %d, данные детерминированные\n\n", N);

    /* TODO 6: run для "insertion", "shell (Knuth)", "heapsort". */

    /* TODO 7: худший случай — массив по убыванию (a[i] = b[i] = N - i):
         "\n--- худший случай: массив по убыванию ---"
         "insertion        сравнений %9ld"
         "heapsort         сравнений %9ld"
         "insertion хуже в %.1f раза — у heapsort худшего случая нет" */

    (void)insertion_sort; (void)shell_sort; (void)heap_sort; (void)run;
    (void)sift_down; (void)swap_i; (void)fill; (void)is_sorted;
    return 0;
}

static void insertion_sort(int *a, int n) { (void)a; (void)n; /* TODO 1 */ }
static void shell_sort(int *a, int n) { (void)a; (void)n; /* TODO 2 */ }
static void sift_down(int *a, int s, int e) { (void)a; (void)s; (void)e; /* TODO 3 */ }
static void heap_sort(int *a, int n) { (void)a; (void)n; /* TODO 4 */ }
static void run(const char *nm, void (*fn)(int *, int), int n) {
    (void)nm; (void)fn; (void)n; /* TODO 5 */
}
