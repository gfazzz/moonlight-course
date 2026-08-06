# Программа курса

**OPERATION MOONLIGHT — 10 сезонов, 80 серий, ~74 часа.**

Это указатель: одна строка на серию, ссылка на неё, ключевой концепт и артефакт. Подробности живут в самих сериях — `README.md` (сюжет и теория), `mission.md` (техзадание) и `theory.md` (разбор вглубь).

---

## Как устроен курс

Каждая серия — один концепт, одна задача, 25–110 минут. Внутри каталога серии:

| Файл | Что содержит |
|---|---|
| `README.md` | сцена, теория, разбор ошибок, итог |
| `mission.md` | техзадание и критерии приёмки |
| `theory.md` | углублённый разбор и книги |
| `starter.c` | каркас (степень готовности зависит от сезона) |
| `artifacts/` | **ваш код** |
| `solution/` | эталон — открывать после честной попытки |
| `tests/` | `test.sh` и `expected.txt` |

```bash
cd season-01-foundations/s01e01-strange-message
cp starter.c artifacts/bytecheck.c
make test
```

Из корня репозитория: `make test` — все 80 серий, `make progress` — сколько пройдено, `make projects` — сквозные проекты сезонов.

---

## Прогрессивная автономия

Главный принцип курса: помощь убывает по мере роста самостоятельности.

| Сезоны | Что даёт каркас |
|---|---|
| S1–S4 | полный каркас с пошаговыми комментариями |
| S5–S6 | каркас с указаниями, что делать в каждом месте |
| S7–S8 | только сигнатуры функций |
| S9 | только интерфейс; спецификация — `mission.md` и формат вывода |
| S10 | только требования; структуру кода выбираете сами |

К десятому сезону вы читаете техзадание и строите решение сами. Это и есть цель.

---

## Сквозные правила

**Флаги компиляции.** `-Wall -Wextra -Werror -std=c11`; `-std=gnu11` там, где нужен POSIX, `-pthread` — где потоки.

**Никаких внешних библиотек, включая `-lm`.** `sqrt`, `exp`, `sin`, `cos`, генератор случайных чисел написаны вручную. Причина не в аскезе, а в воспроизводимости: библиотечные реализации расходятся в последнем бите между платформами, и тесты перестали бы быть детерминированными.

**Тесты проверяют свойства, а не только строки.** Отсортированность обхода дерева, сохранение инварианта при сбое в любой точке, совпадение параллельного счёта с последовательным, отсутствие срабатываний санитайзера, арифметическое сведение баланса.

**Ваш код в `artifacts/` отслеживается git.** Работайте в своей ветке; правки курса — веткой от чистого `main`. Подробности — в [CONTRIBUTING.md](../CONTRIBUTING.md).

**LUNA работает в режиме `Ask`, а не `Agent`.** В режиме агента ассистент правит файлы сам: тест позеленеет, а вы не напишете ни строки.

---

## Переиспользование кода

В сезонах S1–S3 итогом является артефакт финальной серии. Начиная с Season 4 появляется отдельный каталог `project/`, который собирается из модулей **предыдущих** сезонов, а тест проверяет факт подключения, а не копирования.

| Сезон | Проект | Собирается из модулей |
|---|---|---|
| S1 | `moonlight_decoder` | финал серии |
| S2 | `fixed_logger` | финал серии |
| S3 | `intercept` | финал серии |
| S4 | `crypto_toolkit` | S1 + S4 |
| S5 | `trading_system` | S4 + S5 |
| S6 | `iot_controller` | S4 + S6 |
| S7 | `stealth_agent` | S1 + S6 + S7 |
| S8 | `luna_ai` | S4 + S8 |
| S9 | `advanced_systems` | S4 + S6 + S9 |
| S10 | `moonlight_core` | S4 + S6 + S8 + S9 |

Подробнее — [PROJECTS.md](PROJECTS.md).

---

## Хронометраж

| Сезон | Часов | Сезон | Часов |
|---|---|---|---|
| S1 Foundations | 5.5 | S6 Embedded & IoT | 6.2 |
| S2 Memory & Pointers | 5.0 | S7 Systems Programming | 7.2 |
| S3 Networks | 5.6 | S8 AI & Data Science | 7.7 |
| S4 Crypto & Algorithms | 6.2 | S9 Advanced Topics | 12.0 |
| S5 Financial Markets | 6.2 | S10 Final Mission | 12.2 |

**Итого ~74 часа** чистого времени на задачи. С чтением теории и разбором эталонов — вдвое больше.

Рост во второй половине не случаен: с Season 9 подсказки исчезают, и время уходит на проектирование, которого раньше не требовалось.

