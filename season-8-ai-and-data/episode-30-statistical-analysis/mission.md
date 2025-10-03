# Брифинг миссии: Статистический анализ
**Episode 30** | Operation MOONLIGHT — Season 8

---

## 🎯 Цель миссии

**ОСНОВНАЯ ЦЕЛЬ:** Провести комплексный статистический анализ данных Episode 29 для идентификации паттернов атак и подготовки к ML обучению.

**КОНТЕКСТ:**  
День 2 в Stanford AI Lab. Prof. Chen подготовил 172 сэмпла сети, показывающих 2 attack burst. Ваш статистический анализ определит, являются ли атаки значимыми и готовы ли данные для обучения нейросети (Episode 31).

**ВАША ЗАДАЧА:**  
Реализовать статистические методы на C для анализа паттернов атак и доказательства статистической значимости.

**РЕЗУЛЬТАТ:**  
Статистический отчёт, подтверждающий паттерны атак с hypothesis testing (t-test, p-value < 0.05).

---

## 📋 Технические требования

### 1. Описательная статистика

**Функции для реализации:**

```c
double calculate_mean(double *data, size_t n);
double calculate_variance(double *data, size_t n);
double calculate_stddev(double *data, size_t n);
double calculate_percentile(double *data, size_t n, double percentile);
```

**Формулы:**
- **Mean:** μ = (Σ xᵢ) / n
- **Variance:** σ² = Σ(xᵢ - μ)² / n
- **Standard Deviation:** σ = √(variance)
- **Percentile (p):** интерполировать между sorted[floor(p*(n-1))] и sorted[ceil(p*(n-1))]

**Ожидаемые результаты:**
```
network_bytes:
  mean: 3687.42, stddev: 1524.73
  Q1: 2987, median: 3198, Q3: 3421, IQR: 434

response_times:
  mean: 51.24ms, stddev: 13.85ms
  Q1: 44.7ms, median: 46.2ms, Q3: 47.3ms
```

---

### 2. Обнаружение аномалий

**Метод A: Z-score**

```c
int detect_anomaly_zscore(double value, double mean, double stddev) {
    double z = fabs((value - mean) / stddev);
    return z > 3.0;  // Порог: 3 стандартных отклонения
}
```

**Интерпретация:**
- |z| < 2.0: нормально
- 2.0 < |z| < 3.0: подозрительно
- |z| > 3.0: аномалия

**Метод B: IQR (Interquartile Range)**

```c
int detect_anomaly_iqr(double value, double q1, double q3, double iqr) {
    double lower_bound = q1 - 1.5 * iqr;
    double upper_bound = q3 + 1.5 * iqr;
    return value < lower_bound || value > upper_bound;
}
```

**Ожидаемые аномалии:**
- network_bytes: 12 аномалий (20%) с Z-score, 10 (16.67%) с IQR
- response_times: 8 аномалий (15.38%)

---

### 3. Анализ временных рядов

**A. Moving Average**

```c
void moving_average(double *data, size_t n, int window, double *result) {
    // Для каждой точки i, усреднить window/2 точек до и после
    // Сглаживает шумные данные, выявляет тренды
}
```

**B. Exponential Smoothing**

```c
void exponential_smoothing(double *data, size_t n, double alpha, double *result) {
    result[0] = data[0];
    for (i = 1; i < n; i++) {
        result[i] = alpha * data[i] + (1 - alpha) * result[i-1];
    }
}
```

**Параметр Alpha:**
- α = 0.1: тяжёлое сглаживание (медленный отклик)
- α = 0.5: сбалансированный
- α = 0.9: лёгкое сглаживание (быстрый отклик)

**C. Обнаружение тренда**

```c
double calculate_trend(double *data, size_t n) {
    // Наклон линейной регрессии
    // slope > 0: возрастающий тренд
    // slope < 0: убывающий тренд
    // slope ≈ 0: стабильный
}
```

---

### 4. Корреляционный анализ

**Коэффициент корреляции Пирсона:**

```c
double pearson_correlation(double *x, double *y, size_t n) {
    // r = Σ((xᵢ - μₓ)(yᵢ - μᵧ)) / √(Σ(xᵢ - μₓ)² · Σ(yᵢ - μᵧ)²)
}
```

**Интерпретация:**
- r = +1.0: идеальная положительная корреляция
- r = +0.7 to +1.0: сильная положительная
- r = +0.3 to +0.7: умеренная положительная
- r = -0.3 to +0.3: слабая/нет корреляции
- r = -1.0: идеальная отрицательная корреляция

