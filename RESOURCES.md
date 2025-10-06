# 📚 RESOURCES — Рекомендуемые материалы для обучения
## Кураторский список качественных ресурсов

> *"Знание — это сила, но только если знаешь, где его искать."* — LUNA 💡

**Последнее обновление:** 6 октября 2025
**Версия курса:** 0.3.5
**Философия:** Качество > Количество. Только проверенные материалы.

---

## 🎯 Общие принципы выбора ресурсов

✅ **Бесплатные или доступные** — приоритет open-source и free tier
✅ **Высокое качество** — университетский уровень (MIT, Harvard, Stanford)
✅ **Практическая направленность** — hands-on, не только теория
✅ **Актуальные** — 2020+ или классика с вечной ценностью
✅ **На английском** — язык индустрии (русские аналоги где есть)

---

## 🏛️ Фундаментальные ресурсы (для всех сезонов)

### 📖 Обязательные книги (Must Read)

#### **K&R "The C Programming Language"** (2nd ed, 1988)
- **Авторы:** Brian Kernighan, Dennis Ritchie (создатели C!)
- **Статус:** Библия C программирования, классика
- **FREE:** https://kremlin.cc/k&r.pdf (неофициальный PDF)
- **Страниц:** ~270
- **Уровень:** Beginner → Intermediate
- **Комментарий LUNA:** "Если бы Viktor читал одну книгу по C — это была бы K&R. Краткость, точность, элегантность."

#### **CS50 Manual Pages**
- **FREE:** https://manual.cs50.io/
- **Описание:** Упрощённые man pages для новичков
- **Комментарий LUNA:** "Официальные `man` страницы пугают? Начни отсюда. Потом перейдёшь на `man 3`."

### 🎥 Видеокурсы (университетский уровень)

#### **CS50: Introduction to Computer Science** (Harvard)
- **URL:** https://cs50.harvard.edu/x/
- **Преподаватель:** David J. Malan
- **Длительность:** ~100 часов
- **Язык:** Английский (есть субтитры)
- **Уровень:** Beginner
- **Темы:** C, алгоритмы, память, структуры данных
- **Качество:** ⭐⭐⭐⭐⭐ 5/5
- **Комментарий LUNA:** "Лучший курс для начинающих. David Malan объясняет так, что даже сложные концепции кажутся простыми. Season 1-4 отлично дополняются CS50."

#### **MIT Missing Semester** (MIT)
- **URL:** https://missing.csail.mit.edu/
- **Темы:** Command line, Git, Debugging, Profiling
- **Длительность:** ~20 часов
- **Уровень:** Intermediate
- **Качество:** ⭐⭐⭐⭐⭐ 5/5
- **Комментарий LUNA:** "То, что не учат в университетах, но используют каждый день: shell, vim, gdb, git. Must-watch для Season 7."

### 🛠️ Инструменты и документация

#### **GDB (GNU Debugger) Tutorial**
- **URL:** https://www.gdbtutorial.com/
- **Также:** https://sourceware.org/gdb/documentation/
- **Уровень:** Intermediate
- **Комментарий LUNA:** "Viktor нашёл 90% своих багов с GDB. Научись breakpoints, watchpoints, backtrace — и segfaults перестанут пугать."

#### **Valgrind Documentation**
- **URL:** https://valgrind.org/docs/manual/quick-start.html
- **Темы:** Memory leak detection, profiling
- **Уровень:** Intermediate
- **Комментарий LUNA:** "Каждый `malloc` должен иметь `free`. Valgrind покажет, где ты забыл. Season 2 Episode 08 без Valgrind — как полёт вслепую."

#### **cdecl - C gibberish ↔ English**
- **URL:** https://cdecl.org/
- **Описание:** Переводит сложные C объявления в читаемый вид
- **Пример:** `int (*(*foo)[5])(void)` → что это вообще?!
- **Комментарий LUNA:** "Указатели на массивы функций? cdecl спасёт рассудок. Viktor держал эту ссылку в закладках."

---

## 📚 Ресурсы по сезонам

### 🔧 Season 1: Foundations (Основы C)

