# Брифинг миссии: Inter-Process Communication
## Episode 27 — Режим сетевой тишины

---

## 🎯 Цель миссии

**КРИТИЧЕСКАЯ СИТУАЦИЯ:** Выключить всю сетевую активность после обнаружения в Episode 26.  
**МИССИЯ:** Установить скрытые IPC каналы для коммуникации процессов БЕЗ сети.  
**ВЫЗОВ:** Ноль сетевых пакетов (вражеский мониторинг не должен НИЧЕГО видеть).  
**ДЕДЛАЙН:** 8.5 часов до deep scan (Dec 25, 03:00)

---

## 📋 Требования миссии

### 1. Anonymous Pipes
- ✅ Создать пары pipes (коммуникация родитель-потомок)
- ✅ Двунаправленные pipes (2 pipes для request-response)
- ✅ Управление файловыми дескрипторами (закрыть неиспользуемые концы)
- ✅ Обработать вместимость pipe (65KB Linux default)
- ✅ Блокирующее поведение (атомарность PIPE_BUF)

### 2. Named Pipes (FIFOs)
- ✅ Создать FIFOs в файловой системе (mkfifo)
- ✅ Мультипроцессная коммуникация (любой процесс может получить доступ)
- ✅ Стелс FIFOs (Linux: /dev/shm/, macOS/BSD: /tmp/ + случайные имена)
- ✅ Авто-удаление после использования (без следов)
- ✅ Разрешения (0600, только владелец)

### 3. Shared Memory
- ✅ POSIX shared memory (shm_open, mmap)
- ✅ Большие датасеты (50 MB лог активности в реальном времени)
- ✅ Process-shared mutex (PTHREAD_PROCESS_SHARED)
- ✅ Предотвращение race condition (синхронизация mutex)
- ✅ Memory-mapped I/O (прямой доступ, без копирования)

### 4. Message Queues
- ✅ POSIX message queues (mq_open, mq_send, mq_receive)
- ✅ Структурированные сообщения (struct IntelligenceMessage)
- ✅ Доставка на основе приоритета (критические сообщения первыми)
- ✅ Non-blocking I/O (O_NONBLOCK, mq_timedreceive)
- ✅ Асинхронное уведомление (SIGEV_SIGNAL)

### 5. Signals
- ✅ Стандартные сигналы (SIGUSR1, SIGUSR2)
- ✅ Realtime сигналы (SIGRTMIN-SIGRTMAX)
- ✅ Signal payload (sival_int, sival_ptr)
- ✅ Скрытое кодирование (битовые последовательности через типы сигналов)
- ✅ Очередь сигналов (realtime сигналы в очереди, стандартные нет)

### 6. Анализ производительности
- ✅ Сравнение пропускной способности (pipes vs shared memory vs network)
- ✅ Измерение задержки (IPC с субмиллисекундной задержкой)
- ✅ Пропускная способность скрытого канала (сигналы: ~20 байт/сек)
- ✅ Компромиссы (скорость vs стелс vs сложность)

---

## 🧪 Критерии тестирования

### Тесты Pipe
```bash
# Anonymous pipe (родитель-потомок)
./ipc_test --pipe-anonymous

# Named pipe (FIFO)
./ipc_test --pipe-named /tmp/test_fifo

# Двунаправленные pipes
./ipc_test --pipe-bidirectional

# Тест вместимости pipe (блокировка)
./ipc_test --pipe-capacity 100000  # Запись 100 KB (должна блокироваться на 65 KB)
```

### Тесты Shared Memory
```bash
# Создать shared memory
./ipc_test --shm-create /moonlight_test 1048576  # 1 MB

# Подключить несколько процессов
./ipc_test --shm-attach /moonlight_test &  # Процесс 1
./ipc_test --shm-attach /moonlight_test &  # Процесс 2

# Тест race condition (без mutex - должно повредить)
./ipc_test --shm-race-unsafe

# Синхронизация mutex (с mutex - должно работать)
./ipc_test --shm-race-safe
```

### Тесты Message Queue
```bash
# Создать message queue
./ipc_test --mq-create /test_queue

# Тест приоритета (отправить низкий prio, затем высокий prio)
./ipc_test --mq-send /test_queue "Low priority" 1
./ipc_test --mq-send /test_queue "HIGH PRIORITY" 9
./ipc_test --mq-receive /test_queue  # Должен получить HIGH PRIORITY первым

# Non-blocking receive (пустая очередь)
./ipc_test --mq-receive-nonblock /test_queue  # Должен вернуть EAGAIN немедленно
```

