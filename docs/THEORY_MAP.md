# Карта теории

**Указатель: где что разбирается.** Ищете конкретную тему — здесь ссылка на серию, в которой она объясняется, и на те, где возвращается.

Полный список серий — в [CURRICULUM.md](CURRICULUM.md). Разбор каждой темы — в `theory.md` соответствующей серии.

---

## Язык C

### Данные и типы

| Тема | Где | Возвращается |
|---|---|---|
| бит, байт, шестнадцатеричная запись | [s01e01](../season-01-foundations/s01e01-strange-message/) | s06e01 |
| `char` против `unsigned char` | [s01e01](../season-01-foundations/s01e01-strange-message/) | s06e07 |
| массивы, `sizeof` и его ловушка | [s01e02](../season-01-foundations/s01e02-full-message/) | s02e03 |
| нуль-терминатор строк | [s01e02](../season-01-foundations/s01e02-full-message/) | s10e05 |
| `struct` и компоновка полей | [s01e08](../season-01-foundations/s01e08-assemble-tool/) | s03e05 |
| перечисления вместо флагов | [s06e06](../season-06-embedded-iot/s06e06-sensors-fsm/) | s10e07 |
| целочисленное переполнение | [s01e02](../season-01-foundations/s01e02-full-message/) | s08e01, s10e05 |
| знаковость при разборе данных | [s06e08](../season-06-embedded-iot/s06e08-mavlink-drone/) | s09e01 |
| сравнение вещественных чисел | [s01e03](../season-01-foundations/s01e03-chain-of-clues/) | s05e02 |
| порядок байтов | [s03e03](../season-03-networks/s03e03-byte-order/) | s06e05, s06e08 |
| фиксированная точка вместо плавающей | [s06e08](../season-06-embedded-iot/s06e08-mavlink-drone/) | — |

### Управление и функции

| Тема | Где |
|---|---|
| ветвление, истинность в C, сокращённое вычисление | [s01e03](../season-01-foundations/s01e03-chain-of-clues/) |
| циклы и поиск | [s01e04](../season-01-foundations/s01e04-patterns-in-time/) |
| функция как контракт | [s01e05](../season-01-foundations/s01e05-functions/) |
| `.h` и `.c`, компоновка, `static` | [s01e06](../season-01-foundations/s01e06-modules/) |
| рекурсия и условие уменьшения | [s09e02](../season-09-advanced-systems/s09e02-parser-ast/) |
| указатели на функции | [s10e01](../season-10-final-mission/s10e01-architecture/) |

### Память

| Тема | Где | Возвращается |
|---|---|---|
| адресное пространство, ASLR | [s02e01](../season-02-memory-and-pointers/s02e01-memory-map/) | s07e01 |
| указатели: `&`, `*`, `NULL` | [s02e02](../season-02-memory-and-pointers/s02e02-pointers/) | — |
| арифметика указателей | [s02e03](../season-02-memory-and-pointers/s02e03-pointer-arithmetic/) | — |
| куча: `malloc` и `free` | [s02e05](../season-02-memory-and-pointers/s02e05-heap/) | — |
| `realloc` и стратегия удвоения | [s02e06](../season-02-memory-and-pointers/s02e06-realloc/) | — |
| четыре класса ошибок памяти | [s02e07](../season-02-memory-and-pointers/s02e07-memory-errors/) | s10e05 |
| владение и время жизни | [s02e08](../season-02-memory-and-pointers/s02e08-leak-hunt/) | s09e03 |
| статический пул вместо `malloc` | [s09e02](../season-09-advanced-systems/s09e02-parser-ast/) | s10e04 |
| отображение файла в память | [s07e07](../season-07-system-programming/s07e07-mmap-shared/) | s08e01 |
| локальность и кэш | [s08e04](../season-08-ai-and-data/s08e04-shell-heapsort/) | s10e04 |

### Ввод-вывод

| Тема | Где |
|---|---|
| `fopen`, `fread`, аргументы командной строки | [s01e07](../season-01-foundations/s01e07-files-and-args/) |
| буферизация вывода и её режимы | [s07e01](../season-07-system-programming/s07e01-processes-fork/) |
| дескрипторы и их наследование | [s07e02](../season-07-system-programming/s07e02-exec/) |
| потоковое чтение: `read` возвращает меньше | [s07e06](../season-07-system-programming/s07e06-pipes-ipc/) |

---

## Алгоритмы и структуры данных

