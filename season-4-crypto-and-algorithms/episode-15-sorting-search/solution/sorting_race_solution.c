/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 15: Sorting & Search — Промежуточная задача (РЕШЕНИЕ)
 * ═══════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define ARRAY_SIZE 10000

void generate_random(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

void generate_sorted(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void generate_nearly_sorted(int arr[], int size) {
    generate_sorted(arr, size);
    int swaps = size / 100;
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % size;
        int idx2 = rand() % size;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

void generate_reversed(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

bool is_sorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i+1]) {
            return false;
        }
    }
    return true;
}

typedef void (*SortFunc)(int[], int);

long measure_time(SortFunc sort_func, int arr[], int size) {
    int *copy = malloc(size * sizeof(int));
    memcpy(copy, arr, size * sizeof(int));
    
    clock_t start = clock();
    sort_func(copy, size);
    clock_t end = clock();
    
    bool sorted = is_sorted(copy, size);
    free(copy);
    
    if (!sorted) {
        return -1;
    }
    
    return (long)((end - start) * 1000.0 / CLOCKS_PER_SEC);
}

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

void quick_sort_helper(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_helper(arr, low, pi - 1);
        quick_sort_helper(arr, pi + 1, high);
    }
}

void quick_sort(int arr[], int n) {
    quick_sort_helper(arr, 0, n - 1);
}

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

void heapsort(int arr[], int n) {
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

void run_benchmark(const char *test_name, void (*generator)(int[], int)) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("SORTING RACE: %s (%d elements)\n", test_name, ARRAY_SIZE);
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    generator(data, ARRAY_SIZE);
    
    typedef struct {
        const char *name;
        SortFunc func;
    } SortTest;
    
    SortTest tests[] = {
        {"Bubble Sort",    bubble_sort},
        {"Insertion Sort", insertion_sort},
        {"Quick Sort",     quick_sort},
        {"Heapsort",       heapsort}
    };
    
    long min_time = 999999;
    int winner = -1;
    
    for (int i = 0; i < 4; i++) {
        long time_ms = measure_time(tests[i].func, data, ARRAY_SIZE);
        
        printf("%-18s ", tests[i].name);
        
        if (time_ms < 0) {
            printf("ERROR (not sorted!)\n");
        } else {
            printf("%6ld ms    ", time_ms);
            
            if (time_ms < min_time) {
                min_time = time_ms;
                winner = i;
            }
            
            if (time_ms < 50) {
                printf("✅ Fast");
            } else if (time_ms < 200) {
                printf("⚠️  Moderate");
            } else {
                printf("❌ Slow");
            }
            
            if (i == winner && time_ms > 0) {
                printf("  ← Winner!");
            }
            
            printf("\n");
        }
    }
    
    printf("\n💡 Analysis:\n");
    if (strcmp(test_name, "Random Data") == 0) {
        printf("   O(n log n) algorithms (Quick Sort, Heapsort) dominate!\n");
        printf("   O(n²) algorithms (Bubble, Insertion) are too slow.\n");
    } else if (strcmp(test_name, "Already Sorted") == 0) {
        printf("   Insertion Sort = O(n) on sorted data!\n");
        printf("   Quick Sort degrades to O(n²) with naive pivot!\n");
        printf("   Heapsort stays O(n log n) — stable!\n");
    } else if (strcmp(test_name, "Nearly Sorted") == 0) {
        printf("   Insertion Sort shines on nearly sorted data!\n");
        printf("   It's often used for small subarrays in Timsort.\n");
    } else if (strcmp(test_name, "Reversed") == 0) {
        printf("   Worst case for many algorithms.\n");
        printf("   Quick Sort with last element pivot = O(n²)!\n");
    }
    
    free(data);
}

int main() {
    srand(time(NULL));
    
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  SORTING RACE — Algorithm Benchmark                      ║\n");
    printf("║  Episode 15: Sorting & Search                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    run_benchmark("Random Data", generate_random);
    run_benchmark("Already Sorted", generate_sorted);
    run_benchmark("Nearly Sorted", generate_nearly_sorted);
    run_benchmark("Reversed", generate_reversed);
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  📊 KEY TAKEAWAYS                                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  1. NO UNIVERSAL BEST ALGORITHM!                          ║\n");
    printf("║     - Random: Quick Sort wins                             ║\n");
    printf("║     - Sorted: Insertion Sort wins                         ║\n");
    printf("║     - Nearly sorted: Insertion Sort wins                  ║\n");
    printf("║                                                           ║\n");
    printf("║  2. BIG O ≠ REAL PERFORMANCE                              ║\n");
    printf("║     - Constants matter!                                   ║\n");
    printf("║     - Cache locality matters!                             ║\n");
    printf("║     - Input characteristics matter!                       ║\n");
    printf("║                                                           ║\n");
    printf("║  3. REAL-WORLD HYBRIDS:                                   ║\n");
    printf("║     - Timsort (Python): Merge + Insertion                 ║\n");
    printf("║     - Introsort (C++): Quick + Heap + Insertion           ║\n");
    printf("║     - pdqsort: Pattern-defeating Quick Sort               ║\n");
    printf("║                                                           ║\n");
    printf("║  4. EPISODE 15 CONNECTION:                                ║\n");
    printf("║     В миссии нужно отсортировать 10 млн транзакций!       ║\n");
    printf("║     Linear search: 53 часа ❌                             ║\n");
    printf("║     Heapsort + Binary Search: 0.002ms ✅                  ║\n");
    printf("║     = 9,450,000,000× быстрее!                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    return 0;
}

