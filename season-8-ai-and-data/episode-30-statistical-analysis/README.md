# Episode 30: "Statistical Analysis" 📈
## Season 8: AI & Data Science | Episode 30/42

> *"Аномалия — это ключ к разгадке."*

---

## 📋 Briefing

Детекция аномалий в сетевом трафике, анализ паттернов, статистика.

**Задачи:**
1. Mean, variance, std deviation
2. Anomaly detection
3. Time series analysis
4. GNU Scientific Library (GSL)

---

## 📚 Теория

### Basic Statistics

```c
double mean(double *data, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

double variance(double *data, int n) {
    double m = mean(data, n);
    double var = 0;
    for (int i = 0; i < n; i++) {
        var += (data[i] - m) * (data[i] - m);
    }
    return var / n;
}
```

### GSL (GNU Scientific Library)

```c
#include <gsl/gsl_statistics.h>

double mean = gsl_stats_mean(data, 1, n);
double sd = gsl_stats_sd(data, 1, n);
```

---

## 🛠 Практика

**Задачи:**
1. Network traffic anomaly detector
2. Moving average filter
3. Correlation analysis
4. Statistical report generator

---

**Next:** [Episode 31: Neural Networks →](../episode-31-neural-network/)
