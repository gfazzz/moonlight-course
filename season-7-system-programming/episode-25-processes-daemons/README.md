# Episode 25: "Processes & Daemons" 🔧
## Season 7: System Programming | Episode 25/42

> *"В тени системы живут процессы. Мы станем одним из них."*

---

## 📋 Briefing

Физический доступ получен. Теперь нужно закрепиться в системе на программном уровне.

**Задачи:**
1. Создать фоновый процесс (daemon)
2. Управление процессами (fork, exec)
3. Обработка сигналов
4. PID файлы и логирование

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

**Следующий эпизод:** [Episode 26: Threads & Parallelism →](../episode-26-threads-parallelism/)
