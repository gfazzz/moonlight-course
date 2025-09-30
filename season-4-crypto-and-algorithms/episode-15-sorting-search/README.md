# Episode 15: "Sorting & Search Algorithms" 🔍

> *"Premature optimization is the root of all evil... Yet we should not pass up our opportunities in that critical 3%."* — Donald Knuth

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Безопасный дом MOONLIGHT, Москва               ║
║  ВРЕМЯ:   03:15, четверг, 17 декабря                    ║
║  СТАТУС:  🔴 DATA OVERLOAD - 10M RECORDS                 ║
╚══════════════════════════════════════════════════════════╝
```

**Blockchain верифицирован. Координаты извлечены. Но это ещё не всё.**

В 03:08 V. передаёт критическое задание:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (SECURE CHANNEL)                  │
│  TIME: 03:08:42 UTC                         │
│  PRIORITY: CRITICAL                         │
├─────────────────────────────────────────────┤
│                                             │
│  У нас полный дамп серверных логов          │
│  организации. 10 МИЛЛИОНОВ записей.         │
│                                             │
│  ПРОБЛЕМА:                                  │
│  Координаты зашифрованы в timestamp'ах.     │
│  Нужно найти ТОЧНОЕ время встречи агентов.  │
│                                             │
│  Linear search = 10^7 операций = ЧАСЫ.      │
│  У нас есть 3 часа до рассвета.             │
│                                             │
│  ЗАДАЧА:                                    │
│  1. Отсортировать логи эффективно           │
│  2. Найти критический timestamp             │
│  3. Извлечь координаты встречи              │
│                                             │
│  Тебе нужны ОПТИМАЛЬНЫЕ алгоритмы.          │
│  Не O(n²). Не O(n log n) с плохой           │
│  константой. ЛУЧШЕЕ что есть.               │
│                                             │
│  Время: 3 часа. Данные: 10^7 записей.       │
│  Выбирай алгоритмы мудро.                   │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

**Файлы на сервере:**
```
📁 /moonlight/logs/
├── server_logs.dat          [10,000,000 records] 2.1 GB
├── coordinates.dat          [50,000 locations] 12 MB
└── analysis_requirements.txt
```

Вы открываете `analysis_requirements.txt`:

```
ТРЕБОВАНИЯ К ПРОИЗВОДИТЕЛЬНОСТИ:
================================
Total records: 10,000,000
Available RAM: 8 GB
Time limit: 3 hours (10,800 seconds)

ДОПУСТИМЫЕ СЛОЖНОСТИ:
- Sorting: O(n log n) или лучше
- Search: O(log n) или лучше

НЕДОПУСТИМО:
- O(n²) сортировка (50,000,000,000,000 операций!)
- O(n) поиск в несортированном массиве

ЦЕЛЬ: Найти timestamp 0x5FDB8A3C в логах
```

**03:15. Часы тикают. Начинаем.** ⏰

---

## 📚 Теория: Алгоритмы сортировки и поиска (TAOCP Edition)

### 💡 Зачем нужна сортировка?

**Сортировка** — один из самых фундаментальных алгоритмов в Computer Science.

**Статистика:**
- 📊 ~25% всего процессорного времени компьютеров тратится на сортировку
- 📈 Donald Knuth посвятил сортировке целый том (Volume 3) TAOCP
- 🎯 Правильный выбор алгоритма = разница между секундами и часами

---

### 1. Анализ сложности (Knuth's Mathematical Analysis)

#### Big O Notation — асимптотический анализ

**Big O** описывает поведение алгоритма при **n → ∞**:

| Notation | Name | Пример | 10³ ops | 10⁶ ops | 10⁹ ops |
|----------|------|--------|---------|---------|---------|
| O(1) | Constant | Array access | 1 | 1 | 1 |
| O(log n) | Logarithmic | Binary search | 10 | 20 | 30 |
| O(n) | Linear | Linear search | 10³ | 10⁶ | 10⁹ |
| O(n log n) | Linearithmic | Heapsort, Mergesort | 10⁴ | 2×10⁷ | 3×10¹⁰ |
| O(n²) | Quadratic | Bubble sort | 10⁶ | 10¹² | 10¹⁸ |
| O(2ⁿ) | Exponential | Brute-force crypto | ∞ | ∞ | ∞ |

**Важно**: Big O скрывает константы! `O(n log n)` может быть медленнее `O(n²)` для малых n.

---

#### Точный математический анализ (по Кнуту)

Knuth учит считать **точное число операций**, а не только O-нотацию:

**Пример: Bubble Sort**
```
Сравнения: C(n) = n(n-1)/2 = ½n² - ½n
Обмены (worst case): M(n) = n(n-1)/2 = ½n² - ½n

