# Episode 17: Market Microstructure 📊

> "In the short run, the market is a voting machine, but in the long run, it is a weighing machine." — Benjamin Graham

## 📋 Описание

После завершения Season 4 (криптография + алгоритмы), **V.** передает вам торговые данные с крупнейших бирж NYSE и NASDAQ. Организация проводит подозрительные операции на финансовых рынках. Ваша задача — проанализировать микроструктуру рынка и найти аномалии.

**В этом эпизоде вы научитесь:**
- Парсить OHLCV данные (Open, High, Low, Close, Volume)
- Анализировать bid-ask spread
- Визуализировать market depth (стакан заявок)
- Детектировать манипуляции рынком

---

## 🎯 Цели обучения

### Теория (по Шиллеру)
- **Market Microstructure:** как формируется цена в реальном времени
- **Order Book:** структура биржевого стакана
- **Bid-Ask Spread:** разница между ценой покупки и продажи
- **Liquidity:** глубина рынка, проскальзывание (slippage)
- **Market Data:** tick-by-tick данные, агрегация

### Практика
- Парсинг CSV/JSON рыночных данных
- Построение order book из tick data
- Расчет spread, volume, volatility
- ASCII визуализация market depth

---

## 📖 Теоретическая часть

### 1. OHLCV Data

**OHLCV** — стандартный формат свечей (candlesticks):

```
Timestamp,   Open,  High,  Low,   Close, Volume
2025-09-30, 150.00, 152.50, 149.80, 151.20, 1000000
```

- **Open:** цена открытия периода
- **High:** максимальная цена
- **Low:** минимальная цена  
- **Close:** цена закрытия
- **Volume:** объем торгов

**Парсинг в C:**
```c
typedef struct {
    time_t timestamp;
    double open;
    double high;
    double low;
    double close;
    long volume;
} OHLCV;

int parse_ohlcv(const char *line, OHLCV *candle) {
    return sscanf(line, "%ld,%lf,%lf,%lf,%lf,%ld",
                  &candle->timestamp,
                  &candle->open,
                  &candle->high,
                  &candle->low,
                  &candle->close,
                  &candle->volume);
}
```

### 2. Bid-Ask Spread

**Order Book (стакан):**

```
ASKS (Продажа)          BIDS (Покупка)
Price    Volume          Price    Volume
------   -------         ------   -------
$101.50    500    ←      $100.50   1000
$101.00    300           $100.00   1500
$100.75    200           $99.50     800
```

**Best Bid:** $100.50 (максимальная цена покупки)  
**Best Ask:** $100.75 (минимальная цена продажи)  
**Spread:** $100.75 - $100.50 = **$0.25**

**Процент spread:** (0.25 / 100.625) * 100 = **0.25%**

```c
double calculate_spread(double best_bid, double best_ask) {
    return best_ask - best_bid;
}

double spread_percentage(double best_bid, double best_ask) {
    double mid_price = (best_bid + best_ask) / 2.0;
    return (calculate_spread(best_bid, best_ask) / mid_price) * 100.0;
}
```

### 3. Market Depth (глубина рынка)

**Визуализация стакана:**
```
Depth Level | Buy Volume | Price   | Sell Volume
------------|------------|---------|-------------
     3      |     800    | $99.50  |
     2      |    1500    | $100.00 |
     1      |    1000    | $100.50 | ← Best Bid
     0      |            | ------- |
    -1      |            | $100.75 | ← Best Ask   200
    -2      |            | $101.00 |              300
    -3      |            | $101.50 |              500
```

### 4. Volatility (волатильность)

**Historical Volatility:**
```c
double calculate_volatility(double *returns, int n) {
    double mean = 0.0;
    for (int i = 0; i < n; i++) {
        mean += returns[i];
    }
    mean /= n;
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        variance += (returns[i] - mean) * (returns[i] - mean);
    }
    variance /= (n - 1);
    
    return sqrt(variance);
}
```

---

## 💻 Задания

### Задание 1: OHLCV Parser ⭐⭐☆☆☆

Парсинг CSV файла с рыночными данными:

```c
// TODO: Реализовать
int load_ohlcv_file(const char *filename, OHLCV **candles, int *count);
void print_ohlcv(OHLCV *candle);
void calculate_statistics(OHLCV *candles, int count);
```

**Файл:** `market_data.csv`

### Задание 2: Spread Analyzer ⭐⭐⭐☆☆