#### Книги
- **"Head First C"** (David Griffiths, Dawn Griffiths, 2012)
  - Визуальный подход, много примеров
  - FREE: O'Reilly library (trial)
  - Уровень: Beginner
  - 632 страницы
  - ⭐⭐⭐⭐☆ 4/5

- **"C Programming: A Modern Approach"** (K.N. King, 2008)
  - Детальное покрытие C99/C11
  - ~800 страниц, учебник
  - Уровень: Beginner → Advanced
  - ⭐⭐⭐⭐⭐ 5/5

#### Видео
- **CS50 Week 1-2** (Harvard)
  - C basics, типы данных, функции
  - FREE: https://cs50.harvard.edu/x/2024/weeks/1/

#### Интерактивные платформы
- **Learn-C.org**
  - URL: https://www.learn-c.org/
  - Интерактивные упражнения в браузере
  - FREE
  - Уровень: Beginner

#### Референсы
- **cppreference.com (C section)**
  - URL: https://en.cppreference.com/w/c
  - Детальная документация всех функций
  - FREE, searchable
  - Комментарий LUNA: "Лучше, чем официальные ISO docs. Примеры кода, пояснения, версии стандартов."

---

### 💾 Season 2: Memory & Pointers

#### Книги
- **"Understanding and Using C Pointers"** (Richard Reese, 2013)
  - 226 страниц, фокус на указателях
  - O'Reilly
  - Уровень: Intermediate
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Указатели — это НЕ сложно, если понять концепцию. Эта книга объясняет лучше всех."

- **"Effective C"** (Robert C. Seacord, 2020)
  - Современные best practices C17
  - 272 страницы
  - Уровень: Intermediate → Advanced
  - ⭐⭐⭐⭐⭐ 5/5

#### Видео
- **CS50 Week 4: Memory** (Harvard)
  - Hexadecimal, pointers, malloc, file I/O
  - FREE: https://cs50.harvard.edu/x/2024/weeks/4/

- **Computerphile: Pointers Explained**
  - URL: https://www.youtube.com/watch?v=DTxHyVn0ODg
  - 10 минут, визуальные объяснения
  - FREE

#### Инструменты
- **Valgrind Quick Start**
  - URL: https://valgrind.org/docs/manual/quick-start.html
  - Memory leak detection
  - Комментарий LUNA: "Season 2 Episode 08 — это про Valgrind. Изучи `--leak-check=full` до прохождения эпизода."

- **AddressSanitizer (ASan)**
  - URL: https://github.com/google/sanitizers/wiki/AddressSanitizer
  - Compile flag: `-fsanitize=address`
  - Находит memory bugs быстрее Valgrind
  - Комментарий LUNA: "Viktor переключился на ASan в 2020. Быстрее, чем Valgrind, но менее детально."

#### Визуализации
- **Pointer Visualizations (Python Tutor for C)**
  - URL: https://pythontutor.com/c.html
  - Пошаговая визуализация указателей
  - FREE, interactive
  - Комментарий LUNA: "Видеть, как указатели двигаются по памяти — бесценно для понимания."

---

### 🌐 Season 3: Networks

#### Книги
- **"Beej's Guide to Network Programming"** (Brian "Beej" Hall)
  - FREE: https://beej.us/guide/bgnet/
  - ~170 страниц, фокус на sockets
  - Уровень: Intermediate
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Библия network programming на C. Viktor читал это 3 раза. Третий раз — перед каждым проектом с сокетами."

- **"TCP/IP Illustrated, Volume 1"** (W. Richard Stevens, 1993)
  - Классика, теория протоколов
  - ~800 страниц
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5

- **"Unix Network Programming"** (W. Richard Stevens, 2003)
  - Два тома, 2000+ страниц
  - Production-level code
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5

#### Видео
- **CS50 2024: Week 6 (Networks)**
  - TCP/IP basics, HTTP
  - FREE: https://cs50.harvard.edu/x/2024/weeks/6/

#### Интерактивные платформы
- **OverTheWire: Natas** (Web exploitation)
  - URL: https://overthewire.org/wargames/natas/
  - Понимание HTTP, headers, sessions
  - FREE, 34 уровня
  - Уровень: Intermediate

