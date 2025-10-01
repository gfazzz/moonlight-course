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

#### 🎨 Визуальная метафора: Shell Sort = Расчёска с прыжками

```
╔════════════════════════════════════════════════════════════════╗
║  SHELL SORT = РАСЧЁСКА С РЕГУЛИРУЕМЫМИ ЗУБЬЯМИ                 ║
╠════════════════════════════════════════════════════════════════╣
║                                                                ║
║  Представьте 10 человек в очереди (неотсортированная):        ║
║                                                                ║
║  Позиция: 0   1   2   3   4   5   6   7   8   9              ║
║  Рост:    180 165 190 160 175 185 170 195 155 200            ║
║                                                                ║
║  🎯 INSERTION SORT (обычная расчёска, шаг=1):                 ║
║                                                                ║
║     [180][165][190][160][175][185][170][195][155][200]        ║
║       ↓   ↓                                                   ║
║       Сравнить соседей, swap если нужно                       ║
║       Двигаться пошагово: 1→2, 2→3, 3→4...                    ║
║                                                                ║
║     Проблема: Если 155 в конце, а должен в начале →          ║
║               нужно 9 swap'ов чтобы "пробулькать" в начало!   ║
║                                                                ║
║  ⚡ SHELL SORT (расчёска с регулируемыми зубьями):            ║
║                                                                ║
║     ШАГ 1: gap=4 (Knuth: начинаем с большого прыжка)         ║
║     ┌───────────────────────────┐                             ║
║     │ Сравниваем элементы на    │                             ║
║     │ расстоянии 4 друг от друга│                             ║
║     └───────────────────────────┘                             ║
║                                                                ║
║     Позиция: 0   1   2   3   4   5   6   7   8   9          ║
║              ╰───────────────────╯   ← сравнить 0 и 4         ║
║                  ╰───────────────────╯ ← сравнить 1 и 5       ║
║                      ╰───────────────────╯ ← и так далее      ║
║                                                                ║
║     180 165 190 160 [175] 185 170 195 155 [200]              ║
║      ↑               ↑                        ↑                ║
║     swap!           swap!                    ok               ║
║                                                                ║
║     После gap=4: [175][165][170][155][180][185][190][195]... ║
║     → Массив УЖЕ частично упорядочен!                         ║
║                                                                ║
║     ШАГ 2: gap=1 (обычная Insertion Sort)                    ║
║     НО! Теперь элементы почти на месте → быстро!             ║
║                                                                ║
║  📊 СРАВНЕНИЕ:                                                 ║
║                                                                ║
║     Insertion Sort (gap=1 всегда):                            ║
║     [200] в конце, должен в начало:                           ║
║     200 → 195 → 190 → 185 → 180 → 175 → ... (9 swap'ов)      ║
║                                                                ║
║     Shell Sort (gap=4, потом gap=1):                          ║
║     [200] с gap=4 сразу прыгает близко к началу (2 swap'а)   ║
║     Затем gap=1 досортирует (1-2 swap'а)                      ║
║     Всего: 3-4 swap'а вместо 9!                               ║
║                                                                ║
║  💡 ПОЧЕМУ ЭТО РАБОТАЕТ:                                       ║
║                                                                ║
║     🔸 БОЛЬШОЙ GAP (h=13, 40, 121...):                        ║
║        - "Грубая сортировка" — двигаем элементы на большие    ║
║          расстояния быстро                                    ║
║        - Убираем крупные инверсии                             ║
║                                                                ║
║     🔸 СРЕДНИЙ GAP (h=4, 13):                                  ║
║        - Доводим массив до "почти отсортированного"           ║
║                                                                ║
║     🔸 МАЛЫЙ GAP (h=1):                                        ║
║        - Обычная Insertion Sort                               ║
║        - НО на почти sorted = O(n) вместо O(n²)!              ║
║                                                                ║
║  🎯 KNUTH'S SEQUENCE (h = 3h + 1):                             ║
║                                                                ║
║     1, 4, 13, 40, 121, 364, 1093, 3280...                     ║
║                                                                ║
║     Почему именно 3h + 1?                                     ║
║     - Кнут математически доказал оптимальность                ║
║     - Gaps не делятся друг на друга (лучше покрытие)          ║
║     - Баланс между "слишком много gaps" и "слишком мало"      ║
║                                                                ║
║  📈 ПРОИЗВОДИТЕЛЬНОСТЬ:                                        ║
║                                                                ║
║     Worst case: O(n^(3/2)) = O(n^1.5)                         ║
║     Average:    O(n^(5/4)) = O(n^1.25)                        ║
║                                                                ║
║     Сравнение для n=10,000:                                   ║
║     - Insertion Sort: n² = 100,000,000 операций               ║
║     - Shell Sort:     n^1.25 ≈ 3,162,278 операций             ║
║     → В ~30 раз быстрее!                                      ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝
```

**Метафора работает потому что:**
- 🪮 **Расчёска** = проход по массиву с фиксированным шагом
- 📏 **Регулируемые зубья** = изменяющийся gap (h)
- 🎯 **Большие зубья сначала** = грубая сортировка (h=121, 40, 13)
- 🎯 **Мелкие зубья в конце** = точная досортировка (h=1)
- ⚡ **Быстрее** = меньше движений на большие расстояния

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

## 🏋️ Промежуточная задача: Sorting Race Benchmark ⭐⭐⭐☆☆

**ПЕРЕД основной миссией проведите гонку алгоритмов!**

Создайте benchmark-утилиту `sorting_race` для сравнения производительности разных алгоритмов на разных типах данных.

**Файл:** `artifacts/sorting_race.c`

**Что реализовать:**
```c
// 1. Четыре алгоритма сортировки
void bubble_sort(int arr[], int n);         // O(n²)
void insertion_sort(int arr[], int n);      // O(n²), но O(n) на sorted!
void quick_sort(int arr[], int n);          // O(n log n), но O(n²) на sorted!
void heapsort(int arr[], int n);            // O(n log n) всегда

// 2. Генераторы тестовых данных
void generate_random(int arr[], int size);
void generate_sorted(int arr[], int size);
void generate_nearly_sorted(int arr[], int size);  // 99% sorted
void generate_reversed(int arr[], int size);

// 3. Benchmark функция
long measure_time(SortFunc func, int arr[], int size);
void run_benchmark(const char *test_name, void (*generator)(int[], int));
```