Анализ bid-ask spread:

```c
typedef struct {
    double price;
    long volume;
} OrderLevel;

typedef struct {
    OrderLevel *bids;
    OrderLevel *asks;
    int bid_count;
    int ask_count;
} OrderBook;

// TODO: Реализовать
double get_best_bid(OrderBook *book);
double get_best_ask(OrderBook *book);
double calculate_spread(OrderBook *book);
void print_orderbook(OrderBook *book, int depth);
```

### Задание 3: Market Depth Visualization ⭐⭐⭐⭐☆

ASCII визуализация стакана:

```c
// TODO: Реализовать
void visualize_depth(OrderBook *book, int levels);
```

**Вывод:**
```
Market Depth: AAPL
════════════════════════════════════════
BIDS                    |  ASKS
Volume    Price         |  Price         Volume
800       $99.50  ████  |
1500      $100.00 ██████|
1000      $100.50 █████ | ← Best Bid
                        |
                        | ← Best Ask  $100.75 ███   200
                        |             $101.00 ████  300
                        |             $101.50 █████ 500
════════════════════════════════════════
Spread: $0.25 (0.25%)
```

### Задание 4: Anomaly Detection ⭐⭐⭐⭐⭐

Детектирование подозрительных паттернов:

```c
// TODO: Реализовать
bool detect_pump_and_dump(OHLCV *candles, int count);
bool detect_wash_trading(OHLCV *candles, int count);
bool detect_spoofing(OrderBook *book);
```

**Критерии:**
- Pump & Dump: резкий рост объема + цены, затем обвал
- Wash Trading: одинаковые объемы покупки/продажи
- Spoofing: большие заявки, которые отменяются

---

## 🎬 Сюжет

**Дата:** 18 декабря, 10:00  
**Локация:** Штаб MOONLIGHT

После построения криптографической инфраструктуры в Season 4, **V.** раскрывает новую цель:

> **V.:** "Организация манипулирует фондовым рынком. У нас есть торговые данные за последние 6 месяцев. Найди паттерны — pump & dump, wash trading, spoofing. Докажи манипуляции."

**Данные:**
- `market_data.csv` — OHLCV данные за 6 месяцев
- `order_book_snapshot.dat` — снимок стакана заявок
- `suspicious_tickers.txt` — список подозрительных тикеров

**Миссия:** Проанализировать микроструктуру рынка, найти доказательства манипуляций.

---

## 🎓 Дополнительные материалы

### Курсы (основа — Шиллер)
- [Yale: Financial Markets (Robert Shiller)](https://www.coursera.org/learn/financial-markets-global)
- [MIT: Trading and Market Microstructure](https://ocw.mit.edu/)

### Книги
- **"Market Microstructure Theory"** — Maureen O'Hara
- **"Trading and Exchanges"** — Larry Harris
- **"Flash Boys"** — Michael Lewis (про HFT)

### Источники данных
- [Yahoo Finance API](https://finance.yahoo.com/)
- [Alpha Vantage](https://www.alphavantage.co/)
- [IEX Cloud](https://iexcloud.io/)

---

## 🏆 Бонусные задания

### Bonus 1: Real-time Feed Parser ⭐⭐⭐⭐⭐
Парсинг WebSocket потока (симуляция) для real-time данных.

### Bonus 2: VWAP Calculator ⭐⭐⭐⭐☆
Volume Weighted Average Price — средняя цена с учетом объемов.

### Bonus 3: Candlestick Chart ⭐⭐⭐☆☆
ASCII визуализация японских свечей.

---

## 📊 Что вы узнали

После Episode 17 вы умеете:

- ✅ Парсить и анализировать OHLCV данные
- ✅ Работать с order book (стакан заявок)
- ✅ Рассчитывать spread и volatility
- ✅ Визуализировать market depth
- ✅ Детектировать манипуляции рынком

---

## 🔜 Следующий эпизод

В **Episode 18: Trading Algorithms** вы научитесь:
- Реализовывать технические индикаторы (SMA, RSI, MACD)
- Создавать торговые стратегии
- Backtesting engine для проверки стратегий
- Order matching engine (HFT basics)

Найденные манипуляции указывают на сложные торговые алгоритмы организации...

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 17 of 32*

[← Season 4](../../season-4-crypto-and-algorithms/) | [Season 5](../README.md) | [Episode 18 →](../episode-18-trading-algorithms/)

</div>