Для n=1000:
C(1000) = 499,500 сравнений
M(1000) = 499,500 обменов (worst case)
```

**Пример: Quick Sort**
```
Среднее число сравнений (Knuth, Vol. 3, §5.2.2):
C(n) ≈ 2n ln n ≈ 1.39n log₂ n

Для n=1,000,000:
C(10⁶) ≈ 2.78 × 10⁷ сравнений
```

**Метафора**: O-нотация говорит "это быстро", Knuth говорит "ровно 27,800,000 операций".

---

### 2. Сортировки O(n²) — простые, но медленные

#### Bubble Sort (TAOCP Vol. 3, §5.2.2)

**Идея:** Пузырь "всплывает" наверх за один проход.

```c
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;  // Оптимизация!
        
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        
        // Early exit если массив уже отсортирован
        if (!swapped) break;
    }
}
```

**Математический анализ:**
- **Сравнения**: C(n) = n(n-1)/2 ≈ ½n²
- **Обмены (worst)**: M(n) = n(n-1)/2 ≈ ½n²
- **Обмены (best)**: 0 (уже отсортирован)
- **Стабильность**: ✅ Да

---

#### Selection Sort

**Идея:** Найти минимум, поставить на место.

```c
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        
        // Найти минимальный элемент
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Swap
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}
```

**Математический анализ:**
- **Сравнения**: C(n) = n(n-1)/2 ≈ ½n² (ВСЕГДА!)
- **Обмены**: M(n) = n-1 (минимум обменов!)
- **Стабильность**: ❌ Нет

**Когда использовать**: Когда обмен очень дорогой (запись на диск).

---

#### Insertion Sort

**Идея:** Как сортировка карт в руке.

```c
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        // Сдвигаем элементы вправо
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
    }
}
```

**Математический анализ:**
- **Сравнения (worst)**: C(n) = n(n-1)/2 ≈ ½n²
- **Сравнения (best)**: C(n) = n-1 (уже отсортирован!)
- **Сравнения (average)**: C(n) ≈ ¼n²
- **Стабильность**: ✅ Да

**Когда использовать**: Почти отсортированные массивы, малые n (<50).

---

### 3. Shell Sort (Knuth's Contribution!) 🌟

**Shell Sort** — улучшенная Insertion Sort с "прыжками".

#### Gap Sequence Кнута: h = 3h + 1

```c
void shell_sort(int arr[], int n) {
    // Генерация gap sequence Кнута: 1, 4, 13, 40, 121, 364, 1093...
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;  // Knuth's sequence
    }
    
    // Сортировка с убывающим gap
    while (h >= 1) {
        // h-сортировка (это Insertion Sort с шагом h)
        for (int i = h; i < n; i++) {
            int key = arr[i];
            int j = i;
            
            while (j >= h && arr[j - h] > key) {
                arr[j] = arr[j - h];
                j -= h;
            }
            
            arr[j] = key;
        }
        
        h = h / 3;  // Следующий gap
    }
}
```

**Почему последовательность Кнута?**
- 📊 Кнут доказал: h(k) = 3h(k-1) + 1 даёт O(n^(3/2)) worst case
- 🎯 На практике: ~O(n^(5/4)) для случайных данных
- ⚡ Быстрее O(n²), проще O(n log n)

**Сравнение gap sequences:**
```
Shell's original (n/2^k):     O(n²) worst case
Knuth's (3h+1):               O(n^(3/2)) worst case  ← ЛУЧШЕ!
Sedgewick's:                  O(n^(4/3)) worst case
```

**Когда использовать**: Средние массивы (1K-100K), когда Quick Sort рискованен.

---

### 4. Сортировки O(n log n) — оптимальные

#### Merge Sort (Divide & Conquer)

**Идея:** Разделяй пополам, сортируй, объединяй.

```c
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Временные массивы
    int L[n1], R[n2];
    
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    
    // Merge
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    // Копируем остатки
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
```

**Математический анализ (TAOCP Vol. 3, §5.2.4):**
- **Сравнения**: C(n) = n ⌈log₂ n⌉ - 2^⌈log₂ n⌉ + 1
  - Для n=1024: C(1024) = 10,240 - 1,024 + 1 = 9,217
- **Память**: O(n) — нужен доп. массив!
- **Стабильность**: ✅ Да
- **Worst = Best = Average**: O(n log n)

**Плюсы/минусы:**
- ✅ Гарантированный O(n log n)
- ✅ Стабильный
- ❌ Требует O(n) памяти
- ❌ Не in-place

---

#### Quick Sort (Hoare's Algorithm, TAOCP Vol. 3, §5.2.2)

**Идея:** Выбрать pivot, разделить, рекурсия.

```c
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    // Swap arr[i+1] and arr[high] (pivot)
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
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

