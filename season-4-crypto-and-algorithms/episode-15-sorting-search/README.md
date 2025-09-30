# Episode 15: Sorting & Search Algorithms 🔍

> "Bad programmers worry about the code. Good programmers worry about data structures and their relationships." — Linus Torvalds

## 📋 Описание

После верификации blockchain в Episode 14, **V.** передает вам массивные логи операции MOONLIGHT — миллионы записей транзакций, временных меток, координат. Чтобы найти нужную информацию, вам понадобятся эффективные алгоритмы поиска и сортировки.

**В этом эпизоде вы научитесь:**
- Реализовывать классические алгоритмы сортировки
- Понимать временную сложность (Big O notation)
- Применять бинарный поиск
- Оптимизировать производительность кода

---

## 🎯 Цели обучения

### Теория
- **Алгоритмы сортировки:** Bubble, Selection, Insertion, Merge, Quick Sort
- **Алгоритмы поиска:** Linear Search, Binary Search
- **Анализ сложности:** Big O notation, best/average/worst case
- **Оптимизация:** выбор правильного алгоритма для задачи

### Практика
- Реализация сортировок на C
- Измерение производительности
- Анализ временной сложности
- Применение к реальным данным

---

## 📖 Теоретическая часть

### 1. Big O Notation

**Big O** описывает, как растет время выполнения с ростом размера данных:

| Notation | Name | Example |
|----------|------|---------|
| O(1) | Constant | Доступ к элементу массива |
| O(log n) | Logarithmic | Binary search |
| O(n) | Linear | Linear search |
| O(n log n) | Linearithmic | Merge sort, Quick sort |
| O(n²) | Quadratic | Bubble sort, Selection sort |
| O(2ⁿ) | Exponential | Рекурсивный Fibonacci |

### 2. Sorting Algorithms

#### Bubble Sort — O(n²)
```c
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
```

**Метафора:** Пузыри "всплывают" наверх

#### Quick Sort — O(n log n) average
```c
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i + 1;
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}
```

**Метафора:** "Разделяй и властвуй"

### 3. Search Algorithms

#### Binary Search — O(log n)
```c
int binary_search(int arr[], int n, int target) {
    int left = 0, right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    
    return -1;  // Not found
}
```

**Требование:** массив должен быть отсортирован!

---

## 💻 Задания

### Задание 1: Implement Sorting Algorithms ⭐⭐⭐☆☆

Реализуйте 4 алгоритма сортировки:

```c
void bubble_sort(int arr[], int n);
void selection_sort(int arr[], int n);
void merge_sort(int arr[], int left, int right);
void quick_sort(int arr[], int low, int high);
```

**Критерии:**
- Все алгоритмы правильно сортируют массив
- Измерьте время выполнения для разных размеров (100, 1000, 10000)

### Задание 2: Binary Search ⭐⭐☆☆☆

Реализуйте бинарный поиск:

```c
int binary_search(int arr[], int n, int target);
```

**Тест:** Найти конкретную временную метку в отсортированном логе.

### Задание 3: Performance Analysis ⭐⭐⭐⭐☆

Сравните производительность алгоритмов:

```c
void benchmark_sorts(int sizes[], int num_sizes);
void print_performance_table(void);
```

**Вывод:**
```
Algorithm       | n=100  | n=1000  | n=10000
----------------|--------|---------|----------
Bubble Sort     | 0.5ms  | 50ms    | 5000ms
Quick Sort      | 0.1ms  | 2ms     | 25ms
```

---

## 🎬 Сюжет

**Дата:** 17 декабря, 03:15  
**Локация:** Безопасный дом MOONLIGHT

> **V.:** "У нас есть полный дамп серверных логов организации — 10 миллионов записей. Нужно найти точное время и координаты встречи их агентов. Линейный поиск займёт часы. Тебе нужны эффективные алгоритмы."

**Файлы:**
- `logs.dat` — 10 млн записей (timestamp, location_id, event_type)
- `coordinates.dat` — 50K локаций (id, latitude, longitude)

**Миссия:** Отсортировать логи, найти критические события, определить местоположение встречи.

---

## 🏆 Бонусные задания

### Bonus 1: Hybrid Sort ⭐⭐⭐⭐⭐
Реализуйте Timsort (используется в Python) — комбинация Merge + Insertion sort.

### Bonus 2: Parallel Quick Sort ⭐⭐⭐⭐⭐
Распараллельте Quick Sort с помощью `pthread`.

### Bonus 3: Visualization ⭐⭐⭐☆☆
Создайте ASCII-визуализацию процесса сортировки.

---

## 📊 Что вы узнали

После Episode 15 вы умеете:

- ✅ Реализовывать классические алгоритмы сортировки
- ✅ Анализировать временную сложность (Big O)
- ✅ Применять бинарный поиск
- ✅ Измерять производительность кода
- ✅ Выбирать оптимальный алгоритм для задачи

---

## 🔜 Следующий эпизод

В **Episode 16: Data Structures** вы научитесь:
- Создавать linked lists, stacks, queues
- Реализовывать trees и hash tables
- Управлять динамической памятью
- Строить сложные структуры данных

Отсортированные логи раскрывают новые зацепки, но данные нужно организовать в эффективные структуры...

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 15 of 32*

[← Episode 14](../episode-14-blockchain-hashes/) | [Season 4](../README.md) | [Episode 16 →](../episode-16-data-structures/)

</div>
