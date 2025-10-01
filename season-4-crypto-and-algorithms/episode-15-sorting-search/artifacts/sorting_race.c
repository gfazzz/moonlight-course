/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 15: Sorting & Search — Промежуточная задача
 * ═══════════════════════════════════════════════════════════════
 * 
 * ЗАДАЧА: Sorting Race — benchmark разных алгоритмов сортировки
 * 
 * Цель: Понять когда какой алгоритм эффективнее через практику.
 *       Не просто теория O(n²), а реальные измерения!
 * 
 * Что реализовать:
 * 1. bubble_sort() — O(n²) простой
 * 2. insertion_sort() — O(n²) но быстрый на почти отсортированных!
 * 3. quick_sort() — O(n log n) в среднем, O(n²) на sorted!
 * 4. heapsort() — O(n log n) всегда стабильный
 * 
 * Тестовые сценарии:
 * - Random data (10,000 элементов)
 * - Sorted data (уже отсортировано)
 * - Nearly sorted (99% sorted, 1% перемешано)
 * - Reversed (обратный порядок)
 * 
 * Ожидаемый результат:
 * ═══ SORTING RACE: Random Data (10,000) ═══
 * Bubble Sort:     1234 ms    ❌ Slow (O(n²))
 * Insertion Sort:  582 ms     ❌ Slow (O(n²))
 * Quick Sort:      12 ms      ✅ Winner!
 * Heapsort:        18 ms      ✅ Fast
 * 
 * ═══ SORTING RACE: Already Sorted (10,000) ═══
 * Bubble Sort:     1189 ms    ❌ Still O(n²)!
 * Insertion Sort:  3 ms       ✅ Winner! O(n)
 * Quick Sort:      OVERFLOW!  ❌ O(n²) on sorted!
 * Heapsort:        17 ms      ✅ Stable
 * 
 * Компиляция:
 * $ gcc -Wall -Wextra -std=c11 -O2 sorting_race.c -o sorting_race
 * 
 * Запуск:
 * $ ./sorting_race
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define ARRAY_SIZE 10000

// ═══════════════════════════════════════════════════════════════
// Utility functions
// ═══════════════════════════════════════════════════════════════

/**
 * Создаёт массив с случайными числами
 */
void generate_random(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

/**
 * Создаёт уже отсортированный массив
 */
void generate_sorted(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

/**
 * Создаёт почти отсортированный массив (99% sorted)
 */
void generate_nearly_sorted(int arr[], int size) {
    generate_sorted(arr, size);
    
    // Перемешиваем 1% элементов
    int swaps = size / 100;
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % size;
        int idx2 = rand() % size;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

/**
 * Создаёт массив в обратном порядке
 */
void generate_reversed(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

/**
 * Проверяет что массив отсортирован
 */
bool is_sorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i+1]) {
            return false;
        }
    }
    return true;
}

/**
 * Измеряет время выполнения сортировки (в миллисекундах)
 */
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
        return -1;  // Сортировка не работает!
    }
    
    return (long)((end - start) * 1000.0 / CLOCKS_PER_SEC);
}

// ═══════════════════════════════════════════════════════════════
// Sorting algorithms
// ═══════════════════════════════════════════════════════════════

/**
 * Bubble Sort — O(n²)
 * Простой но медленный
 */
void bubble_sort(int arr[], int n) {
    // TODO: Реализовать Bubble Sort
    // Для каждого элемента:
    //   Для каждого следующего:
    //     Если arr[j] < arr[j-1]: swap
    
    (void)arr; (void)n;  // Placeholder
}

/**
 * Insertion Sort — O(n²) в худшем, O(n) на почти sorted!
 * Эффективен для небольших массивов и nearly sorted
 */
void insertion_sort(int arr[], int n) {
    // TODO: Реализовать Insertion Sort
    // Для i от 1 до n:
    //   key = arr[i]
    //   j = i - 1
    //   Сдвигаем элементы > key вправо
    //   Вставляем key на правильную позицию
    
    (void)arr; (void)n;  // Placeholder
}

/**
 * Quick Sort — O(n log n) в среднем, O(n²) на уже sorted!
 * Использует pivot и partition
 */
int partition(int arr[], int low, int high) {
    // TODO: Реализовать partition
    // pivot = arr[high]
    // Разделить на: [< pivot] pivot [> pivot]
    // Вернуть позицию pivot
    
    (void)arr; (void)low; (void)high;  // Placeholder
    return 0;
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

/**
 * Heapsort — O(n log n) ВСЕГДА!
 * Стабильный, не зависит от входных данных
 */
void heapify(int arr[], int n, int i) {
    // TODO: Реализовать heapify
    // Поддерживает max-heap property
    // largest = i
    // left = 2*i + 1
    // right = 2*i + 2
    // Если left > arr[largest]: largest = left
    // Если right > arr[largest]: largest = right
    // Если largest != i: swap и рекурсия
    
    (void)arr; (void)n; (void)i;  // Placeholder
}

void heapsort(int arr[], int n) {
    // TODO: Реализовать heapsort
    // 1. Build max heap
    // 2. Один за другим extract max
    
    (void)arr; (void)n;  // Placeholder
}

// ═══════════════════════════════════════════════════════════════
// Benchmark
// ═══════════════════════════════════════════════════════════════

void run_benchmark(const char *test_name, void (*generator)(int[], int)) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("SORTING RACE: %s (%d elements)\n", test_name, ARRAY_SIZE);
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    generator(data, ARRAY_SIZE);
    
    // Test each algorithm
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
    
    // Run benchmarks
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