**Тестовые сценарии:**
- **Random Data (10,000 элементов)** — среднестатистические данные
- **Already Sorted** — уже отсортировано (лучший случай?)
- **Nearly Sorted** — 99% sorted, 1% перемешано
- **Reversed** — обратный порядок (худший случай!)

**Ожидаемый вывод:**
```
═══ SORTING RACE: Random Data (10,000) ═══
Bubble Sort:      1234 ms    ❌ Slow (O(n²))
Insertion Sort:    582 ms    ❌ Slow (O(n²))
Quick Sort:         12 ms    ✅ Winner!
Heapsort:           18 ms    ✅ Fast

💡 Analysis:
   O(n log n) algorithms dominate on random data!

═══ SORTING RACE: Already Sorted (10,000) ═══
Bubble Sort:      1189 ms    ❌ Still O(n²)!
Insertion Sort:      3 ms    ✅ Winner! O(n) on sorted
Quick Sort:      (stack overflow)  ❌ O(n²) with naive pivot!
Heapsort:           17 ms    ✅ Stable O(n log n)

💡 Analysis:
   Insertion Sort shines on sorted data!
   Quick Sort degrades to O(n²) — CAREFUL!
   Heapsort: consistent performance regardless of input
```

**Что вы узнаете:**
1. **NO универсального лучшего алгоритма!**
   - Random: Quick Sort побеждает
   - Sorted: Insertion Sort побеждает
   - Nearly sorted: Insertion Sort побеждает

2. **Big O ≠ реальная производительность:**
   - Constants matter (Insertion Sort O(n²) может быть быстрее Heapsort O(n log n) для малых n)
   - Cache locality matters
   - Input characteristics КРИТИЧЕСКИ важны!

3. **Real-world гибриды:**
   - Timsort (Python): Merge Sort + Insertion Sort
   - Introsort (C++ std::sort): Quick + Heap + Insertion
   - pdqsort: Pattern-defeating Quick Sort

**Компиляция и запуск:**
```bash
cd artifacts
gcc -Wall -Wextra -std=c11 -O2 sorting_race.c -o sorting_race
./sorting_race
```

**Тест:**
```bash
cd tests
./test_sorting_race.sh
```

**Решение:** `solution/sorting_race_solution.c` (смотреть только если застряли!)

**Связь с миссией Episode 15:**
В основной миссии вам нужно отсортировать **10 миллионов транзакций** для поиска критической информации. Linear search займёт **53 часа**. После сортировки Heapsort + Binary Search: **0.002 миллисекунды**. Разница в **9,450,000,000× раз**! Вот почему выбор алгоритма важен.

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

## 🤔 Контрольные вопросы (интегрированы в сюжет миссии)

**💡 Эти вопросы помогут проверить понимание материала через призму сюжета Episode 15.**

### Базовое понимание (5 вопросов):

<details>
<summary><strong>1. В 03:15 V. дал задачу: отсортировать 10 миллионов записей за 3 часа. Почему Bubble Sort O(n²) не подойдёт?</strong></summary>

**Ответ:**

**Математический расчёт:**

Bubble Sort сложность: **C(n) = n(n-1)/2 ≈ ½n²**

Для n = 10,000,000:
```
Сравнения: ½ × (10^7)² = ½ × 10^14 = 50,000,000,000,000 операций
```

**Время выполнения** (при 1 млрд операций/сек на современном CPU):
```
Время = 50 × 10^12 / 10^9 = 50,000 секунд
      = 833 минуты
      = 13.9 часов ❌
```

**Deadline**: 3 часа = 10,800 секунд

**Вердикт**: Bubble Sort займёт **~14 часов**, а у нас только **3 часа**. Мы НЕ успеем! 🚨

**Сравнение с O(n log n):**

Heapsort сложность: **C(n) ≈ 2n log₂ n**

Для n = 10,000,000:
```
Сравнения: 2 × 10^7 × log₂(10^7) ≈ 2 × 10^7 × 23.3 ≈ 466,000,000 операций

Время = 466 × 10^6 / 10^9 = 0.466 секунды ✅
```

**Вывод**: 
- Bubble Sort: 13.9 часов ❌
- Heapsort: 0.5 секунды ✅
- **Разница: 100,000× раз!**

Вот почему выбор алгоритма критически важен для миссии.

</details>

<details>
<summary><strong>2. В 04:30 вы использовали Heapsort вместо Quick Sort для сортировки логов. Почему Heapsort безопаснее?</strong></summary>

**Ответ:**

**Quick Sort риск:**

Quick Sort в среднем: **O(n log n)**  
НО! В худшем случае: **O(n²)**

Худший случай для Quick Sort (naive pivot = last element):
- Уже отсортированные данные
- Обратно отсортированные данные
- Все элементы одинаковые

**Что если логи частично отсортированы?**

Server logs часто имеют временные паттерны:
```
timestamp: 1000, 1001, 1002, ..., 1500  ← растущие timestamps
```

С naive pivot (последний элемент) Quick Sort деградирует до O(n²):
```
n = 10^7
O(n²) = 10^14 операций = 13.9 часов ❌
```

**Heapsort гарантии:**

Heapsort **ВСЕГДА** O(n log n), независимо от входных данных:
- Best case: O(n log n)
- Average case: O(n log n)
- Worst case: O(n log n) ✅

**Для mission-critical задач:**
```
Quick Sort: быстрый в среднем, но РИСК провала
Heapsort: чуть медленнее, но ГАРАНТИРОВАННО O(n log n)
```

**В коде миссии:**
```c
// ❌ Рискованно (может деградировать до O(n²))
quick_sort(logs, 10000000);

// ✅ Безопасно (всегда O(n log n))
heapsort(logs, 10000000);
```

**Реальный опыт:**
- Linux kernel (до 2003): Quick Sort → иногда O(n²) зависания
- С 2003: Heapsort как fallback (Introsort = Quick + Heap)

