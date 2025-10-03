# Миссия 19: Управление рисками и портфелем

## 🎯 Цель

Проанализировать критический риск портфеля $50M. Вычислить Value at Risk (VaR), построить correlation matrix и оптимизировать портфель используя теорию Марковица.

## 📋 Брифинг миссии

**Дата:** 19 декабря, 18:45 GMT  
**Локация:** City of London, Лондон (51.5074°N, 0.1278°W)  
**Статус:** 🔴🔴 КРИТИЧНО

Портфель организации ($50M) находится под экстремальным риском:
- 70% в криптовалютах (высокая волатильность)
- Leverage 3:1 (утроенная экспозиция = $150M)
- Корреляция 0.95 (отсутствие диверсификации!)

V. нужен профессиональный анализ рисков НЕМЕДЛЕННО.

**Ваша миссия:**
1. Вычислить Value at Risk (VaR) тремя методами
2. Построить correlation matrix
3. Вычислить Sharpe и Sortino ratios
4. Оптимизация портфеля (Markowitz)
5. Stress testing (сценарии 2008, 2020, Flash Crash)

**Временной лимит:** До 23:30 GMT (4 часа 45 минут)

## 📂 Предоставленные файлы

- `artifacts/portfolio_holdings.csv` — Текущие позиции ($50M)
- `artifacts/historical_returns.csv` — 2 года данных доходности
- `artifacts/stress_scenarios.txt` — Кризисные сценарии
- `artifacts/correlation_matrix.dat` — Предрасчитанные корреляции

## ✅ Критерии успеха

Ваша реализация должна:
- Вычислить VaR (Historical, Parametric, Monte Carlo)
- Вычислить Sharpe ratio и Sortino ratio
- Проанализировать correlation matrix
- Оптимизированный портфель снижает VaR на >60%
- Результаты stress test по 3 сценариям

## 💡 Подсказки

**VaR (Value at Risk) методы:**

**1. Historical VaR:**
```c
// Сортировать returns по возрастанию
qsort(returns, n, sizeof(double), compare);

// VaR(95%) = 5-й процентиль
int index = (int)((1.0 - 0.95) * n);
double var_95 = -returns[index];
```

**2. Parametric VaR (нормальное распределение):**
```c
double mean = calculate_mean(returns, n);
double std_dev = calculate_std_dev(returns, n);

// VaR(95%) = mean - 1.645 * std_dev (one-tailed)
double var_95 = -(mean - 1.645 * std_dev);
```

**3. Monte Carlo VaR:**
```c
// Симулировать 10,000 сценариев
for (int i = 0; i < 10000; i++) {
    double sim_return = mean + std_dev * random_normal();
    simulated_returns[i] = sim_return;
}
// Затем Historical VaR на симуляциях
```

**Sharpe Ratio:**
```c
// Annualized Sharpe Ratio
double annual_return = mean_return * 252;  // 252 торговых дня
double annual_vol = std_dev * sqrt(252);
double sharpe = (annual_return - risk_free_rate) / annual_vol;
```

**Sortino Ratio** (учитывает только downside volatility):
```c
// Считать только negative returns для downside deviation
double downside_dev = calculate_downside_deviation(returns, n);
double sortino = (annual_return - risk_free_rate) / downside_dev;
```

**Correlation Matrix:**
```c
double correlation(double *x, double *y, int n) {
    double cov = covariance(x, y, n);
    double std_x = std_dev(x, n);
    double std_y = std_dev(y, n);
    return cov / (std_x * std_y);
}
```

**Portfolio Optimization (упрощённая версия):**
```
Цель: Минимизировать риск при заданной доходности
Ограничения:
• sum(weights) = 1.0
• weights >= 0 (no short selling)
• portfolio_return >= target_return
```

**Stress Test сценарии:**
- **2008 Crisis:** -37% падение рынка
- **2020 COVID:** -34% падение за месяц
- **Flash Crash 2010:** -9% за минуты

## 🔍 Ожидаемый результат

```
╔═══════════════════════════════════════════════════════╗
║  PORTFOLIO RISK ANALYSIS                              ║
╠═══════════════════════════════════════════════════════╣
║  Portfolio Value: $50,000,000                         ║
║  Leverage: 3:1 ($150M exposure)                       ║
║                                                       ║
║  VALUE AT RISK (95%, 1 day):                          ║
║  • Historical VaR:  $14.7M (29.4%)                    ║
║  • Parametric VaR:  $16.2M (32.4%)                    ║
║  • Monte Carlo VaR: $15.8M (31.6%)                    ║
║  • Average VaR:     $15.6M (31.2%) ⚠️ КРИТИЧНО!      ║
║                                                       ║
║  RISK METRICS:                                        ║
║  • Sharpe Ratio:    0.87 (ниже 1 — плохо)            ║
║  • Sortino Ratio:   1.12 (приемлемо)                 ║
║  • Max Drawdown:    -38% (катастрофично)             ║
║  • Avg Correlation: 0.94 (нет диверсификации)        ║
║                                                       ║
║  STRESS TEST:                                         ║
║  • 2008 Scenario:   -$42M (-84%) 💀                  ║
║  • 2020 COVID:      -$28M (-56%) 💀                  ║
║  • Flash Crash:     -$19M (-38%) 💀                  ║
║                                                       ║
║  ✅ OPTIMIZED PORTFOLIO (Markowitz):                  ║
║  • VaR(95%):        $4.2M (8.4%) — снижен в 3.7x!    ║
║  • Sharpe Ratio:    1.95 (отлично)                   ║
║  • Max Drawdown:    -12% (приемлемо)                 ║
║  • Diversification: 15 активов, correlation avg 0.35 ║
║                                                       ║
╚═══════════════════════════════════════════════════════╝
```

## 🏆 Достижение

**"Risk Master"** — Спасение портфеля $50M от катастрофы VaR $15.6M

---

**Следующий эпизод:** [Episode 20: Crypto & DeFi — FINALE](../episode-20-crypto-defi/)