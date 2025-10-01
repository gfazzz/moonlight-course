# 🌳 MOONLIGHT Course Structure
## Полное дерево курса

```
📦 OPERATION MOONLIGHT - Interactive C Course
│
├── 📄 README.md                    # Главная страница курса
├── 📄 SCENARIO.md                  # Сюжет (без спойлеров)
├── 📄 CURRICULUM.md                # Детальный план всех сезонов
├── 📄 GETTING_STARTED.md           # Быстрый старт
├── 📄 COURSE_TREE.md               # Это файл
├── 📄 Makefile                     # Главный Makefile
├── 📄 .gitignore                   # Git ignore
│
├── 📁 season-1-foundations/        # Основы C
│   ├── 📄 README.md
│   │
│   ├── 📁 episode-01-strange-message/
│   │   ├── 📄 README.md            # Теория + сюжет
│   │   ├── 📄 mission.md           # Краткое ТЗ
│   │   ├── 📄 starter.c            # Шаблоны кода
│   │   ├── 📄 Makefile
│   │   ├── 📁 artifacts/           # ← ВАШ КОД ЗДЕСЬ
│   │   ├── 📁 tests/
│   │   │   ├── test.sh
│   │   │   └── expected.txt
│   │   ├── 📁 solution/
│   │   │   └── decoder_solution.c
│   │   └── 📁 build/               # (создаётся при компиляции)
│   │
│   ├── 📁 episode-02-chain-of-clues/
│   │   ├── 📄 README.md
│   │   ├── 📄 mission.md
│   │   ├── 📄 data.dat             # Входные данные
│   │   ├── 📁 artifacts/
│   │   ├── 📁 tests/
│   │   └── 📁 solution/
│   │
│   ├── 📁 episode-03-patterns-in-time/
│   │   ├── 📄 README.md
│   │   ├── 📁 artifacts/
│   │   ├── 📁 tests/
│   │   └── 📁 solution/
│   │
│   └── 📁 episode-04-first-tool/
│       ├── 📄 README.md
│       ├── 📁 artifacts/
│       ├── 📁 tests/
│       └── 📁 solution/
│
├── 📁 season-2-memory-and-pointers/    # Память и указатели
│   ├── 📄 README.md
│   ├── 📁 episode-05-memory-dump/
│   ├── 📁 episode-06-array-evidence/
│   ├── 📁 episode-07-dynamic-expansion/
│   └── 📁 episode-08-reconstruction/
│
├── 📁 season-3-networks/               # Сетевое программирование
│   ├── 📄 README.md
│   ├── 📁 episode-09-first-contact/
│   ├── 📁 episode-10-wiretap/
│   ├── 📁 episode-11-traffic-analysis/
│   └── 📁 episode-12-hidden-protocol/
│
├── 📁 season-4-crypto-and-algorithms/  # Криптография и алгоритмы ✅
│   ├── 📄 README.md
│   ├── 📁 episode-13-crypto-basics/        # ✅ XOR, Caesar, брутфорс
│   ├── 📁 episode-14-blockchain-hashes/    # ⏳ Blockchain, хеши, PoW
│   ├── 📁 episode-15-sorting-search/       # ✅ Сортировки, поиск, Big O
│   └── 📁 episode-16-data-structures/      # ✅ Lists, Stacks, Trees
│
├── 📁 season-5-financial-markets/      # Финансовые рынки ✅💰
│   ├── 📄 README.md
│   ├── 📁 episode-17-market-microstructure/  # ✅ OHLCV, spread, depth
│   ├── 📁 episode-18-trading-algorithms/     # ✅ SMA, RSI, backtesting
│   ├── 📁 episode-19-risk-portfolio/         # ✅ VaR, Sharpe, Markowitz
│   └── 📁 episode-20-crypto-defi/            # ✅ Crypto, DeFi, arbitrage
│
├── 📁 season-6-embedded-iot/           # Embedded & IoT (Arduino + слаботочка)
│   ├── 📄 README.md
│   ├── 📁 episode-21-arduino-gpio/          # Arduino, GPIO, sensors
│   ├── 📁 episode-22-industrial-protocols/  # СКУД, Wiegand, Modbus
│   ├── 📁 episode-23-ip-cameras/            # RTSP, видеонаблюдение
│   └── 📁 episode-24-iot-integration/       # MQTT, system bypass
│
├── 📁 season-7-system-programming/     # Системное программирование
│   ├── 📄 README.md
│   ├── 📁 episode-25-processes-daemons/
│   ├── 📁 episode-26-threads-parallelism/
│   ├── 📁 episode-27-ipc/
│   └── 📁 episode-28-stealth-operation/
│
├── 📁 season-8-ai-and-data/            # ИИ и анализ данных
│   ├── 📄 README.md
│   ├── 📁 episode-29-big-data/              # Big Data parsing
│   ├── 📁 episode-30-statistical-analysis/  # Stats & GSL
│   ├── 📁 episode-31-neural-network/        # Neural nets from scratch
│   └── 📁 episode-32-prediction/            # Predictive models
│
├── 📁 season-9-advanced-systems/      # Продвинутые системы 🚀⚛️
│   ├── 📄 README.md
│   ├── 📁 episode-33-language/              # Compiler/Interpreter
│   ├── 📁 episode-34-database/              # Database engine
│   ├── 📁 episode-35-tui-framework/         # TUI framework
│   ├── 📁 episode-36-ballistics/            # 🎯 Military: Radar & GPS tracking
│   └── 📁 episode-37-quantum/               # ⚛️ Quantum computing
│
└── 📁 season-10-final-mission/        # Финальная миссия 🌌🚀
    ├── 📄 README.md
    ├── 📁 episode-38-architecture/          # System architecture
    ├── 📁 episode-39-dashboard/             # Intelligence dashboard
    ├── 📁 episode-40-offensive-defensive/   # Offensive & Defensive ops
    ├── 📁 episode-41-performance/           # Performance profiling
    └── 📁 episode-42-final-operation/       # 🚀 THE FINAL: Space Operations!
```