Для операции MOONLIGHT с deadline 3 часа — безопасность > скорость!

</details>

<details>
<summary><strong>3. Shell Sort с Knuth's sequence (h = 3h + 1) показал O(n^1.25) на тестовых данных. Это лучше или хуже O(n log n)?</strong></summary>

**Ответ:**

**Сравнение для n = 10,000,000:**

```
Shell Sort: O(n^1.25)
  = (10^7)^1.25
  = 10^8.75
  = 562,341,325 операций

Heapsort: O(n log n)
  = 10^7 × log₂(10^7)
  = 10^7 × 23.3
  = 233,000,000 операций
```

**Вывод**: Heapsort **в 2.4× раза быстрее**!

**Когда Shell Sort лучше?**

Shell Sort имеет **очень маленькую константу**:

Для **малых n** (< 100,000):
```
Shell Sort: n^1.25 с константой ≈ 0.5
Heapsort:   n log n с константой ≈ 2

n = 10,000:
Shell:  0.5 × 10,000^1.25 ≈ 28,000 операций
Heap:   2 × 10,000 × 13.3 ≈ 266,000 операций

Shell Sort побеждает! ✅
```

Для **больших n** (> 1,000,000):
```
n = 10,000,000:
Shell:  0.5 × 562M ≈ 281M операций
Heap:   2 × 233M ≈ 466M операций

Примерно одинаково, но Heap масштабируется лучше
```

**В миссии Episode 15:**

10 миллионов записей → **Heapsort лучше**

Но Shell Sort — отличный выбор для:
- Embedded systems (простой код, малый footprint)
- Средние массивы (10K-100K)
- Когда Quick Sort рискованен

**Метафора**: Shell Sort = швейцарский нож (универсален), Heapsort = специализированный инструмент (для больших данных).

</details>

<details>
<summary><strong>4. Binary Search требует отсортированный массив. Если поиск O(log n) = 23 операции, а сортировка O(n log n) = 233M операций, стоит ли сортировать для одного поиска?</strong></summary>

**Ответ:**

**Анализ trade-off:**

**Вариант 1: Linear Search (без сортировки)**
```c
int linear_search(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

Сложность: O(n)
Для n = 10^7: 10,000,000 операций
Время: ~10 секунд
```

**Вариант 2: Sort + Binary Search**
```c
heapsort(arr, n);           // O(n log n) = 233M операций
binary_search(arr, n, target); // O(log n) = 23 операции

Сложность: O(n log n) + O(log n) ≈ O(n log n)
Для n = 10^7: 233,000,023 операции
Время: ~0.5 секунды
```

**Один поиск:**
```
Linear: 10 секунд
Sort + Binary: 0.5 секунды

Sort + Binary быстрее! ✅
```

**НО! Что если данные меняются?**

Если нужно **k поисков**:

```
Linear: k × n операций
Sort + Binary: n log n + k × log n операций

Break-even точка:
k × n = n log n + k × log n
k × n ≈ n log n  (если k log n << n log n)
k ≈ log n

Для n = 10^7:
k ≈ 23 поиска
```

**Вывод:**

- Если k < 20 поисков → **оба варианта примерно равны**
- Если k > 20 поисков → **Sort + Binary выигрывает**
- Если k = 1 поиск И данные static → **Sort + Binary всё равно быстрее!**

**В Episode 15:**

Нужно найти timestamp 0x5FDB8A3C в 10M логов:
- Linear search: ~10 секунд
- Sort + Binary: ~0.5 секунды

**Sort + Binary в 20× быстрее даже для ОДНОГО поиска!**

**Amortized analysis** (Кнут):
Если планируете делать много поисков → сортировка окупается мгновенно.

</details>

<details>
<summary><strong>5. В 05:47 Binary Search нашёл timestamp за 23 сравнения вместо 10,000,000. Почему именно 23?</strong></summary>

**Ответ:**

**Математика Binary Search:**

На каждом шаге массив делится пополам:

```
Итерация 1: n элементов
Итерация 2: n/2 элементов
Итерация 3: n/4 элементов
...
Итерация k: n/2^k = 1 элемент (нашли!)
```

Решаем для k:
```
n / 2^k = 1
2^k = n
k = log₂ n
```

**Для n = 10,000,000:**
```
k = log₂(10,000,000)
  = log₂(10^7)
  = log₂(10) × 7
  = 3.32 × 7
  = 23.25
  ≈ 23 итерации (в худшем случае)
```

**Визуализация:**
```
Шаг 1: 10,000,000 → проверяем mid
Шаг 2: 5,000,000 → проверяем mid
Шаг 3: 2,500,000 → проверяем mid
Шаг 4: 1,250,000
Шаг 5: 625,000
Шаг 6: 312,500
Шаг 7: 156,250
Шаг 8: 78,125
Шаг 9: 39,062
Шаг 10: 19,531
Шаг 11: 9,765
Шаг 12: 4,882
Шаг 13: 2,441
Шаг 14: 1,220
Шаг 15: 610
Шаг 16: 305
Шаг 17: 152
Шаг 18: 76
Шаг 19: 38
Шаг 20: 19
Шаг 21: 9
Шаг 22: 4
Шаг 23: 2
Шаг 24: 1 ← НАЙДЕНО!
```

**Knuth's formula (worst case):**
```
C(n) = ⌊log₂ n⌋ + 1

Для n = 10^7:
C(10^7) = ⌊23.25⌋ + 1 = 23 + 1 = 24 сравнения (worst case)
```

**Average case** (Кнут, TAOCP Vol. 3):
```
C_avg(n) ≈ log₂ n - 1 ≈ 22 сравнения
```

**Сравнение:**
```
Linear search:   10,000,000 операций
Binary search:   23 операции

Ускорение: 10,000,000 / 23 ≈ 434,782× раз!
```

**Вот почему сортировка + Binary Search критична для миссии!**

</details>

---

### Практическое применение (5 вопросов):

<details>
<summary><strong>6. Insertion Sort показал O(n) на already sorted data, но O(n²) на reversed. Почему такая разница?</strong></summary>

**Ответ:**

