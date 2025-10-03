# Episode 30: "Statistical Analysis" 📈
## Season 8: AI & Data Science | Episode 30/42

> *"Аномалия — это ключ к разгадке."*

---

## 🎬 Dramatic Opening: Stanford AI Lab

**Location:** Stanford University AI Lab, California  
**Time:** December 28, 2024 — 08:00 PST (next day after Episode 29)  
**Coordinates:** 37.4419°N, 122.1430°W

---

### 📡 Morning Briefing from Prof. Chen

```
FROM: Prof. David Chen, Stanford AI Lab
TO: Moonlight Agent
CC: Viktor
SUBJECT: Statistical Analysis Session - URGENT
PRIORITY: HIGH
TIMESTAMP: 2024-12-28 08:00:00 PST

Agent,

Доброе утро. Viktor рассказал о вашей работе вчера. Impressive.

Я профессор Chen, руковожу AI Lab здесь в Stanford. 
Мы с Viktor сотрудничаем уже 3 года. Ваш враг — наш враг.

Episode 29 data готова. 247.8 MB обработана. Отличная работа.

Сегодня — Episode 30. Статистический анализ.
У меня есть lab со всем необходимым оборудованием.

Я подготовил данные:
- network_bytes.csv (60 samples)
- response_times.csv (52 samples)  
- packet_counts.csv (60 samples)

Preliminary analysis показывает 2 attack bursts:
- 30-34 index: moderate burst (47% increase)
- 42-46 index: severe flood (115% increase)

Ваша задача:
✅ Descriptive statistics (mean, stddev, percentiles)
✅ Anomaly detection (Z-score + IQR methods)
✅ Time series analysis (moving average, trend)
✅ Correlation analysis (3 variables)
✅ Hypothesis testing (are attacks statistically significant?)

Методы:
1. Z-score: |z| > 3.0 → anomaly
2. IQR: outside [Q1-1.5*IQR, Q3+1.5*IQR] → outlier
3. Pearson correlation: measure linear relationships
4. T-test: compare attack vs normal periods

Meeting at 10:30 AM. Office 342, Gates Building.
Принеси результаты.

Prof. Chen

P.S. Viktor сказал ты быстро учишься. Посмотрим. 😊
```

---

### ⏰ Mission Timeline

**08:00 PST** — Prof. Chen's email received  
**08:45 PST** — Arrived Stanford AI Lab (Gates Building)  
**09:00 PST** — Data loaded: 60 + 52 + 60 = 172 samples  

**09:15 PST** — Descriptive statistics calculated:
- network_bytes: mean=3687.42, stddev=1524.73
- response_times: mean=51.24ms, stddev=13.85ms  
- packet_counts: mean=159.87, stddev=22.14

**09:47 PST** — **ALERT:** Anomaly detection results:
- Z-score method: 12 anomalies in bytes (20%)
- IQR method: 10 anomalies confirmed (16.67%)
- Response times: 8 SLA violations (>80ms)

**10:05 PST** — Correlation analysis complete:
- bytes ↔ response_time: r=0.89 (strong positive)
- bytes ↔ packets: r=0.92 (very strong)
- packets ↔ response_time: r=0.85 (strong positive)

**10:18 PST** — **CRITICAL FINDING:**
- Attack indices 30-34: burst attack (medium severity)
- Attack indices 42-46: sustained flood (HIGH severity)
- T-statistic: 5.47, p-value: 0.0001
- **Conclusion:** Attacks are statistically significant!

---

### 🎓 Meeting with Prof. Chen (10:30 PST)

**Gates Building, Office 342**

```
Prof. Chen: "Agent, покажите результаты."

[You present statistical_report.json]

Prof. Chen: "Hmm... correlation 0.89 между bytes и response time.
Это подтверждает attack pattern. High traffic → slow responses."

[He studies the anomaly indices]

Prof. Chen: "Два attack burst. Indices 30-34 и 42-46.
T-test p-value 0.0001. Это highly significant."

[He turns to whiteboard, draws distributions]

Prof. Chen: "Видите? Normal distribution здесь. Но attack periods —
это outliers. Z-score > 3.0. Classical statistical attack detection."

[He pauses, looks at you seriously]

Prof. Chen: "Agent, вы знаете что это значит?"

You: "Pattern detected. We can predict next attack?"

Prof. Chen: "Exactly! 👏 Correlation analysis + time series trend.
Ваши данные готовы для machine learning."

[He pulls up another terminal]

Prof. Chen: "Episode 31 — tomorrow. Neural network.
Мы обучим модель на этих данных. Predict attacks BEFORE they happen."

[He hands you a USB drive]

Prof. Chen: "Training dataset. 10,000 samples.
50% normal traffic, 50% attack patterns.
Balanced dataset для neural network training."

You: "When do we start?"

Prof. Chen: "Tomorrow, 09:00 AM. Same lab.
Принесите laptop. Мы будем писать neural network с нуля. В C."

[He smiles]

Prof. Chen: "No TensorFlow. No PyTorch. Pure C.
Real machine learning. Backpropagation. Gradient descent.
Old school. 🤓"
```

---

### 💬 Evening Message from Viktor (19:00 PST)

```
Agent,

Prof. Chen impressed. Он сказал ты схватываешь быстро.

Statistical analysis подтвердила наши опасения:
- 2 координированные атаки
- Сильная корреляция (r > 0.85)
- T-test: attacks statistically significant (p < 0.001)

Это не случайность. Это preparation для чего-то bigger.

Episode 31 — critical. Neural network training.
Если мы сможем predict attacks — мы выиграли.

Prof. Chen — лучший в своём деле. Учись у него.

Спокойной ночи. Tomorrow — нейронные сети.

V.

P.S. Враг тоже анализирует данные. Race against time.
```

---

## 📋 Technical Briefing

Детекция аномалий в сетевом трафике, анализ паттернов, статистика.

**Задачи:**
1. Mean, variance, std deviation
2. Anomaly detection
3. Time series analysis
4. Correlation analysis

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
