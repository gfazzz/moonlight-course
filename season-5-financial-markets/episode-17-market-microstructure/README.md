# Episode 17: "Market Microstructure" 📊
## Season 5, Episode 1

> *"In markets, the truth is always in the details. The devil — in the spread."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Manhattan, Financial District, Нью-Йорк, США  ║
║  ВРЕМЯ:   19:00 (EST), среда, 18 декабря                ║
║  СТАТУС:  🟡 FINANCIAL DATA INTERCEPTED                  ║
║  GPS:     40.7074°N, 74.0113°W (Wall Street)            ║
╚══════════════════════════════════════════════════════════╝
```

**18 декабря, 18:00. Красная площадь, Москва.**

Встреча с V. завершена. В руках — зашифрованный USB накопитель, тяжелый от информации.

**18:30.** Вы садитесь в такси Uber. Аэропорт Шереметьево. Экстренный рейс в Нью-Йорк.

> *"Анализ нужно проводить там, где всё начинается. На Уолл-стрит. Билет оплачен. Квартира арендована. Координаты отправлю. — V."*

**Рейс Aeroflot SU100:** Москва (SVO) → Нью-Йорк (JFK)  
**Время в пути:** 10 часов  
**Посадка:** 18 декабря, 16:30 EST (01:30 MSK +1 день)

---

**16:45 EST. John F. Kennedy International Airport.**

Таможня пройдена. Yellow Cab везёт вас на Manhattan.

**17:30 EST. Manhattan, Financial District.**

Квартира на 42-м этаже небоскрёба с видом на Уолл-стрит. Огни города. Биржа закрылась час назад (16:00), но город не спит.

**18:00 EST (01:00 MSK). Координаты:** 40.7074°N, 74.0113°W

```
┌─────────────────────────────────────────────┐
│  USB DRIVE: MOONLIGHT_FINANCIAL             │
│  ENCRYPTED: AES-256                         │
│  SIZE: 2.7 GB                               │
│                                             │
│  PASSWORD: "Black-Scholes forever"          │
│  (отсылка к финалу Season 4)               │
└─────────────────────────────────────────────┘
```

**19:00 EST. Квартира на Manhattan.**

За окном — огни Уолл-стрит. Статуя Свободы вдали. Шум города не затихает даже ночью.

Вы подключаете USB. Расшифровка завершена. На экране появляются файлы:

```
📁 MOONLIGHT_FINANCIAL/
├── NYSE_6months.csv           [OHLCV] 892 MB — 6 месяцев торгов
├── NASDAQ_6months.csv         [OHLCV] 1.2 GB — данные NASDAQ  
├── order_book_snapshots.dat   [BINARY] 450 MB — снимки стакана
├── suspicious_tickers.txt     [TEXT] 14 KB — 47 подозрительных акций
└── analysis_notes.txt         [TEXT] "They manipulate. Prove it. — V."
```

Вы открываете `suspicious_tickers.txt`:

```
TICKER,REASON,PRIORITY
ACME,Abnormal spread 0.5% (norm 0.01%),HIGH
BCOM,Volume spike x10 at 23:00-01:00,CRITICAL
DTEK,Order book spoofing detected,HIGH
...
```

**Что это значит:**

Нормальный bid-ask spread на NYSE/NASDAQ — **0.01-0.05%** для ликвидных акций. Здесь — **0.5%** (в 10-50 раз выше!). Это либо ошибка... либо манипуляция.

**V. оставил записку:**

> *"Организация контролирует рынок через HFT боты. Они создают искусственные спреды, манипулируют объемом, спуфят order book. $10 миллионов отмыты за 6 месяцев. Найди доказательства в микроструктуре. Время — 5 часов до смены алгоритмов (00:00). Удачи."*

**Часы показывают 19:00. До 00:00 — 5 часов.**

**Ваша миссия:**
1. Разобраться в OHLCV данных (Open, High, Low, Close, Volume)
2. Проанализировать bid-ask spread (разница между покупкой и продажей)
3. Визуализировать market depth (глубину рынка)
4. Найти доказательства манипуляций

**Начинаем.**

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

## 🎬 DEBRIEFING

**23:47. Анализ завершён.**

На экране — результаты:

```
╔═══════════════════════════════════════════════════════════════════╗
║  MARKET MICROSTRUCTURE ANALYSIS — РЕЗУЛЬТАТЫ                      ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  📊 Проанализировано:                                             ║
║  • 2,847,392 свечей OHLCV за 6 месяцев                           ║
║  • 14,523 снимков order book                                     ║
║  • 47 подозрительных тикеров                                     ║
║                                                                   ║
║  🚨 Аномалии обнаружены:                                          ║
║  • Spread 0.5% (норма 0.01%) — 12 тикеров                        ║
║  • Volume spike x10 в 23:00-01:00 — 8 тикеров                    ║
║  • Order book spoofing — 5 тикеров                               ║
║  • Volatility spike +300% — 3 тикера                             ║
║                                                                   ║
║  💰 Оценка манипуляций:                                           ║
║  • Прибыль от spread manipulation: ~$2.4M                        ║
║  • Профит от spoofing: ~$1.8M                                    ║
║  • ИТОГО за 6 месяцев: $4.2M                                     ║
║                                                                   ║
║  ✅ Доказательства собраны                                        ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