**Insertion Sort работа:**

```c
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        // Сдвигаем элементы > key вправо
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
    }
}
```

**Best case (already sorted):**

Массив: `[1, 2, 3, 4, 5]`

```
i=1: key=2, arr[0]=1 (1 < 2) → while НЕ выполняется → 0 swap
i=2: key=3, arr[1]=2 (2 < 3) → while НЕ выполняется → 0 swap
i=3: key=4, arr[2]=3 (3 < 4) → while НЕ выполняется → 0 swap
...

Всего сравнений: n-1 (по одному на каждое i)
Сложность: O(n) ✅
```

**Worst case (reversed):**

Массив: `[5, 4, 3, 2, 1]`

```
i=1: key=4, arr[0]=5 → сдвигаем 5 вправо → [4,5,3,2,1]  (1 сравнение)
i=2: key=3, arr[1]=5 → сдвиг, arr[0]=4 → сдвиг → [3,4,5,2,1]  (2 сравнения)
i=3: key=2 → сдвигаем 5,4,3 → [2,3,4,5,1]  (3 сравнения)
i=4: key=1 → сдвигаем 5,4,3,2 → [1,2,3,4,5]  (4 сравнения)

Всего сравнений: 1 + 2 + 3 + 4 = 10 = n(n-1)/2
Сложность: O(n²) ❌
```

**Математическое объяснение:**

**Best case**: каждый элемент уже на своём месте
```
C(n) = n - 1  (одно сравнение на элемент)
M(n) = 0  (нет обменов)
```

**Worst case**: каждый элемент в обратном порядке
```
C(n) = 1 + 2 + 3 + ... + (n-1) = n(n-1)/2 ≈ ½n²
M(n) = n(n-1)/2  (столько же обменов)
```

**Почему важно для Episode 15:**

Server logs часто **частично отсортированы** (timestamps растут):
```
Logs: [1000, 1001, 1002, ..., 999, ..., 1500]
      ↑ 99% sorted, 1% перемешано
```

Insertion Sort на таких данных ≈ **O(n)**, очень эффективен!

Вот почему Timsort (Python) использует Insertion Sort для малых подмассивов.

</details>

<details>
<summary><strong>7. Quick Sort с pivot = last element дал stack overflow на sorted data (10M записей). Что произошло? Как исправить?</strong></summary>

**Ответ:**

**Проблема:**

Quick Sort делает рекурсивные вызовы:
```c
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // pivot = arr[high]
        quick_sort(arr, low, pi - 1);   // Левая половина
        quick_sort(arr, pi + 1, high);  // Правая половина
    }
}
```

**На sorted data** с pivot = last element:

Массив: `[1, 2, 3, 4, 5]`, pivot = 5

```
Partition результат:
[1, 2, 3, 4] | 5 | []
     ↑         ↑   ↑
  левая     pivot правая (пустая!)

Рекурсия:
quick_sort([1,2,3,4])  → pivot=4 → [1,2,3] | 4 | []
  quick_sort([1,2,3])  → pivot=3 → [1,2] | 3 | []
    quick_sort([1,2])  → pivot=2 → [1] | 2 | []
      quick_sort([1])  → base case

Глубина рекурсии: n уровней!
```

**Stack overflow:**
```
Для n = 10,000,000:
Stack depth = 10M уровней
Stack size per call ≈ 100 байт
Total stack = 10M × 100 = 1 GB ❌

Typical stack limit: 8 MB
→ STACK OVERFLOW!
```

**Решения:**

**1. Random pivot (лучший практический выбор):**
```c
int random_pivot(int low, int high) {
    int random = low + rand() % (high - low + 1);
    return random;
}

// В partition:
int pivot_idx = random_pivot(low, high);
swap(&arr[pivot_idx], &arr[high]);  // Переставляем pivot в конец
int pivot = arr[high];
```

**2. Median-of-three:**
```c
int median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    
    // Сортируем arr[low], arr[mid], arr[high]
    // Возвращаем median
    
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    
    return mid;  // median
}
```

**3. Introsort (C++ std::sort):**
```c
// Quick Sort с depth limit
void introsort(int arr[], int low, int high, int depth_limit) {
    if (low < high) {
        if (depth_limit == 0) {
            // Переключаемся на Heapsort!
            heapsort_range(arr, low, high);
            return;
        }
        
        int pi = partition_random(arr, low, high);
        introsort(arr, low, pi - 1, depth_limit - 1);
        introsort(arr, pi + 1, high, depth_limit - 1);
    }
}

// Initial call
introsort(arr, 0, n-1, 2 * log2(n));
```

**В Episode 15:**

```c
// ❌ Опасно на логах (могут быть частично sorted)
quick_sort_naive(logs, 0, 10000000);

// ✅ Безопасно
quick_sort_random_pivot(logs, 0, 10000000);

// ✅ Ещё безопаснее
heapsort(logs, 10000000);  // всегда O(n log n), нет риска
```

</details>

<details>
<summary><strong>8. Knuth Shuffle использует rand() % (i+1) для генерации случайного индекса. Почему не rand() % n?</strong></summary>

**Ответ:**

**Knuth Shuffle (Fisher-Yates) алгоритм:**

```c
void knuth_shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        // ✅ Правильно: случайный индекс от 0 до i
        int j = rand() % (i + 1);
        
        // Swap arr[i] и arr[j]
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
```

**Почему `rand() % (i+1)`, а не `rand() % n`?**

**Математическое доказательство (Knuth, TAOCP Vol. 2):**

Для **равномерного** распределения каждая перестановка должна иметь вероятность **ровно 1/n!**

**Правильный алгоритм (i+1):**
```
Итерация i: выбираем из (i+1) позиций
Вероятность для конкретной перестановки:
P = (1/n) × (1/(n-1)) × ... × (1/2) × (1/1) = 1/n!  ✅
```

**Неправильный (всегда n):**
```c
// ❌ НЕПРАВИЛЬНО!
for (int i = n - 1; i > 0; i--) {
    int j = rand() % n;  // Всегда от 0 до n-1
    swap(&arr[i], &arr[j]);
}
```