#### Инструменты
- **Wireshark Tutorials**
  - URL: https://www.wireshark.org/docs/
  - Packet analysis (GUI)
  - Комментарий LUNA: "Season 3 Episode 11 — packet sniffing. Wireshark покажет, что ты должен извлечь программно."

- **tcpdump Manual**
  - URL: https://www.tcpdump.org/manpages/tcpdump.1.html
  - Command-line packet analyzer
  - Комментарий LUNA: "Viktor предпочитал tcpdump. Быстрее, легче автоматизировать. Season 3 — про raw sockets."

- **netcat (nc) Guide**
  - URL: https://nc110.sourceforge.io/
  - Swiss army knife для TCP/UDP
  - Комментарий LUNA: "Тестирование TCP servers. `nc localhost 8080` — и ты внутри."

#### RFC Documents
- **RFC 793 (TCP)**
  - URL: https://datatracker.ietf.org/doc/html/rfc793
  - Оригинальная спецификация TCP
  - Уровень: Advanced

- **RFC 791 (IP)**
  - URL: https://datatracker.ietf.org/doc/html/rfc791
  - Internet Protocol
  - Уровень: Advanced

---

### 🔐 Season 4: Crypto & Algorithms

#### Книги
- **"The Art of Computer Programming" (TAOCP)** (Donald Knuth)
  - Volume 1: Fundamental Algorithms
  - Volume 2: Seminumerical Algorithms
  - Volume 3: Sorting and Searching
  - ~2000+ страниц (3 тома)
  - Уровень: Advanced → Expert
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Библия алгоритмов. Knuth — это мета-уровень. Season 4 даёт базовые алгоритмы, но TAOCP пригодится в **Season 8-9**: Heapsort, Shell Sort (S8E29), Knuth Shuffle (S8E30), advanced structures (S9E34). Не обязательно читать всё, но держи рядом для Season 8+."

- **"Introduction to Algorithms" (CLRS)** (Cormen, Leiserson, Rivest, Stein, 2009)
  - 1312 страниц, MIT учебник
  - Уровень: Intermediate → Advanced
  - ⭐⭐⭐⭐⭐ 5/5

- **"Cryptography Engineering"** (Ferguson, Schneier, Kohno, 2010)
  - Практическая криптография
  - 384 страницы
  - Уровень: Intermediate
  - ⭐⭐⭐⭐☆ 4/5

#### Видео
- **CS50 Week 5: Data Structures** (Harvard)
  - Linked lists, hash tables, trees
  - FREE: https://cs50.harvard.edu/x/2024/weeks/5/

- **MIT 6.006: Introduction to Algorithms** (Fall 2011)
  - URL: https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/
  - Full course, FREE
  - Уровень: Advanced

#### Интерактивные платформы
- **VisuAlgo** (Algorithm Visualizations)
  - URL: https://visualgo.net/
  - Визуализация sorting, BST, hash tables
  - FREE, interactive
  - Комментарий LUNA: "Видеть, как Quick Sort разбивает массив — лучше тысячи слов. Season 4 Episode 15."

- **HackerRank: Data Structures**
  - URL: https://www.hackerrank.com/domains/data-structures
  - Практика на задачах
  - FREE tier

- **CryptoHack**
  - URL: https://cryptohack.org/
  - Hands-on cryptography challenges
  - FREE, gamified
  - Уровень: Intermediate → Advanced
  - Комментарий LUNA: "Season 4 Episode 13 — XOR, Caesar. CryptoHack — следующий уровень."

#### Референсы
- **Big-O Cheat Sheet**
  - URL: https://www.bigocheatsheet.com/
  - Complexity всех алгоритмов
  - FREE, searchable

- **OpenSSL Documentation**
  - URL: https://www.openssl.org/docs/
  - Production cryptography library
  - Уровень: Advanced

---

### 💰 Season 5: Financial Markets (FinTech)

#### Книги
- **"Algorithmic Trading"** (Ernie Chan, 2009)
  - Практический подход к algo trading
  - 240 страниц
  - Уровень: Intermediate
  - ⭐⭐⭐⭐☆ 4/5

