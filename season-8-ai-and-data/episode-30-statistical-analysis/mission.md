# Mission Brief: Statistical Analysis
**Episode 30** | Operation MOONLIGHT — Season 8

---

## 🎯 Mission Objective

**PRIMARY GOAL:** Perform comprehensive statistical analysis on Episode 29 data to identify attack patterns and prepare for ML training.

**CONTEXT:**  
Day 2 at Stanford AI Lab. Prof. Chen has prepared 172 network samples showing 2 attack bursts. Your statistical analysis will determine if attacks are significant and ready for neural network training (Episode 31).

**YOUR TASK:**  
Implement statistical methods in C to analyze attack patterns and prove statistical significance.

**DELIVERABLE:**  
Statistical report confirming attack patterns with hypothesis testing (t-test, p-value < 0.05).

---

## 📋 Technical Requirements

### 1. Descriptive Statistics

**Functions to Implement:**

```c
double calculate_mean(double *data, size_t n);
double calculate_variance(double *data, size_t n);
double calculate_stddev(double *data, size_t n);
double calculate_percentile(double *data, size_t n, double percentile);
```

**Formulas:**
- **Mean:** μ = (Σ xᵢ) / n
- **Variance:** σ² = Σ(xᵢ - μ)² / n
- **Standard Deviation:** σ = √(variance)
- **Percentile (p):** interpolate between sorted[floor(p*(n-1))] and sorted[ceil(p*(n-1))]

**Expected Results:**
```
network_bytes:
  mean: 3687.42, stddev: 1524.73
  Q1: 2987, median: 3198, Q3: 3421, IQR: 434

response_times:
  mean: 51.24ms, stddev: 13.85ms
  Q1: 44.7ms, median: 46.2ms, Q3: 47.3ms
```

---

### 2. Anomaly Detection

**Method A: Z-score**

```c
int detect_anomaly_zscore(double value, double mean, double stddev) {
    double z = fabs((value - mean) / stddev);
    return z > 3.0;  // Threshold: 3 standard deviations
}
```

**Interpretation:**
- |z| < 2.0: normal
- 2.0 < |z| < 3.0: suspicious
- |z| > 3.0: anomaly

**Method B: IQR (Interquartile Range)**

```c
int detect_anomaly_iqr(double value, double q1, double q3, double iqr) {
    double lower_bound = q1 - 1.5 * iqr;
    double upper_bound = q3 + 1.5 * iqr;
    return value < lower_bound || value > upper_bound;
}
```

**Expected Anomalies:**
- network_bytes: 12 anomalies (20%) with Z-score, 10 (16.67%) with IQR
- response_times: 8 anomalies (15.38%)

---

### 3. Time Series Analysis

**A. Moving Average**

```c
void moving_average(double *data, size_t n, int window, double *result) {
    // For each point i, average window/2 points before and after
    // Smooths noisy data, reveals trends
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

**Alpha parameter:**
- α = 0.1: heavy smoothing (slow response)
- α = 0.5: balanced
- α = 0.9: light smoothing (fast response)

**C. Trend Detection**

```c
double calculate_trend(double *data, size_t n) {
    // Linear regression slope
    // slope > 0: increasing trend
    // slope < 0: decreasing trend
    // slope ≈ 0: stable
}
```

---

### 4. Correlation Analysis

**Pearson Correlation Coefficient:**

```c
double pearson_correlation(double *x, double *y, size_t n) {
    // r = Σ((xᵢ - μₓ)(yᵢ - μᵧ)) / √(Σ(xᵢ - μₓ)² · Σ(yᵢ - μᵧ)²)
}
```

**Interpretation:**
- r = +1.0: perfect positive correlation
- r = +0.7 to +1.0: strong positive
- r = +0.3 to +0.7: moderate positive
- r = -0.3 to +0.3: weak/no correlation
- r = -1.0: perfect negative correlation

**Expected Correlations:**
- bytes ↔ response_time: r = 0.89 (strong positive)
- bytes ↔ packets: r = 0.92 (very strong positive)
- packets ↔ response_time: r = 0.85 (strong positive)

---

### 5. Hypothesis Testing

**Two-Sample T-Test:**

```c
double calculate_t_statistic(double *sample1, size_t n1, 
                             double *sample2, size_t n2) {
    double mean1 = calculate_mean(sample1, n1);
    double mean2 = calculate_mean(sample2, n2);
    double var1 = calculate_variance(sample1, n1);
    double var2 = calculate_variance(sample2, n2);
    
    // Standard error
    double se = sqrt(var1/n1 + var2/n2);
    
    return (mean1 - mean2) / se;
}
```

**Hypothesis:**
- H₀ (null): Attack periods have same mean as normal periods
- H₁ (alternative): Attack periods have significantly different mean

**Decision Rule:**
- If |t| > critical value (≈2.0 for α=0.05): reject H₀
- Expected: t = 5.47, p < 0.001 → REJECT H₀

---

## 🧪 Testing Criteria

### Test 1: Descriptive Statistics
```bash
cd solution
make
./statistical_analysis --stats ../artifacts/network_bytes.csv
```

**Expected Output:**
```
📊 Dataset - Descriptive Statistics:
  Count: 60
  Mean: 3687.42
  StdDev: 1524.73
  Q1 (25%): 2987.00
  Median (50%): 3198.00
  Q3 (75%): 3421.00
  IQR: 434.00
