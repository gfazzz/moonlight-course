# Episode 27: "Inter-Process Communication" 📡
## Season 7: System Programming | Episode 27/42

> *"Процессы должны говорить. Мы их научим."*

---

## 🎬 BRIEFING — NETWORK SILENCE (Продолжение Episode 26)

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 18:30 (MSK) — 30 минут после Episode 26  
**Статус:** 🔴 DAEMON SHUT DOWN — Network Silence Required

---

После Episode 26 discovery (packet 047): Enemy suspects us (67% confidence).  
Deep scan scheduled: Dec 25, 03:00 (**~8.5 hours** remaining).

**18:30. V.'s emergency order: SHUT DOWN ALL NETWORK ACTIVITY.**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED - EMERGENCY)           │
│  TIME: 18:30:05                             │
│  STATUS: 🔴 NETWORK SILENCE MODE            │
│                                             │
│  НЕМЕДЛЕННО:                                │
│  1. Shutdown moonlight_monitor daemon       │
│  2. Close ALL network connections           │
│  3. NO TCP/UDP traffic — НИЧЕГО             │
│                                             │
│  Враг мониторит сеть. Один пакет — и ты     │
│  обнаружен. Network silence до 03:00.       │
│                                             │
│  НО: Работа продолжается. Нужна связь       │
│  между процессами БЕЗ сети.                 │
│                                             │
│  РЕШЕНИЕ: IPC (Inter-Process Communication) │
│  - Pipes (anonymous, named FIFOs)           │
│  - Shared Memory (fastest, no copying)      │
│  - Message Queues (structured, priority)    │
│  - Signals (covert, low bandwidth)          │
│                                             │
│  Все IPC — kernel-level. ZERO network.      │
│  Enemy network monitoring: видит НОЛЬ.      │
│                                             │
│  8.5 часов. Установи covert IPC channels.   │
│  Episode 28 — финал. Timing attacks. Stealth│
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**18:30:10. Daemon shutdown initiated. Network connections closed.**

**Mission:** Establish covert IPC communication channels (NO network traffic).

---

## 📋 Задачи миссии — COVERT IPC SETUP

