# Episode 25: "Processes & Daemons" 🔧
## Season 7: System Programming | Episode 25/42

> *"В тени системы живут процессы. Мы станем одним из них."*

---

## 🎬 BRIEFING — Продолжение после Season 6

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 14:00 (MSK) — 48 часов после Episode 24  
**Статус:** 🔴 ENEMY TRACKING DETECTED

---

После Season 6 Episode 24 finale прошло 2 дня. Вы скрылись после IoT battle. Но враг не сдался.

**14:00. Система под наблюдением.**

V. присылает экстренное сообщение:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED)                       │
│  TIME: 14:00:03                             │
│  STATUS: 🔴 CRITICAL — ENEMY IN SYSTEM      │
│                                             │
│  Они внутри. СЕЙЧАС.                        │
│                                             │
│  Detected:                                  │
│  - PID 3012: ./tracker (surveillance)       │
│  - PID 3156: surveillance_d (daemon)        │
│                                             │
│  Они ищут ТЕБЯ. Систему сканируют.          │
│  Твоё лицо с Episode 24 — у них.            │
│                                             │
│  Season 7 начинается. Война в тенях.        │
│                                             │
│  ДЕЙСТВУЙ:                                  │
│  1. Изучи их процессы (fork, signals)      │
│  2. Создай counter-daemon (stealth mode)    │
│  3. Мониторь их активность (не убивай!)    │
│  4. Собери intelligence                     │
│                                             │
│  Это не IoT игры. Это — system level war.  │
│  Процессы. Сигналы. Daemons. Твой уровень.  │
│                                             │
│  48 часов до их следующего шага.            │
│  Go deep. Become a ghost.                   │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**14:05. Анализ начат. Они в системе. Вы тоже.**

**Война переходит на новый уровень: system programming.**

---

## 📋 Задачи миссии

**Цель:** Создать counter-surveillance daemon, мониторить врага, не быть обнаруженным.

**Техническая реализация:**
1. Изучить процессы врага (fork, exec, process tree)
2. Понять daemon architecture (surveillance_d)
3. Освоить signal handling (SIGTERM, SIGHUP, SIGUSR1/2)
4. Создать свой daemon (moonlight_monitor)
5. Реализовать stealth операции (низкий CPU, маскировка)
6. Собрать intelligence (процессы, сеть, файлы)

---

## 📚 Теория

### Процессы в Linux

```c
#include <unistd.h>
#include <sys/types.h>

// Создание нового процесса
pid_t pid = fork();
if (pid == 0) {
    // Дочерний процесс
    execl("/bin/ls", "ls", "-la", NULL);
} else if (pid > 0) {
    // Родительский процесс
    wait(NULL);
}
```

### Создание Daemon

```c
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);  // Родитель завершается
    
    setsid();  // Новая сессия
    chdir("/");  // Рабочая директория
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
```

---

## 🛠 Практика

**Задачи:**
1. Создать простой daemon
2. Обработка SIGTERM, SIGHUP
3. PID файл в /var/run/
4. Логирование в syslog

**Файлы:**
- `moonlight_daemon.c`
- `process_manager.c`

**Компиляция:**
```bash
make daemon
sudo ./moonlight_daemon start
```

---

## 🎬 Финал Episode 25 — GHOST IN THE SYSTEM

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 17:30 (MSK)  
**Статус:** 🟡 COUNTER-DAEMON DEPLOYED

---

**17:15. Counter-daemon создан. Intelligence собран.**

3 часа работы. Анализ вражеских процессов завершён:
- PID 3012 (tracker) → killed после анализа
- PID 3156 (surveillance_d) → monitored, не трогаем
- PID 3789 (moonlight_monitor) → наш counter-daemon, работает

**17:28. ALERT — Enemy daemon пытается обнаружить monitoring:**

```
$ ps aux | grep monitor
```

Ваш daemon использует stealth:
- Process name randomized каждые 5 минут
- CPU usage < 0.5%
- Mimics system processes
- **Не обнаружен. Success.**

**17:30. Intelligence report готов:**

```
╔═══════════════════════════════════════════════════════════════════╗
║  INTELLIGENCE SUMMARY — Enemy Surveillance                        ║
╠═══════════════════════════════════════════════════════════════════╣
║  Duration: 3h 27min                                               ║
║  Enemy processes: 2 (1 killed, 1 monitored)                       ║
║  Network packets: 324 captured                                    ║
║  C2 server: 185.220.101.42:8080                                   ║
║  Reporting: Every 60 seconds                                      ║
║  Data exfiltrated: System info, process list, network connections ║
║  Objective: FIND YOU (from Season 6 Episode 24)                   ║
║                                                                   ║
║  ⚠️ THEY KNOW YOU'RE IN THE SYSTEM                                ║
║  ⚠️ THEY'RE HUNTING YOU                                           ║
║  ⚠️ TIME UNTIL DISCOVERY: 6-12 HOURS                              ║
╚═══════════════════════════════════════════════════════════════════╝
```

**17:31. Телефон — V.:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED)                       │
│  TIME: 17:31:47                             │
│                                             │
│  Отличная работа. Daemon работает.          │
│  Intelligence собран. Они не знают, что     │
│  мы их мониторим.                           │
│                                             │
│  Но времени мало. 324 пакета показывают —   │
│  они активно ищут. Анализируют КАЖДЫЙ       │
│  процесс в системе.                         │
│                                             │
│  Episode 26 — СРОЧНО.                       │
│  Нужны threads для параллельного анализа.   │
│  У них много данных, мы должны быть быстрее │
│                                             │
│  Один daemon — недостаточно. Нужна скорость.│
│  Multithreading. Producer-consumer.         │
│  Параллельная обработка их логов.           │
│                                             │
│  6-12 часов до discovery. Действуй быстро.  │
│                                             │
│  — V.                                       │
│                                             │
│  P.S. Твой daemon — отлично спрятан.        │
│       Но они улучшают алгоритмы детекции.   │
│       Episode 26 — race against time.       │
└─────────────────────────────────────────────┘
```

**17:32. Episode 25 завершён. Enemy monitored. Counter-daemon deployed.**

**Достижение разблокировано:** 🏆 **"Process Master"** — daemon creation & signal handling освоены

**Cliffhanger:** У вас 6-12 часов до обнаружения. Episode 26 — race against time.

---

**РЕЗУЛЬТАТ EPISODE 25:**
- ✅ Процессы врага проанализированы (fork, exec, daemon architecture)
- ✅ Signal handling освоен (SIGTERM, SIGHUP, SIGUSR1/2, SIGCHLD)
- ✅ Counter-daemon создан (moonlight_monitor, PID 3789)
- ✅ Stealth mode работает (не обнаружен)
- ✅ Intelligence собран (324 packets, C2 server identified)
- ⚠️ **Враг активен, ищет вас**
- ⚠️ **6-12 часов до discovery**

**Artifacts созданы:**
- `system_processes.log` — полный лог процессов (14:00-17:30)
- `signal_traces.txt` — signal handling traces (47 signals)
- `daemon_config.conf` — counter-daemon configuration

---

**Следующий эпизод:** [Episode 26: Threads & Parallelism — URGENT →](../episode-26-threads-parallelism/)

**Назад:** [← Season 6 Finale](../../season-6-embedded-iot/episode-24-iot-integration/)

---

*MOONLIGHT Protocol: We are the ghosts in the machine. They can't see us. Yet. 🔧*