### Тесты Signal
```bash
# Базовые сигналы
./ipc_test --signal-basic <PID>  # Отправить SIGUSR1, SIGUSR2

# Realtime сигналы с payload
./ipc_test --signal-rtmin <PID> 4789  # Отправить SIGRTMIN со значением 4789

# Кодирование сигналов (отправить строку через битовую последовательность)
./ipc_test --signal-encode <PID> "ALERT"  # 40 сигналов (5 байт × 8 бит)
```

### Тесты производительности
```bash
# Пропускная способность pipe
time ./ipc_test --perf-pipe 52428800  # Передача 50 MB
# Ожидается: ~19.5 MB/s

# Пропускная способность shared memory
time ./ipc_test --perf-shm 52428800  # Передача 50 MB
# Ожидается: ~561.8 MB/s (быстрее в 28.8x)

# Пропускная способность сигналов
time ./ipc_test --perf-signal 1000  # 1000 байт через сигналы
# Ожидается: ~50 секунд (~20 байт/сек)
```

---

## 📦 Результаты миссии

### Артефакты (3 файла, 1,090 строк):
- ✅ `pipe_communication.log` (238 строк)
  - Последовательность выключения daemon (сетевая тишина)
  - Создание anonymous pipe и передача данных
  - Named pipes (FIFOs) для мультипроцессинга
  - Стелс FIFOs (Linux: /dev/shm/, macOS/BSD: /tmp/, случайные имена, авто-удаление)
  - Двунаправленные pipes (установка 2-pipe)
  - IPC на основе сигналов (бонус: низкопропускной скрытый канал)
  
- ✅ `shared_memory.log` (435 строк)
  - Создание POSIX shared memory (shm_open, ftruncate, mmap)
  - Подключение процесса (3 процесса используют 50 MB)
  - Структура данных (ActivityLog с 10,000 записями)
  - Мониторинг в реальном времени (47 событий за 1 минуту)
  - Сравнение производительности (быстрее в 28.8x чем pipes)
  - Демо race condition и синхронизация mutex
  - Конфигурация process-shared mutex
  
- ✅ `signals_and_message_queues.log` (417 строк)
  - Сигналы для скрытой коммуникации (SIGUSR1/2, SIGRTMIN)
  - Realtime сигналы с payload (int передача)
  - Скрытое кодирование на основе сигналов (мультибит, пример "ALERT")
  - Надёжность очереди сигналов (standard vs realtime)
  - POSIX message queues (mq_open, mq_send, mq_receive)
  - Доставка на основе приоритета (высокий приоритет первым)
  - Non-blocking I/O и таймауты
  - Асинхронное уведомление (SIGEV_SIGNAL)

### Код:
- ✅ `solution/ipc.c` (280 строк)
- ✅ `starter.c` (166 строк)
- ✅ `solution/Makefile` + корневой `Makefile`

---

## 📊 Критерии успеха

- [x] Сетевой трафик: НОЛЬ (daemon выключен, все сокеты закрыты)
- [x] 5 IPC механизмов развёрнуты (pipes, FIFOs, shared memory, message queues, signals)
- [x] Скрытые каналы установлены (сигналы = ультра-стелс, только kernel)
- [x] Мониторинг в реальном времени (50 MB shared memory, 47 событий логировано)
- [x] Производительность измерена (shared memory быстрее в 28.8x чем pipes)
- [x] Новая угроза идентифицирована (tracker2, PID 4789)
- [x] Intelligence собран (47 событий: 12 критических, 23 предупреждения, 12 инфо)
- [x] Обнаружена эксфильтрация данных (2.3 MB на C2 сервер)

---

## ⚠️ Статус миссии

**ВЫПОЛНЕНА** ✅

Вся сетевая активность выключена. IPC каналы работают.  
Вражеский сетевой мониторинг: не видит НИЧЕГО.

**КРИТИЧЕСКАЯ НАХОДКА:**
- Новый вражеский процесс: tracker2 (PID 4789) - обновлённый инструмент наблюдения
- Активность: доступ к /etc/passwd, сканирование портов, попытки ptrace, эксфильтрация данных
- Обратный отсчёт до deep scan: осталось 5.5 часов (Dec 25, 03:00)

**СРОЧНО:** Немедленно переходить к Episode 28.  
Требуется стелс-операция. Timing attacks. Финальная конфронтация.

---

**Следующая миссия:** [Episode 28: Stealth Operation →](../episode-28-stealth-operation/)

---

*MOONLIGHT Protocol: Тихий. Невидимый. Неотслеживаемый.* 📡