- **"Flash Boys"** (Michael Lewis, 2014)
  - История HFT (High-Frequency Trading)
  - 288 страниц, популярная литература
  - Уровень: Beginner (non-technical)
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Чтобы понять, ЗАЧЕМ нужна скорость C в финансах. Разница в микросекунды = миллионы долларов."

#### Видео
- **Yale: Financial Markets** (Robert Shiller, Nobel Prize)
  - URL: https://www.coursera.org/learn/financial-markets-global
  - FREE audit доступен
  - Теория финансовых рынков
  - Уровень: Intermediate
  - Комментарий LUNA: "Season 5 базируется на Shiller. Но фокус MOONLIGHT — практика C для FinTech, не академическая теория."

#### Инструменты
- **TA-Lib (Technical Analysis Library)**
  - URL: https://ta-lib.org/
  - C library для технических индикаторов
  - FREE, open-source
  - Комментарий LUNA: "SMA, RSI, MACD — всё готово. Season 5 Episode 18. Можно реализовать с нуля или использовать TA-Lib."

- **QuantLib (C++)**
  - URL: https://www.quantlib.org/
  - Financial instruments library
  - FREE, open-source
  - Уровень: Advanced
  - Комментарий LUNA: "C++, но концепции те же. Можно изучить API design для Season 5 projects."

#### Данные
- **Alpha Vantage API**
  - URL: https://www.alphavantage.co/
  - FREE tier: 25 requests/day
  - Real-time & historical market data
  - Комментарий LUNA: "Season 5 Episodes 17-20 — работа с реальными данными. Alpha Vantage — бесплатный источник."

- **Yahoo Finance API (unofficial)**
  - URL: https://github.com/ranaroussi/yfinance (Python wrapper)
  - Концепция применима к C
  - FREE

---

### 📟 Season 6: Embedded & IoT

#### Книги
- **"Making Embedded Systems"** (Elecia White, 2011)
  - 324 страницы, практический подход
  - O'Reilly
  - Уровень: Intermediate
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Дмитрий Орлов (из Season 1-4) читал это перед работой в Аэрофлот. Embedded — это другой мир."

- **"Arduino Cookbook"** (Michael Margolis, 2020)
  - 3rd edition, 800+ страниц
  - Рецепты для Arduino проектов
  - Уровень: Beginner → Intermediate
  - ⭐⭐⭐⭐☆ 4/5

#### Видео
- **Arduino Official Tutorials**
  - URL: https://www.arduino.cc/en/Tutorial/HomePage
  - FREE, step-by-step
  - Уровень: Beginner

#### Инструменты
- **PlatformIO**
  - URL: https://platformio.org/
  - IDE для embedded (VSCode extension)
  - FREE, open-source
  - Комментарий LUNA: "Arduino IDE хорошо для начала, PlatformIO — для серьёзных проектов. Season 6 Episode 21."

- **Fritzing**
  - URL: https://fritzing.org/
  - Схемы электроники, breadboard визуализация
  - FREE/Donate

#### Протоколы
- **Modbus Documentation**
  - URL: https://modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf
  - Industrial protocol (Season 6 Episode 22)
  - FREE PDF

- **MAVLink Protocol** (Drones/UAV)
  - URL: https://mavlink.io/en/
  - Micro Air Vehicle communication
  - FREE, open-source
  - Комментарий LUNA: "Season 6 — дроны, UAV. MAVLink — стандарт индустрии. C library доступна."

---

### ⚙️ Season 7: System Programming

#### Книги
- **"Advanced Programming in the UNIX Environment" (APUE)** (W. Richard Stevens, 2013)
  - 3rd edition, 1032 страницы
  - Библия UNIX programming
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Viktor держал эту книгу на столе. Processes, threads, IPC — всё здесь. Season 7 основан на APUE."

- **"The Linux Programming Interface"** (Michael Kerrisk, 2010)
  - 1552 страницы, самая детальная книга по Linux
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5

- **"The Design and Implementation of the FreeBSD Operating System"** (McKusick et al., 2014)
  - 928 страниц, internals FreeBSD
  - Уровень: Expert
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Season 7 поддерживает FreeBSD. Эта книга — must для понимания kernel."

