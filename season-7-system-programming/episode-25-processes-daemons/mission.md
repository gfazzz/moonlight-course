# Брифинг миссии: Процессы & Демоны
## Episode 25 — Война на системном уровне

---

## 🎯 Цель миссии

**КРИТИЧЕСКАЯ СИТУАЦИЯ:** Обнаружено вражеское наблюдение в системе (PIDs 3012, 3156).  
**МИССИЯ:** Создать counter-surveillance daemon, отслеживать врага, собрать intelligence.  
**ДЕДЛАЙН:** 6-12 часов до обнаружения  
**СТАТУС:** Season 7 начинается — война в тенях

---

## 📋 Требования миссии

### 1. Анализ процессов
- ✅ Идентифицировать вражеские процессы (./tracker, surveillance_d)
- ✅ Проанализировать дерево процессов (fork, exec, orphans)
- ✅ Понять архитектуру daemon
- ✅ Отследить сетевые соединения к C2 серверу

### 2. Создание Daemon
- ✅ Реализовать корректный UNIX daemon (double fork, setsid)
- ✅ Управление PID файлом (/var/run/moonlight_monitor.pid)
- ✅ Отключиться от терминала (close stdin/stdout/stderr)
- ✅ Фоновое выполнение (no controlling TTY)

### 3. Обработка сигналов
- ✅ SIGTERM handler (graceful shutdown + cleanup)
- ✅ SIGHUP handler (reload configuration)
- ✅ SIGUSR1/SIGUSR2 (управление verbosity)
- ✅ SIGCHLD handler (reap zombie processes)
- ✅ Игнорировать SIGINT, SIGPIPE (robustness)

### 4. Стелс-операции
- ✅ Рандомизация имени процесса (каждые 5 мин)
- ✅ CPU usage < 0.5% (избегать обнаружения)
- ✅ Имитация поведения системных процессов
- ✅ Смешивание с сетевым трафиком

### 5. Сбор Intelligence
- ✅ Перехват сетевых пакетов (324+ пакетов)
- ✅ Логирование активности врага (system_processes.log)
- ✅ Идентификация C2 сервера (185.220.101.42:8080)
- ✅ Создание intelligence отчёта

---

## 🧪 Критерии тестирования

### Управление процессами
```bash
# Создать daemon
./moonlight_daemon start

# Проверить запущен ли daemon
ps aux | grep moonlight_monitor

# Отправить сигналы
kill -USR1 <PID>   # Увеличить verbosity
kill -HUP <PID>    # Перезагрузить config
kill -TERM <PID>   # Graceful shutdown

# Проверить отсутствие zombie
ps aux | grep defunct
```

### Проверка стелса
```bash
# Проверить CPU usage (должно быть < 0.5%)
top -p <PID>

# Проверить ротацию имени процесса
watch -n 10 'ps aux | grep <PID>'

# Анализ сетевого трафика
tcpdump -i eth0 host 185.220.101.42
```

---

## 📦 Результаты миссии

### Артефакты (3 файла, 785 строк):
- ✅ `system_processes.log` (281 строк)
  - Полный лог мониторинга процессов (14:00-17:30)
  - Обнаружение вражеских процессов (PID 3012, 3156)
  - Таймлайн развёртывания counter-daemon
  - Задокументирована ситуация Mexican standoff
  
- ✅ `signal_traces.txt` (374 строки)
  - Полные трейсы обработки сигналов (47 сигналов)
  - Примеры SIGTERM, SIGHUP, SIGUSR1/2, SIGCHLD
  - Обработка race condition с сигналами
  - SIGKILL атака + восстановление
  - Best practices & защитные меры
  
- ✅ `daemon_config.conf` (130 строк)
  - Профессиональная конфигурация daemon
  - Настройки мониторинга целей
  - Параметры stealth mode
  - Конфигурация logging, alerting, watchdog

### Код:
- ✅ `solution/processes_daemons.c` (440 строк)
- ✅ `starter.c` (105 строк)
- ✅ `solution/Makefile` + корневой `Makefile`

---

## 📊 Критерии успеха

- [x] Вражеские процессы идентифицированы и проанализированы
- [x] Counter-daemon создан (PID 3789)
- [x] Stealth mode работает (не обнаружен врагом)
- [x] Intelligence собран (324 пакета, C2 идентифицирован)
- [x] Обработка сигналов надёжна (47 сигналов обработано)
- [x] Zombie процессы предотвращены (SIGCHLD handler)
- [x] Выиграно 6-12 часов до обнаружения

---

## ⚠️ Статус миссии

**ВЫПОЛНЕНА** ✅

Counter-surveillance daemon развёрнут. Враг под наблюдением, но не нейтрализован.  
Времени до обнаружения: 6-12 часов.

**СРОЧНО:** Немедленно переходить к Episode 26.  
Требуется параллельный анализ логов (threads, producer-consumer).

---

**Следующая миссия:** [Episode 26: Threads & Parallelism →](../episode-26-threads-parallelism/)

---

*MOONLIGHT Protocol: Призрак в машине. Невидимый. Неслышимый. Неостановимый.* 🔧
