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
│   ├── 📁 episode-29-big-data/
│   ├── 📁 episode-30-statistical-analysis/
│   ├── 📁 episode-31-neural-network/
│   └── 📁 episode-32-prediction/
│
└── 📁 final-operation/                 # Финальный проект
    ├── 📄 README.md
    ├── 📄 briefing.md
    ├── 📄 spec.md
    ├── 📁 modules/                     # Модули из предыдущих сезонов
    └── 📁 integration/                 # Код интеграции
```

---

## 📊 Статистика курса

### По сезонам:

| Сезон | Эпизоды | Часы | Основные темы |
|-------|---------|------|---------------|
| Season 1 ✅ | 1-4 | 9-12 | Основы C, типы, циклы, функции |
| Season 2 ✅ | 5-8 | 10-14 | Память, указатели, динамическая память |
| Season 3 ✅ | 9-12 | 12-16 | TCP/IP, сокеты, протоколы |
| Season 4 ⏳ | 13-16 | 12-16 | Криптография, blockchain, алгоритмы, структуры данных |
| Season 5 ✅💰 | 17-20 | 12-16 | Финансовые рынки, трейдинг, риски, crypto (Шиллер) |
| Season 6 | 21-24 | 14-18 | Embedded & IoT: Arduino, СКУД, видео, промышленные протоколы |
| Season 7 | 25-28 | 12-16 | Процессы, потоки, IPC, системные вызовы |
| Season 8 | 29-32 | 14-18 | Большие данные, статистика, ML на C |
| Final | - | 20-30 | Комплексный проект |
| **ИТОГО** | **32+** | **~108-150** | **Полный стек C** |

---

## 🎯 Прогрессия сложности

```
Сложность
    ↑
    │                                              ╱╲  Final
    │                                            ╱    ╲
    │                                          ╱        ╲
    │                                  ╱────╲            ╲
    │                          ╱────╲╱  S7   ╲      S8    ╲
    │                  ╱────╲╱  S5   ╲────────╲──────────  ╲
    │          ╱────╲╱  S3   ╲────────╲   S6   ╲            ╲
    │  ╱────╲╱  S1   ╲────────╲   S4   ╲────────╲            ╲
    │╱  Intro ╲──────────╲S2   ╲────────╲────────╲────────────╲
    └──────────────────────────────────────────────────────────→ Время
```

**Легенда:**
- S1-S2: Основы (gentle slope) ✅
- S3: Сети (networking) ✅
- S4: Криптография + алгоритмы ⏳
- S5: Финансы (финтех + трейдинг) ✅💰
- S6: Embedded & IoT (практика)
- S7-S8: Продвинутое (system programming + AI)
- Final: Интеграция всего

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
                                      FINAL OPERATION
```

**Обязательный порядок:**
- Season 1 → всё остальное
- Season 2 → Season 7
- Seasons 3-6 можно проходить в любом порядке (но лучше по номерам)
- Season 7-8 требуют знания предыдущих
- Final требует всё

---

## 📦 Артефакты по сезонам

Код из каждого сезона формирует компоненты финального проекта:

| Сезон | Артефакты | Использование в Final |
|-------|-----------|----------------------|
| S1 | `crypto_utils`, `file_io` | Базовые утилиты |
| S2 | `memory_manager`, `data_structures` | Управление памятью |
| S3 | `moonlight_client`, `packet_sniffer` | Сетевая коммуникация |
| S4 | `libmoonlight-crypto`, `algos` | Криптография |
| S5 | Arduino прошивки, embedded код | Управление устройствами |
| S6 | `access_controller`, `camera_api` | Слаботочные системы |
| S7 | `moonlight-agent`, IPC модули | Системный уровень |
| S8 | `moonlight-prophet`, ML модели | Анализ данных |

**Финал:** Всё объединяется в единую систему MOONLIGHT

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
✅ Алгоритмы  
✅ Криптография  
✅ Оптимизация  

### Season 5: Embedded
✅ Arduino  
✅ GPIO, сенсоры  
✅ I2C, SPI  

### Season 6: Low-voltage
✅ СКУД  
✅ Видеонаблюдение  
✅ Modbus  

### Season 7: System Programming
✅ Процессы, потоки  
✅ IPC  
✅ Системные вызовы  

### Season 8: AI & Data
✅ Большие данные  
✅ Статистика  
✅ Нейронные сети  

---

## 🚀 Рекомендуемый темп

### Интенсивный (full-time обучение):
- **1-2 эпизода в день**
- **Season 1:** 2-4 дня
- **Весь курс:** ~1-1.5 месяца

### Умеренный (part-time):
- **2-3 эпизода в неделю**
- **Season 1:** 1-2 недели
- **Весь курс:** ~3-4 месяца

### Расслабленный (выходные):
- **1 эпизод в выходные**
- **Season 1:** ~1 месяц
- **Весь курс:** ~6-8 месяцев

**Совет:** Качество важнее скорости. Лучше понять глубоко, чем пройти быстро.

---

## 📁 Размер файлов (примерно)

```
custom/
├── README, docs                ~50 KB
├── Season 1 (4 эпизода)        ~500 KB
├── Season 2-8 (28 эпизодов)    ~3.5 MB
├── Final operation             ~500 KB
├── Ваш код (artifacts/)        ~2-5 MB
└── Скомпилированные бинарники  ~10-20 MB

ИТОГО: ~15-30 MB
```

**Курс очень компактный!** Основной объём — ваш код и знания.

---

## 🎯 С чего начать?

```bash
cd /Users/sergiokilluminati/Dev/dev-c/custom
cat GETTING_STARTED.md
cd season-1-foundations/episode-01-strange-message/
cat README.md
```

**Удачи, агент!** 🚀
