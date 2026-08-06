# Материалы

**Библиография курса.** Здесь то, на что курс опирается на самом деле: 170 источников, процитированных в `theory.md` восьмидесяти серий, сведённые в один список.

Полный разбор каждой темы — в самих сериях. Указатель тем — в [THEORY_MAP.md](THEORY_MAP.md).

---

## Как этим пользоваться

Список не предназначен для чтения подряд. Разумный порядок такой:

1. **Проходите серию.** В `theory.md` две-три книги под конкретную тему.
2. **Зацепило — читайте оттуда**, а не с начала этого файла.
3. **Держите под рукой K&R и справочник по стандартной библиотеке.** Остальное — по мере необходимости.

Единственная книга, которую стоит прочитать целиком и параллельно с курсом, — K&R. Она короткая (около 270 страниц) и написана авторами языка.

---

## Основа

Эти шесть книг цитируются чаще всех и покрывают бо́льшую часть курса.

| Книга | О чём | Где нужна |
|---|---|---|
| **Kernighan & Ritchie, «Язык программирования C»** | сам язык | S1–S2, дальше как справочник |
| **Kernighan & Pike, «Практика программирования»** | как писать, а не как компилировать | S1, полезна всегда |
| **CLRS, «Алгоритмы: построение и анализ»** | алгоритмы и структуры данных | S4, S8, S9, S10 |
| **Kerrisk, «The Linux Programming Interface»** | системные вызовы, 1500 страниц | S7, S8 |
| **Stevens & Rago, «Advanced Programming in the UNIX Environment»** | то же, с историей и объяснениями | S7 |
| **Drepper, «What Every Programmer Should Know About Memory»** | память и кэш; свободно доступно | S1, S2, S10 |

Про две последние строки: Керриск подробнее, Стивенс объясняет лучше. Держать имеет смысл обе.

---

## По сезонам

### Season 1 — Основы

- **K&R**, гл. 1–4 — по одной главе на серию, ровно в темпе сезона.
- **Kernighan & Pike, «Практика программирования»** — про именование, отладку и то, почему простой код лучше умного.
- **Goldberg, «What Every Computer Scientist Should Know About Floating-Point»** — почему сравнивать вещественные числа через `==` нельзя.
- **Dijkstra, EWD831** — две страницы о том, почему нумерация начинается с нуля.
- **CWE-20, CWE-125, CWE-170, CWE-787** — четыре класса ошибок, с которыми курс будет работать до конца.

### Season 2 — Память и указатели

- **K&R**, гл. 5 — указатели у авторов языка.
- **Van der Linden, «Expert C Programming»** — глава про указатели и массивы объясняет разницу лучше всех.
- **Стандарт C11, §6.5.6** — арифметика указателей формально.
- **Drepper** — почему последовательный обход быстрее блуждания.
- **`include/linux/list.h`** — двусвязный список в ядре Linux; двести строк, которые стоит прочесть.
- **Valgrind и AddressSanitizer** — документация; инструменты понадобятся немедленно.

### Season 3 — Сети

- **Stevens, «TCP/IP Illustrated», том 1** — канон. Главы 3, 17, 18 — по темам сезона.
- **Beej's Guide to Network Programming** — свободно доступное введение в сокеты, читается за вечер.
- **RFC 791, RFC 9293** — первоисточники по IP и TCP; RFC 9293 заменил классический 793 в 2022 году.
- **Kurose & Ross, «Computer Networking: A Top-Down Approach»** — если нужен учебник, а не справочник.

### Season 4 — Криптография и алгоритмы

- **Singh, «Книга шифров»** — история криптографии; читается как детектив.
- **Schneier, «Прикладная криптография»** — классика; первая глава задаёт словарь.
- **Knuth, TAOCP, том 3, §6.4** — хеширование с полным анализом.
- **CLRS**, гл. 10–11 — списки, хеш-таблицы, разрешение коллизий.
- **Ferguson, Schneier, Kohno, «Cryptography Engineering»** — про то, как криптографию ломают на практике, а не в теории.

