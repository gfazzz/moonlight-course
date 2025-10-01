# Season 5: Financial Markets 💰

> *"Follow the money. It never lies."*

```
╔══════════════════════════════════════════════════════════╗
║                 OPERATION MOONLIGHT                      ║
║                    SEASON 5 / 10                         ║
║                                                          ║
║  СТАТУС:  🟢 АКТИВНА                                     ║
║  ТЕМА:    Financial Markets & Trading                   ║
║  УРОВЕНЬ: Advanced                                       ║
╚══════════════════════════════════════════════════════════╝
```

---

## 📖 О сезоне

**"Деньги не спят. Алгоритмы не лгут. Рынки помнят всё."**

**Season 5: Financial Markets** основан на знаменитом курсе Yale University "Financial Markets" профессора **Robert Shiller** (Нобелевская премия по экономике 2013).

После встречи с V. на Красной площади (Season 4 finale), вы получаете новое задание: **проанализировать финансовые потоки операции MOONLIGHT**. Кто-то манипулирует рынками, отмывает миллионы через биржи и DeFi. Ваша задача — найти доказательства.

### 🎬 Сюжет сезона

**18 декабря, 18:00. Красная площадь, Москва.**

V. передаёт вам зашифрованный USB:

> *"Настоящая работа начинается. Blockchain — это только начало. Организация манипулирует финансовыми рынками. NYSE, NASDAQ, Binance — везде их следы. $10 миллионов отмыты за 6 месяцев. Докажи это."*

**Временная линия Season 5:**
```
18 ДЕК, 19:00 → Анализ микроструктуры рынка (Episode 17)
19 ДЕК, 08:30 → Реверс торговых алгоритмов (Episode 18)
19 ДЕК, 18:45 → Оценка рисков портфеля (Episode 19)
20 ДЕК, 02:30 → Раскрытие crypto схемы (Episode 20 — ФИНАЛ)
```

---

## 🎯 Цели сезона

После прохождения Season 5 вы сможете:

### Финансовые концепции (по Шиллеру)
- ✅ **Market Microstructure:** order books, bid-ask spread, market depth, liquidity
- ✅ **Trading Strategies:** technical indicators (SMA, RSI, MACD), backtesting, signal generation
- ✅ **Risk Management:** VaR, Sharpe ratio, portfolio optimization, correlation analysis
- ✅ **Crypto Trading:** DeFi, liquidity pools, arbitrage detection, pump & dump

### Технические навыки
- ✅ Парсинг и анализ рыночных данных (OHLCV)
- ✅ Реализация торговых алгоритмов на C
- ✅ Статистический анализ и Monte Carlo симуляции
- ✅ Real-time data processing и order matching
- ✅ Детектирование манипуляций (wash trading, spoofing, pump & dump)

---

## 📚 Эпизоды

### [Episode 17: "Market Microstructure"](episode-17-market-microstructure/) 📊
```
┌─────────────────────────────────────────────┐
│  📅 18 ДЕК, 19:00 → 23:47                   │
│  🎯 Анализ микроструктуры рынка             │
│  ⏱️  ~3-4 часа                               │
│  🔥 Сложность: ⭐⭐⭐☆☆                      │
└─────────────────────────────────────────────┘
```

**Сюжет:**  
После встречи с V. вы получаете торговые данные NYSE/NASDAQ за 6 месяцев. В данных скрыты следы манипуляций — аномальные спреды, подозрительный объем, необычные паттерны в order book.

**Что изучите:**
- ✅ OHLCV данные и японские свечи
- ✅ Order book (bid-ask spread, market depth)
- ✅ Volatility расчет и детектирование аномалий
- ✅ Визуализация стакана заявок (ASCII)

**Артефакт:** `market_analyzer` — анализатор микроструктуры  
**Достижение:** 🏆 **"Market Cartographer"**  
**Время прохождения:** ~3-4 часа

---

### [Episode 18: "Trading Algorithms"](episode-18-trading-strategies/) 📈
```
┌─────────────────────────────────────────────┐
│  📅 19 ДЕК, 08:30 → 14:15                   │
│  🎯 Реверс-инжиниринг алгоритмов            │
│  ⏱️  ~4-5 часов                              │
│  🔥 Сложность: ⭐⭐⭐⭐☆                     │
└─────────────────────────────────────────────┘
```

**Сюжет:**  
Обнаружены автоматизированные торговые стратегии организации. Нужно воспроизвести их логику: реализовать технические индикаторы (SMA, RSI, MACD), создать backtesting framework и найти слабости в стратегиях.

**Что изучите:**
- ✅ Moving averages (SMA, EMA, WMA)
- ✅ Momentum indicators (RSI, MACD, Stochastic)
- ✅ Backtesting engine с историческими данными
- ✅ Signal generation и order execution logic