#### Видео
- **MIT 6.828: Operating System Engineering**
  - URL: https://ocw.mit.edu/courses/6-828-operating-system-engineering-fall-2012/
  - Full course, FREE
  - Уровень: Advanced

#### Инструменты
- **strace / dtrace / dtruss**
  - Linux: `strace` (system call tracer)
  - FreeBSD: `dtrace`
  - macOS: `dtruss` (wrapper around dtrace)
  - Комментарий LUNA: "Season 7 Episode 25-28 — debugging system calls. `strace ./program` покажет всё."

- **perf (Linux Performance Tools)**
  - URL: https://perf.wiki.kernel.org/
  - CPU profiling, performance counters
  - Уровень: Advanced

#### Референсы
- **POSIX.1-2017 Specification**
  - URL: https://pubs.opengroup.org/onlinepubs/9699919799/
  - Официальная спецификация POSIX
  - FREE online

- **Linux man-pages**
  - URL: https://man7.org/linux/man-pages/
  - Все man pages онлайн
  - FREE, searchable

---

### 🤖 Season 8: AI & Data Science (LUNA AI Project!)

#### Книги
- **"The Art of Computer Programming" (TAOCP) Vol. 3** (Donald Knuth)
  - **КРИТИЧНО для Season 8 Episodes 29-30!**
  - Volume 3: Sorting and Searching
  - ~800 страниц (том 3)
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5
  - **Темы для S8:**
    - **Episode 29:** Heapsort (Section 5.2.3), Shell Sort (Section 5.2.1), priority queues
    - **Episode 30:** Knuth Shuffle (Section 3.4.2), Monte Carlo methods, random sampling
  - Комментарий LUNA: "Season 4 пропустили продвинутые алгоритмы Knuth. Season 8 — они здесь! Episode 29-30 основаны на TAOCP Vol. 3. Математика, доказательства, efficiency analysis — всё как завещал Великий Knuth."

- **"Programming Collective Intelligence"** (Toby Segaran, 2007)
  - Machine learning algorithms в Python
  - 334 страницы
  - Уровень: Intermediate
  - ⭐⭐⭐⭐☆ 4/5
  - Комментарий LUNA: "Python, но алгоритмы портируются на C. Season 8 Episode 31 — neural network from scratch на C."

- **"Neural Networks from Scratch"** (Harrison Kinsley & Daniel Kukiela, 2020)
  - Python, но математика универсальна
  - FREE online: https://nnfs.io/
  - Уровень: Intermediate

- **"Introduction to Statistical Learning"** (James, Witten, Hastie, Tibshirani, 2021)
  - FREE PDF: https://www.statlearning.com/
  - 607 страниц, R примеры
  - Уровень: Intermediate → Advanced
  - ⭐⭐⭐⭐⭐ 5/5

#### Видео
- **Stanford CS229: Machine Learning** (Andrew Ng)
  - URL: https://www.youtube.com/playlist?list=PLoROMvodv4rMiGQp3WXShtMGgzqpfVfbU
  - Full course, FREE
  - Уровень: Advanced

- **3Blue1Brown: Neural Networks Series**
  - URL: https://www.youtube.com/playlist?list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi
  - Визуальные объяснения backpropagation
  - FREE, 4 видео
  - Комментарий LUNA: "Лучшее объяснение neural networks визуально. Season 8 Episode 31 будет проще после просмотра."

#### Инструменты
- **BLAS / LAPACK** (Linear Algebra Libraries)
  - URL: http://www.netlib.org/blas/ , http://www.netlib.org/lapack/
  - C/Fortran библиотеки для matrix operations
  - FREE, open-source
  - Комментарий LUNA: "Neural networks = matrix multiplication. BLAS — оптимизированные операции."

- **Gnuplot**
  - URL: http://www.gnuplot.info/
  - Plotting из C программ
  - FREE, open-source
  - Комментарий LUNA: "Визуализация результатов ML. Season 8 Episodes 29-32."

#### Datasets
- **UCI Machine Learning Repository**
  - URL: https://archive.ics.uci.edu/
  - 600+ datasets для практики
  - FREE