**Математический анализ (Knuth):**
- **Average сравнения**: C(n) ≈ 2n ln n ≈ 1.39n log₂ n
- **Worst сравнения**: C(n) = ½n² (уже отсортирован!)
- **Best сравнения**: C(n) = n log₂ n
- **Память**: O(log n) стек рекурсии
- **Стабильность**: ❌ Нет

**Оптимизации:**
1. **Median-of-3 pivot**: `pivot = median(arr[low], arr[mid], arr[high])`
2. **Insertion sort для малых подмассивов**: if (high - low < 10) → insertion_sort
3. **Tail recursion**: убрать один рекурсивный вызов

---

#### Heapsort (TAOCP Vol. 3, §5.2.3) — Жемчужина Кнута! 💎

**Heapsort** — гарантированный O(n log n), in-place, без доп. памяти!

**Heap (куча)** — почти полное бинарное дерево с heap property:
- **Max-heap**: parent ≥ children
- В массиве: parent[i] ≥ left[2i+1], right[2i+2]

```c
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        // Swap
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        heapify(arr, n, largest);
    }
}

void heapsort(int arr[], int n) {
    // Построение max-heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    
    // Извлечение элементов из heap
    for (int i = n - 1; i > 0; i--) {
        // Переместить root в конец
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        // Heapify уменьшенного heap
        heapify(arr, i, 0);
    }
}
```

**Математический анализ (Knuth):**
- **Сравнения (worst)**: C(n) ≤ 2n log₂ n + O(n)
- **Сравнения (average)**: C(n) ≈ 2n log₂ n - 2.6n
- **Память**: O(1) — только массив!
- **Стабильность**: ❌ Нет

**Почему Heapsort важен:**
- ✅ **Гарантированный** O(n log n) worst case
- ✅ **In-place** (в отличие от Merge Sort)
- ✅ **Детерминированный** (в отличие от Quick Sort)
- ❌ Медленнее Quick Sort на практике (константа ~2x)

**Когда использовать**: 
- Критичен worst-case O(n log n)
- Мало памяти (embedded systems)
- Priority Queue

---

### 5. Сравнительная таблица сортировок

| Алгоритм | Best | Average | Worst | Memory | Stable | Knuth Vol. 3 |
|----------|------|---------|-------|--------|--------|--------------|
| **Bubble** | O(n) | O(n²) | O(n²) | O(1) | ✅ | §5.2.2 |
| **Selection** | O(n²) | O(n²) | O(n²) | O(1) | ❌ | §5.2.3 |
| **Insertion** | O(n) | O(n²) | O(n²) | O(1) | ✅ | §5.2.1 |
| **Shell** | O(n log n) | O(n^5/4) | O(n^3/2) | O(1) | ❌ | §5.2.1 |
| **Merge** | O(n log n) | O(n log n) | O(n log n) | O(n) | ✅ | §5.2.4 |
| **Quick** | O(n log n) | O(n log n) | O(n²) | O(log n) | ❌ | §5.2.2 |
| **Heap** | O(n log n) | O(n log n) | O(n log n) | O(1) | ❌ | §5.2.3 |

