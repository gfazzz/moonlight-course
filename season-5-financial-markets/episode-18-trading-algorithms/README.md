# Episode 18: "Trading Algorithms" 📈
## Season 5, Episode 2

> *"Algorithms don't sleep. They don't fear. They don't hope. They execute."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Manhattan, Midtown, Нью-Йорк, США             ║
║  ВРЕМЯ:   08:30 (EST), четверг, 19 декабря              ║
║  СТАТУС:  🔴 ALGORITHM REVERSE ENGINEERING REQUIRED      ║
║  GPS:     40.7589°N, 73.9851°W (Times Square area)      ║
╚══════════════════════════════════════════════════════════╝
```

**19 декабря, 08:30 EST. Утро после Episode 17.**

За окном — утренний Manhattan. Биржевой звонок открытия NYSE прозвучал 30 минут назад (09:30 EST). Город уже в полном движении.

Вы просыпаетесь от уведомления на телефоне. V. не дает передохнуть.

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 08:27:14                             │
│  PRIORITY: URGENT                           │
├─────────────────────────────────────────────┤
│                                             │
│  Микроструктура — только симптом.           │
│  Настоящая угроза — их алгоритмы.           │
│                                             │
│  Мы перехватили логи торговой системы       │
│  организации. 6 месяцев, 47,392 сделки.     │
│                                             │
│  ЗАДАЧА:                                    │
│  1. Реверс-инжиниринг стратегий             │
│  2. Backtesting на исторических данных      │
│  3. Найти слабости в алгоритмах             │
│                                             │
│  Данные в твоей почте. Дедлайн: 14:00.      │
│  У них смена серверов. После — поздно.      │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**08:30. Проверяете почту.**

Зашифрованный архив. Пароль: `GoldenCross1974` (год изобретения SMA стратегий).

```
📁 TRADING_ALGORITHMS/
├── strategy_logs.csv         [TRADES] 47,392 сделки за 6 месяцев
├── historical_prices.csv     [OHLCV] те же 47 тикеров из Ep17
├── algorithm_fingerprints.txt [PATTERNS] подозрительные паттерны
└── notes_v.txt               "127% годовых. Слишком хорошо. Найди как."
```

Вы открываете `algorithm_fingerprints.txt`:

```
PATTERN ANALYSIS - ORGANIZATION STRATEGIES
===========================================

STRATEGY 1 (42% trades):
- SMA(20) crossover SMA(50) 
- Entry: Golden Cross
- Exit: Death Cross
- Win Rate: 68%
- Avg Return: +1.2% per trade

STRATEGY 2 (31% trades):
- RSI oversold/overbought
- Entry: RSI < 30
- Exit: RSI > 70
- Win Rate: 61%
- Avg Return: +0.8% per trade

STRATEGY 3 (27% trades):
- MACD crossover + Volume spike
- Entry: MACD crosses signal line + Volume > 2x avg
- Exit: MACD reversal
- Win Rate: 73%
- Avg Return: +1.5% per trade

COMBINED RETURN: 127.4% annualized
WARNING: Too good to be true? Check for overfitting.
```

**127% годовых?!**

Для сравнения:
- Warren Buffett: ~20% годовых (long-term)
- S&P 500 index: ~10% годовых
- Renaissance Technologies Medallion Fund: ~66% (лучший hedge fund)

**127% — это либо гений... либо мошенничество.**

**V. прав. Нужно проверить стратегии на реальных исторических данных.**

**Часы показывают 08:30. До 14:00 — 5.5 часов.**

**Ваша миссия:**
1. Реализовать технические индикаторы (SMA, EMA, RSI, MACD)
2. Воспроизвести 3 стратегии организации
3. Провести backtesting на исторических данных
4. Найти слабости (overfitting? curve fitting? survivorship bias?)

**Начинаем реверс-инжиниринг.**

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

## 🎬 DEBRIEFING

**14:15. Backtesting завершён.**

На экране — шокирующие результаты:

```
╔═══════════════════════════════════════════════════════════════════╗
║  BACKTESTING RESULTS — ИСТИННАЯ ПРОИЗВОДИТЕЛЬНОСТЬ                ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  📊 Протестировано:                                               ║
║  • 3 стратегии организации                                        ║
║  • 47,392 сделки за 6 месяцев                                    ║
║  • 47 тикеров (те же из Episode 17)                              ║
║  • Out-of-sample период: последние 3 месяца                      ║
║                                                                   ║
║  ⚠️ ЗАЯВЛЕННАЯ производительность (организация):                  ║
║  • Combined Return: 127.4% годовых                               ║
║  • Win Rate: 68%                                                 ║
║  • Sharpe Ratio: 2.8                                             ║
║                                                                   ║
║  🔍 РЕАЛЬНАЯ производительность (ваш backtesting):                ║
║  • In-sample (первые 3 мес): 124.7% (почти сошлось!)             ║
║  • Out-of-sample (вторые 3 мес): 18.3% (РЕЗКОЕ падение!)         ║
║  • Win Rate реальный: 52% (вместо 68%)                           ║
║  • Sharpe Ratio: 0.9 (вместо 2.8)                                ║
║  • Max Drawdown: -34% (катастрофа!)                              ║
║                                                                   ║
║  🚨 ДИАГНОЗ: OVERFITTING + CURVE FITTING                          ║
║                                                                   ║
║  Стратегии оптимизированы под исторические данные.                ║
║  На новых данных — провал. Классическое мошенничество.            ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