**Ожидаемые корреляции:**
- bytes ↔ response_time: r = 0.89 (сильная положительная)
- bytes ↔ packets: r = 0.92 (очень сильная положительная)
- packets ↔ response_time: r = 0.85 (сильная положительная)

---

### 5. Проверка гипотез

**Two-Sample T-Test:**

```c
double calculate_t_statistic(double *sample1, size_t n1, 
                             double *sample2, size_t n2) {
    double mean1 = calculate_mean(sample1, n1);
    double mean2 = calculate_mean(sample2, n2);
    double var1 = calculate_variance(sample1, n1);
    double var2 = calculate_variance(sample2, n2);
    
    // Стандартная ошибка
    double se = sqrt(var1/n1 + var2/n2);
    
    return (mean1 - mean2) / se;
}
```

**Гипотеза:**
- H₀ (нулевая): Периоды атак имеют такое же среднее как нормальные периоды
- H₁ (альтернативная): Периоды атак имеют значительно отличающееся среднее

**Правило решения:**
- Если |t| > критическое значение (≈2.0 для α=0.05): отклонить H₀
- Ожидается: t = 5.47, p < 0.001 → ОТКЛОНИТЬ H₀

---

## 🧪 Критерии тестирования

### Тест 1: Описательная статистика
```bash
cd solution
make
./statistical_analysis --stats ../artifacts/network_bytes.csv
```

**Ожидаемый вывод:**
```
📊 Dataset - Описательная статистика:
  Количество: 60
  Среднее: 3687.42
  StdDev: 1524.73
  Q1 (25%): 2987.00
  Медиана (50%): 3198.00
  Q3 (75%): 3421.00
  IQR: 434.00
```

### Тест 2: Обнаружение аномалий
```bash
./statistical_analysis --anomaly ../artifacts/network_bytes.csv
```

**Ожидаемый вывод:**
```
🔍 Отчёт об аномалиях (метод Z-score):
  Всего точек данных: 60
  Обнаружено аномалий: 12
  Процент аномалий: 20.00%
  ⚠️  ПРЕДУПРЕЖДЕНИЕ: Обнаружен высокий процент аномалий!
```

### Тест 3: Анализ временных рядов
```bash
./statistical_analysis --timeseries ../artifacts/response_times.csv
```

**Ожидаемый вывод:**
```
📈 Анализ временных рядов:
  Точек данных: 52
  Тренд (наклон): 0.47
  ⚠️  Обнаружен возрастающий тренд
```

### Тест 4: Полный pipeline
```bash
./statistical_analysis --all
```

**Должен обработать все 3 CSV файла и вывести комплексный анализ.**

---

## 📦 Результаты миссии

### Файлы для создания:

1. **`solution/statistical_analysis.c`**  
   Полная реализация (~480-500 строк)
   - Описательная статистика
   - Обнаружение аномалий (Z-score + IQR)
   - Временные ряды (MA, exponential smoothing, trend)
   - Корреляционный анализ
   - Проверка гипотез

2. **`solution/Makefile`**  
   Кроссплатформенная сборка (Linux/macOS/FreeBSD)

3. **`starter.c`**  
   Скелет кода с TODO (~170 строк)

### Предоставленные файлы (в `artifacts/`):

1. **`network_bytes.csv`** (61 строка)  
   - 60 сэмплов переданных байт в секунду
   - Содержит 2 attack burst (indices 30-34, 42-46)

2. **`response_times.csv`** (53 строки)  
   - 52 сэмпла времён отклика в миллисекундах
   - 8 нарушений SLA (>80ms порог)

3. **`packet_counts.csv`** (61 строка)  
   - 60 сэмплов пакетов в секунду
   - Коррелирует с bytes_transferred

4. **`statistical_report.json`** (165 строк)  
   - Комплексные результаты анализа
   - Идентификация паттерна атак
   - Корреляционная матрица
   - Результаты проверки гипотез
   - Заметка одобрения Viktor

---

## 🎯 Критерии успеха

✅ **Статистическая точность:**  
- Mean/stddev совпадают с ожидаемыми значениями (±0.1 допуск)
- Персентили корректно рассчитаны (Q1, median, Q3)
- IQR = Q3 - Q1

✅ **Обнаружение аномалий:**  
- Метод Z-score: 12 аномалий обнаружено (20%)
- Метод IQR: 10 аномалий обнаружено (16.67%)
- Оба метода идентифицируют одинаковые индексы атак

