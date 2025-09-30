# Episode 18: Trading Algorithms 📈

> "The goal of a successful trader is to make the best trades. Money is secondary." — Alexander Elder

## 📋 Описание

Анализ микроструктуры в Episode 17 выявил сложные торговые паттерны организации. **V.** требует реверс-инжиниринг их торговых алгоритмов. Необходимо реализовать технические индикаторы, создать торговые стратегии и протестировать их через backtesting.

**В этом эпизоде вы научитесь:**
- Реализовывать технические индикаторы (SMA, EMA, RSI, MACD)
- Создавать торговые стратегии (momentum, mean reversion)
- Строить backtesting framework
- Анализировать результаты стратегий

---

## 🎯 Цели обучения

### Теория
- **Technical Indicators:** SMA, EMA, RSI, MACD, Bollinger Bands
- **Trading Strategies:** momentum, mean reversion, trend following
- **Backtesting:** исторические данные, performance metrics
- **Order Types:** market, limit, stop-loss, take-profit

### Практика
- Расчет индикаторов на исторических данных
- Генерация торговых сигналов (buy/sell)
- Backtesting engine с учетом комиссий и проскальзывания
- Performance analysis (Sharpe ratio, max drawdown)

---

## 📖 Теоретическая часть

### 1. Simple Moving Average (SMA)

**SMA** — средняя цена за N периодов:

```c
double calculate_sma(double *prices, int n, int period) {
    if (n < period) return 0.0;
    
    double sum = 0.0;
    for (int i = n - period; i < n; i++) {
        sum += prices[i];
    }
    
    return sum / period;
}
```

**Применение:** Когда цена выше SMA(50) — восходящий тренд.

### 2. Relative Strength Index (RSI)

**RSI** — индекс относительной силы (0-100):

- RSI > 70: перекупленность (overbought) → сигнал на продажу
- RSI < 30: перепроданность (oversold) → сигнал на покупку

```c
double calculate_rsi(double *prices, int n, int period) {
    double gains = 0.0, losses = 0.0;
    
    for (int i = n - period; i < n; i++) {
        double change = prices[i] - prices[i-1];
        if (change > 0) gains += change;
        else losses += -change;
    }
    
    double avg_gain = gains / period;
    double avg_loss = losses / period;
    
    if (avg_loss == 0) return 100.0;
    
    double rs = avg_gain / avg_loss;
    return 100.0 - (100.0 / (1.0 + rs));
}
```

### 3. Trading Strategy Example

**Simple Momentum Strategy:**
```c
typedef enum { NO_SIGNAL, BUY, SELL } Signal;

Signal momentum_strategy(double *prices, int n) {
    double sma_short = calculate_sma(prices, n, 20);
    double sma_long = calculate_sma(prices, n, 50);
    double current_price = prices[n-1];
    
    // Golden Cross: SMA(20) пересекает SMA(50) снизу вверх
    if (sma_short > sma_long && current_price > sma_short) {
        return BUY;
    }
    
    // Death Cross: SMA(20) пересекает SMA(50) сверху вниз
    if (sma_short < sma_long && current_price < sma_short) {
        return SELL;
    }
    
    return NO_SIGNAL;
}
```

### 4. Backtesting Framework

```c
typedef struct {
    double initial_capital;
    double current_capital;
    double shares;
    int trades;
    double max_drawdown;
    double *equity_curve;
} BacktestResult;

BacktestResult backtest_strategy(
    OHLCV *candles, 
    int count,
    Signal (*strategy)(double*, int),
    double commission
) {
    BacktestResult result = {0};
    result.initial_capital = 10000.0;
    result.current_capital = 10000.0;
    
    for (int i = 50; i < count; i++) {
        Signal sig = strategy(/* prices до i */, i);
        
        if (sig == BUY && result.shares == 0) {
            // Покупка
            double cost = candles[i].close * 100;  // 100 акций
            cost += commission;
            
            if (result.current_capital >= cost) {
                result.shares = 100;
                result.current_capital -= cost;
                result.trades++;
            }
        }
        else if (sig == SELL && result.shares > 0) {
            // Продажа
            double revenue = candles[i].close * result.shares;
            revenue -= commission;
            
            result.current_capital += revenue;
            result.shares = 0;
            result.trades++;
        }
        
        // Track equity curve
        double equity = result.current_capital + 
                        (result.shares * candles[i].close);
        // result.equity_curve[i] = equity;
        
        // Calculate drawdown
        // ...
    }
    
    return result;
}
```

---

## 💻 Задания

### Задание 1: Technical Indicators ⭐⭐⭐☆☆

Реализуйте базовые индикаторы:

```c
double calculate_sma(double *prices, int n, int period);
double calculate_ema(double *prices, int n, int period);
double calculate_rsi(double *prices, int n, int period);
void calculate_macd(double *prices, int n, 
                    double *macd, double *signal, double *histogram);
```

### Задание 2: Trading Strategy ⭐⭐⭐⭐☆

Создайте торговую стратегию:

```c
Signal rsi_strategy(double *prices, int n);
Signal crossover_strategy(double *prices, int n);
Signal mean_reversion_strategy(double *prices, int n);
```

### Задание 3: Backtesting Engine ⭐⭐⭐⭐⭐

Постройте backtesting framework:

```c
BacktestResult backtest(
    OHLCV *candles, 
    int count,
    Signal (*strategy)(double*, int),
    double initial_capital,
    double commission
);

void print_backtest_report(BacktestResult *result);
```

**Метрики:**
- Total Return (%)
- Sharpe Ratio
- Max Drawdown
- Win Rate
- Number of Trades

---

## 🎬 Сюжет

> **V.:** "Торговые логи организации показывают использование продвинутых алгоритмов. Реализуй их стратегии. Проверь на исторических данных. Найди слабости."

**Файлы:**
- `strategy_logs.csv` — история сделок организации
- `historical_prices.csv` — исторические данные акций

**Миссия:** Реверс-инжиниринг торговых алгоритмов, поиск уязвимостей.

---

## 🏆 Бонусные задания

### Bonus 1: Machine Learning Signal ⭐⭐⭐⭐⭐
Простая линейная регрессия для предсказания цены.

### Bonus 2: Portfolio Optimization ⭐⭐⭐⭐⭐
Оптимизация весов нескольких стратегий.

### Bonus 3: Walk-Forward Analysis ⭐⭐⭐⭐☆
Скользящее окно для избежания overfitting.

---

## 📊 Что вы узнали

- ✅ Реализация технических индикаторов
- ✅ Создание торговых стратегий
- ✅ Backtesting на исторических данных
- ✅ Анализ производительности стратегий

---

## 🔜 Следующий эпизод

В **Episode 19: Risk & Portfolio Management** вы научитесь:
- Вычислять Value at Risk (VaR)
- Оптимизировать портфель (Markowitz)
- Рассчитывать Sharpe ratio, Sortino ratio
- Управлять рисками в реальном времени

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 18 of 32*

[← Episode 17](../episode-17-market-microstructure/) | [Season 5](../README.md) | [Episode 19 →](../episode-19-risk-portfolio/)

</div>