**Артефакт:** `strategy_tester` — система бэктестинга  
**Достижение:** 🏆 **"Algorithm Engineer"**  
**Время прохождения:** ~4-5 часов

---

### [Episode 19: "Risk & Portfolio"](episode-19-risk-portfolio/) 💼
```
┌─────────────────────────────────────────────┐
│  📅 19 ДЕК, 18:45 → 23:30                   │
│  🎯 Оценка рисков портфеля                  │
│  ⏱️  ~4-5 часов                              │
│  🔥 Сложность: ⭐⭐⭐⭐⭐                    │
└─────────────────────────────────────────────┘
```

**Сюжет:**  
Раскрыт портфель организации: $50M в акциях и криптовалютах. Риски зашкаливают — нужен профессиональный анализ. Реализуйте VaR через Monte Carlo, постройте эффективную границу Марковица, рассчитайте Sharpe ratio.

**Что изучите:**
- ✅ Value at Risk (VaR) — Monte Carlo симуляция
- ✅ Sharpe ratio и Sortino ratio
- ✅ Portfolio optimization (Markowitz)
- ✅ Correlation matrix и диверсификация

**Артефакт:** `portfolio_manager` — система управления рисками  
**Достижение:** 🏆 **"Risk Master"**  
**Время прохождения:** ~4-5 часов

---

### [Episode 20: "Crypto Trading & DeFi"](episode-20-crypto-defi/) 🪙 — **ФИНАЛ**
```
┌─────────────────────────────────────────────┐
│  📅 20 ДЕК, 02:30 → 06:47                   │
│  🎯 Раскрытие crypto схемы                  │
│  ⏱️  ~4 часа                                 │
│  🔥 Сложность: ⭐⭐⭐⭐⭐ (FINALE)          │
└─────────────────────────────────────────────┘
```

**Сюжет:**  
Финал! Организация использует DeFi для отмывания. Pump & dump на низколиквидных altcoins, межбиржевой арбитраж, liquidity pool манипуляции. Общая сумма — **$10M+**. Соберите доказательства и закройте дело.

**Что изучите:**
- ✅ Cryptocurrency markets (Bitcoin, Ethereum, altcoins)
- ✅ DeFi basics (AMM, liquidity pools, yield farming)
- ✅ Arbitrage (межбиржевой и triangular)
- ✅ Pump & dump detection и wash trading

**Артефакт:** `crypto_forensics` — криптовалютная криминалистика  
**Достижение:** 🏆 **"Crypto Detective"** (финал Season 5)  
**Время прохождения:** ~4 часа

**Финал сезона:** Схема раскрыта. $10M доказательств. Но организация узнаёт о расследовании и начинает контратаку через IoT...

---

## 🎭 Сюжетная арка Season 5

```
╔═══════════════════════════════════════════════════════════════════╗
║                    SEASON 5 TIMELINE                              ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  18 ДЕК, 18:00 → Красная площадь, встреча с V.                   ║
║  18 ДЕК, 19:00 → USB получен: торговые данные NYSE/NASDAQ        ║
║  18 ДЕК, 23:47 → Обнаружены аномалии в market depth              ║
║                                                                   ║
║  19 ДЕК, 08:30 → Начат реверс торговых алгоритмов                ║
║  19 ДЕК, 14:15 → 3 стратегии организации расшифрованы            ║
║                                                                   ║
║  19 ДЕК, 18:45 → Раскрыт портфель: $50M под риском               ║
║  19 ДЕК, 23:30 → VaR рассчитан: потери могут достичь $15M        ║
║                                                                   ║
║  20 ДЕК, 02:30 → Crypto схема обнаружена                         ║
║  20 ДЕК, 06:47 → Доказательства собраны: $10M+ манипуляций       ║
║                                                                   ║
║  РЕЗУЛЬТАТ: Финансовая схема раскрыта, но контратака начата...   ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

### АКТ I: Микроструктура (Episode 17)
```
19:00 → Торговые данные за 6 месяцев
     → Аномальные спреды на NYSE (0.5% вместо 0.01%)
     → Подозрительный объем в 23:00-01:00 (x10 от нормы)
23:47 → Order book манипуляции обнаружены: spoofing
```

### АКТ II: Алгоритмы (Episode 18)
```
08:30 → Автоматизированные стратегии организации
     → Reverse engineering: SMA crossover + RSI filter
     → Backtesting показывает 127% годовых (подозрительно)
14:15 → Слабость найдена: стратегии уязвимы к flash crashes
```

### АКТ III: Риски (Episode 19)
```
18:45 → Портфель раскрыт: $50M (70% crypto, 30% stocks)
     → Корреляция 0.95 (недиверсифицированный!)
     → VaR (95%, 1 day) = $5.2M