---

## 📊 Статистика курса

### По сезонам:

| Сезон | Эпизоды | Часы | Основные темы | Статус |
|-------|---------|------|---------------|--------|
| Season 1 | 1-4 | 9-12 | Основы C, типы, циклы, функции | 🟢 80% |
| Season 2 | 5-8 | 10-14 | Память, указатели, динамическая память | 🟢 80% |
| Season 3 | 9-12 | 12-16 | TCP/IP, сокеты, протоколы | 🟢 80% |
| Season 4 | 13-16 | 12-16 | Криптография, blockchain, алгоритмы, структуры данных | 🟢 80% |
| Season 5 💰 | 17-20 | 12-16 | Финансовые рынки, трейдинг, риски, crypto (Шиллер) | 🟡 20% |
| Season 6 📟 | 21-24 | 14-18 | Embedded & IoT: Arduino, СКУД, видео, промышленные протоколы | 🟡 20% |
| Season 7 🖥️ | 25-28 | 12-16 | Процессы, потоки, IPC, системные вызовы | 🟡 20% |
| Season 8 🤖 | 29-32 | 14-18 | Большие данные, статистика, ML на C | 🟡 20% |
| Season 9 🚀⚛️ | 33-37 | 18-24 | Компиляторы, БД, TUI, военные системы, квантовая физика | 🟡 20% |
| Season 10 🌌 | 38-42 | 20-30 | Финальная интеграция: Архитектура, Offensive/Defensive, КОСМОС! | 🟡 20% |
| **ИТОГО** | **42** | **145-200** | **Полный стек C от основ до космоса** 🌍→🚀→🌌 |

---

## 🎯 Прогрессия сложности

```
Сложность
    ↑
    │                                                            🚀 S10 (SPACE!)
    │                                                          ╱╲
    │                                                        ╱    ╲
    │                                              ⚛️ S9  ╱        ╲
    │                                            ╱────╲╱            ╲
    │                                    🤖 S8╱        ╲              ╲
    │                          🖥️ S7  ╱────╲            ╲              ╲
    │                  📟 S6 ╱────╲╱        ╲              ╲            ╲
    │          💰 S5 ╱────╲╱        ╲   🔐 S4╲              ╲            ╲
    │  🌐 S3 ╱────╲╱        ╲          ╲────────╲              ╲          ╲
    │╱────╲╱  S1   ╲  🧠 S2  ╲            ╲        ╲              ╲          ╲
    │  Intro ╲──────────╲────────╲────────╲────────╲──────────────╲──────────╲
    └──────────────────────────────────────────────────────────────────────────→ Время
```

**Легенда:**
- S1-S4: Готовы к прохождению (80%) 🟢
- S5-S10: Базовая структура (20%) 🟡
- Версия курса: v0.3.0
- Цель v1.0: все сезоны на 80%

---

## 🔄 Зависимости между сезонами

```
Season 1 (Foundations) ✅
    ↓
    ├──→ Season 2 (Memory & Pointers) ✅ ──→ Season 7 (System Programming)
    │                                          ↓
    ├──→ Season 3 (Networks) ✅ ───────────────┤
    │                                          ↓
    ├──→ Season 4 (Crypto & Algorithms) ⏳ ────┤
    │         ↓                                ↓
    ├──→ Season 5 (Financial Markets) ✅💰 ────┤
    │                                          ↓
    ├──→ Season 6 (Embedded & IoT) ────────────┤
    │                                          ↓
    └──────────────────────────────→ Season 8 (AI & Data)
                                               ↓
                                      Season 9 (Advanced)
                                               ↓
                                     Season 10 (FINAL 🚀)
```

**Обязательный порядок:**
- Season 1 → всё остальное (ОБЯЗАТЕЛЬНА первой!)
- Season 2 → Season 7 (память → системное программирование)
- Seasons 3-6 можно проходить в любом порядке (но рекомендуется по номерам)
- Season 7-8 требуют знания предыдущих
- Season 9 требует знания Seasons 1-8 (продвинутый уровень)
- Season 10 требует ВСЁ (финальная интеграция всего курса)