- **Kaggle Datasets**
  - URL: https://www.kaggle.com/datasets
  - Тысячи datasets
  - FREE, registration required

---

### 🚀 Season 9: Advanced Systems

#### Книги
- **"The Art of Computer Programming" (TAOCP) Vol. 3** (Donald Knuth)
  - **КРИТИЧНО для Season 9 Episode 34!**
  - Volume 3: Sorting and Searching
  - ~800 страниц
  - Уровень: Advanced → Expert
  - ⭐⭐⭐⭐⭐ 5/5
  - **Темы для S9:**
    - **Episode 34:** AVL Trees (Section 6.2.3), B-Trees (Section 6.2.4), Advanced Hash Tables (Section 6.4), Merkle Trees
  - Комментарий LUNA: "Database Engine требует продвинутых структур данных. TAOCP Vol. 3 — библия balanced trees, hashing, searching. Episode 34 использует AVL, hash tables с open addressing, sentinel optimization. Knuth объясняет математику за O(log n) гарантиями."

- **"Crafting Interpreters"** (Robert Nystrom, 2021)
  - FREE online: https://craftinginterpreters.com/
  - Построение языка программирования с нуля
  - ~700 страниц
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Season 9 Episode 33 — interpreter. Эта книга — пошаговая инструкция. Два интерпретатора (tree-walk + bytecode)."

- **"Database Internals"** (Alex Petrov, 2019)
  - 373 страницы, как работают БД изнутри
  - O'Reilly
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Season 9 Episode 34 — database from scratch. B+Tree, WAL, MVCC — всё здесь."

- **"Quantum Computing: An Applied Approach"** (Jack Hidary, 2019)
  - 379 страниц, практический подход
  - Уровень: Advanced (requires math)
  - ⭐⭐⭐⭐☆ 4/5
  - Комментарий LUNA: "Season 9 Episode 37 — quantum simulation на C. Математика сложная, но C справится."

#### Видео
- **MIT 6.824: Distributed Systems**
  - URL: https://pdos.csail.mit.edu/6.824/
  - Full course, FREE
  - Уровень: Advanced

#### Инструменты
- **ncurses Library**
  - URL: https://invisible-island.net/ncurses/
  - Terminal UI framework
  - FREE, open-source
  - Комментарий LUNA: "Season 9 Episode 35 — TUI framework. ncurses — стандарт для terminal apps."

- **SQLite Source Code**
  - URL: https://www.sqlite.org/src/doc/trunk/README.md
  - ~150K строк C кода
  - FREE, public domain
  - Комментарий LUNA: "Лучший пример database engine на C. Season 9 Episode 34 — изучи архитектуру SQLite."

---

### 🌌 Season 10: Final Mission

#### Книги
- **"Fundamentals of Astrodynamics"** (Bate, Mueller, White, 1971)
  - Классика orbital mechanics
  - FREE: https://www.Dover.com или PDF онлайн
  - Уровень: Advanced (math-heavy)
  - ⭐⭐⭐⭐⭐ 5/5
  - Комментарий LUNA: "Season 10 Episode 42 — satellite tracking, orbital mechanics. Математика NASA."

- **"Performance Engineering of Software Systems"** (Leiserson, Bentley, 2018)
  - MIT course book
  - FREE: https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/
  - Уровень: Advanced
  - ⭐⭐⭐⭐⭐ 5/5

#### Видео
- **NASA: Orbital Mechanics Course**
  - URL: https://www.youtube.com/playlist?list=PLYu7z3I8tdEkUvuFXCLoSpwYJBUA0xY0k
  - FREE, university level

#### Инструменты
- **GNU Scientific Library (GSL)**
  - URL: https://www.gnu.org/software/gsl/
  - Numerical computations (vectors, matrices, integration)
  - FREE, open-source
  - Комментарий LUNA: "Season 10 — сложные вычисления. GSL — как NumPy для C."

- **SIMD Intrinsics Guide**
  - URL: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
  - CPU optimization (Season 10 Episode 41)
  - Уровень: Expert

---

## 🎮 Интерактивные платформы (общие)

### Hands-on Practice