23:30 → Monte Carlo: вероятность краха 15% в течение месяца
```

### АКТ IV: Crypto — ФИНАЛ (Episode 20)
```
02:30 → DeFi схема: pump & dump на 15 altcoins
     → Межбиржевой арбитраж: Binance-Kraken-Coinbase
     → Liquidity pool манипуляции: $3M через Uniswap
06:47 → Доказательства собраны: $10,247,832 отмыто
     → Схема раскрыта. Организация узнаёт...
     → Начинается контратака через IoT (→ Season 6)
```

---

## 🏆 Достижения Season 5

```
╔════════════════════════════════════════════════════════╗
║  🎖️ MARKET CARTOGRAPHER                                ║
║  Освоено: OHLCV, order books, spread analysis         ║
╠════════════════════════════════════════════════════════╣
║  🎖️ ALGORITHM ENGINEER                                 ║
║  Освоено: SMA, RSI, MACD, backtesting framework       ║
╠════════════════════════════════════════════════════════╣
║  🎖️ RISK MASTER                                        ║
║  Освоено: VaR, Sharpe ratio, Monte Carlo, Markowitz   ║
╠════════════════════════════════════════════════════════╣
║  🎖️ CRYPTO DETECTIVE                                   ║
║  Освоено: DeFi, arbitrage, pump & dump detection      ║
╚════════════════════════════════════════════════════════╝
```

**Финальная награда:** Доказательства финансовых преступлений на $10M+ 💰

---

## 📊 Прогресс Season 5

```
╔══════════════════════════════════════════════════════════╗
║  SEASON 5 PROGRESS                                       ║
╠══════════════════════════════════════════════════════════╣
║                                                          ║
║  [ ] Episode 17: Market Microstructure    (3-4ч)  ⭐⭐⭐  ║
║  [ ] Episode 18: Trading Algorithms       (4-5ч)  ⭐⭐⭐⭐║
║  [ ] Episode 19: Risk & Portfolio         (4-5ч)  ⭐⭐⭐⭐⭐║
║  [ ] Episode 20: Crypto & DeFi (FINALE)  (4ч)    ⭐⭐⭐⭐⭐║
║                                                          ║
║  Всего: ~15-18 часов                                     ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

### Контрольный список:
- [ ] 📊 Проанализировать микроструктуру рынка
- [ ] 📈 Расшифровать торговые алгоритмы
- [ ] 💼 Рассчитать риски портфеля
- [ ] 🪙 Раскрыть crypto схему
- [ ] 🏆 Собрать $10M+ доказательств

---

## 🥚 Easter Eggs Season 5

<details>
<summary>🔍 Отсылки и секреты (спойлеры!)</summary>

### Episode 17:
- **$0.5% spread** — типичный признак market manipulation
- **23:00-01:00 volume** — Asian session, низкая ликвидность
- **Spoofing** — техника flash crash May 6, 2010 (Flash Crash)
- **Order book depth visualization** — как в Bloomberg Terminal

### Episode 18:
- **SMA crossover** — классическая стратегия из "Turtle Traders"
- **127% годовых** — отсылка к Bernie Madoff (120% обещаний)
- **RSI 30/70** — границы Wilder (J. Welles Wilder Jr., 1978)
- **Backtesting** — концепция из "Market Wizards" (Jack Schwager)

### Episode 19:
- **VaR 95%** — Basel III требование для банков
- **Sharpe ratio** — Nobel Prize William Sharpe (1990)
- **Markowitz optimization** — Nobel Prize Harry Markowitz (1990)
- **Monte Carlo** — метод из Manhattan Project (1940s)
- **15% crash probability** — Black Monday reference (Oct 19, 1987)

### Episode 20:
- **$10,247,832** — точная сумма Mt. Gox hack (2014)
- **Pump & dump altcoins** — BitConnect схема ($2B, 2018)
- **Uniswap** — крупнейший DEX (2020)
- **Triangular arbitrage** — техника из Forex trading
- **02:30-06:47** — Asian/European overlap (максимальная ликвидность crypto)

**Философия Season 5**: *"Markets are efficient until they're not. Follow the money, trust the math."*

</details>

---

## 🚀 Как начать

### Предварительные требования

Перед началом Season 5 убедитесь, что вы прошли:
- ✅ **Season 1:** Основы C, переменные, циклы, функции
- ✅ **Season 2:** Память, указатели, динамическая память
- ✅ **Season 3:** Сети, TCP/IP, сокеты
- ✅ **Season 4:** Криптография, алгоритмы, blockchain, структуры данных

**Важно:** Season 5 использует знания из Season 4 (hash tables, sorting, статистика).

