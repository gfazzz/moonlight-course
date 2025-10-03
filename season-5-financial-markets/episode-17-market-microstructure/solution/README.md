# Episode 17: Market Microstructure - SOLUTION

## 📖 О решении

Это **полное решение** Episode 17 с реализацией всех требуемых функций для анализа микроструктуры рынка.

**Версия:** v2.0 Enhanced Edition

## ✅ Реализованные функции

### 1. OHLCV Parser
- ✅ Парсинг CSV с поддержкой заголовков
- ✅ Валидация данных (High >= Low, корректные значения)
- ✅ Динамическое выделение памяти для candles
- ✅ Поддержка множества ticker'ов
- ✅ Обработка ошибок и предупреждений

### 2. Market Metrics
- ✅ Bid-Ask Spread: `(High - Low) / Close * 100%`
- ✅ Average Volume per ticker
- ✅ Volatility: стандартное отклонение returns
- ✅ Daily return: `(Close - Open) / Open`
- ✅ Price range (min/max)

### 3. Anomaly Detection
- ✅ Volume Spikes (>3x average)
- ✅ Price Jumps (>5% in single candle)
- ✅ Spread Anomalies (>2x average)
- ✅ Anomaly scoring system
- ✅ Suspicion level classification

### 4. Reporting
- ✅ Per-ticker statistics
- ✅ Comprehensive manipulation analysis
- ✅ Top 5 suspicious tickers
- ✅ Estimated manipulation profit
- ✅ Beautiful ASCII formatting

## 🛠️ Компиляция

```bash
make
```

Или вручную:
```bash
gcc -Wall -Wextra -std=c11 -pedantic -O2 -o market_microstructure market_microstructure.c -lm
```

## 🚀 Запуск

```bash
# С тестовыми данными
make run

# Или напрямую
./market_microstructure ../artifacts/market_data.csv

# Запустить и протестировать
make test
```

## 📊 Формат входных данных

CSV файл с заголовком:
```
TICKER,TIMESTAMP,OPEN,HIGH,LOW,CLOSE,VOLUME
ACME,2024-12-01T09:30,150.00,152.50,149.80,151.20,1000000
```

**Поля:**
- `TICKER` - символ акции (строка, до 15 символов)
- `TIMESTAMP` - время (строка, до 31 символа)
- `OPEN` - цена открытия (double)
- `HIGH` - максимальная цена (double)
- `LOW` - минимальная цена (double)
- `CLOSE` - цена закрытия (double)
- `VOLUME` - объем торгов (long)

## 📈 Пример вывода

```
╔═══════════════════════════════════════════════════════════════════╗
║         Episode 17: Market Microstructure Analysis               ║
║                   Operation MOONLIGHT - Season 5                  ║
╚═══════════════════════════════════════════════════════════════════╝

📂 Loading market data from: ../artifacts/market_data.csv
✅ Loaded 7 tickers

=== ACME Statistics ===
Candles: 10
Avg Volume: 2860000
Avg Spread: 1.8503%
Volatility: 0.0079 (0.79%)

🚨 Anomalies:
  Volume Spikes (>3.0x avg): 1
  Price Jumps (>5%): 0
  Spread Anomalies (>2.0x avg): 0
  Total Anomaly Score: 1.0
  ✅ NORMAL - No significant anomalies

📊 Price Range: $148.00 - $155.00 (4.73% range)

...

╔═══════════════════════════════════════════════════════════════════╗
║           MARKET MANIPULATION ANALYSIS - FINAL REPORT             ║
╚═══════════════════════════════════════════════════════════════════╝

💰 Estimated Manipulation Profit:
  Conservative estimate: $0.3M

🎯 Top 5 Most Suspicious Tickers:
  1. DTEK       - Score: 2.0 (V:0, P:0, S:1)
  ...
```

## 🔍 Алгоритмы

### Volume Spike Detection
```c
double avg_volume = calculate_avg_volume(candles, count);
if (current_volume > avg_volume * 3.0) {
    // Volume spike detected
}
```

### Volatility Calculation
```c
// Calculate daily returns
returns[i] = (close[i] - close[i-1]) / close[i-1];

// Standard deviation
variance = Σ(returns[i] - mean)² / (n-1)
volatility = sqrt(variance)
```

### Anomaly Scoring
```
score = volume_spikes × 1.0
      + price_jumps × 1.5
      + spread_anomalies × 2.0
```

**Suspicion Levels:**
- `score > 10` → HIGH Suspicion (likely manipulation)
- `score > 5` → MODERATE Suspicion (further investigation)
- `score ≤ 5` → NORMAL (no significant anomalies)

## 🎓 Учебные цели

Это решение демонстрирует:

1. **Парсинг CSV** — работа с файлами, обработка ошибок
2. **Динамическая память** — malloc/realloc/free для candles
3. **Статистика** — среднее, дисперсия, стандартное отклонение
4. **Алгоритмы** — детектирование аномалий, сортировка
5. **Форматирование вывода** — красивые ASCII таблицы

## 🏆 Achievement

После успешного запуска разблокируется:

**"Market Cartographer"** — You've mastered market microstructure analysis.

## ➡️ Что дальше?

Episode 18: Trading Algorithms
- Реализация SMA, RSI, MACD
- Backtesting engine
- Strategy optimization

---

**Operation MOONLIGHT** — Season 5, Episode 17  
v2.0 Enhanced Edition