Вероятность:
```
P = (1/n)^n ≠ 1/n!  ❌

Пример n=3:
Перестановок: 3! = 6
Каждая должна иметь P = 1/6

Но неправильный алгоритм:
Всего комбинаций: n^n = 3^3 = 27
27 не делится на 6!
→ Некоторые перестановки чаще, другие реже
```

**Пример n=3:**

Массив: [A, B, C]

**Правильный Knuth:**
```
i=2: выбираем из {0,1,2} (3 варианта)
i=1: выбираем из {0,1} (2 варианта)
i=0: (стоп)

Всего: 3 × 2 = 6 комбинаций = 3! ✅

Каждая перестановка встречается ровно 1 раз
```

**Неправильный (всегда n):**
```
i=2: выбираем из {0,1,2} (3 варианта)
i=1: выбираем из {0,1,2} (3 варианта) ❌

Всего: 3 × 3 = 9 комбинаций

9 / 6 = 1.5 → некоторые перестановки дублируются!
```

**Почему это важно:**

В Episode 15 используем Knuth Shuffle для генерации **по-настоящему случайных** тестовых данных:

```c
// Генерируем отсортированный массив
for (int i = 0; i < n; i++) arr[i] = i;

// Перемешиваем Knuth Shuffle
knuth_shuffle(arr, n);

// Теперь ГАРАНТИРОВАННО равномерное распределение!
// Каждая перестановка = 1/n!
```

Если алгоритм неправильный → bias в тестовых данных → неправильные выводы о производительности!

</details>

<details>
<summary><strong>9. Sentinel optimization в Linear Search убирает одну проверку в цикле. Даёт ли это реальное ускорение?</strong></summary>

**Ответ:**

**Без sentinel:**
```c
int linear_search(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {        // Проверка 1: i < n
        if (arr[i] == key) {             // Проверка 2: arr[i] == key
            return i;
        }
    }
    return -1;
}

Проверок на итерацию: 2 (i < n AND arr[i] == key)
```

**С sentinel:**
```c
int linear_search_sentinel(int arr[], int n, int key) {
    int last = arr[n];       // Сохранить последний
    arr[n] = key;            // Поставить sentinel
    
    int i = 0;
    while (arr[i] != key) {  // ТОЛЬКО ОДНА проверка!
        i++;
    }
    
    arr[n] = last;           // Восстановить
    
    if (i < n || last == key) return i;
    return -1;
}

Проверок на итерацию: 1 (только arr[i] == key)
```

**Теоретическое ускорение: 50%** (2 проверки → 1 проверка)

**Реальное ускорение?**

Тестируем на n = 10,000,000:

**Без sentinel:**
```c
// Скомпилировано с -O2
for (int i = 0; i < n; i++) {
    if (arr[i] == key) return i;
}

Assembly (примерно):
loop:
    cmp    i, n          ; Проверка 1
    jge    end
    cmp    [arr+i], key  ; Проверка 2
    je     found
    inc    i
    jmp    loop
```

**С sentinel:**
```c
while (arr[i] != key) {
    i++;
}

Assembly:
loop:
    cmp    [arr+i], key  ; Только одна проверка!
    je     end
    inc    i
    jmp    loop
```

**Benchmark результаты** (GCC -O2, 10M элементов):

```
Без sentinel:    42 ms
С sentinel:      28 ms

Ускорение: 42/28 = 1.5× (50%!) ✅
```

**НО! Есть нюансы:**

1. **Cache locality:**
   - Sentinel требует доступа к arr[n] (может быть вне cache)
   - Сохранение/восстановление last = дополнительные операции

2. **Modern CPU branch prediction:**
   - CPU предсказывает `i < n` с ~99% точностью (loop обычно продолжается)
   - Ветвление почти бесплатно!

3. **Compiler optimizations:**
   - `-O3` может развернуть цикл (loop unrolling)
   - Разница становится меньше

**Практическое применение:**

**Используйте sentinel:**
- В критичных inner loops
- Когда профилирование показало bottleneck
- В embedded systems (каждый цикл важен)

**НЕ используйте:**
- Если код становится нечитаемым
- Если разница < 5% (premature optimization!)
- Современные компиляторы часто делают это автоматически

**Knuth's мнение** (TAOCP):
> "Sentinel — классический пример оптимизации которая РЕАЛЬНО работает,  
> в отличие от многих 'улучшений' которые люди придумывают."

**В Episode 15:**

Linear search на 10M записей:
- Без sentinel: ~42 ms
- С sentinel: ~28 ms
- Экономия: 14 ms

Для mission critical — каждая миллисекунда важна!

</details>

<details>
<summary><strong>10. Binary Search: `mid = left + (right - left) / 2` вместо `mid = (left + right) / 2`. В чём разница?</strong></summary>

**Ответ:**

**Проблема: Integer Overflow**

```c
// ❌ ОПАСНО!
int mid = (left + right) / 2;
```

Если `left + right` > INT_MAX (2,147,483,647):
```
Пример:
left = 2,000,000,000
right = 2,000,000,000

left + right = 4,000,000,000  ← OVERFLOW! (> INT_MAX)

В C (32-bit int):
4,000,000,000 → wraparound → отрицательное число!
mid = negative / 2 = negative  ❌

Программа крашится или бесконечный цикл!
```

**Решение 1: Защищённое вычисление**
```c
// ✅ БЕЗОПАСНО
int mid = left + (right - left) / 2;

Пример:
left = 2,000,000,000
right = 2,000,000,000

right - left = 0
mid = 2,000,000,000 + 0/2 = 2,000,000,000  ✅

Даже если right = INT_MAX:
right - left = INT_MAX - left < INT_MAX  ✅
mid = left + (большое положительное) / 2  ✅
```

**Решение 2: Unsigned arithmetic**
```c
// ✅ Также работает
unsigned int mid = ((unsigned int)left + (unsigned int)right) / 2;

// Или в modern C:
int mid = left + (right - left) / 2;  // Стандартный способ
```

**Математическое объяснение:**

```
Что мы хотим: mid = (left + right) / 2

Преобразуем:
mid = (left + right) / 2
    = (left + left + right - left) / 2
    = (2×left + right - left) / 2
    = left + (right - left) / 2  ✅

Это МАТЕМАТИЧЕСКИ идентично!
Но избегает overflow.
```

