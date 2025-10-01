// Episode 15: Sorting & Search Algorithms - SOLUTION
// Complete implementation of all major sorting and search algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// SORTING ALGORITHMS
// ============================================================

// Bubble Sort - O(n²)
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Selection Sort - O(n²)
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Insertion Sort - O(n²) worst, O(n) best
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Shell Sort - O(n log n) to O(n²)
void shell_sort(int arr[], int n) {
    // Knuth's gap sequence: 1, 4, 13, 40, 121, ...
    int gap = 1;
    while (gap < n / 3) {
        gap = gap * 3 + 1;
    }
    
    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 3;
    }
}

// Merge Sort - O(n log n)
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) {
        arr[k++] = L[i++];
    }
    
    while (j < n2) {
        arr[k++] = R[j++];
    }
    
    free(L);
    free(R);
}

void merge_sort_recursive(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_recursive(arr, l, m);
        merge_sort_recursive(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge_sort(int arr[], int n) {
    merge_sort_recursive(arr, 0, n - 1);
}

// Quick Sort - O(n log n) average, O(n²) worst
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void quick_sort_recursive(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_recursive(arr, low, pi - 1);
        quick_sort_recursive(arr, pi + 1, high);
    }
}

void quick_sort(int arr[], int n) {
    quick_sort_recursive(arr, 0, n - 1);
}

// Heapsort - O(n log n) worst case
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        heapify(arr, n, largest);
    }
}

void heap_sort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    
    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        heapify(arr, i, 0);
    }
}

// ============================================================
// SEARCH ALGORITHMS
// ============================================================

// Linear Search - O(n)
int linear_search(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Binary Search - O(log n)
int binary_search(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

// Knuth Shuffle (Fisher-Yates) - O(n)
void knuth_shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Print array
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Check if sorted
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Benchmark function
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
// MAIN - DEMONSTRATION
// ============================================================

int main() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║  Episode 15: Sorting & Search        ║\n");
    printf("║  Complete Algorithm Implementation   ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    srand(time(NULL));
    
    // Test with small array
    printf("=== SMALL ARRAY TEST ===\n");
    int test_arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    int n = sizeof(test_arr) / sizeof(test_arr[0]);
    
    printf("Original array: ");
    print_array(test_arr, n);
    
    int sorted[10];
    memcpy(sorted, test_arr, sizeof(test_arr));
    quick_sort(sorted, n);
    printf("Sorted array:   ");
    print_array(sorted, n);
    printf("\n");
    
    // Search demo
    printf("=== SEARCH DEMO ===\n");
    int target = 25;
    int pos = linear_search(test_arr, n, target);
    printf("Linear search for %d: found at index %d\n", target, pos);
    
    pos = binary_search(sorted, n, target);
    printf("Binary search for %d: found at index %d\n", target, pos);
    printf("\n");
    
    // Benchmark with larger array
    printf("=== PERFORMANCE BENCHMARK ===\n");
    int size = 10000;
    printf("Array size: %d elements\n\n", size);
    
    int *large_arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        large_arr[i] = rand() % 100000;
    }
    
    printf("Sorting algorithms:\n");
    benchmark_sort(bubble_sort, "Bubble Sort", large_arr, size);
    benchmark_sort(selection_sort, "Selection Sort", large_arr, size);
    benchmark_sort(insertion_sort, "Insertion Sort", large_arr, size);
    benchmark_sort(shell_sort, "Shell Sort", large_arr, size);
    benchmark_sort(merge_sort, "Merge Sort", large_arr, size);
    benchmark_sort(quick_sort, "Quick Sort", large_arr, size);
    benchmark_sort(heap_sort, "Heap Sort", large_arr, size);
    
    // Search benchmark
    printf("\n=== SEARCH BENCHMARK ===\n");
    int *sorted_arr = malloc(size * sizeof(int));
    memcpy(sorted_arr, large_arr, size * sizeof(int));
    quick_sort(sorted_arr, size);
    
    int search_target = sorted_arr[size / 2];
    
    clock_t start = clock();
    for (int i = 0; i < 1000; i++) {
        linear_search(sorted_arr, size, search_target);
    }
    clock_t end = clock();
    double linear_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    for (int i = 0; i < 1000; i++) {
        binary_search(sorted_arr, size, search_target);
    }
    end = clock();
    double binary_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Linear search (1000x): %.6f sec\n", linear_time);
    printf("Binary search (1000x): %.6f sec\n", binary_time);
    printf("Speedup: %.2fx\n", linear_time / binary_time);
    
    // Cleanup
    free(large_arr);
    free(sorted_arr);
    
    printf("\n✓ All algorithms tested successfully!\n");
    
    return 0;
}