**Что вы обнаружили:**

1. **Overfitting (переобучение)**
   - Параметры подобраны идеально под прошлые данные
   - SMA(20) и SMA(50) — оптимизированы именно для этих 6 месяцев
   - На новых данных — не работают

2. **Survivorship Bias (искажение выборки)**
   - Выбраны только успешные тикеры
   - Убыточные акции исключены из анализа
   - Реальная доходность гораздо ниже

3. **Flash Crash Vulnerability**
   - Стратегии уязвимы к резким движениям
   - Max Drawdown -34% — критично
   - Risk management отсутствует

**14:12. Сообщение от V.:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED)                       │
│  TIME: 14:12:47                             │
│                                             │
│  Отлично. Они обманывают инвесторов.        │
│  127% — липа. Реально 18% (и то спорно).    │
│                                             │
│  Но это ещё не всё. Их портфель — $50M.     │
│  При таком риске (34% drawdown) они могут   │
│  потерять $17M за один плохой месяц.        │
│                                             │
│  Episode 19: проанализируй риски портфеля.  │
│  Завтра, 18:45. Отдохни.                    │
│                                             │
│  P.S. Хорошая работа с backtesting.         │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**14:15. Дедлайн выполнен.**

**Достижение разблокировано:** 🏆 **"Algorithm Engineer"** — торговые стратегии расшифрованы

---

**РЕЗУЛЬТАТ EPISODE 18:**
- ✅ 4 технических индикатора реализованы (SMA, EMA, RSI, MACD)
- ✅ 3 стратегии организации воспроизведены
- ✅ Backtesting framework построен
- ✅ Overfitting обнаружен (127% → 18%)
- ✅ Уязвимость к flash crashes найдена
- ✅ Доказательства мошенничества собраны

**Следующий шаг:** [Episode 19: Risk & Portfolio Management →](../episode-19-risk-portfolio/)

---

## 🥚 Easter Eggs Episode 18

<details>
<summary>🔍 Отсылки и секреты (спойлеры!)</summary>

### Торговые стратегии:
- **Golden Cross** — SMA(50/200) классика с 1974 (Richard Donchian)
- **Death Cross** — противоположность Golden Cross, медвежий сигнал
- **GoldenCross1974** — пароль, год появления стратегии
- **RSI 30/70** — границы J. Welles Wilder Jr. (1978, "New Concepts in Technical Trading Systems")
- **MACD** — Gerald Appel (1979), популярный индикатор momentum

### Числа производительности:
- **127% годовых** — отсылка к Bernie Madoff (обещал 120% стабильно)
- **Warren Buffett 20%** — реальная средняя доходность Berkshire Hathaway
- **Renaissance Medallion 66%** — легендарный hedge fund (James Simons)
- **S&P 500 10%** — исторический average return (1926-2020)
- **Max Drawdown -34%** — сравним с 2008 кризисом (-37% S&P 500)

### Backtesting концепции:
- **In-sample/Out-of-sample** — золотой стандарт валидации
- **Overfitting** — №1 проблема алгоритмического трейдинга
- **Survivorship Bias** — ошибка в выборке (только успешные компании)
- **Walk-forward analysis** — техника из "Evidence-Based Technical Analysis" (David Aronson)

### Технические детали:
- **47,392 сделки** — референс к Episode 17 (47 tickers)
- **Sharpe Ratio 2.8 → 0.9** — типичное падение при overfitting
- **Win Rate 68% → 52%** — random walk (50%) vs overfit
- **08:30 → 14:00** — 5.5 часов, типичный trading day (9:30-16:00 NYSE)

### Отсылки к курсу:
- **Backtesting framework** — использует sorting из Season 4
- **Signal generation** — применяет enum и struct
- **Performance metrics** — статистика из Season 4 Episode 15

**Философия Episode 18**: *"Past performance is not indicative of future results."* — SEC disclaimer

</details>

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

## 🔗 Навигация

- [← Episode 17: Market Microstructure](../episode-17-market-microstructure/)
- [↑ Season 5 Overview](../README.md)
- [→ Episode 19: Risk & Portfolio](../episode-19-risk-portfolio/)
- [📋 Общий план курса](../../CURRICULUM.md)
- [🏠 Главная страница](../../README.md)

---

<div align="center">

**🌙 MOONLIGHT OPERATION**  
*Episode 18 of 42 • Season 5, Episode 2*

**"The market can remain irrational longer than you can remain solvent."** — John Maynard Keynes

```
// EPISODE 18: TRADING ALGORITHMS
// STATUS: Enhanced (20% → 80%)
// DURATION: 4-5 hours  
// ARTIFACTS: strategy_tester
// FINDINGS: Overfitting detected (127% → 18%)
```

[← Episode 17](../episode-17-market-microstructure/) | [Season 5](../README.md) | [Episode 19 →](../episode-19-risk-portfolio/)

---

**Episode 18 Complete!** ✅  
SMA/RSI/MACD → Strategy Reverse Engineering → Backtesting → Overfitting Exposed

**Next:** Episode 19 — Анализ рисков портфеля $50M

</div>
