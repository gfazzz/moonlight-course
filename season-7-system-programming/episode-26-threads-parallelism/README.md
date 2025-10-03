# Episode 26: "Threads & Parallelism" 🧵
## Season 7: System Programming | Episode 26/42

> *"Один процесс хорош. Множество потоков — лучше."*

---

## 🎬 BRIEFING — URGENT (Продолжение Episode 25)

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 18:00 (MSK) — 30 минут после Episode 25  
**Статус:** 🔴 RACE AGAINST TIME — 6-12 hours until discovery

---

После Episode 25 у вас есть intelligence: **324 captured packets** от enemy daemon.

**Проблема:** Анализ одним потоком займёт **32 минуты**. У вас **~9 hours** до next enemy scan.

Но packets нужно проанализировать **СЕЙЧАС** — может быть критическая информация.

**17:45. Телефон — V.:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED - URGENT)              │
│  TIME: 17:45:23                             │
│  STATUS: 🔴 TIME CRITICAL                   │
│                                             │
│  324 пакета. 32 минуты single-threaded.     │
│  Каждую минуту — риск обнаружения.          │
│                                             │
│  У твоей машины 8 CPU cores. ИСПОЛЬЗУЙ ИХ.  │
│                                             │
│  Threads. Parallelism. Producer-consumer.   │
│  8 threads = ~4 minutes. Speedup x8.        │
│                                             │
│  Packet 047 содержит КРИТИЧЕСКУЮ инфу:      │
│  "target_identified" — confidence 67%.      │
│  Найди детали. Пока не поздно.              │
│                                             │
│  DEADLINE: 18:05 (20 minutes max на code)   │
│  Race against time. Literally.              │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**17:47. Начинается race against time.**

---

## 📋 Задачи миссии — URGENT EXECUTION

**Цель:** Parallel log analysis using threads (8-core CPU, speed up 8x)

**Техническая реализация:**
1. Создать thread pool (8 worker threads)
2. Producer-consumer pattern (packet queue, bounded buffer)
3. Синхронизация: mutex + condition variables
4. Избежать race conditions (shared memory protection)
5. Prevent deadlocks (consistent lock ordering)
6. Analyze 324 packets → find packet 047 (CRITICAL INFO)

---

## 📚 Теория

### POSIX Threads

```c
#include <pthread.h>

void *worker(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d running\n", id);
    return NULL;
}

int main() {
    pthread_t threads[4];
    int ids[4];
    
    for (int i = 0; i < 4; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
}
```

### Mutex

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *safe_increment(void *arg) {
    pthread_mutex_lock(&lock);
    counter++;  // Критическая секция
    pthread_mutex_unlock(&lock);
    return NULL;
}
```

---

## 🛠 Практика

**Задачи:**
1. Parallel file processor
2. Thread-safe queue
3. Producer-Consumer pattern
4. Deadlock detection

**Компиляция:**
```bash
gcc -pthread threads.c -o threads
```

---

## 🎬 Финал Episode 26 — CRITICAL DISCOVERY

**Локация:** Novosibirsk Akademgorodok  
**Время:** 24 декабря, 18:04 (MSK)  
**Статус:** 🚨 CRITICAL FINDING DETECTED

---

**18:00. Thread pool создан. 8 threads активны. Parallel analysis started.**

Producer-consumer pattern работает. Queue filling rapidly. Consumers processing in parallel.

**18:02:00. ALERT — Thread 6 (TID 4518) нашёл packet 047:**

```
🚨🚨🚨 CRITICAL ALERT 🚨🚨🚨
Pattern match: "target_identified"
Data payload (decrypted): 
  {
    "hostname": "novosibirsk-research-01",
    "suspicious_process": "moonlight_monitor",
    "confidence": 67%,
    "action_required": "deep_scan"
  }
```

**ОНИ ПОДОЗРЕВАЮТ ВАШ DAEMON!**

Confidence 67% — suspected but NOT confirmed yet.

**18:04:12. Analysis complete. 324 packets processed за 4 minutes 12 seconds.**

Speedup: **7.6x** (vs 32 minutes single-threaded)

**18:04:15. V. — emergency message:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED)                       │
│  TIME: 18:04:15                             │
│  STATUS: 🚨 CRITICAL DISCOVERY              │
│                                             │
│  Packet 047 — ОНИ НАС ПОДОЗРЕВАЮТ.          │
│  Confidence 67%. Пока не уверены.           │
│                                             │
│  Packets 048-053: Deep scan scheduled       │
│  Time: Dec 25, 03:00 (9 hours from now)     │
│  Detection probability: 85-95%              │
│                                             │
│  У нас 9 часов. После — они нас найдут.     │
│                                             │
│  Episode 27 — СРОЧНО.                       │
│  IPC. Covert channels. No network traffic.  │
│                                             │
│  Shutdown moonlight_monitor daemon СЕЙЧАС.  │
│  Используй pipes, shared memory для связи.  │
│  Нельзя оставлять следы в сети.             │
│                                             │
│  9 hours. IPC setup. Covert communication.  │
│  Это последний шанс остаться невидимым.     │
│                                             │
│  — V.                                       │
│                                             │
│  P.S. Threads спасли тебя. 4 min vs 32 min. │
│       Без параллелизма — опоздал бы.        │
│       Episode 27 — без threads. Stealth.    │
└─────────────────────────────────────────────┘
```

**18:05. Episode 26 завершён. Deep scan через 9 hours. Time is running out.**

**Достижение разблокировано:** 🏆 **"Thread Master"** — parallel processing освоен

**Cliffhanger:** Daemon под подозрением (67%). Deep scan через 9 hours. Episode 27 — covert IPC channels.

---

**РЕЗУЛЬТАТ EPISODE 26:**
- ✅ Thread pool created (8 threads)
- ✅ Producer-consumer pattern implemented
- ✅ 324 packets analyzed in 4 minutes (7.6x speedup)
- ✅ Race conditions prevented (mutex synchronization)
- ✅ Deadlocks avoided (consistent lock ordering)
- ✅ **Packet 047 found:** Enemy suspects us (67% confidence)
- ⚠️ **Deep scan scheduled:** Dec 25, 03:00 (9 hours away)
- ⚠️ **Detection probability:** 85-95% if daemon still active

**Artifacts созданы:**
- `thread_analysis_log.txt` — полный log thread pool execution (372 lines)
- `race_condition_demo.log` — race condition educational demo (238 lines)
- `producer_consumer_detailed.log` — detailed producer-consumer log (316 lines)

**Технические достижения:**
- Parallel speedup: 7.6x (close to ideal 8x)
- Mutex contentions: 47 (handled without deadlock)
- Producer blocks: 23 times (queue management working)
- Consumer blocks: 47 times (balanced load)
- Average queue occupancy: 54.6% (optimal)

---

**Следующий эпизод:** [Episode 27: IPC — URGENT →](../episode-27-ipc/)

**Назад:** [← Episode 25: Processes & Daemons](../episode-25-processes-daemons/)

---

*MOONLIGHT Protocol: Speed matters. Parallelism saves time. Time saves lives. 🧵*