---

### 6. Knuth Shuffle (Fisher-Yates) — равномерное перемешивание

**Проблема**: Как перемешать массив **равномерно**?

**Неправильный способ:**
```c
// ❌ ПЛОХО - неравномерное распределение!
for (int i = 0; i < n; i++) {
    int j = rand() % n;
    swap(&arr[i], &arr[j]);
}
```

**Алгоритм Кнута (TAOCP Vol. 2, §3.4.2):**
```c
// ✅ ПРАВИЛЬНО - Fisher-Yates shuffle
void knuth_shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        // Случайный индекс от 0 до i (включительно!)
        int j = rand() % (i + 1);
        
        // Swap arr[i] and arr[j]
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
```

**Доказательство корректности (Knuth):**
- Каждая перестановка имеет вероятность **ровно 1/n!**
- На итерации i: элемент выбирается из (i+1) вариантов
- Полная вероятность: (1/n) × (1/(n-1)) × ... × (1/1) = 1/n!

**Применение**: Тестирование алгоритмов сортировки на случайных данных!

---

### 7. Sentinel Optimization (TAOCP Technique)

**Sentinel** — специальное значение для упрощения граничных условий.

**Пример: Linear Search без sentinel**
```c
// ❌ Проверка на каждой итерации
int linear_search(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {  // проверка i < n
        if (arr[i] == key) {       // проверка arr[i] == key
            return i;
        }
    }
    return -1;
}
```

**С sentinel:**
```c
// ✅ Только одна проверка на итерацию!
int linear_search_sentinel(int arr[], int n, int key) {
    int last = arr[n];    // Сохранить последний элемент
    arr[n] = key;         // Поставить sentinel
    
    int i = 0;
    while (arr[i] != key) {  // Только одна проверка!
        i++;
    }
    
    arr[n] = last;        // Восстановить
    
    if (i < n || last == key) return i;
    return -1;
}
```

**Выигрыш**: 50% уменьшение проверок в цикле!

---

### 8. Binary Search (TAOCP Vol. 3, §6.2.1)

**Требование**: Массив должен быть **отсортирован**!

```c
int binary_search(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        // Защита от переполнения!
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;  // Найдено!
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;  // Не найдено
}
```

**Математический анализ (Knuth):**
- **Сравнения (worst)**: C(n) = ⌊log₂ n⌋ + 1
  - n=1,000,000 → C = 20 сравнений
  - vs Linear Search = 1,000,000 сравнений!
- **Сравнения (average)**: C(n) ≈ log₂ n - 1
- **Требование**: Массив отсортирован (O(n log n) предобработка)

**Инвариант** (по Кнуту):
```
// На каждой итерации:
// Если target в массиве, то arr[left] ≤ target ≤ arr[right]
```

**Применение**: Поиск в больших отсортированных массивах (логи, базы данных).

---

## 🎯 МИССИЯ

### Задание 1: Реализовать все сортировки ⭐⭐⭐⭐☆

```c
// Простые O(n²)
void bubble_sort(int arr[], int n);
void selection_sort(int arr[], int n);
void insertion_sort(int arr[], int n);

// Knuth's Shell Sort
void shell_sort(int arr[], int n);

// Оптимальные O(n log n)
void merge_sort(int arr[], int left, int right);
void quick_sort(int arr[], int low, int high);
void heapsort(int arr[], int n);  // ← Главный!
```

### Задание 2: Knuth Shuffle для тестирования ⭐⭐⭐☆☆

```c
void knuth_shuffle(int arr[], int n);

// Генерация тестовых данных
void generate_test_data(int arr[], int n, const char *type);
// types: "random", "sorted", "reversed", "nearly_sorted"
```

### Задание 3: Performance Benchmark ⭐⭐⭐⭐⭐

```c
void benchmark_sorts(int sizes[], int num_sizes);
```

