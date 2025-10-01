// Episode 15: Sorting & Search Algorithms - Starter Templates
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// SORTING ALGORITHMS TEMPLATES
// ============================================================

// Bubble Sort - O(n²)
void bubble_sort(int arr[], int n) {
    // TODO: Реализовать
}

// Selection Sort - O(n²)
void selection_sort(int arr[], int n) {
    // TODO: Реализовать
}

// Insertion Sort - O(n²) worst, O(n) best
void insertion_sort(int arr[], int n) {
    // TODO: Реализовать
}

// Merge Sort - O(n log n)
void merge(int arr[], int l, int m, int r) {
    // TODO: Реализовать merge helper
}

void merge_sort(int arr[], int l, int r) {
    // TODO: Реализовать
}

// Quick Sort - O(n log n) average, O(n²) worst
int partition(int arr[], int low, int high) {
    // TODO: Реализовать partition
    return 0;
}

void quick_sort(int arr[], int low, int high) {
    // TODO: Реализовать
}

// ============================================================
// SEARCH ALGORITHMS TEMPLATES
// ============================================================

// Linear Search - O(n)
int linear_search(int arr[], int n, int target) {
    // TODO: Реализовать
    return -1;
}

// Binary Search - O(log n)
int binary_search(int arr[], int n, int target) {
    // TODO: Реализовать (массив должен быть отсортирован!)
    return -1;
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

// Печать массива
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Проверка отсортированности
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Benchmark функция
void benchmark_sort(void (*sort_func)(int[], int), const char* name, int arr[], int n) {
    int* copy = malloc(n * sizeof(int));
    memcpy(copy, arr, n * sizeof(int));
    
    clock_t start = clock();
    sort_func(copy, n);
    clock_t end = clock();
    
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%20s: %.6f sec [%s]\n", name, time, is_sorted(copy, n) ? "✓" : "✗");
    
    free(copy);
}

// ============================================================
// MAIN - ДЕМОНСТРАЦИЯ
// ============================================================

int main() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║  Episode 15: Sorting & Search        ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    int test_arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    int n = sizeof(test_arr) / sizeof(test_arr[0]);
    
    printf("Original array: ");
    print_array(test_arr, n);
    printf("\n");
    
    // TODO: Добавить benchmark для всех алгоритмов
    
    printf("\n--- SEARCH DEMO ---\n");
    printf("TODO: Реализовать linear и binary search\n");
    
    return 0;
}