| Тема | Где | Возвращается |
|---|---|---|
| двоичный поиск | [s04e06](../season-04-crypto-and-algorithms/s04e06-binary-search/) | s10e03 |
| сортировки: `O(n²)` против `O(n log n)` | [s04e05](../season-04-crypto-and-algorithms/s04e05-sorting/) | s08e04 |
| сортировка Шелла, шаги Кнута | [s08e04](../season-08-ai-and-data/s08e04-shell-heapsort/) | — |
| пирамидальная сортировка, куча | [s08e04](../season-08-ai-and-data/s08e04-shell-heapsort/) | — |
| хеш-функции и лавинный эффект | [s04e03](../season-04-crypto-and-algorithms/s04e03-hash-functions/) | s10e08 |
| хеш-таблица и коллизии | [s04e04](../season-04-crypto-and-algorithms/s04e04-hash-table/) | s09e03 |
| связный список, цикл Флойда | [s04e07](../season-04-crypto-and-algorithms/s04e07-linked-list/) | — |
| кольцевой буфер | [s06e06](../season-06-embedded-iot/s06e06-sensors-fsm/) | s08e01 |
| B-дерево и страницы | [s09e04](../season-09-advanced-systems/s09e04-btree-storage/) | — |
| синтаксическое дерево и обходы | [s09e02](../season-09-advanced-systems/s09e02-parser-ast/) | — |
| топологическая сортировка | [s10e01](../season-10-final-mission/s10e01-architecture/) | — |
| автомат состояний | [s06e06](../season-06-embedded-iot/s06e06-sensors-fsm/) | s09e01 |
| перемешивание Фишера — Йетса | [s08e03](../season-08-ai-and-data/s08e03-prng-shuffle/) | — |
| резервуарная выборка | [s08e01](../season-08-ai-and-data/s08e01-bigdata-mmap/) | s08e03 |

---

## Сети и протоколы

| Тема | Где |
|---|---|
| IP-адрес как 32-битное число | [s03e01](../season-03-networks/s03e01-ip-addressing/) |
| маски, CIDR, «свой или чужой» | [s03e02](../season-03-networks/s03e02-subnets-cidr/) |
| сокеты, TCP как поток без границ | [s03e04](../season-03-networks/s03e04-sockets/) |
| разбор заголовков пакета | [s03e05](../season-03-networks/s03e05-packet-structure/) |
| контрольная сумма: целостность против подлинности | [s03e06](../season-03-networks/s03e06-checksum/) |
| сборка потока по позициям | [s03e07](../season-03-networks/s03e07-reassembly/) |
| UART: кадр, скорость, чётность | [s06e02](../season-06-embedded-iot/s06e02-uart/) |
| I²C и SPI: шина и адресация | [s06e03](../season-06-embedded-iot/s06e03-i2c-spi/) |
| Wiegand: разбор поля бит | [s06e04](../season-06-embedded-iot/s06e04-wiegand-access/) |
| Modbus RTU и CRC-16 | [s06e05](../season-06-embedded-iot/s06e05-modbus/) |
| MAVLink и проверка версии суммой | [s06e08](../season-06-embedded-iot/s06e08-mavlink-drone/) |
| доплеровский сдвиг в канале связи | [s10e03](../season-10-final-mission/s10e03-comm-window/) |

---

## Криптография

| Тема | Где | Возвращается |
|---|---|---|
| XOR: свойства и стойкость однобайтового ключа | [s01e01](../season-01-foundations/s01e01-strange-message/) | s04e01 |
| шифр Цезаря, пространство ключей | [s04e01](../season-04-crypto-and-algorithms/s04e01-caesar-cipher/) | — |
| частотный анализ, хи-квадрат | [s04e02](../season-04-crypto-and-algorithms/s04e02-frequency-analysis/) | — |
| хеш-функции: djb2, FNV-1a, лавина | [s04e03](../season-04-crypto-and-algorithms/s04e03-hash-functions/) | s10e08 |
| блокчейн как цепочка хешей | [s04e08](../season-04-crypto-and-algorithms/s04e08-blockchain/) | — |
| целостность против подлинности | [s03e06](../season-03-networks/s03e06-checksum/) | s06e05 |
| постквантовая перспектива, Шор и Гровер | [s09e08](../season-09-advanced-systems/s09e08-quantum/) | — |
| сравнение секретов за постоянное время | [s10e05](../season-10-final-mission/s10e05-hardening/) | — |

---

## Системное программирование