**Реальный баг:**

Java Arrays.binarySearch() имел этот баг **15 лет** (1997-2012)!

```java
// ❌ Старый код (до 2012)
int mid = (low + high) / 2;

// ✅ Исправленный (после 2012)
int mid = (low + high) >>> 1;  // unsigned right shift
```

Баг проявлялся только для массивов > 2^30 элементов.

**В Episode 15:**

```c
// Логи: 10,000,000 записей
// Индексы: 0 ... 9,999,999

left = 0, right = 9,999,999
mid = (0 + 9,999,999) / 2 = 4,999,999  ✅ OK

Но если бы массив был 2,147,483,647 элементов:
left = 0, right = 2,147,483,646
(left + right) = 2,147,483,646  ← Почти overflow!

left + (right - left) / 2 = безопасно ✅
```

**Вывод:**

```c
// ❌ НИКОГДА не делайте так
int mid = (left + right) / 2;

// ✅ ВСЕГДА делайте так
int mid = left + (right - left) / 2;
```

**Bonus:** некоторые компиляторы оптимизируют оба варианта одинаково, но зачем рисковать?

</details>

---

### Анализ алгоритмов (3 вопроса):

<details>
<summary><strong>11. Heapsort делает build_heap за O(n), но по формуле вызывает heapify n/2 раз. Почему не O(n log n)?</strong></summary>

**Ответ:**

**Наивный анализ (неправильный):**

```
Build heap = вызвать heapify для каждого node
Узлов: n/2 (внутренние узлы)
heapify = O(log n)

→ O(n/2 × log n) = O(n log n)  ❌ НЕПРАВИЛЬНО!
```

**Правильный анализ (Knuth, TAOCP Vol. 3):**

heapify работает по-разному на разных уровнях дерева:

```
Heap высота: h = log₂ n

Уровень 0 (корень):     1 узел,    heapify = O(h)
Уровень 1:              2 узла,    heapify = O(h-1)
Уровень 2:              4 узла,    heapify = O(h-2)
...
Уровень h-1 (листья):   2^(h-1),   heapify = O(1)
```

**Подсчёт операций:**

```
Сумма = Σ (количество узлов на уровне i) × (высота от уровня i)
      = Σ 2^i × (h - i)  для i = 0 до h-1

Упрощение (математическая магия):
= 2^h × Σ (h-i)/2^i
= n × Σ (h-i)/2^i

Knuth доказал:
Σ (h-i)/2^i < 2  (geometric series)

→ Total = n × 2 = O(n)  ✅
```

**Интуитивное объяснение:**

Большинство узлов находятся **внизу** дерева (около листьев):

```
Heap с 15 узлами:

          1              ← 1 узел уровень 0, heapify = 3 шага
       /     \
      2       3          ← 2 узла уровень 1, heapify = 2 шага
    /  \    /  \
   4    5  6    7        ← 4 узла уровень 2, heapify = 1 шаг
  / \  / \/\  /\
 8 9 10 11 12 13 14 15   ← 8 узлов уровень 3, heapify = 0 шагов

Работа:
1 × 3 + 2 × 2 + 4 × 1 + 8 × 0 = 3 + 4 + 4 + 0 = 11 операций

O(n) = O(15) = 15
11 < 15  ✅
```

Половина узлов — листья (не нужно heapify вообще!).

**Код build_heap:**

```c
void build_heap(int arr[], int n) {
    // Начинаем с ПОСЛЕДНЕГО внутреннего узла
    // Листья уже valid heaps (один элемент)
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// O(n)! Не O(n log n)!
```

**Почему важно:**

```
Heapsort полная сложность:
build_heap:    O(n)         ← Быстро!
extract_max:   n × O(log n) = O(n log n)

Если бы build_heap был O(n log n):
Total = O(n log n) + O(n log n) = O(n log n)  (то же)

НО! Для частичной сортировки (top-k elements):
build_heap + k×extract = O(n) + O(k log n)

Для k << n: почти O(n)! ✅
```

**В Episode 15:**

Если нужно найти топ-1000 timestamp'ов из 10M:
- Full sort: O(10M log 10M) = 233M операций
- Heap select: O(10M + 1000 log 10M) = 10M + 23K ≈ 10M операций

**В 23× быстрее!**

</details>

<details>
<summary><strong>12. Quick Sort average case C(n) ≈ 1.39 n log₂ n, Heapsort C(n) ≈ 2 n log₂ n. Почему Quick называют "быстрым" если 1.39 < 2?</strong></summary>

**Ответ:**

**Теоретически Heapsort медленнее:**

```
Quick Sort:  C(n) ≈ 1.39 n log₂ n
Heapsort:    C(n) ≈ 2.00 n log₂ n

Heapsort делает ~43% больше сравнений!
```

**НО на практике Quick Sort БЫСТРЕЕ! Почему?**

**1. Cache locality (память решает!):**

```c
// Quick Sort: хороший cache locality
void partition(int arr[], int low, int high) {
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {  // Последовательный доступ!
            swap(&arr[i], &arr[j]);
        }
    }
}

→ Обращения к памяти ПОСЛЕДОВАТЕЛЬНЫЕ
→ CPU prefetcher работает идеально
→ Cache miss ≈ 1-2%
```

```c
// Heapsort: плохой cache locality
void heapify(int arr[], int n, int i) {
    int left = 2*i + 1;   // Прыжок в памяти!
    int right = 2*i + 2;  // Ещё прыжок!
    
    // Обращения к arr[i], arr[left], arr[right]
    // → Случайные адреса в heap
}

→ Обращения НЕПОСЛЕДОВАТЕЛЬНЫЕ
→ Cache miss ≈ 30-50%!
→ Cache miss = ~100× медленнее!
```

**2. Branch prediction:**

Quick Sort: предсказуемые ветвления
```c
if (arr[j] < pivot) {  // ~50% true, ~50% false (случайно)
    // CPU предсказывает хорошо
}
```

Heapsort: непредсказуемые сравнения в heap