---

## Все 80 серий

### Season 1 — Foundations

> *«Всё начинается с простого. Но ничто не кончается так, как начиналось»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s01e01](../season-01-foundations/s01e01-strange-message/) | Первый байт | биты, hex, XOR одного байта | `bytecheck.c` |
| [s01e02](../season-01-foundations/s01e02-full-message/) | Всё сообщение | массивы, `sizeof`, `'\0'` | `decoder.c` |
| [s01e03](../season-01-foundations/s01e03-chain-of-clues/) | Цепочка зацепок | ветвление, валидация | `classifier.c` |
| [s01e04](../season-01-foundations/s01e04-patterns-in-time/) | Паттерны во времени | циклы, поиск, интервалы | `pattern_finder.c` |
| [s01e05](../season-01-foundations/s01e05-functions/) | Функции | функции как контракт | `decoder.c` на функциях |
| [s01e06](../season-01-foundations/s01e06-modules/) | Модули и сборка | `.h`/`.c`, линковка | модуль `crypto` |
| [s01e07](../season-01-foundations/s01e07-files-and-args/) | Файлы и аргументы | `fopen`/`fread`, `argv` | `decoder.c` из файла |
| [s01e08](../season-01-foundations/s01e08-assemble-tool/) | Сборка инструмента | `struct`, Makefile | **`moonlight_decoder`** |

### Season 2 — Memory & Pointers

> *«Знать, где хранится информация, — половина успеха»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s02e01](../season-02-memory-and-pointers/s02e01-memory-map/) | Карта памяти | адреса, `sizeof`, ASLR | `memmap.c` |
| [s02e02](../season-02-memory-and-pointers/s02e02-pointers/) | Указатели | `&`, `*`, `NULL` | `swap.c` |
| [s02e03](../season-02-memory-and-pointers/s02e03-pointer-arithmetic/) | Арифметика указателей | `p[i] ≡ *(p+i)`, разность | `ptr_arith.c` |
| [s02e04](../season-02-memory-and-pointers/s02e04-chain-navigation/) | Навигация по цепочке | `->`, связанные данные | `navigator.c` |
| [s02e05](../season-02-memory-and-pointers/s02e05-heap/) | Куча | `malloc`/`free` | `heap.c` |
| [s02e06](../season-02-memory-and-pointers/s02e06-realloc/) | Растущие буферы | `realloc`, удвоение | `grow.c` |
| [s02e07](../season-02-memory-and-pointers/s02e07-memory-errors/) | Ошибки памяти | четыре класса, ASan | `safe.c` |
| [s02e08](../season-02-memory-and-pointers/s02e08-leak-hunt/) | Охота на утечки | владение, освобождение | **`fixed_logger.c`** |

### Season 3 — Networks

> *«Всё связано. Нужно лишь найти нить»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s03e01](../season-03-networks/s03e01-ip-addressing/) | IP-адресация | адрес как 32-битное число | `ipinfo.c` |
| [s03e02](../season-03-networks/s03e02-subnets-cidr/) | Подсети и CIDR | маска, свой/чужой | `subnet.c` |
| [s03e03](../season-03-networks/s03e03-byte-order/) | Порядок байтов | сетевое представление | `byteorder.c` |
| [s03e04](../season-03-networks/s03e04-sockets/) | Сокеты | TCP как поток | `echo.c` |
| [s03e05](../season-03-networks/s03e05-packet-structure/) | Структура пакета | разбор заголовков | `dissect.c` |
| [s03e06](../season-03-networks/s03e06-checksum/) | Контрольная сумма | целостность vs подлинность | `checksum.c` |
| [s03e07](../season-03-networks/s03e07-reassembly/) | Сборка потока | позиция вместо очереди | `reassemble.c` |
| [s03e08](../season-03-networks/s03e08-encrypted-channel/) | Шифрованный канал | конвейер целиком | **`intercept.c`** |

### Season 4 — Crypto & Algorithms

> *«Секреты защищены не замками, а математикой. Плохая математика — плохой замок»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s04e01](../season-04-crypto-and-algorithms/s04e01-caesar-cipher/) | Шифр Цезаря | подстановка, пространство ключей | `caesar.c` |
| [s04e02](../season-04-crypto-and-algorithms/s04e02-frequency-analysis/) | Частотный анализ | хи-квадрат, взлом статистикой | `freq.c` |
| [s04e03](../season-04-crypto-and-algorithms/s04e03-hash-functions/) | Хеш-функции | djb2, FNV-1a, лавина | `hashes.c` |
| [s04e04](../season-04-crypto-and-algorithms/s04e04-hash-table/) | Хеш-таблица | адрес вместо поиска | `hashtable.c` |
| [s04e05](../season-04-crypto-and-algorithms/s04e05-sorting/) | Сортировки | O(n²) против O(n log n) | `sorts.c` |
| [s04e06](../season-04-crypto-and-algorithms/s04e06-binary-search/) | Двоичный поиск | отсекать половину | `bsearch.c` |
| [s04e07](../season-04-crypto-and-algorithms/s04e07-linked-list/) | Связный список | три указателя, цикл Флойда | `list.c` |
| [s04e08](../season-04-crypto-and-algorithms/s04e08-blockchain/) | Блокчейн | цепь хешей, PoW | **`blockchain.c`** |