---

## 📦 Артефакты по сезонам

Код из каждого сезона формирует компоненты финального проекта:

| Сезон | Артефакты | Использование в Final |
|-------|-----------|----------------------|
| S1 | `crypto_utils`, `file_io` | Базовые утилиты |
| S2 | `memory_manager`, `data_structures` | Управление памятью |
| S3 | `moonlight_client`, `packet_sniffer` | Сетевая коммуникация |
| S4 | `libmoonlight-crypto`, `blockchain`, `algos` | Криптография + алгоритмы |
| S5 | `finance_engine`, `trading_algo` | Финансовый анализ |
| S6 | Arduino прошивки, `iot_controller` | Embedded & IoT |
| S7 | `moonlight-agent`, IPC модули | Системный уровень |
| S8 | `moonlight-prophet`, ML модели | Анализ данных, AI |
| S9 | `moonlang`, `moondb`, `MoonUI`, `quantum_sim` | Продвинутые инструменты |
| S10 | **ПОЛНАЯ ИНТЕГРАЦИЯ** | Вся система MOONLIGHT 🌌 |

**Season 10:** Всё объединяется в единую систему MOONLIGHT — от Земли до космоса! 🌍→🚀→🌌

---

## 🎓 Навыки по сезонам

### Season 1: Foundations
✅ Синтаксис C  
✅ Типы данных  
✅ Функции  
✅ Простая криптография  

### Season 2: Memory & Pointers
✅ Указатели  
✅ Динамическая память  
✅ Структуры данных  

### Season 3: Networks
✅ TCP/IP  
✅ Сокеты  
✅ Протоколы  

### Season 4: Crypto & Algorithms
✅ Криптография (XOR, Caesar, blockchain)  
✅ Алгоритмы (сортировки, поиск)  
✅ Структуры данных (деревья, хеш-таблицы)  

### Season 5: Financial Markets 💰
✅ Микроструктура рынка  
✅ Торговые алгоритмы  
✅ Риск-менеджмент  
✅ Crypto & DeFi  

### Season 6: Embedded & IoT 📟
✅ Arduino & GPIO  
✅ Промышленные протоколы (Modbus, Wiegand, СКУД)  
✅ IP-камеры (RTSP), видеонаблюдение  
✅ IoT интеграция (MQTT)  

### Season 7: System Programming 🖥️
✅ Процессы, демоны  
✅ Потоки, параллелизм  
✅ IPC (pipes, queues, shared memory)  
✅ Системные вызовы  

### Season 8: AI & Data 🤖
✅ Big Data processing  
✅ Статистический анализ  
✅ Нейронные сети с нуля  
✅ Предсказательные модели  

### Season 9: Advanced Systems 🚀⚛️
✅ Компиляторы/интерпретаторы  
✅ Базы данных с нуля  
✅ TUI фреймворки  
✅ Военные системы (радар, GPS, Kalman) 🎯  
✅ Квантовые вычисления (Shor, Grover) ⚛️  

### Season 10: Final Mission 🌌🚀
✅ Системная архитектура  
✅ Intelligence Dashboard  
✅ Offensive & Defensive Ops  
✅ Профилирование производительности  
✅ **КОСМИЧЕСКАЯ ОПЕРАЦИЯ** (спутники, орбитальная механика) 🛰️  

---

## 🚀 Рекомендуемый темп

### Интенсивный (full-time обучение):
- **2 эпизода в день**
- **Season 1:** 2-3 дня
- **Весь курс (42 эпизода):** ~3-4 недели

### Умеренный (part-time):
- **3-4 эпизода в неделю**
- **Season 1:** 1-2 недели
- **Весь курс:** ~3-4 месяца

### Расслабленный (выходные):
- **1-2 эпизода в неделю**
- **Season 1:** ~3-4 недели
- **Весь курс:** ~6-10 месяцев

**Совет:** Качество важнее скорости. Лучше понять глубоко, чем пройти быстро.

---

## 📁 Размер файлов (примерно)

```
moonlight-course/
├── README, docs                ~80 KB
├── Seasons 1-5 (20 эпизодов)   ~2.5 MB
├── Seasons 6-8 (12 эпизодов)   ~1.8 MB
├── Seasons 9-10 (10 эпизодов)  ~1.5 MB
├── Ваш код (artifacts/)        ~3-8 MB
└── Скомпилированные бинарники  ~15-30 MB

ИТОГО: ~20-40 MB (621 файл | 42 эпизода | 10 сезонов)
```

**Курс очень компактный!** Основной объём — ваш код и знания.

---

## 🎯 С чего начать?

```bash
cd ~/moonlight-course
cat GETTING_STARTED.md
cd season-1-foundations/episode-01-strange-message/
cat README.md
```

**Удачи, агент!** 🚀