✅ **Корреляционный анализ:**  
- bytes ↔ response_time: r > 0.85
- bytes ↔ packets: r > 0.90
- Все корреляции положительные (атака вызывает задержки)

✅ **Проверка гипотез:**  
- T-statistic > 3.0
- P-value < 0.05 (статистически значимо)
- Вывод: Атаки НЕ случайны

✅ **Качество кода:**  
- Кроссплатформенность (Linux/macOS/FreeBSD)
- Нет утечек памяти (valgrind clean)
- Эффективные алгоритмы (макс O(n log n))

---

## 💡 Подсказки

1. **Расчёт персентиля:**  
   - Сначала нужно отсортировать данные (использовать qsort)
   - Интерполировать между соседними значениями
   - Обработать граничные случаи (n=0, n=1)

2. **Численная стабильность:**  
   - Для variance использовать двупроходный алгоритм
   - Избегать катастрофического сокращения
   - Проверять деление на ноль

3. **Временные ряды:**  
   - Moving average: аккуратно обрабатывать границы
   - Exponential smoothing: инициализировать первым значением
   - Trend: использовать простую линейную регрессию

4. **Корреляция:**  
   - Pearson требует парных данных (одинаковой длины)
   - Обработать случай когда stddev = 0
   - Результат должен быть в [-1, 1]

5. **Проверка гипотез:**  
   - Разделить нормальные vs периоды атак вручную
   - Использовать unpaired t-test (вариант Welch)
   - Больший |t| = более сильное доказательство

---

## 📊 Одобрение Prof. Chen

**По завершении Prof. Chen говорит:**
```
"Отличная работа, Agent! 👏

Ваш статистический анализ подтверждает:
- 2 attack burst (indices 30-34, 42-46)
- Сильная корреляция (r > 0.85)
- T-test: атаки статистически значимы (p < 0.001)

Это доказывает, что атаки НЕ случайны. Они координированы.

Завтра (Episode 31) мы обучим нейросеть на этих данных.
Цель: предсказать атаки ДО того как они произойдут.

Принесите laptop. Мы закодируем нейросеть с нуля на C.
Никаких фреймворков. Чистая backpropagation и gradient descent.

Увидимся в 09:00 AM. Gates Building, Lab 342.

- Prof. Chen"
```

---

**Удачи, Agent!** 🚀  
**Помните:** Аномалия — это ключ к разгадке.

---

**БРИФИНГ МИССИИ:**

**Локация:** Stanford University AI Lab, Gates Building, Lab 342  
**Время:** December 28, 2024 — 08:00 PST (день 2)  
**Контакт:** Prof. David Chen (Stanford AI Lab)

**Контекст:**
После Episode 29 (247.8 MB обработано, 347 угроз найдено), Prof. Chen подготовил refined dataset для статистического анализа:
- network_bytes.csv (60 сэмплов)
- response_times.csv (52 сэмпла)
- packet_counts.csv (60 сэмплов)

**Предварительный анализ показывает:**
- 2 attack bursts:
  * 30-34 index: moderate burst (47% увеличение)
  * 42-46 index: severe flood (115% увеличение)

**Задачи:**
1. Descriptive statistics (mean, stddev, percentiles)
2. Anomaly detection (Z-score + IQR методы)
3. Time series analysis (moving average, trend)
4. Correlation analysis (3 переменные)
5. Hypothesis testing (атаки статистически значимы?)

**Методы:**
- Z-score: |z| > 3.0 → anomaly
- IQR: вне [Q1-1.5*IQR, Q3+1.5*IQR] → outlier
- Pearson correlation: измерить линейные связи
- T-test: сравнить периоды атак vs нормальные

**Ожидаемые результаты:**
- Корреляции: bytes↔response_time (r=0.89), bytes↔packets (r=0.92)
- T-test: t=5.47, p<0.001 (HIGHLY SIGNIFICANT)
- Аномалии: 12 (Z-score), 10 (IQR)

**Встреча:** 10:30 AM, Office 342, Gates Building — представить результаты

**Артефакты созданы:**
- network_bytes.csv, response_times.csv, packet_counts.csv
- statistical_report.json (165 строк)

**Успешный результат:** Доказана статистическая значимость атак, данные готовы для Episode 31 (neural network training).

---

**Следующий эпизод:** [Episode 31: Neural Networks →](../episode-31-neural-network/)