**Ключевые находки:**

1. **ACME Corp** — spread манипуляции  
   - Нормальный spread: 0.01%  
   - Обнаружено: 0.5% (x50!)  
   - HFT боты создают искусственную неликвидность

2. **BCOM Inc** — volume manipulation  
   - Обычный volume: 100K акций/день  
   - В 23:00-01:00: 1M+ акций (x10)  
   - Совпадает с Asian session (низкая ликвидность)

3. **DTEK Ltd** — order book spoofing  
   - Крупные заявки размещаются, затем отменяются  
   - Манипуляция ценой без реальных сделок  
   - Flash crash potential

**V. присылает сообщение:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 23:52                                │
│                                             │
│  Отличная работа. Микроструктура не врёт.   │
│  Но это только верхушка айсберга.           │
│                                             │
│  Следующий шаг: расшифровать их торговые    │
│  алгоритмы. Episode 18 ждёт.                │
│                                             │
│  Спи. Завтра будет тяжелый день.            │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**Время: 00:00. Алгоритмы организации сменились. Но вы успели.**

**Достижение разблокировано:** 🏆 **"Market Cartographer"** — освоена микроструктура рынка

---

**РЕЗУЛЬТАТ EPISODE 17:**
- ✅ OHLCV данные проанализированы
- ✅ Bid-ask spread аномалии найдены
- ✅ Market depth визуализирован
- ✅ $4.2M манипуляций обнаружено
- ✅ Доказательства собраны

**Следующий шаг:** [Episode 18: Trading Algorithms →](../episode-18-trading-algorithms/)

---

## 🥚 Easter Eggs Episode 17

<details>
<summary>🔍 Отсылки и секреты (спойлеры!)</summary>

### Финансовые референсы:
- **Benjamin Graham quote** — учитель Warren Buffett, автор "The Intelligent Investor"
- **Spread 0.5%** — реальный показатель манипуляции (SEC investigation случаи)
- **23:00-01:00 volume** — Asian session, техника реальных манипуляторов
- **Order book spoofing** — техника Navinder Sarao (Flash Crash 2010)
- **HFT bots** — High-Frequency Trading, "Flash Boys" (Michael Lewis)

### Технические детали:
- **NYSE tick size** — $0.01 (penny increment rule с 2001)
- **NASDAQ level 2** — real-time order book data
- **Market depth visualization** — аналог Bloomberg Terminal
- **OHLCV** — стандарт с 1600s (Amsterdam Stock Exchange)
- **Candlestick charts** — изобретены в Японии (1700s, Munehisa Homma)

### Числа:
- **2.7 GB data** — реальный размер 6 месяцев tick-by-tick NYSE
- **$4.2M profit** — средняя сумма market manipulation case (SEC 2018-2020)
- **47 tickers** — референс к "47 ronin" (японская история о верности)
- **19:00 → 00:00** — 5 часов, как в Season 4 Episode 13

### Отсылки к Season 4:
- **"Black-Scholes forever"** — пароль со встречи на Red Square (криптография)
- **Database Engine** — используется для хранения OHLCV
- **Hash tables** — для быстрого поиска тикеров
- **Sorting algorithms** — для сортировки по timestamp

**Философия Episode 17**: *"Markets are conversations. Learn to listen."*

</details>

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

## 🔗 Навигация

- [← Season 4 Finale](../../season-4-crypto-and-algorithms/episode-16-data-structures/)
- [↑ Season 5 Overview](../README.md)
- [→ Episode 18: Trading Algorithms](../episode-18-trading-algorithms/)
- [📋 Общий план курса](../../CURRICULUM.md)
- [🏠 Главная страница](../../README.md)

---

<div align="center">

**🌙 MOONLIGHT OPERATION**  
*Episode 17 of 42 • Season 5, Episode 1*

**"Markets don't lie. People lie. Algorithms don't lie. But they can be manipulated."** — V.

```
// EPISODE 17: MARKET MICROSTRUCTURE
// STATUS: Enhanced (20% → 80%)
// DURATION: 3-4 hours
// ARTIFACTS: market_analyzer
// EVIDENCE: $4.2M manipulations found
```

[← Season 4](../../season-4-crypto-and-algorithms/) | [Season 5](../README.md) | [Episode 18 →](../episode-18-trading-algorithms/)

---

**Episode 17 Complete!** ✅  
OHLCV Analysis → Spread Detection → Market Depth → Manipulation Evidence

**Next:** Episode 18 — Reverse engineering торговых алгоритмов организации

</div>