| Тема | Где |
|---|---|
| процесс, `fork`, копирование при записи | [s07e01](../season-07-system-programming/s07e01-processes-fork/) |
| зомби и сироты | [s07e01](../season-07-system-programming/s07e01-processes-fork/) |
| семейство `exec`, коды возврата | [s07e02](../season-07-system-programming/s07e02-exec/) |
| сессии, группы, демонизация | [s07e03](../season-07-system-programming/s07e03-daemons/) |
| потоки POSIX | [s07e04](../season-07-system-programming/s07e04-threads/) |
| состояние гонки и мьютексы | [s07e05](../season-07-system-programming/s07e05-race-mutex/) |
| каналы, `dup2`, конвейеры | [s07e06](../season-07-system-programming/s07e06-pipes-ipc/) |
| разделяемая память, межпроцессный мьютекс | [s07e07](../season-07-system-programming/s07e07-mmap-shared/) |
| сигналы, чистое завершение | [s07e08](../season-07-system-programming/s07e08-signals-stealth/) |
| сырой режим терминала | [s09e06](../season-09-advanced-systems/s09e06-tui/) |
| регистры и работа с железом | [s06e01](../season-06-embedded-iot/s06e01-gpio-registers/) |

---

## Данные, статистика, модели

| Тема | Где | Возвращается |
|---|---|---|
| однопроходные алгоритмы, метод Уэлфорда | [s08e01](../season-08-ai-and-data/s08e01-bigdata-mmap/) | — |
| катастрофическое вычитание | [s08e01](../season-08-ai-and-data/s08e01-bigdata-mmap/) | s10e02 |
| устойчивые оценки, точка пробоя | [s08e02](../season-08-ai-and-data/s08e02-statistics-outliers/) | — |
| квантили, размах, правило Тьюки | [s08e02](../season-08-ai-and-data/s08e02-statistics-outliers/) | — |
| генераторы случайных чисел и их слабости | [s08e03](../season-08-ai-and-data/s08e03-prng-shuffle/) | s05e07 |
| градиентный спуск | [s08e05](../season-08-ai-and-data/s08e05-linear-regression/) | s10e02 |
| сигмоида и кросс-энтропия | [s08e06](../season-08-ai-and-data/s08e06-perceptron/) | — |
| линейная разделимость, предел перцептрона | [s08e06](../season-08-ai-and-data/s08e06-perceptron/) | — |
| обратное распространение ошибки | [s08e07](../season-08-ai-and-data/s08e07-backprop/) | — |
| метрики классификации, F-мера | [s08e08](../season-08-ai-and-data/s08e08-luna-ai/) | s10e08 |
| деньги в фиксированной точке, округление | [s05e02](../season-05-real-world-data/s05e02-fixed-point-money/) | s05e05, s10e07 |
| конечный автомат разбора, битые данные | [s05e03](../season-05-real-world-data/s05e03-csv-parser/) | s08e02, s09e01, s10e07 |
| эпоха, часовые пояса, разрывы шкалы | [s05e04](../season-05-real-world-data/s05e04-time-zones/) | s10e03 |
| подгонка и проверка вне выборки | [s05e05](../season-05-real-world-data/s05e05-backtest/) | s08e03, s08e08 |
| бинарный формат, выравнивание, порядок байтов | [s05e06](../season-05-real-world-data/s05e06-tick-format/) | s06e05, s06e08, s09e04, s10e07 |
| Монте-Карло и воспроизводимость | [s05e07](../season-05-real-world-data/s05e07-monte-carlo/) | s07e04 |
| UTF-8, ширина текста, двойники | [s05e08](../season-05-real-world-data/s05e08-unicode-names/) | s09e06, s10e06 |
| фильтр Калмана, ковариация | [s09e07](../season-09-advanced-systems/s09e07-radar-kalman/) | — |

---

## Компиляторы, хранилища, интерфейсы

| Тема | Где |
|---|---|
| лексический анализ, максимальное совпадение | [s09e01](../season-09-advanced-systems/s09e01-lexer/) |
| рекурсивный спуск, приоритеты, ассоциативность | [s09e02](../season-09-advanced-systems/s09e02-parser-ast/) |
| окружение, отложенное вычисление ветвей | [s09e03](../season-09-advanced-systems/s09e03-interpreter/) |
| страничная организация хранилища | [s09e04](../season-09-advanced-systems/s09e04-btree-storage/) |
| журнал упреждающей записи, откат и повтор | [s09e05](../season-09-advanced-systems/s09e05-wal-durability/) |
| буфер кадра, отправка разницы | [s09e06](../season-09-advanced-systems/s09e06-tui/) |
| модель и представление, возраст данных | [s10e06](../season-10-final-mission/s10e06-dashboard/) |
| модульная архитектура и слои | [s10e01](../season-10-final-mission/s10e01-architecture/) |

---

## Физика и математика

| Тема | Где |
|---|---|
| законы Кеплера, уравнение Кеплера | [s10e02](../season-10-final-mission/s10e02-orbital/) |
| метод Ньютона и его сходимость | [s10e02](../season-10-final-mission/s10e02-orbital/) |
| деление отрезка пополам | [s10e03](../season-10-final-mission/s10e03-comm-window/) |
| геометрия видимости спутника | [s10e03](../season-10-final-mission/s10e03-comm-window/) |
| квантовые состояния и гейты | [s09e08](../season-09-advanced-systems/s09e08-quantum/) |
| интерференция и алгоритм Гровера | [s09e08](../season-09-advanced-systems/s09e08-quantum/) |