#### **OverTheWire**
- **URL:** https://overthewire.org/wargames/
- **Темы:** Linux command line, security, networking
- **Wargames:**
  - **Bandit** — Linux basics (Season 1-2)
  - **Natas** — Web security (Season 3)
  - **Leviathan** — Binary exploitation (Season 7)
- **FREE:** Полностью бесплатно
- **Уровень:** Beginner → Advanced
- **Комментарий LUNA:** "Лучший способ научиться — делать. OverTheWire — это sandbox для экспериментов. Viktor прошёл все wargames."

#### **HackTheBox**
- **URL:** https://www.hackthebox.com/
- **Темы:** Pentesting, CTF, system exploitation
- **FREE tier:** Доступен (limited machines)
- **Уровень:** Intermediate → Advanced
- **Комментарий LUNA:** "После Season 7-8 попробуй HackTheBox. Реальные системы, реальные уязвимости (в sandbox)."

#### **Exercism (C Track)**
- **URL:** https://exercism.org/tracks/c
- **Описание:** 100+ упражнений по C, code review от менторов
- **FREE:** Полностью бесплатно
- **Уровень:** Beginner → Intermediate
- **Комментарий LUNA:** "Альтернатива Season 1-4. Упражнения + feedback от живых программистов."

---

## 📺 YouTube Каналы (рекомендованные)

### **Computerphile**
- **URL:** https://www.youtube.com/@Computerphile
- **Темы:** CS concepts, security, networking
- **Качество:** ⭐⭐⭐⭐⭐ 5/5
- **Комментарий LUNA:** "Профессора объясняют сложные темы просто. Pointers, TCP/IP, encryption — всё есть."

### **LiveOverflow**
- **URL:** https://www.youtube.com/@LiveOverflow
- **Темы:** Binary exploitation, reverse engineering
- **Уровень:** Advanced
- **Качество:** ⭐⭐⭐⭐⭐ 5/5
- **Комментарий LUNA:** "После Season 7 — про buffer overflows, memory corruption. Реалистичный подход."

### **Ben Eater**
- **URL:** https://www.youtube.com/@BenEater
- **Темы:** Computer architecture, electronics
- **Качество:** ⭐⭐⭐⭐⭐ 5/5
- **Комментарий LUNA:** "Как работает компьютер на уровне transistors. Season 6 (embedded) будет понятнее."

---

## 🔗 Полезные сайты и инструменты

### **Compiler Explorer (Godbolt)**
- **URL:** https://godbolt.org/
- **Описание:** Видишь Assembly код для своего C кода
- **FREE:** Полностью онлайн
- **Комментарий LUNA:** "Хочешь понять, что компилятор делает с твоим кодом? Godbolt покажет. Season 10 Episode 41 — optimization."

### **C Quiz (Deep C Secrets)**
- **URL:** http://www.gowrikumar.com/c/
- **Описание:** 16 вопросов, которые взорвут мозг
- **Уровень:** Advanced
- **Комментарий LUNA:** "Думаешь, знаешь C? Пройди C Quiz. Viktor ответил правильно на 12/16. Ты?"

### **Stack Overflow**
- **URL:** https://stackoverflow.com/questions/tagged/c
- **Описание:** Q&A для программистов
- **FREE:** Registration optional
- **Комментарий LUNA:** "Застрял? 90% вопросов уже задали до тебя. Научись искать и читать SO."

---

## 📋 Чеклисты для прогресса

### Season 1 Checklist
- [ ] Прочитать K&R Chapters 1-4
- [ ] Пройти CS50 Weeks 1-2
- [ ] Попрактиковаться на Learn-C.org (10+ exercises)
- [ ] Завершить MOONLIGHT Season 1 (Episodes 01-04)

### Season 2 Checklist
- [ ] Прочитать "Understanding and Using C Pointers" (или K&R Chapter 5)
- [ ] Пройти CS50 Week 4 (Memory)
- [ ] Установить Valgrind, найти 5+ memory leaks в примерах
- [ ] Завершить MOONLIGHT Season 2 (Episodes 05-08)

### Season 3 Checklist
- [ ] Прочитать Beej's Guide (весь или Chapters 1-8)
- [ ] Понять TCP handshake (визуализация)
- [ ] Попробовать tcpdump/Wireshark
- [ ] Завершить MOONLIGHT Season 3 (Episodes 09-12)

