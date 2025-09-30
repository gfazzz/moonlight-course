# Episode 20: Crypto Trading & DeFi 🪙

> "Bitcoin is a remarkable cryptographic achievement... The ability to create something which is not duplicable in the digital world has enormous value." — Eric Schmidt

## 📋 Описание

Финальный эпизод Season 5! После анализа традиционных финансовых рынков (Episodes 17-19), **V.** раскрывает главную схему организации — манипуляции на криптовалютных биржах и использование DeFi протоколов для отмывания средств.

**В этом эпизоде вы научитесь:**
- Анализировать криптовалютные рынки (Bitcoin, Ethereum)
- Понимать основы DeFi (Decentralized Finance)
- Находить арбитражные возможности между биржами
- Детектировать манипуляции на crypto exchanges

---

## 🎯 Цели обучения

### Теория
- **Cryptocurrency Markets:** Bitcoin, Ethereum, altcoins, market cap
- **DeFi Basics:** liquidity pools, AMM (Automated Market Maker), yield farming
- **Arbitrage:** межбиржевой арбитраж, triangular arbitrage
- **On-chain Analysis:** blockchain explorer, transaction tracking

### Практика
- Парсинг данных с crypto exchanges (REST API симуляция)
- Расчет арбитражных возможностей
- Анализ liquidity pools
- Детектирование pump & dump на altcoins

---

## 📖 Теоретическая часть

### 1. Cryptocurrency Markets

**Market Structure:**
```
Market Cap = Price × Circulating Supply

Bitcoin (BTC):   $50,000 × 19M = $950B market cap
Ethereum (ETH):  $3,000 × 120M = $360B market cap
```

**Key Differences from Traditional Markets:**
- 24/7 trading (no market close)
- High volatility
- Lower liquidity (особенно altcoins)
- Multiple exchanges with different prices

**Price Data Structure:**
```c
typedef struct {
    char symbol[10];        // "BTCUSDT"
    double price;
    double volume_24h;
    double market_cap;
    time_t timestamp;
} CryptoTicker;

int parse_ticker(const char *json, CryptoTicker *ticker) {
    // Parse JSON response from exchange API
    // {"symbol":"BTCUSDT","price":"50000.00","volume":"1000000000"}
    return 0;
}
```

### 2. Arbitrage Opportunities

**Межбиржевой арбитраж:**
```
Binance:  BTC = $50,000
Coinbase: BTC = $50,200
Kraken:   BTC = $49,900

Возможность:
1. Купить на Kraken ($49,900)
2. Продать на Coinbase ($50,200)
3. Профит: $300 - fees
```

**Расчет:**
```c
typedef struct {
    char exchange_buy[20];
    char exchange_sell[20];
    double buy_price;
    double sell_price;
    double profit_percentage;
} ArbitrageOpportunity;

ArbitrageOpportunity find_arbitrage(
    CryptoTicker *tickers, 
    int n_exchanges,
    const char *symbol
) {
    ArbitrageOpportunity arb = {0};
    double min_price = DBL_MAX;
    double max_price = 0.0;
    int buy_idx = -1, sell_idx = -1;
    
    for (int i = 0; i < n_exchanges; i++) {
        if (strcmp(tickers[i].symbol, symbol) != 0) continue;
        
        if (tickers[i].price < min_price) {
            min_price = tickers[i].price;
            buy_idx = i;
        }
        if (tickers[i].price > max_price) {
            max_price = tickers[i].price;
            sell_idx = i;
        }
    }
    
    if (buy_idx >= 0 && sell_idx >= 0) {
        arb.buy_price = min_price;
        arb.sell_price = max_price;
        arb.profit_percentage = ((max_price - min_price) / min_price) * 100.0;
    }
    
    return arb;
}
```

### 3. DeFi Basics (Conceptual)

**Liquidity Pool** — пул монет для торговли без order book:

```
Pool: ETH/USDT
ETH reserve: 100 ETH
USDT reserve: 300,000 USDT

Price = USDT_reserve / ETH_reserve = 3000 USDT/ETH
```

**Constant Product Formula (Uniswap):**
```
x * y = k (константа)

При обмене:
(x + Δx) * (y - Δy) = k

Где:
Δx - сколько ETH добавили
Δy - сколько USDT получили
```

**Упрощенный расчет:**
```c
typedef struct {
    double reserve_a;  // ETH
    double reserve_b;  // USDT
    double k;          // Constant product
} LiquidityPool;

double calculate_swap_output(LiquidityPool *pool, double input_amount) {
    // x * y = k
    // (x + dx) * (y - dy) = k
    // dy = y - k / (x + dx)
    
    double new_reserve_a = pool->reserve_a + input_amount;
    double new_reserve_b = pool->k / new_reserve_a;
    double output_amount = pool->reserve_b - new_reserve_b;
    
    return output_amount * 0.997;  // 0.3% fee
}

double get_price(LiquidityPool *pool) {
    return pool->reserve_b / pool->reserve_a;
}
```

### 4. Pump & Dump Detection

**Характеристики Pump & Dump:**
1. Резкий рост объема (+500% за час)
2. Быстрый рост цены (+50-100%)
3. Обвал после пика (-80%)
4. Low market cap coins (< $10M)