```

### Test 2: Anomaly Detection
```bash
./statistical_analysis --anomaly ../artifacts/network_bytes.csv
```

**Expected Output:**
```
🔍 Anomaly Detection Report (Z-score method):
  Total data points: 60
  Anomalies detected: 12
  Anomaly rate: 20.00%
  ⚠️  WARNING: High anomaly rate detected!
```

### Test 3: Time Series Analysis
```bash
./statistical_analysis --timeseries ../artifacts/response_times.csv
```

**Expected Output:**
```
📈 Time Series Analysis:
  Data points: 52
  Trend (slope): 0.47
  ⚠️  Increasing trend detected
```

### Test 4: Full Pipeline
```bash
./statistical_analysis --all
```

**Should process all 3 CSV files and output comprehensive analysis.**

---

## 📦 Deliverables

### Files to Create:

1. **`solution/statistical_analysis.c`**  
   Complete implementation (~480-500 lines)
   - Descriptive statistics
   - Anomaly detection (Z-score + IQR)
   - Time series (MA, exponential smoothing, trend)
   - Correlation analysis
   - Hypothesis testing

2. **`solution/Makefile`**  
   Cross-platform build (Linux/macOS/FreeBSD)

3. **`starter.c`**  
   Skeleton code with TODOs (~170 lines)

### Files Provided (in `artifacts/`):

1. **`network_bytes.csv`** (61 lines)  
   - 60 samples of bytes transferred per second
   - Contains 2 attack bursts (indices 30-34, 42-46)

2. **`response_times.csv`** (53 lines)  
   - 52 samples of response times in milliseconds
   - 8 SLA violations (>80ms threshold)

3. **`packet_counts.csv`** (61 lines)  
   - 60 samples of packets per second
   - Correlates with bytes_transferred

4. **`statistical_report.json`** (165 lines)  
   - Comprehensive analysis results
   - Attack pattern identification
   - Correlation matrix
   - Hypothesis testing results
   - Viktor's approval note

---

## 🎯 Success Criteria

✅ **Statistical Accuracy:**  
- Mean/stddev match expected values (±0.1 tolerance)
- Percentiles correctly calculated (Q1, median, Q3)
- IQR = Q3 - Q1

✅ **Anomaly Detection:**  
- Z-score method: 12 anomalies detected (20%)
- IQR method: 10 anomalies detected (16.67%)
- Both methods identify same attack indices

✅ **Correlation Analysis:**  
- bytes ↔ response_time: r > 0.85
- bytes ↔ packets: r > 0.90
- All correlations positive (attack causes delays)

✅ **Hypothesis Testing:**  
- T-statistic > 3.0
- P-value < 0.05 (statistically significant)
- Conclusion: Attacks are NOT random

✅ **Code Quality:**  
- Cross-platform (Linux/macOS/FreeBSD)
- No memory leaks (valgrind clean)
- Efficient algorithms (O(n log n) max)

---

## 💡 Hints

1. **Percentile Calculation:**  
   - Must sort data first (use qsort)
   - Interpolate between adjacent values
   - Handle edge cases (n=0, n=1)

2. **Numerical Stability:**  
   - For variance, use two-pass algorithm
   - Avoid catastrophic cancellation
   - Check for division by zero

3. **Time Series:**  
   - Moving average: handle boundaries carefully
   - Exponential smoothing: initialize with first value
   - Trend: use simple linear regression

4. **Correlation:**  
   - Pearson requires paired data (same length)
   - Handle case where stddev = 0
   - Result should be in [-1, 1]

5. **Hypothesis Testing:**  
   - Separate normal vs attack periods manually
   - Use unpaired t-test (Welch's variant)
   - Larger |t| = stronger evidence

---

## 📊 Prof. Chen's Approval

**Upon completion, Prof. Chen says:**
```
"Excellent work, Agent! 👏

Your statistical analysis confirms:
- 2 attack bursts (indices 30-34, 42-46)
- Strong correlation (r > 0.85)
- T-test: attacks are statistically significant (p < 0.001)

This proves attacks are NOT random. They're coordinated.

Tomorrow (Episode 31), we train neural network on this data.
Goal: predict attacks BEFORE they happen.

Bring your laptop. We'll code neural network from scratch in C.
No frameworks. Pure backpropagation and gradient descent.

See you at 09:00 AM. Gates Building, Lab 342.

- Prof. Chen"
```

---

**Good luck, Agent!** 🚀  
**Remember:** Anomaly is the key to the solution.
