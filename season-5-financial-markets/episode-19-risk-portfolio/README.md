# Episode 19: Risk & Portfolio Management 💼

> "Risk comes from not knowing what you're doing." — Warren Buffett

## 📋 Описание

После декодирования торговых стратегий в Episode 18, **V.** требует оценить риски портфеля MOONLIGHT. Организация может атаковать финансовую позицию операции. Необходимо построить систему risk management.

**В этом эпизоде вы научитесь:**
- Вычислять Value at Risk (VaR)
- Оптимизировать портфель по Марковицу
- Рассчитывать Sharpe ratio, Sortino ratio
- Проводить стресс-тестирование портфеля

---

## 🎯 Цели обучения

### Теория (Шиллер + Modern Portfolio Theory)
- **Value at Risk (VaR):** максимальный убыток с заданной вероятностью
- **Portfolio Optimization:** теория Марковица, эффективная граница
- **Risk Metrics:** Sharpe ratio, Sortino ratio, beta, correlation
- **Monte Carlo Simulation:** моделирование случайных сценариев

### Практика
- Расчет VaR (Historical, Parametric, Monte Carlo)
- Построение эффективной границы портфеля
- Корреляционный анализ активов
- Stress testing и scenario analysis

---

## 📖 Теоретическая часть

### 1. Value at Risk (VaR)

**VaR** — максимальный убыток с вероятностью X% за период Y.

**Пример:** VaR(95%, 1 день) = $10,000  
Означает: с вероятностью 95% убыток не превысит $10K за 1 день.

**Historical VaR:**
```c
double calculate_historical_var(double *returns, int n, double confidence) {
    // Сортируем returns по возрастанию
    qsort(returns, n, sizeof(double), compare_double);
    
    // Находим (1 - confidence) квантиль
    int index = (int)((1.0 - confidence) * n);
    
    return -returns[index];  // Убыток = отрицательный return
}

// Пример: VaR(95%) из 1000 returns
double var_95 = calculate_historical_var(returns, 1000, 0.95);
printf("VaR(95%%): $%.2f\n", var_95 * portfolio_value);
```

**Monte Carlo VaR:**
```c
double monte_carlo_var(double mean, double std_dev, int simulations, double confidence) {
    double *simulated_returns = malloc(simulations * sizeof(double));
    
    for (int i = 0; i < simulations; i++) {
        // Генерируем случайный return из нормального распределения
        simulated_returns[i] = mean + std_dev * random_normal();
    }
    
    double var = calculate_historical_var(simulated_returns, simulations, confidence);
    free(simulated_returns);
    
    return var;
}
```

### 2. Sharpe Ratio

**Sharpe Ratio** = (Return - RiskFreeRate) / Volatility

Показывает доходность с учетом риска.

```c
double calculate_sharpe_ratio(double *returns, int n, double risk_free_rate) {
    double mean_return = 0.0;
    for (int i = 0; i < n; i++) {
        mean_return += returns[i];
    }
    mean_return /= n;
    
    double std_dev = calculate_std_dev(returns, n);
    
    // Annualized Sharpe (если returns дневные)
    double annual_return = mean_return * 252;  // 252 торговых дня
    double annual_vol = std_dev * sqrt(252);
    
    return (annual_return - risk_free_rate) / annual_vol;
}
```

**Интерпретация:**
- Sharpe > 1: хорошо
- Sharpe > 2: отлично  
- Sharpe > 3: исключительно

### 3. Portfolio Optimization (Markowitz)

**Цель:** Найти веса активов для минимального риска при заданной доходности.

```c
typedef struct {
    double *weights;
    int n_assets;
    double expected_return;
    double volatility;
    double sharpe_ratio;
} Portfolio;

// Упрощенная версия (без оптимизации)
Portfolio optimize_portfolio(
    double *expected_returns,
    double **covariance_matrix,
    int n_assets,
    double target_return
) {
    Portfolio port = {0};
    port.n_assets = n_assets;
    port.weights = malloc(n_assets * sizeof(double));
    
    // TODO: Quadratic programming для оптимизации
    // Минимизировать: w^T * Cov * w
    // При ограничениях: sum(w) = 1, w^T * E(R) >= target_return
    
    return port;
}
```

