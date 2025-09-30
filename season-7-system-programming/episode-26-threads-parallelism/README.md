# Episode 26: "Threads & Parallelism" 🧵
## Season 7: System Programming | Episode 26/42

> *"Один процесс хорош. Множество потоков — лучше."*

---

## 📋 Briefing

Daemon запущен. Теперь нужна параллельная обработка: мониторинг сети, анализ данных, криптография.

**Задачи:**
1. Работа с pthread
2. Синхронизация (mutex, semaphore)
3. Thread pool
4. Deadlock prevention

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

**Следующий эпизод:** [Episode 27: IPC →](../episode-27-ipc/)
