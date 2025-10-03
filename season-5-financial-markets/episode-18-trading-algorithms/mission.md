# Миссия 18: Торговые алгоритмы — Реверс-инжиниринг

## 🎯 Цель

Провести реверс-инжиниринг торговых алгоритмов организации. Реализовать технические индикаторы (SMA, RSI, MACD) и построить backtesting framework для разоблачения overfitting.

## 📋 Брифинг миссии

**Дата:** 19 декабря, 08:30 EST  
**Локация:** Manhattan, Financial District, Нью-Йорк  
**Статус:** 🔴 СРОЧНО

V. перехватил торговые логи системы организации. Они заявляют о 127% годовой доходности. Ваша миссия — проверить эти утверждения через backtesting.

**Ваша миссия:**
1. Реализовать технические индикаторы (SMA, EMA, RSI, MACD)
2. Воссоздать их 3 торговые стратегии
3. Построить backtesting engine
4. Разоблачить overfitting (127% → реальная доходность)

**Временной лимит:** До 14:00 EST (5.5 часов)

## 📂 Предоставленные файлы

- `artifacts/strategy_logs.csv` — 47,392 сделки за 6 месяцев
- `artifacts/historical_prices.csv` — OHLCV данные для backtesting
- `artifacts/algorithm_fingerprints.txt` — Паттерны стратегий

## ✅ Критерии успеха

Ваша реализация должна:
- Реализовать индикаторы: SMA, EMA, RSI, MACD
- Генерировать сигналы BUY/SELL
- Проводить backtesting с комиссиями и slippage
- Вычислять Sharpe ratio и max drawdown
- Разоблачить overfitting

## 💡 Подсказки

**Стратегии организации:**
```
STRATEGY 1 (42% сделок): SMA crossover
• Entry: Golden Cross (SMA20 > SMA50)
• Exit: Death Cross (SMA20 < SMA50)
• Win Rate: 68% (заявлено)

STRATEGY 2 (31% сделок): RSI oversold/overbought
• Entry: RSI < 30 (oversold)
• Exit: RSI > 70 (overbought)
• Win Rate: 61% (заявлено)

STRATEGY 3 (27% сделок): MACD + Volume
• Entry: MACD пересекает signal line + Volume > 2x avg
• Exit: MACD reversal
• Win Rate: 73% (заявлено)
```

**Формулы индикаторов:**
```c
// SMA (Simple Moving Average)
double sma = sum(prices, period) / period;

// RSI (Relative Strength Index)
double rs = avg_gain / avg_loss;
double rsi = 100.0 - (100.0 / (1.0 + rs));

// MACD (Moving Average Convergence Divergence)
double macd = EMA(12) - EMA(26);
double signal = EMA(macd, 9);
```

**Backtesting метрики:**
- **Sharpe Ratio:** `(return - risk_free_rate) / std_dev`
- **Max Drawdown:** максимальное падение от пика
- **Win Rate:** `profitable_trades / total_trades`

## 🏆 Достижение

**"Algorithm Engineer"** — Разоблачение overfitting мошенничества (127% → 18%)

---

**Следующий эпизод:** [Episode 19: Risk & Portfolio](../episode-19-risk-portfolio/)

