# 🎯 OPERATION MOONLIGHT
## Интерактивный курс по программированию на C

> *"В мире, где информация — это оружие, код — это ключ к истине"*

---

## 📖 О курсе

Добро пожаловать в **Operation Moonlight** — интерактивный курс программирования на C, где каждая строка кода приближает вас к разгадке глобального заговора.

Это не просто курс программирования. Это детективная история, где вы — главный герой, специалист по кибербезопасности, распутывающий сеть интриг от подпольных серверов до защищённых систем управления.

### 🎬 Особенности курса

- **Сквозной сюжет**: Захватывающая история в стиле Watch Dogs, Matrix, "Враг государства" и "Семнадцать мгновений весны"
- **🎭 Accumulating Cast**: Персонажи появляются постепенно и возвращаются в новых ролях — мир расширяется с каждым сезоном
- **Interleaving**: Теория, вопросы и задачи перемешаны с сюжетом для максимального вовлечения
- **Практический подход**: Задачи-матрёшки, где каждое упражнение — часть большого проекта
- **Реальные технологии**: FreeBSD, Arduino, сетевые протоколы, криптография, embedded systems
- **Прогрессивное обучение**: От основ до продвинутых концепций
- **🔗 Cumulative Code Reuse**: Переиспользование своего кода через `#include` — строишь арсенал инструментов! (см. [PROJECTS.md](PROJECTS.md))

### 🛠 Предметные области

- 🔐 **Кибербезопасность** — взлом, защита, анализ уязвимостей
- 🌐 **Сетевое программирование** — TCP/IP, сокеты, протоколы
- 🔢 **Алгоритмы и криптография** — шифрование, хеширование, blockchain
- 💰 **Финансовые рынки** — трейдинг, риски, crypto, DeFi (Season 5)
- 📟 **Embedded & IoT** — Arduino, СКУД, IP-камеры, промышленные протоколы
- 🖥️ **Системное программирование** — процессы, потоки, IPC, демоны
- 🤖 **AI и нейросети** — ML на C, статистика, предсказания
- 🚀 **Advanced** — компиляторы, БД, TUI
- 🎯 **Милитари** — баллистические расчеты (Season 9)
- ⚛️ **Квантовая физика** — квантовые вычисления (Season 9)

### 📚 Теоретическая база

Курс основан на проверенных источниках:
- **CS50** (Harvard) — педагогический подход
- **Head First C** — практические примеры
- **K&R "The C Programming Language"** — классика

Всё объяснено понятным языком с метафорами и реальными примерами.

---

## 🎭 Персонажи (Accumulating Cast)

Каждый сезон вводит новых персонажей, но **старые возвращаются** в новых ролях. Мир расширяется, отношения развиваются.

### Season 1: Foundations (введение)
- 👤 **Виктор Петров** — бывший напарник из ФСБ, официально "погиб" 2 года назад
- 🤝 **Дмитрий Орлов** — Embedded Engineer @ Аэрофлот, помогает с XOR
- 👻 **"Призрак"** — анонимный хакер (смайлик :)), мотивы неясны
- 💼 **Ева Циммерман** — криптограф UBS Bank, Zürich, знала Виктора
- ⚔️ **Полковник Крылов** — ФСБ отдел "К", антагонист, ловушка #1723

### Season 2-4: Возврат + новые
- 🔄 **Дмитрий** возвращается (memory forensics, embedded systems)
- 🔄 **"Призрак"** раскрывает личность в Season 3
- 🔄 **Ева** — главная роль в Season 4 (криптография!)
- 🔄 **Крылов** — преследование усиливается
- 🔄 **Виктор** появляется лично в Season 4
- 🆕 Новые: forensics specialist, немецкий бизнесмен, представитель "Новой Эры"

### Season 5-10: Финал
- Все персонажи развиваются
- Season 6: **Дмитрий** в главной роли (дроны, авионика)
- Season 10: **Все** собираются для финальной операции

> **Концепция:** Персонажи не исчезают. Они возвращаются, помогают, предают, меняются. Мир живой.

---

## 🗂 Структура курса

### Season 1: Foundations (Основы) ✅ v3.0
*"Всё начинается с простого..."*
- Episode 01: Типы данных, переменные, операторы — byte operations
- Episode 02: Условия (if/else/switch) — классификация данных
- Episode 03: Циклы (for/while/do-while) — поиск паттернов
- Episode 04: Функции и модульность — многофайловые программы
- **Season Project**: `moonlight_decoder` — твой первый инструмент для расшифровки сообщений V.

### Season 2: Memory & Pointers (Память и указатели) ✅
*"Знание того, где хранится информация — половина успеха"*
- Episode 05: Массивы и строки — парсинг дампов памяти
- Episode 06: Указатели — навигация по цепочкам данных
- Episode 07: Динамическая память — malloc/free, загрузчик архивов
- Episode 08: Утечки памяти — поиск с valgrind/leaks
- **Season Project**: `memory_manager` — контроль памяти + leak detector