```c
bool detect_pump_and_dump(CryptoTicker *history, int n) {
    if (n < 24) return false;  // Минимум 24 часа данных
    
    // Проверка роста объема
    double avg_volume = 0.0;
    for (int i = 0; i < n - 1; i++) {
        avg_volume += history[i].volume_24h;
    }
    avg_volume /= (n - 1);
    
    double current_volume = history[n-1].volume_24h;
    if (current_volume < avg_volume * 5.0) {
        return false;  // Объем не вырос минимум в 5 раз
    }
    
    // Проверка роста цены
    double price_change = (history[n-1].price - history[n-24].price) / 
                          history[n-24].price * 100.0;
    
    if (price_change > 50.0 && current_volume > avg_volume * 5.0) {
        return true;  // Вероятный pump
    }
    
    return false;
}
```

---

## 💻 Задания

### Задание 1: Crypto Price Tracker ⭐⭐☆☆☆

Парсинг и отслеживание цен:

```c
int fetch_ticker(const char *exchange, const char *symbol, CryptoTicker *ticker);
void track_prices(const char *symbol, int interval_seconds);
void print_price_table(CryptoTicker *tickers, int n);
```

### Задание 2: Arbitrage Finder ⭐⭐⭐⭐☆

Поиск арбитражных возможностей:

```c
ArbitrageOpportunity find_best_arbitrage(
    CryptoTicker **exchanges,
    int n_exchanges,
    const char **symbols,
    int n_symbols
);

void print_arbitrage_opportunities(ArbitrageOpportunity *opps, int n);
```

### Задание 3: DeFi Pool Analyzer ⭐⭐⭐⭐⭐

Анализ liquidity pools:

```c
double calculate_impermanent_loss(
    double initial_price,
    double current_price
);

double estimate_apr(LiquidityPool *pool, double trading_volume_24h);
```

### Задание 4: Pump & Dump Detector ⭐⭐⭐⭐⭐

Детектирование манипуляций:

```c
bool detect_pump_and_dump(CryptoTicker *history, int n);
bool detect_wash_trading(CryptoTicker *history, int n);
void generate_suspicious_coins_report(CryptoTicker **coins, int n);
```

---

## 🎬 Сюжет — Финал Season 5

**Дата:** 20 декабря, 02:30  
**Локация:** Подпольный дата-центр

> **V.:** "Мы нашли их. Организация использует сеть ботов для pump & dump на низколиквидных altcoins. Затем отмывают через DeFi mixer. Общая сумма — $10 миллионов. Собери доказательства."

**Данные:**
- `suspicious_wallets.txt` — адреса кошельков
- `pump_history.csv` — история pump & dump
- `defi_transactions.json` — DeFi транзакции
- `arbitrage_logs.csv` — межбиржевой арбитраж

**Миссия:** Раскрыть криптовалютную схему, собрать доказательства для властей.

**Финал:** Вы находите доказательства манипуляций на $10M+. Схема раскрыта. Но организация узнает о расследовании и начинает контратаку через IoT устройства (переход к Season 6)...

---

## 🎓 Дополнительные материалы

### Cryptocurrency
- [Bitcoin Whitepaper](https://bitcoin.org/bitcoin.pdf) — Satoshi Nakamoto
- [Ethereum Whitepaper](https://ethereum.org/en/whitepaper/)
- [Mastering Bitcoin](https://github.com/bitcoinbook/bitcoinbook) — Andreas Antonopoulos

### DeFi
- [Uniswap V2 Documentation](https://docs.uniswap.org/)
- [DeFi Pulse](https://defipulse.com/) — DeFi analytics
- [How to DeFi](https://landing.coingecko.com/how-to-defi/) — beginner guide

### APIs (для практики)
- [CoinGecko API](https://www.coingecko.com/en/api)
- [CryptoCompare API](https://min-api.cryptocompare.com/)
- [Binance API](https://binance-docs.github.io/apidocs/)

---

## 🏆 Бонусные задания

### Bonus 1: Triangular Arbitrage ⭐⭐⭐⭐⭐
BTC→ETH→USDT→BTC — найти прибыльный треугольник.

### Bonus 2: On-Chain Analysis ⭐⭐⭐⭐⭐
Парсинг blockchain (симуляция) для отслеживания крупных транзакций (whales).

### Bonus 3: MEV Bot (Conceptual) ⭐⭐⭐⭐⭐
Maximal Extractable Value — front-running detection.

---

## 📊 Что вы узнали

После Episode 20 вы умеете:

- ✅ Анализировать криптовалютные рынки
- ✅ Находить арбитражные возможности
- ✅ Понимать основы DeFi (liquidity pools, AMM)
- ✅ Детектировать pump & dump и wash trading
- ✅ Применять blockchain концепции из Season 4

---

## 🎯 Milestone: Season 5 Complete!

**Достижения Season 5:**
- ✅ Market Microstructure освоена
- ✅ Trading Algorithms реализованы
- ✅ Risk Management построен
- ✅ Crypto схема организации раскрыта

**Награда:** Доказательства финансовых преступлений на $10M+! 🏆

---

## 🔜 Следующий сезон

**Season 6: Embedded & IoT** ждет вас!

После раскрытия финансовой схемы организация переходит к контратаке. Они используют IoT устройства для слежки и атак. Необходимо взломать систему безопасности НИИ, где хранятся критические данные...

- Episode 21: Arduino & GPIO Fundamentals
- Episode 22: Industrial Protocols & Access Control
- Episode 23: IP Cameras & Video Surveillance
- Episode 24: IoT Integration & System Bypass

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 20 of 32 — Season 5 Finale*

[← Episode 19](../episode-19-risk-portfolio/) | [Season 5](../README.md) | [Season 6 →](../../season-6-embedded-iot/)

**🎉 SEASON 5 COMPLETE! 🎉**

</div>