### Season 5 — Финансовые рынки

- **Goldberg, «What Every Computer Scientist Should Know About Floating-Point Arithmetic» (1991)** — почему `0.1` не бывает точной; доступна бесплатно.
- **Fowler, «Patterns of Enterprise Application Architecture»**, паттерн **Money** — округление как часть контракта, а не деталь реализации.
- **RFC 4180** — единственная попытка описать CSV; четыре страницы.
- **Hinnant, «`chrono`-Compatible Low-Level Date Algorithms»** — перевод даты в дни и обратно с полным выводом; доступна бесплатно.
- **RFC 3339** — запись даты и времени.
- **Pelkonen et al. «Gorilla: A Fast, Scalable, In-Memory Time Series Database» (VLDB 2015)** — сжатие временных рядов; доступна бесплатно.
- **UTS #39, «Unicode Security Mechanisms»** — двойники, скелет строки, смешанные алфавиты; самый практичный документ Unicode.
- **RFC 3629** — UTF-8 в трёх страницах, включая запрет переусложнённой записи.
- **Harris, «Trading and Exchanges»** — как устроены рынки; лучшая книга по теме.
- **Glasserman, «Monte Carlo Methods in Financial Engineering»** — моделирование и генерация случайных чисел.
- **Shiller, «Financial Markets» (Yale, открытый курс)** — рыночный контекст сезона; к программированию отношения не имеет и потому идёт в сериях отдельным блоком «Если тема зацепила». Доступен бесплатно.
- **Lowenstein, «When Genius Failed»** — история LTCM; лучшая иллюстрация того, чем кончается недооценённый риск.
- **Taleb, «Одураченные случайностью»** — про то, как отличают навык от везения.

### Season 6 — Встраиваемые системы

- **Horowitz & Hill, «The Art of Electronics»**, гл. 14 — микроконтроллеры и интерфейсы.
- **Ganssle, «A Guide to Debouncing»** — тридцать страниц про дребезг контактов с измерениями.
- **Samek, «Practical UML Statecharts in C/C++»** — автоматы состояний всерьёз.
- **Szeliski, «Computer Vision: Algorithms and Applications»** — детект движения; доступна бесплатно.
- **MAVLink Developer Guide** — спецификация протокола телеметрии.
- **Antonakakis et al., «Understanding the Mirai Botnet»** — что бывает с устройствами без смены пароля.

### Season 7 — Системное программирование

- **Kerrisk, «The Linux Programming Interface»** — гл. 24–30, 37, 44, 49; основной источник сезона.
- **Stevens & Rago, «APUE»** — гл. 8, 10, 13, 15.
- **Butenhof, «Programming with POSIX Threads»** — канон по потокам; написан в 1997 и не устарел.
- **`man 7 signal-safety`** — список функций, безопасных в обработчике сигнала.

### Season 8 — Данные и модели

- **Knuth, TAOCP, том 2, §3.4.2 и §4.2.2** — генераторы случайных чисел, устойчивая дисперсия.
- **Knuth, TAOCP, том 3, §5.2** — сортировки Шелла и пирамидальная.
- **Tukey, «Exploratory Data Analysis»** — правило выбросов и ящик с усами; первоисточник.
- **Bishop, «Pattern Recognition and Machine Learning»** — линейные модели и классификация.
- **Goodfellow, Bengio, Courville, «Deep Learning»** — оптимизация и обратное распространение.
- **Nielsen, «Neural Networks and Deep Learning»** — лучшее интуитивное объяснение; бесплатно.
- **Minsky & Papert, «Perceptrons»** — доказательство предела, остановившее область на пятнадцать лет.
- **Hastie, Tibshirani, Friedman, «The Elements of Statistical Learning»** — оценка моделей; бесплатно.

### Season 9 — Инженерия систем