### Season 3: Networks (Сети) ✅
*"Всё связано. Нужно лишь найти нить"*
- Episode 09: TCP/IP, IP-адресация, порты — анализатор сетей
- Episode 10: Socket programming — TCP клиент-сервер
- Episode 11: Raw sockets — packet sniffer (требует root)
- Episode 12: Перехват зашифрованных коммуникаций (финал Season 3)
- **Season Project**: `network_interceptor` — перехват и анализ трафика агентов Z.

### Season 4: Crypto & Algorithms ✅ (упрощён v0.3.1)
*"Секреты защищены не замками, а математикой"*
- Episode 13: Crypto Basics (XOR, Caesar, frequency analysis)
- Episode 14: Blockchain & Hashes (Proof-of-Work, djb2)
- Episode 15: Sorting & Search (5 базовых алгоритмов, Big O)
- Episode 16: Data Structures (linked lists, stack/queue, BST, hash table basic)
- **Season Project**: `crypto_toolkit` — криптография + алгоритмы + структуры данных
- **Время**: ~12-15ч (упрощён на 40% для плавной кривой обучения)

### Season 5: Financial Markets 💰
*"Follow the money"*
- Episode 17: Market Microstructure (OHLCV, bid-ask, depth)
- Episode 18: Trading Algorithms (SMA, RSI, backtesting)
- Episode 19: Risk & Portfolio (VaR, Sharpe ratio, Markowitz)
- Episode 20: Crypto & DeFi (exchanges, liquidity pools, arbitrage)
- **Season Project**: `trading_system` — **использует Seasons 1-4** (crypto, memory, network, algorithms)

### Season 6: Embedded & IoT
*"Реальный мир управляется кодом"*
- Episode 21: Arduino & GPIO (sensors, actuators)
- Episode 22: Industrial Protocols (СКУД, Wiegand, Modbus)
- Episode 23: IP Cameras (RTSP, видеонаблюдение) + Drones/UAV 🚁
- Episode 24: IoT Integration (MQTT, system bypass)
- **Season Project**: `iot_controller` — **использует Seasons 1-4** (network для MAVLink, crypto для команд)

### Season 7: System Programming
*"ОС — это поле боя"*
- Episode 25: Processes & Daemons (FreeBSD/macOS/Linux)
- Episode 26: Threads & Parallelism
- Episode 27: IPC (pipes, queues, shared memory)
- Episode 28: Stealth Operation
- **Season Project**: `stealth_agent` — **использует Seasons 1-6** (все инструменты для скрытой операции)

### Season 8: AI & Data Science
*"Паттерны везде. Нужно научиться их видеть"*
- Episode 29: Big Data Processing (mmap, stream processing) — Heapsort, Shell Sort перенесены сюда
- Episode 30: Statistical Analysis (anomaly detection) — Knuth Shuffle для Monte Carlo
- Episode 31: Neural Networks from Scratch (C + Python для сравнения)
- Episode 32: Predictive Models
- **Season Project**: `ml_predictor` — **использует Seasons 1-7** (memory для datasets, algorithms для matrix ops)