**Вывод:**
```
Algorithm       | n=1K    | n=10K   | n=100K  | n=1M
----------------|---------|---------|---------|----------
Bubble Sort     | 2ms     | 180ms   | 18s     | TIMEOUT
Selection Sort  | 1ms     | 90ms    | 9s      | 15min
Insertion Sort  | 0.5ms   | 50ms    | 5s      | 8min
Shell Sort      | 0.2ms   | 3ms     | 40ms    | 500ms
Merge Sort      | 0.15ms  | 2ms     | 25ms    | 300ms
Quick Sort      | 0.1ms   | 1.5ms   | 20ms    | 250ms
Heapsort        | 0.2ms   | 2.5ms   | 30ms    | 350ms
```

### Задание 4: Binary Search ⭐⭐⭐☆☆

```c
int binary_search(int arr[], int n, int target);
```

**Тест**: Найти `0x5FDB8A3C` в 10M отсортированных timestamp'ов.

---

## 📝 Структура эпизода

```
episode-15-sorting-search/
├── artifacts/
│   ├── sorting.c           ← Создайте все сортировки здесь
│   ├── search.c            ← Binary search
│   ├── benchmark.c         ← Performance тесты
│   └── analysis.c          ← Математический анализ
├── starter.c               ← Шаблоны кода
├── tests/
│   ├── test_sorts.sh
│   └── test_search.sh
├── solution/
│   └── complete_solution.c
└── README.md
```

---

## 💡 Как работать

