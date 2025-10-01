# Solution for Episode 15: Sorting & Search Algorithms

## Complete Implementation

This solution implements all major sorting and search algorithms with performance benchmarking.

## Files

- `sorting_algorithms.c` — Complete implementation with:
  - 7 sorting algorithms (Bubble, Selection, Insertion, Shell, Merge, Quick, Heap)
  - 2 search algorithms (Linear, Binary)
  - Knuth shuffle
  - Performance benchmarking
  - Correctness verification

## Sorting Algorithms Implemented

| Algorithm | Time Complexity | Space | Stable |
|-----------|----------------|-------|--------|
| Bubble Sort | O(n²) | O(1) | Yes |
| Selection Sort | O(n²) | O(1) | No |
| Insertion Sort | O(n²) avg, O(n) best | O(1) | Yes |
| Shell Sort | O(n log n) - O(n²) | O(1) | No |
| Merge Sort | O(n log n) | O(n) | Yes |
| Quick Sort | O(n log n) avg, O(n²) worst | O(log n) | No |
| Heap Sort | O(n log n) | O(1) | No |

## Search Algorithms

| Algorithm | Time Complexity | Prerequisites |
|-----------|----------------|---------------|
| Linear Search | O(n) | None |
| Binary Search | O(log n) | Sorted array |

## Features

✅ Complete algorithm implementations  
✅ Performance benchmarking with timing  
✅ Correctness verification  
✅ Knuth shuffle for unbiased randomization  
✅ Demonstration with multiple array sizes  

## Compilation

```bash
gcc -o sorting_algorithms sorting_algorithms.c -Wall
```

## Usage

```bash
./sorting_algorithms
```

The program will:
1. Demonstrate algorithms on small array
2. Benchmark all sorting algorithms on 10,000 elements
3. Compare search algorithms performance

## Example Output

```
Sorting algorithms:
       Bubble Sort: 0.123456 sec [✓]
    Selection Sort: 0.098765 sec [✓]
    Insertion Sort: 0.087654 sec [✓]
       Shell Sort: 0.012345 sec [✓]
       Merge Sort: 0.008765 sec [✓]
       Quick Sort: 0.006543 sec [✓]
        Heap Sort: 0.009876 sec [✓]

Linear search (1000x): 0.245678 sec
Binary search (1000x): 0.000123 sec
Speedup: 1997.22x
```

## Key Concepts

### Sorting Strategies

- **Comparison-based**: Compare elements and swap
- **Divide-and-conquer**: Split, sort, merge (Merge/Quick Sort)
- **Heap-based**: Use heap data structure (Heap Sort)

### Time Complexity Classes

- **O(n²)**: Bubble, Selection, Insertion (simple but slow)
- **O(n log n)**: Merge, Quick, Heap (optimal for comparison sorts)
- **O(n)**: Insertion sort on nearly sorted data

### Search Complexity

- **Linear O(n)**: Check every element
- **Binary O(log n)**: Divide search space in half each step

## Shell Sort Gap Sequence (Knuth)

```
gap = 1
while gap < n/3:
    gap = gap * 3 + 1
# Results in: 1, 4, 13, 40, 121, 364, ...
```

## Quick Sort Pivot Selection

Uses **last element as pivot** (can be improved with random pivot or median-of-three for better worst-case performance).