- **Nystrom, «Crafting Interpreters»** — лучшая современная книга про интерпретаторы; бесплатно.
- **Aho, Lam, Sethi, Ullman, «Compilers»** («книга дракона») — теория разбора.
- **Cooper & Torczon, «Engineering a Compiler»** — практическая сторона.
- **Petrov, «Database Internals»** — B-деревья, страницы, журналирование.
- **Mohan et al., «ARIES» (1992)** — алгоритм восстановления, лежащий под большинством СУБД.
- **«Build Your Own Text Editor» (kilo)** — сырой режим терминала по шагам.
- **Labbe, «Kalman and Bayesian Filters in Python»** — лучшее введение в фильтр; бесплатно.
- **Nielsen & Chuang, «Quantum Computation and Quantum Information»** — канон квантовых вычислений.

### Season 10 — Финал

- **Parnas, «On the Criteria To Be Used in Decomposing Systems into Modules» (1972)** — пять страниц про сокрытие информации.
- **Martin, «Clean Architecture»** — направление зависимостей.
- **Vallado, «Fundamentals of Astrodynamics and Applications»** — орбитальная механика, стандарт отрасли.
- **Knuth, «Structured Programming with go to Statements» (1974)** — источник цитаты про преждевременную оптимизацию.
- **Seacord, «Secure Coding in C and C++»** — канон по безопасному коду.
- **Kleppmann, «Designing Data-Intensive Applications»** — потоки и гарантии доставки.
- **Nygard, «Release It!»** — деградация, мониторинг, приёмка.
- **Claessen & Hughes, «QuickCheck» (2000)** — тестирование свойств вместо примеров.

---

## Инструменты

| Инструмент | Зачем | С какого сезона |
|---|---|---|
| `gcc` / `clang` | компиляция с `-Wall -Wextra -Werror` | S1 |
| `make` | сборка; в каждой серии свой `Makefile` | S1 |
| `gdb` / `lldb` | отладка | S1 |
| AddressSanitizer | выход за границы, использование после освобождения | S2 |
| UndefinedBehaviorSanitizer | переполнения, некорректные сдвиги | S2 |
| Valgrind | утечки памяти | S2 |
| `tcpdump` / Wireshark | наблюдение за трафиком | S3 |
| ThreadSanitizer | состояния гонки | S7 |
| `perf` | профилирование | S10 |
| AFL++ / libFuzzer | фаззинг разборщиков | S10 |

Санитайзеры стоит включать привычно, а не когда что-то сломалось: они дёшевы и находят то, чего не видно.

---

## Свободно доступное

Ссылки не приводятся намеренно — они устаревают. Всё перечисленное находится поиском по названию.

- **Nystrom, «Crafting Interpreters»** — полный текст на сайте автора.
- **Nielsen, «Neural Networks and Deep Learning»** — полный текст.
- **Hastie et al., «The Elements of Statistical Learning»** — PDF на сайте Стэнфорда.
- **Smith, «The Scientist and Engineer's Guide to DSP»** — полный текст.
- **Szeliski, «Computer Vision»** — PDF от автора.
- **Labbe, «Kalman and Bayesian Filters in Python»** — тетради Jupyter на GitHub.
- **Drepper, «What Every Programmer Should Know About Memory»** — PDF.
- **Beej's Guide to Network Programming** — сайт автора.
- **Fog, «Optimizing software in C++»** — сайт автора.
- **RFC** — все на сайте IETF.
- **`man`-страницы** — первоисточник по системным вызовам; раздел ERRORS часто полезнее описания.

---

## Чего здесь нет

Намеренно отсутствуют: видеокурсы, платформы с задачами на скорость, обзорные статьи и «топ-10 книг». Курс опирается на первоисточники и книги, которые не устаревают за пять лет.

Отдельно про перевод: русские издания существуют почти у всего в списке основы. Для книг после 2015 года чаще есть только оригинал.

---

## Что читать рядом

| Документ | О чём |
|---|---|
| [CURRICULUM.md](CURRICULUM.md) | все 80 серий одним списком |
| [THEORY_MAP.md](THEORY_MAP.md) | указатель тем: где что разбирается |
| [PROJECTS.md](PROJECTS.md) | сквозные проекты и модули |