### 4. Correlation Matrix

**Корреляция** показывает, как движутся активы друг относительно друга.

```c
double calculate_correlation(double *asset1, double *asset2, int n) {
    double mean1 = 0.0, mean2 = 0.0;
    for (int i = 0; i < n; i++) {
        mean1 += asset1[i];
        mean2 += asset2[i];
    }
    mean1 /= n;
    mean2 /= n;
    
    double covariance = 0.0, var1 = 0.0, var2 = 0.0;
    for (int i = 0; i < n; i++) {
        double diff1 = asset1[i] - mean1;
        double diff2 = asset2[i] - mean2;
        covariance += diff1 * diff2;
        var1 += diff1 * diff1;
        var2 += diff2 * diff2;
    }
    
    return covariance / sqrt(var1 * var2);
}

// Correlation matrix
void build_correlation_matrix(double **returns, int n_assets, int n_periods, 
                               double **corr_matrix) {
    for (int i = 0; i < n_assets; i++) {
        for (int j = 0; j < n_assets; j++) {
            corr_matrix[i][j] = calculate_correlation(
                returns[i], returns[j], n_periods
            );
        }
    }
}
```

---

## 💻 Задания

### Задание 1: VaR Calculator ⭐⭐⭐⭐☆

Реализуйте VaR тремя методами:

```c
double historical_var(double *returns, int n, double confidence);
double parametric_var(double mean, double std, double confidence);
double monte_carlo_var(double mean, double std, int sims, double conf);
```

### Задание 2: Risk Metrics ⭐⭐⭐☆☆

Рассчитайте метрики риска:

```c
double sharpe_ratio(double *returns, int n, double rf_rate);
double sortino_ratio(double *returns, int n, double rf_rate);
double max_drawdown(double *equity_curve, int n);
```

### Задание 3: Portfolio Optimizer ⭐⭐⭐⭐⭐

Оптимизация портфеля:

```c
Portfolio optimize_min_variance(
    double **covariance_matrix,
    int n_assets
);

Portfolio optimize_max_sharpe(
    double *expected_returns,
    double **covariance_matrix,
    int n_assets,
    double risk_free_rate
);
```

### Задание 4: Stress Testing ⭐⭐⭐⭐⭐

Стресс-тестирование:

```c
double stress_test_2008_crash(Portfolio *port);
double stress_test_covid_crash(Portfolio *port);
void scenario_analysis(Portfolio *port, double **scenarios, int n);
```

---

## 🎬 Сюжет

> **V.:** "Портфель MOONLIGHT под угрозой. Организация может атаковать наши позиции. Рассчитай VaR. Если превышает $500K — экстренная ликвидация. Оптимизируй портфель."

**Данные:**
- `portfolio_positions.csv` — текущие позиции
- `historical_returns.csv` — исторические доходности
- `risk_scenarios.csv` — стресс-сценарии

**Миссия:** Защитить портфель MOONLIGHT от финансовых атак.

---

## 🏆 Бонусные задания

### Bonus 1: Efficient Frontier ⭐⭐⭐⭐⭐
Визуализация эффективной границы портфеля (ASCII).

### Bonus 2: CVaR (Conditional VaR) ⭐⭐⭐⭐⭐
Expected Shortfall — средний убыток при превышении VaR.

### Bonus 3: Black-Litterman Model ⭐⭐⭐⭐⭐
Продвинутая модель оптимизации с прогнозами.

---

## 📊 Что вы узнали

- ✅ Расчет Value at Risk (VaR)
- ✅ Portfolio optimization (Markowitz)
- ✅ Risk metrics (Sharpe, Sortino, max drawdown)
- ✅ Correlation analysis
- ✅ Stress testing

---

## 🔜 Следующий эпизод

В **Episode 20: Crypto Trading & DeFi** вы научитесь:
- Анализировать криптовалютные рынки
- Работать с DeFi протоколами (концептуально)
- Находить арбитражные возможности
- Детектировать манипуляции на crypto exchanges

Финал Season 5 — раскрытие криптовалютной схемы организации на $10M+!

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 19 of 32*

[← Episode 18](../episode-18-trading-algorithms/) | [Season 5](../README.md) | [Episode 20 →](../episode-20-crypto-defi/)

</div>