**Цель:** Setup inter-process communication WITHOUT network (enemy can't detect)

**Техническая реализация:**
1. **Pipes** (anonymous + named FIFOs) — byte stream communication
2. **Shared Memory** — high-speed large data sharing (50 MB real-time)
3. **Message Queues** — structured priority-based messaging
4. **Signals** — ultra-covert low-bandwidth channel (kernel-only)
5. **Performance analysis** — measure speed vs network alternatives

---

## 📚 Теория

### Pipes

```c
int pipefd[2];
pipe(pipefd);

if (fork() == 0) {
    // Дочерний
    close(pipefd[1]);  // Закрыть write
    char buf[100];
    read(pipefd[0], buf, sizeof(buf));
    printf("Received: %s\n", buf);
} else {
    // Родитель
    close(pipefd[0]);  // Закрыть read
    write(pipefd[1], "Hello", 5);
}
```

### Shared Memory

```c
#include <sys/shm.h>

int shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0666);
char *shmaddr = shmat(shmid, NULL, 0);

strcpy(shmaddr, "Shared data");

shmdt(shmaddr);
```

---

## 🛠 Практика

**Задачи:**
1. Pipe-based logger
2. Message queue для задач
3. Shared memory counter
4. Unix socket server/client

---

## 🎬 Финал Episode 27 — IPC CHANNELS ESTABLISHED

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 21:30 (MSK)  
**Статус:** ✅ COVERT IPC OPERATIONAL — Zero Network Traffic

---

**21:00. IPC channels tested and deployed. 3 hours of work.**

Results:
- ✅ **Pipes + FIFOs:** Command & control communication (0.023 sec latency)
- ✅ **Shared Memory:** Real-time monitoring (50 MB, 47 events/min, 28.8x faster than pipes)
- ✅ **Message Queues:** Priority alerts (critical messages delivered first)
- ✅ **Signals:** Ultra-covert channel (~20 bytes/sec, pure kernel-level)

**Enemy network monitoring status:** ZERO traffic detected ✅

**21:15. New enemy process discovered via IPC intelligence:**

```
╔═══════════════════════════════════════════════════════════════════╗
║  INTELLIGENCE REPORT — IPC Monitoring Results                     ║
╠═══════════════════════════════════════════════════════════════════╣
║  Duration: 3 hours (18:30-21:30)                                  ║
║  IPC method: Shared memory (50 MB real-time log)                  ║
║  Events logged: 47 (12 critical, 23 warnings, 12 info)            ║
║                                                                   ║
║  🚨 NEW THREAT DETECTED:                                          ║
║  PID 4789: /usr/bin/tracker2 (upgraded enemy surveillance)        ║
║  Activity:                                                        ║
║    - FILE_ACCESS: /etc/passwd, /etc/shadow, /proc/*/cmdline      ║
║    - NETWORK_SCAN: ports 22, 80, 443, 9000-9010                   ║
║    - SYSTEM_CALL: ptrace(PTRACE_ATTACH) — trying to attach!      ║
║    - DATA_EXFILTRATION: 2.3 MB to C2 server 185.220.101.42       ║
║                                                                   ║
║  ⚠️ ANALYSIS: Enemy upgraded tools after Episode 26               ║
║  ⚠️ tracker2 = more sophisticated than tracker                    ║
║  ⚠️ Deep scan countdown: 5.5 hours (Dec 25, 03:00)                ║
╚═══════════════════════════════════════════════════════════════════╝
```

**21:30. V. — final briefing before Episode 28:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED)                       │
│  TIME: 21:30:47                             │
│  STATUS: 🔴 FINAL EPISODE PREPARATION       │
│                                             │
│  IPC отлично работает. Сеть молчит.         │
│  Enemy видит НОЛЬ активности.               │
│                                             │
│  НО: tracker2 (PID 4789) активен.           │
│  Это — upgraded tool. Опаснее tracker v1.   │
│                                             │
│  Intelligence:                              │
│  - Пытается ptrace attach (мониторит процессы)│
│  - Сканирует порты (ищет backdoors)         │
│  - Exfiltrates data (2.3 MB to C2)          │
│  - Deep scan через 5.5 hours                │
│                                             │
│  Episode 28 — ФИНАЛ Season 7.               │
│  Stealth Operation. Covert channels.        │
│  Timing attacks. Side-channel analysis.     │
│                                             │
│  ЗАДАЧА:                                    │
│  1. Analyze tracker2 binary (reverse eng.)  │
│  2. Find vulnerabilities (timing attacks)   │
│  3. Extract secrets (cache side-channels)   │
│  4. Neutralize OR evade before 03:00        │
│                                             │
│  Это последняя ночь перед deep scan.        │
│  5.5 hours. Episode 28. Final stand.        │
│                                             │
│  Ты прошёл:                                 │
│  - Episode 25: Processes & Daemons ✅        │
│  - Episode 26: Threads & Parallelism ✅      │
│  - Episode 27: IPC Mastery ✅               │
│                                             │
│  Episode 28 — всё объединяется.             │
│  System programming at its finest.          │
│  Stealth. Precision. Survival.              │
│                                             │
│  Good luck. Ты понадобишься.                │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**21:31. Episode 27 завершён. IPC operational. Network silent. Episode 28 — final battle.**

**Достижение разблокировано:** 🏆 **"IPC Master"** — all communication methods mastered

**Cliffhanger:** Enemy tracker2 active. 5.5 hours until deep scan. Episode 28 — stealth operation, final confrontation.

---

**РЕЗУЛЬТАТ EPISODE 27:**
- ✅ 5 IPC mechanisms deployed (pipes, FIFOs, shared memory, message queues, signals)
- ✅ Zero network traffic (enemy monitoring sees NOTHING)
- ✅ Covert channels established (signals = ultra-stealth)
- ✅ Real-time monitoring (50 MB shared memory, 47 events)
- ✅ Performance measured (shared memory 28.8x faster than pipes)
- ✅ New threat identified (tracker2, PID 4789)
- ⚠️ **Deep scan countdown:** 5.5 hours remaining
- ⚠️ **Enemy upgraded:** tracker → tracker2 (more sophisticated)

**Artifacts созданы:**
- `pipe_communication.log` — pipes & FIFOs communication (238 lines)
- `shared_memory.log` — high-speed data sharing (435 lines)
- `signals_and_message_queues.log` — covert signaling & structured messaging (417 lines)

**Техническая статистика:**
- Pipe throughput: 19.5 MB/s
- Shared memory throughput: 561.8 MB/s (28.8x faster)
- Signal bandwidth: ~20 bytes/sec (covert)
- Message queue throughput: 1,764 msg/sec (2.2x faster than pipes)

---

**Следующий эпизод:** [Episode 28: Stealth Operation — FINAL →](../episode-28-stealth-operation/)

**Назад:** [← Episode 26: Threads & Parallelism](../episode-26-threads-parallelism/)

---

*MOONLIGHT Protocol: Silent communication. Zero traces. Pure stealth. 📡*