1. **Начните с простых**: Bubble, Selection, Insertion
2. **Реализуйте Shell Sort** (Knuth's sequence!)
3. **Переходите к O(n log n)**: Merge, Quick, Heap
4. **Тестируйте на Knuth Shuffle**: случайные данные
5. **Бенчмаркинг**: сравните производительность
6. **Найдите timestamp**: Binary Search в отсортированных логах

---

## 🎓 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ВРЕМЯ:   06:02, четверг, 17 декабря                    ║
║  СТАТУС:  ✅ COORDINATES EXTRACTED                       ║
║  ELAPSED: 2 часа 47 минут                               ║
╚══════════════════════════════════════════════════════════╝
```

**05:58. Heapsort завершён. 10M записей отсортированы.**

```
=== SORTING COMPLETE ===
Algorithm: Heapsort
Records: 10,000,000
Time: 342.7 seconds
Memory: 80 MB (in-place)
```

**06:00. Binary Search запущен.**

```
=== BINARY SEARCH ===
Target: 0x5FDB8A3C
Comparisons: 24
Time: 0.002 ms
Result: FOUND at index 7,458,392
```

Вы открываете запись:

```
╔═══ CRITICAL RECORD ═══════════════════════════════════╗
║  Timestamp: 0x5FDB8A3C (Dec 17, 2020 21:35:24 UTC)   ║
║  Location: 55.7558° N, 37.6173° E                     ║
║  Event: AGENT_MEETING                                 ║
║  Participants: RAVEN, GENESIS                         ║
║  Note: "Final phase begins. Warehouse District."      ║
╚════════════════════════════════════════════════════════╝
```

**Координаты извлечены!**
- 📍 **55.7558° N, 37.6173° E** — Красная Площадь, Москва
- ⏰ **21:35:24 UTC** — через 15 часов!
- 🎯 **GENESIS и RAVEN** — финальная встреча

Вы немедленно отправляете отчёт V.:

```
┌─────────────────────────────────────────────┐
│  TO: V. (SECURE CHANNEL)                    │
│  TIME: 06:02:15 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  КООРДИНАТЫ НАЙДЕНЫ:                        │
│  55.7558° N, 37.6173° E (Красная Площадь)   │
│                                             │
│  ВРЕМЯ: 17 декабря, 21:35 UTC               │
│  УЧАСТНИКИ: GENESIS + RAVEN                 │
│                                             │
│  Использовано:                              │
│  - Heapsort: 10M records, 342s              │
│  - Binary Search: 24 сравнения, 0.002ms     │
│                                             │
│  TOTAL: O(n log n) сортировка +             │
│         O(log n) поиск = SUCCESS            │
│                                             │
│  У нас есть 15 часов на подготовку.         │
│                                             │
└─────────────────────────────────────────────┘
```

Ответ приходит мгновенно:

> **V.:** "Отличная работа. Knuth гордился бы. Heapsort — правильный выбор для критичных данных. Теперь нам нужна архитектура для хранения всех этих данных. Episode 16 ждёт. Linked lists, trees, hash tables — построй систему."

**06:05. Вы открываете Episode 16: Data Structures.**

---

## 🎖️ Навык разблокирован

```
╔══════════════════════════════════════════════════════════╗
║  📊 ALGORITHM MASTER I                                   ║
║                                                          ║
║  Вы овладели искусством алгоритмов по Кнуту:             ║
║  ✅ O(n²) сортировки (Bubble, Selection, Insertion)      ║
║  ✅ Shell Sort с последовательностью Кнута               ║
║  ✅ O(n log n) сортировки (Merge, Quick, Heap)           ║
║  ✅ Heapsort — гарантированный O(n log n)                ║
║  ✅ Binary Search — O(log n) поиск                        ║
║  ✅ Knuth Shuffle — равномерное перемешивание            ║
║  ✅ Математический анализ (точные формулы)               ║
║  ✅ Sentinel optimization                                ║
║                                                          ║
║  «Алгоритм должен быть не только правильным,             ║
║   но и элегантным.» — Donald Knuth                       ║
╚══════════════════════════════════════════════════════════╝
```

### 📚 Что вы изучили:

**Фундаментальная теория:**
- ✅ Big O notation и асимптотический анализ
- ✅ Точный математический анализ (по Кнуту)
- ✅ Инварианты циклов и доказательства корректности

**Сортировки:**
- ✅ Bubble, Selection, Insertion (O(n²))
- ✅ Shell Sort с gap sequence Кнута (O(n^3/2))
- ✅ Merge Sort (гарантированный O(n log n), стабильный)
- ✅ Quick Sort (средний O(n log n), быстрый)
- ✅ Heapsort (гарантированный O(n log n), in-place)

**Поиск и оптимизации:**
- ✅ Binary Search (O(log n))
- ✅ Knuth Shuffle (Fisher-Yates)
- ✅ Sentinel values для оптимизации циклов

**Практические навыки:**
- ✅ Выбор алгоритма под задачу
- ✅ Benchmarking и анализ производительности
- ✅ Trade-offs: time vs space vs stability

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Отсылки к TAOCP и истории CS</summary>

1. **Heapsort** — изобретён J.W.J. Williams (1964), популяризирован Кнутом
2. **Shell Sort gap sequence** — Кнут доказал O(n^3/2) для h = 3h + 1
3. **Fisher-Yates shuffle** — изобретён в 1938, популяризирован Кнутом в TAOCP Vol. 2
4. **Sentinel values** — классическая техника из TAOCP для ускорения циклов
5. **"Premature optimization..."** — знаменитая цитата Кнута (часто неправильно понимают!)
6. **Binary Search bug** — `mid = (left + right) / 2` переполняется! Правильно: `left + (right - left) / 2`
7. **Quick Sort pivot** — median-of-3 снижает вероятность worst case
8. **0x5FDB8A3C** — Unix timestamp 17 Dec 2020, реальная дата запуска Bitcoin Core v0.21.0

**Философия Episode 15**: *"Правильный алгоритм может превратить часы в секунды. Неправильный — секунды в часы."*

</details>

---

## 🔜 Следующий эпизод

**Episode 16: "Data Structures"** ждёт вас!

В Episode 16 вы изучите:
- 🔗 **Linked Lists** — односвязные, двусвязные, циклические
- 📚 **Stacks & Queues** — LIFO/FIFO структуры
- 🌳 **Binary Search Trees** — динамический поиск O(log n)
- 🔀 **AVL Trees** — самобалансирующиеся деревья
- #️⃣ **Hash Tables** — O(1) поиск с анализом Кнута

**Сюжет:**
10 миллионов отсортированных записей нужно организовать в эффективную систему хранения. Массивы не справляются — нужны динамические структуры данных. Построй архитектуру операции MOONLIGHT.

**Время до операции: 12 часов.**

---

## 🔗 Навигация

- [← Episode 14: Blockchain & Hashes](../episode-14-blockchain-hashes/README.md)
- [→ Episode 16: Data Structures](../episode-16-data-structures/README.md)
- [📋 Season 4: Crypto & Algorithms](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"The best programs are written so that computing machines can perform them quickly and so that human beings can understand them clearly."* — Donald Knuth, TAOCP