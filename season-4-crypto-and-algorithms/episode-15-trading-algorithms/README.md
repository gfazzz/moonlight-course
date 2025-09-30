# Episode 15: Trading Algorithms 📈

> "Markets can remain irrational longer than you can remain solvent." — John Maynard Keynes

## 📋 Описание

Анализ blockchain в Episode 14 выявил подозрительные финансовые транзакции организации на криптовалютных биржах. **V.** передает вам дамп торговых данных. Необходимо написать анализатор для выявления манипуляций рынком.

**В этом эпизоде вы научитесь:**
- Реализовывать order matching engine
- Оптимизировать алгоритмы сортировки для HFT
- Анализировать market data в реальном времени
- Детектировать market manipulation (pump & dump, spoofing)

---

## 🎯 Цели обучения

### Теория (по Шиллеру)
- **Структура рынков:** order books, bid/ask, spread
- **Типы ордеров:** market, limit, stop-loss
- **Market microstructure:** price discovery, liquidity
- **Market manipulation:** wash trading, spoofing, front-running

### Практика
- Реализация order book (priority queue)
- Fast sorting для price-time priority
- Market data analysis (OHLCV)
- Anomaly detection в торговых паттернах

---

## 📖 Теоретическая часть

### 1. Order Book Structure

**Order Book** — список всех buy/sell ордеров:

```
BUY ORDERS (Bids)         SELL ORDERS (Asks)
Price  | Volume          Price  | Volume
-------|--------         -------|--------
$99.50 | 100    ◄─────   $100.50| 50
$99.00 | 200             $101.00| 150
$98.50 | 150             $101.50| 200
       ↑                        ↑
    Best Bid            Best Ask
```

**Spread** = Best Ask - Best Bid = $100.50 - $99.50 = $1.00

### 2. Order Matching Algorithm

```c
typedef struct Order {
    int id;
    char side;      // 'B' = buy, 'S' = sell
    double price;
    int quantity;
    time_t timestamp;
} Order;

// Price-Time Priority:
// 1. Best price first (highest for buy, lowest for sell)
// 2. Earlier timestamp first for same price
```

### 3. Trading Algorithms

**Market Order** — исполняется немедленно по лучшей цене  
**Limit Order** — исполняется только по указанной цене или лучше  
**VWAP** (Volume Weighted Average Price) — средняя цена с учетом объемов

---

## 💻 Задания

### Задание 1: Order Book Implementation ⭐⭐⭐☆☆

Реализуйте order book с сортировкой.

```c
typedef struct OrderBook {
    Order *bids;    // Sorted by price DESC, time ASC
    Order *asks;    // Sorted by price ASC, time ASC
    int bid_count;
    int ask_count;
} OrderBook;
```

### Задание 2: Market Analysis ⭐⭐⭐⭐☆

Найдите аномалии: pump & dump, spoofing.

### Задание 3: VWAP Calculator ⭐⭐⭐⭐⭐

Вычислите VWAP и детектируйте отклонения цены.

---

## 🎬 Сюжет

> **V.:** "Организация манипулирует криптовалютными рынками. Торговые данные указывают на координированную атаку. Найди паттерн."

**Миссия:** Проанализировать торговые данные, найти манипуляции, вычислить профит организации.

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 15 of 32*

[← Episode 14](../episode-14-blockchain-hashes/) | [Season 4](../README.md) | [Episode 16 →](../episode-16-data-structures/)

</div>
