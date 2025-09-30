# 🔧 Season 7: System Programming
> *"To control the system, you must become the system."*

## 📋 Обзор сезона

**Уровень:** 🔥🔥🔥🔥 Advanced  
**Эпизоды:** 25-28 (4 эпизода)  
**Время прохождения:** ~12-16 часов  
**Пререквизиты:** Seasons 1-6

### 🎯 Цели сезона

Погрузитесь в **системное программирование UNIX/Linux/FreeBSD**:
- Процессы и демоны
- Многопоточность и параллелизм
- Межпроцессное взаимодействие (IPC)
- Системные вызовы и API ОС

После этого сезона вы будете **думать как ядро ОС**.

---

## 📚 Эпизоды

### Episode 25: Processes & Daemons 🔄
**"Процессы-невидимки"**

**Сюжет:**  
Обнаружен процесс на сервере, который не должен там быть. Научитесь создавать, анализировать и контролировать процессы. Создайте собственный daemon.

**Теория:**
- `fork()`, `exec()`, `wait()`
- Process states (running, zombie, orphan)
- Daemons и background processes
- PID, PPID, process tree
- Signal handling (SIGTERM, SIGKILL, SIGUSR)

**Практика:**
- Process monitor
- Daemon creation
- Signal handlers
- Process management tool

---

### Episode 26: Threads & Parallelism ⚡
**"Многопоточность"**

**Сюжет:**  
Анализ огромного лог-файла занимает часы. Распараллельте работу используя потоки. Научитесь избегать race conditions и deadlocks.

**Теория:**
- POSIX threads (pthread)
- Thread creation & joining
- Mutexes и синхронизация
- Race conditions & deadlocks
- Thread-safe code

**Практика:**
- Multi-threaded log analyzer
- Producer-consumer pattern
- Thread pool
- Lock-free algorithms (bonus)

---

### Episode 27: Inter-Process Communication 📡
**"Межпроцессное взаимодействие"**

**Сюжет:**  
Система MOONLIGHT состоит из множества компонентов. Они должны обмениваться данными. Реализуйте все виды IPC.

**Теория:**
- Pipes & FIFOs
- Message queues
- Shared memory
- Semaphores
- Sockets (Unix domain)

**Практика:**
- Multi-process architecture
- Message bus
- Shared memory database
- Service coordination

---

### Episode 28: Stealth Operation 🥷
**"Скрытая операция"**

**Сюжет:**  
Финальная проверка: создайте stealth-процесс, который работает незаметно, мониторит систему и отправляет данные. Применяем все навыки Season 7.

**Теория:**
- Process hiding techniques
- System call hooking (ethical!)
- Resource limits (rlimit)
- chroot jails
- Capabilities

**Практика:**
- Stealth monitoring agent
- Resource-efficient daemon
- Self-healing process
- Complete system integration

---

## 🎓 Что вы освоите

- ✅ Process management
- ✅ Multi-threading
- ✅ IPC механизмы
- ✅ System call API
- ✅ Daemon programming
- ✅ Signal handling
- ✅ Concurrency patterns

**Уровень:** Senior Systems Engineer 🎖️

---

## 🛠 Требования

```bash
# POSIX-совместимая система
# Linux, FreeBSD, macOS

# Библиотеки
- pthread (многопоточность)
- rt (real-time extensions)
```

---

## 📖 Литература

- "Advanced Programming in the UNIX Environment" — Stevens & Rago ⭐⭐⭐
- "The Linux Programming Interface" — Michael Kerrisk ⭐⭐⭐
- "Unix Network Programming" — Stevens
- Man pages: `man 2 fork`, `man 3 pthread_create`

---

## ⏭️ Следующий сезон

**Season 8: AI & Data Science** — применяем системное программирование для обработки больших данных и ML.

```bash
cd ../season-8-ai-and-data/episode-29-big-data/
cat README.md
```