### Season 4 Checklist
- [ ] Изучить Big-O notation (CLRS Chapter 3 или онлайн)
- [ ] Визуализировать 5 sorting algorithms на VisuAlgo
- [ ] Попрактиковаться с HackerRank Data Structures (10+ tasks)
- [ ] Завершить MOONLIGHT Season 4 (Episodes 13-16)

### Seasons 5-10
- **Season 5:** Yale Financial Markets (audit) + Alpha Vantage API практика
- **Season 6:** Arduino tutorials + один embedded проект
- **Season 7:** Прочитать APUE Chapters 1-10 + strace практика
- **Season 8:** 📚 **TAOCP Vol. 3 Sections 5.2 (Heapsort, Shell Sort) + 3.4.2 (Knuth Shuffle)** + Neural Networks from Scratch (online) + один ML проект на C
- **Season 9:** 📚 **TAOCP Vol. 3 Section 6.2-6.4 (AVL, B-Trees, Hash Tables)** + "Crafting Interpreters" Part I + "Database Internals" Chapters 1-5
- **Season 10:** Orbital mechanics basics + GSL library практика

---

## 🎓 Философия обучения

### Принципы MOONLIGHT:
1. **70% Practice / 30% Theory** — делай, а не только читай
2. **Quality > Quantity** — одна хорошая книга > 10 посредственных
3. **Hands-on First** — пиши код до чтения теории
4. **Cumulative Learning** — каждый ресурс строится на предыдущих
5. **No Tutorial Hell** — не застревай в бесконечном обучении, создавай проекты

### Как использовать ресурсы:
- ✅ **MOONLIGHT Season X** → базовый материал (сюжет + практика)
- ✅ **Книга/Видео** → углубление теории (если застрял или хочешь больше)
- ✅ **Интерактивная платформа** → дополнительная практика (если мало заданий)
- ✅ **Референсы** → справочник (когда забыл синтаксис)

**Не пытайся изучить всё сразу!** Проходи Season → если интересна тема → читай рекомендованную книгу → продолжай дальше.

---

## 💬 Отзывы от Viktor (Easter Eggs)

> *"K&R — это как Библия. Читаешь каждый раз и находишь что-то новое."* — Viktor, 2019

> *"GDB спас мне жизнь 47 раз. Буквально. Однажды segfault случился во время операции..."* — Viktor, 2021

> *"Beej's Guide я читал в метро по дороге на первое собеседование. Получил offer."* — Viktor, 2017

> *"APUE — это не книга. Это grimoire. 1032 страницы магии UNIX."* — Viktor, 2020

> *"Не изучай Machine Learning, пока не поймёшь указатели. Иначе это просто магия."* — Viktor, 2022

---

## 📞 Обратная связь

Нашли отличный ресурс, которого нет в списке? Создайте Issue на GitHub!

**Критерии для добавления:**
- ✅ Высокое качество (university-level или expert-authored)
- ✅ Бесплатный или доступный (free tier / trial)
- ✅ Актуальный (2015+) или классика (вечная ценность)
- ✅ Практическая направленность (hands-on)

---

## 🎯 Итоговый совет от LUNA

> *"Ресурсов тысячи, но время одно. Выбери качественные, изучи глубоко, практикуй постоянно.
> Viktor стал экспертом не потому, что прочитал 100 книг, а потому что 5 книг прочитал 20 раз.
> Фокус > Широта."*

**Рекомендованный путь:**
1. **Пройди MOONLIGHT Season 1-4** (основа)
2. **Прочитай K&R** (классика)
3. **Практикуйся на OverTheWire** (hands-on)
4. **Углубляйся по интересам** (Season 5-10 + специализированные книги)
5. **Создавай проекты** (твой GitHub — это твой сертификат!)

---

*"Knowledge is power. But only if you know where to look."* — LUNA 💡

**Версия:** 1.0
**Дата:** 6 октября 2025
**Автор:** LUNA AI (Logic Understanding & Neural Algorithm)
**Курс:** OPERATION MOONLIGHT v0.3.5