### Season 9: Advanced Systems 🚀⚛️
*"От компиляторов до квантовой физики"*
- Episode 33: Building a Language (interpreter, AST, GC)
- Episode 34: Database from Scratch (B+Tree, SQL parser) — AVL Trees, Advanced Hash Tables, Merkle Trees
- Episode 35: Terminal UI Framework (ncurses, widgets)
- Episode 36: Military Systems & Radar 🎯 (tracking, Kalman filter, GPS, drones)
- Episode 37: Quantum Computing ⚛️ (qubits, Shor's algorithm, C для скорости, C++ для ООП паттернов)
- **Season Project**: `advanced_systems` — **использует Seasons 1-8** (database engine = memory + data structures + IPC + ML)

### Season 10: FINAL MISSION 🌌🚀 (Mission Impossible)
*"Всё сходится воедино. До луны и дальше!"*
- Episode 38: System Architecture (microservices, message bus)
- Episode 39: Intelligence Dashboard (real-time TUI)
- Episode 40: Offensive & Defensive Ops (pentesting, IDS/IPS, honeypots)
- Episode 41: Performance Optimization (profiling, perf, SIMD)
- Episode 42: The Final Operation 🚀 (satellite tracking, orbital mechanics!)
- **Season Project**: `moonlight_core` — **использует ВСЕ Seasons 1-9** — ФИНАЛЬНАЯ ИНТЕГРАЦИЯ! 🌍→🌌
- **Сложность**: Максимальная автономия, минимум подсказок, ты сам ставишь задачи

---

## 🚀 Начало работы

### Требования
- **OS:** macOS или Linux (Ubuntu 20.04+, Debian, Fedora и др.)
- **IDE:** Cursor (рекомендуется) или любой редактор
- **Знания:** Базовые навыки программирования
- **Компилятор:** GCC/Clang
  - macOS: уже установлен (Xcode Command Line Tools)
  - Ubuntu/Debian: `sudo apt install build-essential`
  - Fedora: `sudo dnf install gcc make`

### Установка
```bash
# Клонируйте репозиторий
git clone https://github.com/yourusername/moonlight-course.git
cd ~/moonlight-course

# Изучите структуру курса
cat README.md
cat SCENARIO.md
cat CURRICULUM.md

# Начните с первого эпизода
cd season-1-foundations/episode-01-strange-message
cat README.md
```

### Первые шаги
1. Прочитайте [SCENARIO.md](SCENARIO.md) — узнайте предысторию
2. Изучите [CURRICULUM.md](CURRICULUM.md) — детальный план обучения
3. Прочитайте [PROJECTS.md](PROJECTS.md) — узнайте о Season Projects и переиспользовании кода
4. Начните с `season-1-foundations/episode-01/`

---

## 📋 Статус разработки курса

```
╔═══════════════════════════════════════════════════════════════╗
║  MOONLIGHT COURSE - Development Status v0.3.2                 ║
╠═══════════════════════════════════════════════════════════════╣
║  🟢 Season 1: Foundations          [██████░░]  80% v3.0       ║
║      Expanded Cast + Interleaving + Cliffhangers              ║
║  🟢 Season 2: Memory & Pointers    [██████░░]  80%            ║
║  🟢 Season 3: Networks             [██████░░]  80%            ║
║  🟢 Season 4: Crypto & Algorithms  [██████░░]  80%            ║
║  🟡 Season 5: Financial Markets    [██░░░░░░]  20%            ║
║  🟡 Season 6: Embedded & IoT       [██░░░░░░]  20%            ║
║  🟡 Season 7: System Programming   [██░░░░░░]  20%            ║
║  🟡 Season 8: AI & Data Science    [██░░░░░░]  20%            ║
║  🟡 Season 9: Advanced Systems     [██░░░░░░]  20%            ║
║  🟡 Season 10: Final Mission       [██░░░░░░]  20%            ║
╠═══════════════════════════════════════════════════════════════╣
║  📊 Overall Progress: 42% (v0.3.2)                            ║
║  🎯 Next: Apply Expanded Cast to Season 2-4                   ║
║  🎯 Milestone: v1.0 (все сезоны на 80%)                       ║
╚═══════════════════════════════════════════════════════════════╝

📊 Season 1: 80% v3.0 Expanded Cast Edition
   ✅ 5 персонажей (постепенное введение)
   ✅ Interleaving (теория→вопросы→задачи→сюжет)
   ✅ Прогрессивные cliffhanger'ы
   ✅ Финальное разрешение (Виктор ЖИВ!)
   ⏳ До 100%: тестирование студентами, Season Project
   
📊 Season 2-4: 80% (сюжет, теория, контрольные вопросы)
   ⏳ Требуется применить методологию Expanded Cast
   
📊 Season 5-10: 20% (базовая структура, требует расширения)
🎯 Версия 1.0: когда все 10 сезонов достигнут 80%
```

---

## 💡 Как проходить курс

1. **Читайте теорию** — каждый эпизод начинается с объяснения концепций
2. **Решайте задачи** — практика важнее всего
3. **Собирайте артефакты** — код из эпизодов формирует **Season Project**
4. **Переиспользуйте свой код** — начиная с Season 5, подключай инструменты через `#include` (см. [PROJECTS.md](PROJECTS.md))
5. **Используйте AI** — учитесь работать с Cursor и ChatGPT как инструментами
6. **Экспериментируйте** — ломайте и чините, это лучший способ учиться

---

## 🎯 Цели курса

После прохождения вы сможете:
- ✅ Писать эффективный и безопасный код на C
- ✅ Работать с сетевыми протоколами и сокетами
- ✅ Программировать встроенные системы (Arduino)
- ✅ Понимать низкоуровневую работу компьютера
- ✅ Реализовывать криптографические алгоритмы
- ✅ Анализировать и защищать системы безопасности
- ✅ Эффективно использовать AI для программирования

---

## 📞 Поддержка

Возникли вопросы? Застряли на задаче?
- Используйте комментарии в коде с вопросами
- Экспериментируйте с Cursor AI
- Помните: в программировании нет глупых вопросов

---

## ⚠️ Дисклеймер

Этот курс создан исключительно в образовательных целях. Все техники и методы должны использоваться только в легальных целях и с разрешения владельцев систем. Автор не несёт ответственности за неправомерное использование полученных знаний.

---

## 💡 Источник идеи

Этот курс создан на основе идеи с платформы [Eurecable](https://eurecable.com/ideas/971) — площадки для инновационных проектов и креативных идей.

Концепция демонстрирует, как с помощью современного AI можно создать персонализированный интерактивный курс обучения программированию с захватывающим сюжетом и практическими задачами.

**См. также:** [CREDITS.md](CREDITS.md) — полные благодарности и источники вдохновения

---

**Готовы начать операцию? Удачи, агент. Мир зависит от вашего кода.**

*— The Architect*
