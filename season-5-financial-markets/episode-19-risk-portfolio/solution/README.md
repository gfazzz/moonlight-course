# Episode 19: Risk & Portfolio Management - SOLUTION

## 📖 О решении

Профессиональная система анализа портфельных рисков с VaR, Monte Carlo симуляцией и stress testing.

**Версия:** v2.0 Enhanced Edition

## ✅ Реализованные функции

### 1. Value at Risk (VaR)
- ✅ **Historical VaR** — на основе прошлых данных
- ✅ **Parametric VaR** — предположение нормального распределения
- ✅ **Monte Carlo VaR** — симуляция 10,000 сценариев

### 2. Risk Metrics
- ✅ **Sharpe Ratio** — risk-adjusted returns
- ✅ **Sortino Ratio** — только downside risk
- ✅ **Max Drawdown** — максимальное падение
- ✅ **Correlation Matrix** — диверсификация

### 3. Portfolio Analysis
- ✅ Mean return и volatility (daily + annualized)
- ✅ Weighted portfolio returns
- ✅ Average correlation calculation
- ✅ Leverage analysis

### 4. Stress Testing
- ✅ 2008 Financial Crisis (-40%)
- ✅ 2020 COVID-19 Crash (-30%)
- ✅ Flash Crash 2010 (-15%)
- ✅ Crypto Winter 2022 (-65%)

### 5. Risk Assessment
- ✅ Automatic risk level classification
- ✅ Diversification scoring
- ✅ Recommendations generation

## 🛠️ Компиляция

```bash
make
```

## 🚀 Запуск

```bash
./risk_portfolio ../artifacts/portfolio_holdings.csv
```

## 📊 Формат входных данных

CSV файл с портфелем:
```
ASSET,VALUE,WEIGHT,TYPE
BTC,$17500000,35.0%,CRYPTO
ETH,$12250000,24.5%,CRYPTO
...
```

## 📈 Пример вывода

```
╔═══════════════════════════════════════════════════════════════════╗
║                       RISK ANALYSIS                               ║
╚═══════════════════════════════════════════════════════════════════╝

⚠️  Value at Risk (95% confidence, 1 day):
   Historical VaR:   $5.57M
   Parametric VaR:   $5.36M
   Monte Carlo VaR:  $5.39M

📊 Risk Metrics:
   Sharpe Ratio:     -0.60
   Sortino Ratio:    -0.58
   Max Drawdown:     38.02%
   Avg Correlation:  0.05

📊 Scenario: 2008 Financial Crisis
   Loss: $-60.00M (-120.0%)
   ⚠️  LIQUIDATION - Total capital loss!
```

## 🔍 Методы расчета

### Historical VaR
```c
// Sort historical returns
// VaR = -returns[quantile_index]
VaR(95%) = -returns[5th percentile]
```

### Parametric VaR
```c
z_score = 1.645  // 95% confidence
VaR = -(mean_return - z_score * std_dev) * portfolio_value
```

### Monte Carlo VaR
```c
// Generate 10,000 scenarios using Box-Muller transform
for (i = 0; i < 10000; i++) {
    scenario[i] = random_normal(mean, std_dev)
}
VaR = historical_var(scenarios)
```

### Sharpe Ratio
```c
Sharpe = (annual_return - risk_free_rate) / annual_volatility

// Annualization (252 trading days)
annual_return = daily_return * 252
annual_volatility = daily_volatility * sqrt(252)
```

**Интерпретация:**
- `> 2.0` — Excellent
- `1.0-2.0` — Good  
- `0-1.0` — Acceptable
- `< 0` — Poor (losing money)

### Sortino Ratio
```c
// Similar to Sharpe but only considers downside deviation
downside_deviation = sqrt(sum(negative_returns²) / count)
Sortino = (return - rf) / downside_deviation
```

**Advantage:** Не штрафует за positive volatility

### Correlation Matrix
```c
corr(X, Y) = cov(X, Y) / (std(X) * std(Y))

// Interpretation:
//  1.0 = perfect positive correlation
//  0.0 = no correlation
// -1.0 = perfect negative correlation
```

### Stress Testing
```c
scenario_loss = portfolio_value * scenario_return
leveraged_loss = scenario_loss * leverage
remaining = portfolio_value + leveraged_loss
```

## 🎓 Учебные цели

Это решение демонстрирует:

1. **Value at Risk** — стандарт risk management в банках
2. **Monte Carlo Simulation** — стохастическое моделирование
3. **Modern Portfolio Theory** — Markowitz optimization
4. **Stress Testing** — extreme scenario analysis
5. **Risk Metrics** — профессиональные индикаторы
6. **Leverage Analysis** — понимание риска заемных средств

## 💡 Ключевые выводы

### Leverage — это обоюдоострый меч

**Без leverage (1:1):**
- Потеря 40% → убыток $20M
- Остается: $30M

**С leverage 3:1:**
- Потеря 40% → убыток $60M  
- Остается: $-10M (LIQUIDATION!)

### Диверсификация спасает

**Плохая диверсификация (correlation 0.95):**
- Все активы падают вместе
- VaR может быть > 30%

**Хорошая диверсификация (correlation 0.3):**
- Активы движутся независимо
- VaR снижается на 50-70%

### VaR не учитывает хвосты распределения

- **Limitation:** VaR показывает "обычные" потери
- **Tail Risk:** Экстремальные события (Black Swans) могут быть намного хуже
- **Solution:** Stress testing + CVaR (Conditional VaR)

## ⚠️ Важное замечание

**Этот код создан для образовательных целей!**

- ❌ НЕ является инвестиционным советом
- ⚠️ VaR — не гарантия (может быть хуже)
- ⚠️ Прошлые данные ≠ будущие результаты
- 🔴 Leverage опасен для неопытных инвесторов

**В реальности:**
- Используйте профессиональные системы (Bloomberg, FactSet)
- Консультируйтесь с риск-менеджерами
- Учитывайте ликвидность, operational risk, regulatory risk

## 🏆 Achievement

После успешного запуска разблокируется:

**"Risk Master"** — You've mastered portfolio risk management.

## ➡️ Что дальше?

Episode 20: Crypto & DeFi (FINALE)
- Arbitrage detection
- Pump & dump analysis
- DeFi protocol analysis
- Money laundering evidence

---

**Operation MOONLIGHT** — Season 5, Episode 19  
v2.0 Enhanced Edition