**3. Константа в O-нотации:**

Big O скрывает константы!

```
Quick Sort реальная формула:
T(n) = c₁ × 1.39 n log n + c₂ × n

Heapsort реальная формула:
T(n) = c₃ × 2.0 n log n + c₄ × n

Где:
c₁ ≈ 1 (простая операция, cache-friendly)
c₃ ≈ 5 (сложная операция, cache miss)

Реально:
Quick: 1 × 1.39 = 1.39 условных единиц
Heap:  5 × 2.0 = 10 условных единиц!
```

**Benchmark реальный (n=10M):**

```
Quick Sort (random pivot):  0.42 секунды
Heapsort:                   1.23 секунды

Quick в 2.9× быстрее! ✅
```

**Почему Heapsort всё ещё полезен?**

1. **Гарантированный O(n log n)** (Quick может O(n²))
2. **In-place** (Merge Sort требует O(n) памяти)
3. **Stable worst-case** (mission-critical системы)

**Современные решения:**

**Introsort** (C++ std::sort):
```
Начинаем с Quick Sort (быстро!)
Если рекурсия слишком глубокая (2 log n):
  → Переключаемся на Heapsort (безопасно!)
  
Best of both worlds! ✅
```

**В Episode 15:**

Для 10M логов:
- Quick Sort (random): ~0.4 сек, но РИСК O(n²)
- Heapsort: ~1.2 сек, но ГАРАНТИЯ
- Introsort: ~0.4 сек + безопасность ✅

Выбор зависит от:
- Deadline критичен? → Quick Sort
- Надёжность критична? → Heapsort
- Хочешь оба? → Introsort

</details>

<details>
<summary><strong>13. Shell Sort gap sequence: Knuth (3h+1), Sedgewick, Hibbard. В чём разница? Почему Knuth лучше?</strong></summary>

**Ответ:**

**Shell Sort работает с gap sequence:**

Большие gaps → грубая сортировка  
Малые gaps → точная досортировка  
Последний gap всегда = 1 (обычная Insertion Sort)

**Разные последовательности:**

**1. Shell's original (1959):**
```
h(k) = n / 2^k

Для n=100:
50, 25, 12, 6, 3, 1

Сложность: O(n²)  ❌ Плохо!
```

**2. Hibbard (1963):**
```
h(k) = 2^k - 1

Последовательность:
1, 3, 7, 15, 31, 63, 127, 255...

Сложность: O(n^(3/2))  ⚠️ Лучше, но не оптимально
```

**3. Knuth (1973, TAOCP):**
```
h(k) = (3^k - 1) / 2
или проще:
h₁ = 1
h(k+1) = 3 × h(k) + 1

Последовательность:
1, 4, 13, 40, 121, 364, 1093, 3280, 9841...

Сложность: O(n^(3/2)) worst, O(n^(5/4)) average  ✅
```

**4. Sedgewick (1986):**
```
Чередование:
9 × 4^i - 9 × 2^i + 1
и
4^i - 3 × 2^i + 1

Последовательность:
1, 5, 19, 41, 109, 209, 505, 929, 2161...

Сложность: O(n^(4/3)) worst  ✅✅ Лучшее теоретически!
```

**Почему Knuth популярнее Sedgewick?**

**1. Простота генерации:**

```c
// Knuth: 2 строки кода
int h = 1;
while (h < n/3) h = 3*h + 1;

// Sedgewick: сложная формула
int h = ...сложная математика с 4^i и 2^i...
```

**2. Практическая производительность:**

Benchmark (n=100,000):

```
Knuth:     28 ms
Sedgewick: 24 ms

Разница: 4 ms (16%)
```

Разница есть, но **маленькая**. Knuth проще → предпочтительнее.

**3. Теоретический анализ:**

```
          Worst case    Average      Простота
Knuth:    O(n^(3/2))   O(n^(5/4))      ✅✅
Sedgewick: O(n^(4/3))  O(n^(7/6))      ⚠️ сложно
```

Knuth: лучший **баланс** между производительностью и простотой.

**Почему gaps не должны делиться?**

**Плохая последовательность (делятся):**
```
h = {16, 8, 4, 2, 1}

Проблема: все чётные!
h=16 сортирует индексы {0,16,32,...}
h=8 сортирует {0,8,16,24,32,...} ← ПЕРЕКРЫТИЕ!
h=4 сортирует {0,4,8,12,16,...} ← ЕЩЁ перекрытие!

Много дублирующейся работы.
```

**Хорошая последовательность (Knuth, не делятся):**
```
h = {121, 40, 13, 4, 1}

gcd(121, 40) = 1  ✅
gcd(40, 13) = 1   ✅
gcd(13, 4) = 1    ✅

Минимальное перекрытие → эффективнее!
```

**Математическое объяснение (Knuth):**

Если gaps coprime (взаимно простые), каждый gap "покрывает" массив по-новому.

**В Episode 15:**

```c
// Используем Knuth sequence
void shell_sort(int arr[], int n) {
    int h = 1;
    while (h < n/3) h = 3*h + 1;  // Generate Knuth gaps
    
    while (h >= 1) {
        // h-sort (Insertion Sort with gap=h)
        for (int i = h; i < n; i++) {
            int key = arr[i];
            int j = i;
            while (j >= h && arr[j-h] > key) {
                arr[j] = arr[j-h];
                j -= h;
            }
            arr[j] = key;
        }
        h = h / 3;  // Next gap
    }
}
```

Простой код, эффективный результат! ✅

</details>

---

### Сюжет и контекст (2 вопроса):

<details>
<summary><strong>14. В 06:02 миссия завершена за 2ч 47мин. Если бы использовали Bubble Sort, сколько бы времени заняло?</strong></summary>

**Ответ:**

**Реальная миссия (Heapsort + Binary Search):**

```
Сортировка (Heapsort):     0.466 секунды
Поиск (Binary Search):     0.000023 секунды (23 сравнения)

Всего: ~0.5 секунды  ✅
```

Но студент потратил **2 часа 47 минут** (10,020 секунд). Почему?

