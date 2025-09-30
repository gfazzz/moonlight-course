# Episode 27: "Inter-Process Communication" 📡
## Season 7: System Programming | Episode 27/42

> *"Процессы должны говорить. Мы их научим."*

---

## 📋 Briefing

Множество процессов работают. Нужна коммуникация: pipes, message queues, shared memory.

**Задачи:**
1. Pipes & Named Pipes (FIFO)
2. Message Queues
3. Shared Memory
4. Unix Domain Sockets

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

**Следующий эпизод:** [Episode 28: Stealth Operation →](../episode-28-stealth-operation/)