---

## Сквозные инженерные принципы

Эти линии тянутся через весь курс и возвращаются в разных обличьях. Именно они, а не синтаксис, составляют содержание курса.

### Проверять до использования

| Где | В какой форме |
|---|---|
| [s01e02](../season-01-foundations/s01e02-full-message/) | контрольная сумма перед разбором |
| [s03e06](../season-03-networks/s03e06-checksum/) | целостность против подлинности |
| [s06e04](../season-06-embedded-iot/s06e04-wiegand-access/) | чётность до извлечения номера |
| [s06e05](../season-06-embedded-iot/s06e05-modbus/) | сумма до чтения полей |
| [s06e08](../season-06-embedded-iot/s06e08-mavlink-drone/) | сумма и версия протокола |
| [s10e05](../season-10-final-mission/s10e05-hardening/) | граница доверия, безопасная арифметика |

### Инвариант вместо значения

| Где | Что проверяется |
|---|---|
| [s05e01](../season-05-real-world-data/s05e01-ohlcv-parser/) | инвариант свечи |
| [s07e01](../season-07-system-programming/s07e01-processes-fork/) | свойства вместо идентификаторов процессов |
| [s07e05](../season-07-system-programming/s07e05-race-mutex/) | итог под мьютексом точно равен ожидаемому |
| [s09e05](../season-09-advanced-systems/s09e05-wal-durability/) | инвариант держится при сбое в любой точке |
| [s10e02](../season-10-final-mission/s10e02-orbital/) | энергия и момент импульса |
| [s10e07](../season-10-final-mission/s10e07-pipeline/) | принято плюс отвергнуто равно поданному |
| [s10e08](../season-10-final-mission/s10e08-final-operation/) | свойство вместо константы |

### Воспроизводимость важнее удобства

| Где | Что делается вручную |
|---|---|
| [s05e04](../season-05-real-world-data/s05e04-time-zones/) | календарь и часовые пояса без `<time.h>` |
| [s05e08](../season-05-real-world-data/s05e08-unicode-names/) | разбор UTF-8 и ширина текста без библиотеки |
| [s05e07](../season-05-real-world-data/s05e07-monte-carlo/) | собственный генератор случайных чисел |
| [s07e04](../season-07-system-programming/s07e04-threads/) | сид от номера задачи, а не общий генератор |
| [s08e06](../season-08-ai-and-data/s08e06-perceptron/) | `exp` рядом с приведением аргумента |
| [s10e02](../season-10-final-mission/s10e02-orbital/) | тригонометрия рядами |

### Измерять, а не предполагать

| Где | Что измеряется |
|---|---|
| [s08e04](../season-08-ai-and-data/s08e04-shell-heapsort/) | операции, а не время |
| [s09e04](../season-09-advanced-systems/s09e04-btree-storage/) | чтения страниц, а не сравнения |
| [s09e06](../season-09-advanced-systems/s09e06-tui/) | байты в терминал |
| [s10e04](../season-10-final-mission/s10e04-profiling/) | горячий путь и закон Амдала |

### Деградация вместо падения

| Где | В какой форме |
|---|---|
| [s06e06](../season-06-embedded-iot/s06e06-sensors-fsm/) | отказ датчика как отдельное состояние |
| [s07e02](../season-07-system-programming/s07e02-exec/) | провал запуска — обычная ошибка |
| [s09e01](../season-09-advanced-systems/s09e01-lexer/) | восстановление после неизвестного символа |
| [s10e01](../season-10-final-mission/s10e01-architecture/) | каскадная блокировка зависимых модулей |
| [s10e07](../season-10-final-mission/s10e07-pipeline/) | переполнение как штатный исход |

### Отсутствие данных — тоже данные

| Где | В какой форме |
|---|---|
| [s06e07](../season-06-embedded-iot/s06e07-ip-cameras/) | пустая зона на карте изменений |
| [s08e02](../season-08-ai-and-data/s08e02-statistics-outliers/) | выброс как сигнал, а не мусор |
| [s10e06](../season-10-final-mission/s10e06-dashboard/) | возраст показаний наравне со значением |

---

## Что читать рядом

| Документ | О чём |
|---|---|
| [CURRICULUM.md](CURRICULUM.md) | все 80 серий одним списком |
| [PROJECTS.md](PROJECTS.md) | сквозные проекты и переиспользование модулей |
| [RESOURCES.md](RESOURCES.md) | книги по темам курса |
| [SCENARIO.md](SCENARIO.md) | сюжет, персонажи, хронология |