### Переход к Season 5

```bash
cd season-5-financial-markets/
cat README.md

# Начните с Episode 17
cd episode-17-market-microstructure/
cat README.md
```

---

## 🎓 Дополнительные ресурсы

### Основа курса (Шиллер)
- 📚 [Yale: Financial Markets (Robert Shiller)](https://www.coursera.org/learn/financial-markets-global) — бесплатно на Coursera
- 📹 [Lecture videos на YouTube](https://www.youtube.com/playlist?list=PL8FB14A2200B87185)
- 📖 [Course materials](https://oyc.yale.edu/economics/econ-252)

### Книги по финтеху
- **"A Random Walk Down Wall Street"** — Burton Malkiel (теория эффективного рынка)
- **"Irrational Exuberance"** — Robert Shiller (Nobel Prize, 2013)
- **"Flash Boys"** — Michael Lewis (HFT и микроструктура)
- **"Options, Futures, and Other Derivatives"** — John Hull (деривативы)
- **"Algorithmic Trading"** — Ernest Chan (практика торговых алгоритмов)

### Технические ресурсы
- [QuantConnect](https://www.quantconnect.com/) — backtesting платформа
- [Investopedia](https://www.investopedia.com/) — финансовая теория
- [TradingView](https://www.tradingview.com/) — графики и индикаторы

---

## 💡 Советы по прохождению

1. **Изучите финансовые основы** — акции, облигации, опционы (хотя бы на уровне Investopedia)
2. **Смотрите лекции Shiller** — Yale курс даст контекст для эпизодов
3. **Практикуйтесь с реальными данными** — Yahoo Finance API, Alpha Vantage
4. **Визуализируйте результаты** — ASCII графики помогают понять паттерны
5. **Не торгуйте реальными деньгами!** — это учебный курс, не финансовый совет
6. **Используйте математику из Season 4** — статистика, сортировки, hash tables

---

## 🛠️ Технический стек

### Базовые инструменты (у вас уже есть)
```bash
gcc --version
make --version
```

### Библиотеки C
```c
#include <math.h>      // sqrt, pow для статистики
#include <stdlib.h>    // qsort, rand для Monte Carlo
#include <time.h>      // временные метки
```

### Опционально (для реальных данных)
- [cURL](https://curl.se/) — для HTTP запросов к биржевым API
- [cJSON](https://github.com/DaveGamble/cJSON) — парсинг JSON ответов

---

## ⚠️ Disclaimer

**ВАЖНО:** Этот курс создан исключительно в образовательных целях.

**Не является:**
- ❌ Финансовым советом
- ❌ Рекомендацией к торговле
- ❌ Гарантией прибыли

**Торговля на финансовых рынках:**
- Сопряжена с высокими рисками
- Требует профессиональных знаний
- Может привести к полной потере капитала

**Всегда консультируйтесь с лицензированными финансовыми консультантами** перед принятием инвестиционных решений.

Авторы курса не несут ответственности за финансовые потери, связанные с применением полученных знаний.

---

## 🎬 Что дальше?

После завершения Season 5 вас ждёт:

### 📟 [Season 6: Embedded & IoT](../season-6-embedded-iot/)
**Сюжет:** Организация узнаёт о расследовании и начинает контратаку через IoT устройства  
**Навыки:** Arduino, GPIO, промышленные протоколы, СКУД, IP-камеры  
**Временная линия:** 20 декабря, 08:00 → 22 декабря

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║  "Они знают. Они идут. Их оружие — IoT."                ║
║                                                   — V.   ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

---

## 🔗 Навигация

- [← Season 4: Crypto & Algorithms](../season-4-crypto-and-algorithms/README.md)
- [→ Season 6: Embedded & IoT](../season-6-embedded-iot/README.md)
- [📋 Общий план курса](../CURRICULUM.md)
- [🏠 Главная страница](../README.md)

---

<div align="center">

**🌙 MOONLIGHT OPERATION**  
*Season 5 of 10*

**"In markets, everyone leaves traces. Learn to read them."** — V.

```
// SEASON 5: FINANCIAL MARKETS
// STATUS: Enhanced (20% → 80%)
// SЮЖЕТ: Follow the money
// EPISODES: 4 (Market → Algorithms → Risk → Crypto)
// REWARDS: $10M+ evidence
```

[← Season 4](../season-4-crypto-and-algorithms/) | [Main README](../README.md) | [Season 6 →](../season-6-embedded-iot/)

---

**Season 5 Ready!** 💰  
Market Microstructure → Trading Algorithms → Risk Management → Crypto Forensics

**4 Episodes • 15-18 Hours • $10M+ Evidence • Nobel Prize Theory**

</div>