### Season 5 — Financial Markets

> *«Число без процедуры, которой оно получено, не значит ничего»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s05e01](../season-05-financial-markets/s05e01-ohlcv-parser/) | Первая свеча | OHLCV, инвариант, битые данные | `parse.c` |
| [s05e02](../season-05-financial-markets/s05e02-fixed-point-money/) | Деньги не бывают double | фиксированная точка, округление | `money.c` |
| [s05e03](../season-05-financial-markets/s05e03-moving-averages/) | Скользящие средние | сглаживание и запаздывание | `ma.c` |
| [s05e04](../season-05-financial-markets/s05e04-rsi/) | RSI | сила движения, расхождение | `rsi.c` |
| [s05e05](../season-05-financial-markets/s05e05-backtest/) | Бэктест и подгонка | проверка вне выборки | `backtest.c` |
| [s05e06](../season-05-financial-markets/s05e06-volatility-var/) | Волатильность и VaR | риск как распределение | `var.c` |
| [s05e07](../season-05-financial-markets/s05e07-monte-carlo/) | Монте-Карло | моделирование, воспроизводимость | `montecarlo.c` |
| [s05e08](../season-05-financial-markets/s05e08-arbitrage/) | Арбитраж и разгон | схема как совпадение признаков | **`arbitrage.c`** |

### Season 6 — Embedded & IoT

> *«Между битом и физическим миром — один вольт разницы»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s06e01](../season-06-embedded-iot/s06e01-gpio-registers/) | Регистры | бит как физическое состояние | `gpio.c` |
| [s06e02](../season-06-embedded-iot/s06e02-uart/) | Провод | кадр, скорость, чётность | `uart.c` |
| [s06e03](../season-06-embedded-iot/s06e03-i2c-spi/) | Шина | адресация, карта регистров | `i2c.c` |
| [s06e04](../season-06-embedded-iot/s06e04-wiegand-access/) | Карта доступа | разбор поля бит, двойная чётность | `wiegand.c` |
| [s06e05](../season-06-embedded-iot/s06e05-modbus/) | Промышленный протокол | контрольная сумма, порядок байтов | `modbus.c` |
| [s06e06](../season-06-embedded-iot/s06e06-sensors-fsm/) | Датчики | кольцевой буфер, автомат, гистерезис | `sensors.c` |
| [s06e07](../season-06-embedded-iot/s06e07-ip-cameras/) | Видеонаблюдение | разность кадров, цена порога | `motion.c` |
| [s06e08](../season-06-embedded-iot/s06e08-mavlink-drone/) | Телеметрия | кадр MAVLink, фиксированная точка | **`mavlink.c`** |

### Season 7 — Systems Programming

> *«В тени системы живут процессы. Мы станем одним из них»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s07e01](../season-07-system-programming/s07e01-processes-fork/) | Раздвоение | `fork`, ожидание, зомби | `fork.c` |
| [s07e02](../season-07-system-programming/s07e02-exec/) | Подмена | `exec`, коды возврата | `exec.c` |
| [s07e03](../season-07-system-programming/s07e03-daemons/) | Жизнь без терминала | сессии, демонизация | `daemon.c` |
| [s07e04](../season-07-system-programming/s07e04-threads/) | Одновременность | потоки, воспроизводимость | `threads.c` |
| [s07e05](../season-07-system-programming/s07e05-race-mutex/) | Потерянное обновление | гонка, мьютекс | `race.c` |
| [s07e06](../season-07-system-programming/s07e06-pipes-ipc/) | Канал | `pipe`, `dup2`, конвейер | `pipes.c` |
| [s07e07](../season-07-system-programming/s07e07-mmap-shared/) | Общая страница | `mmap`, общее и приватное | `shared.c` |
| [s07e08](../season-07-system-programming/s07e08-signals-stealth/) | Оклик | сигналы, чистый выход | **`signals.c`** |

### Season 8 — AI & Data Science