Потому что нужно было:
1. Реализовать все алгоритмы (1.5 часа)
2. Тестировать на разных данных (45 минут)
3. Бенчмаркинг и выбор лучшего (30 минут)
4. Запустить финальную сортировку и поиск (0.5 секунды!)

**Если бы использовали Bubble Sort:**

```
Bubble Sort на 10,000,000:
C(n) = ½ × (10^7)² = 50 × 10^12 операций

CPU: 1 млрд операций/сек
Время = 50 × 10^12 / 10^9 = 50,000 секунд
      = 833 минуты
      = 13.9 часов ❌
```

**Сравнение:**

```
Heapsort:    0.466 секунды
Bubble Sort: 50,000 секунд

Bubble в 107,296× медленнее!
```

**Deadline = 3 часа (10,800 секунд):**

```
Bubble Sort: 50,000 секунд → НЕ УСПЕЛИ БЫ!
Миссия провалена! 🚨
```

**Что если бы взяли Quick Sort (naive pivot)?**

На partially sorted логах (timestamps растут):
```
Quick Sort worst case: O(n²) = те же 50,000 секунд ❌
```

**Таблица выбора алгоритма:**

| Алгоритм | Время | Успели? | Риск |
|----------|-------|---------|------|
| Bubble Sort | 13.9 ч | ❌ | Нет риска, просто медленно |
| Insertion Sort | ~13 ч | ❌ | Может O(n) на sorted, но обычно O(n²) |
| Shell Sort (Knuth) | ~45 мин | ✅ | Безопасно |
| Quick Sort (naive) | 0.4 сек | ✅ | **РИСК O(n²) = 13.9ч!** |
| Quick Sort (random) | 0.4 сек | ✅ | Малый риск |
| Heapsort | 0.5 сек | ✅ | **Нет риска, ГАРАНТИЯ** |
| Merge Sort | 0.6 сек | ✅ | Требует +10M памяти |

**Выбор в миссии:**

С deadline 3 часа и mission-critical данными → **Heapsort** единственный безопасный выбор O(n log n) всегда!

**Мораль Episode 15:**

> "Выбор алгоритма — не просто теория.  
> Это разница между успехом миссии (0.5 сек)  
> и провалом (13.9 часов)."  
> — V.

</details>

<details>
<summary><strong>15. Timestamp 0x5FDB8A3C появляется в Episode 14 (Genesis Block) и Episode 15 (логи). Совпадение?</strong></summary>

**Ответ:**

**НЕТ! Это сквозная сюжетная линия!**

**Episode 14 "Blockchain & Hashes" (09:18):**
```
Genesis Block:
┌───────────────────────┐
│ index: 0              │
│ timestamp: 1577836800 │ ← Jan 1, 2020 00:00:00 UTC
│ data: "Operation MOONLIGHT initialized"
│ prev_hash: 0          │
│ hash: 0x5FDB8A3C      │ ← ЭТОТ ХЕШ!
└───────────────────────┘
```

**Episode 15 "Sorting & Search" (06:02):**
```
ЦЕЛЬ: Найти timestamp 0x5FDB8A3C в 10M логах

Binary Search результат:
Found at index: 7,250,815
Timestamp: 0x5FDB8A3C
Decoded: Jan 1, 2020 00:00:00 UTC
Location: 55.7558°N, 37.6173°E  ← КРАСНАЯ ПЛОЩАДЬ!
```

**Связь:**

1. **0x5FDB8A3C = hash Genesis Block** из Episode 14
2. В логах этот timestamp = **метка начала операции MOONLIGHT**
3. Координаты в этой записи → место первой встречи
4. Genesis Block инициализировал операцию с этих координат!

**Полная временная линия:**

```
Season 2 Ep08 (финал):
18:47  Координаты 55.7558°N, 37.6173°E → Красная площадь

Season 3 Ep09:
03:15  IP 203.0.113.42 → те же координаты

Season 3 Ep12 (финал):
08:00  Встреча с Viktor на Красной площади
       USB "Season 4"

Season 4 Ep14:
09:18  Genesis Block hash 0x5FDB8A3C
       Block #7250: координаты 55.7558°N, 37.6173°E

Season 4 Ep15:
06:02  Timestamp 0x5FDB8A3C найден в логах!
       Та же локация: 55.7558°N, 37.6173°E
       
       ЭТО ВСЁ СВЯЗАНО!
```

**Что это означает:**

1. **Операция MOONLIGHT началась** 1 января 2020, 00:00:00 UTC
2. **Genesis Block blockchain** = первая запись операции
3. **Координаты Красной площади** = штаб операции
4. **Все логи, blockchain, координаты** ведут к одному месту
5. Viktor управляет операцией с этого location

**Декодирование timestamp:**

```c
// 0x5FDB8A3C в decimal
unsigned int timestamp = 0x5FDB8A3C = 1,607,958,076

// Конвертируем в Unix time
time_t t = (time_t)timestamp;
struct tm *tm_info = gmtime(&t);

// Результат:
2020-12-15 03:27:56 UTC  ← Текущее время Episode 15!

НО ПОСТОЙТЕ! Это НЕ Jan 1, 2020!
```

**ТВИСТ!**

Timestamp 0x5FDB8A3C != 1 января 2020  
Timestamp 0x5FDB8A3C = **15 декабря 2020, 03:27:56** ← СЕЙЧАС!

**Это значит:**

Hash Genesis Block использовался как **ключ поиска** в логах!

Запись с этим timestamp содержит:
```
Record #7,250,815:
timestamp: 0x5FDB8A3C (Dec 15, 2020 03:27:56)
event: "FINAL_COORDINATES"
location: 55.7558°N, 37.6173°E
message: "Meeting point confirmed. Season finale begins."
```

**Easter Egg:**

Номер записи **7,250,815**  
Episode 14: Block **#7250** был corruption point!

7,250 → 7,250,815 (добавили 815)

**815** в hex = **0x32F** → ищите в Season 5! 😉

**Подготовка к Episode 16:**

Координаты найдены. Время известно. Blockchain верифицирован.

Осталось последнее: **построить Database Engine** для извлечения всех данных.

→ Episode 16: Data Structures (FINALE Season 4)

</details>

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