> *«В данных скрыта истина. Нужно лишь суметь её достать — и не обмануть себя по дороге»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s08e01](../season-08-ai-and-data/s08e01-bigdata-mmap/) | Слишком много | отображение, один проход, Уэлфорд | `bigdata.c` |
| [s08e02](../season-08-ai-and-data/s08e02-statistics-outliers/) | Что считать аномалией | медиана, размах, маскировка | `stats.c` |
| [s08e03](../season-08-ai-and-data/s08e03-prng-shuffle/) | Честная монета | смещение тасования, отбраковка | `shuffle.c` |
| [s08e04](../season-08-ai-and-data/s08e04-shell-heapsort/) | Порядок | шаги Кнута, куча в массиве | `sorts.c` |
| [s08e05](../season-08-ai-and-data/s08e05-linear-regression/) | Прямая | градиентный спуск, скорость обучения | `regression.c` |
| [s08e06](../season-08-ai-and-data/s08e06-perceptron/) | Один нейрон | сигмоида, кросс-энтропия, предел | `perceptron.c` |
| [s08e07](../season-08-ai-and-data/s08e07-backprop/) | Ошибка идёт назад | скрытый слой, цепное правило | `backprop.c` |
| [s08e08](../season-08-ai-and-data/s08e08-luna-ai/) | luna_ai | полный цикл, честные метрики | **`luna_ai.c`** |

### Season 9 — Advanced Topics

> *«Ты научился пользоваться чужими системами. Теперь придётся строить свои»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s09e01](../season-09-advanced-systems/s09e01-lexer/) | Слова | символы в токены, максимальное совпадение | `lexer.c` |
| [s09e02](../season-09-advanced-systems/s09e02-parser-ast/) | Структура | рекурсивный спуск, приоритеты | `parser.c` |
| [s09e03](../season-09-advanced-systems/s09e03-interpreter/) | Смысл | окружение, отложенное вычисление | `interp.c` |
| [s09e04](../season-09-advanced-systems/s09e04-btree-storage/) | Страницы | B-дерево, расщепление, чтения | `btree.c` |
| [s09e05](../season-09-advanced-systems/s09e05-wal-durability/) | Намерение | журнал, точка фиксации, откат | `wal.c` |
| [s09e06](../season-09-advanced-systems/s09e06-tui/) | Экран | буфер кадра, отправка разницы | `tui.c` |
| [s09e07](../season-09-advanced-systems/s09e07-radar-kalman/) | Кому верить | оценивание при шуме, ковариация | `kalman.c` |
| [s09e08](../season-09-advanced-systems/s09e08-quantum/) | Два состояния сразу | суперпозиция, интерференция, Гровер | **`quantum.c`** |

### Season 10 — Final Mission

> *«Десять сезонов ты строил инструменты. Сегодня они отвечают за результат»*

| ID | Название | Концепт | Артефакт |
|---|---|---|---|
| [s10e01](../season-10-final-mission/s10e01-architecture/) | Порядок сборки | слои, топологическая сортировка | `core.c` |
| [s10e02](../season-10-final-mission/s10e02-orbital/) | Небесная механика | уравнение Кеплера, Ньютон | `orbit.c` |
| [s10e03](../season-10-final-mission/s10e03-comm-window/) | Двенадцать минут | видимость, бисекция, Доплер | `window.c` |
| [s10e04](../season-10-final-mission/s10e04-profiling/) | Где на самом деле время | горячий путь, закон Амдала | `profile.c` |
| [s10e05](../season-10-final-mission/s10e05-hardening/) | Враждебный эфир | валидация, санитайзеры | `harden.c` |
| [s10e06](../season-10-final-mission/s10e06-dashboard/) | Один экран | модель и представление, возраст данных | `dashboard.c` |
| [s10e07](../season-10-final-mission/s10e07-pipeline/) | Ничего не потерялось | сведение баланса | `pipeline.c` |
| [s10e08](../season-10-final-mission/s10e08-final-operation/) | Восемь минут сорок четыре секунды | приёмка по критериям | **`operation.c`** |


---

## Что читать рядом

| Документ | О чём |
|---|---|
| [SCENARIO.md](SCENARIO.md) | сюжет, персонажи, хронология, кроссовер |
| [LOCATIONS.md](LOCATIONS.md) | география курса и реальные координаты |
| [THEORY_MAP.md](THEORY_MAP.md) | указатель тем: где что разбирается |
| [PROJECTS.md](PROJECTS.md) | сквозные проекты и переиспользование модулей |
| [RESOURCES.md](RESOURCES.md) | книги и материалы по темам курса |
| [CONTRIBUTING.md](../CONTRIBUTING.md) | как устроен репозиторий и как вносить правки |
