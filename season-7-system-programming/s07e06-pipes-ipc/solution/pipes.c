/*
 * OPERATION MOONLIGHT — s07e06 "Каналы (IPC)"
 * Эталонное решение: pipes.c
 *
 * Концепт серии: у процессов раздельная память (s07e01) — общую переменную не
 *                подсунешь. Связь идёт через ядро: pipe() даёт пару дескрипторов,
 *                один на запись, другой на чтение. Дескрипторы наследуются при
 *                fork(), поэтому канал соединяет родителя и потомка.
 *                Ключевое правило: КАЖДАЯ сторона закрывает НЕиспользуемый конец —
 *                иначе читатель никогда не увидит EOF и повиснет.
 * Задача: передать данные потомку и обратно; собрать конвейер как в оболочке.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("=== pipe(): связь между процессами ===\n\n");

    /* ---------- 1. Родитель -> потомок ---------- */
    int down[2];                       /* down[0] — чтение, down[1] — запись */
    if (pipe(down) != 0) { perror("pipe"); return 1; }

    /* Обратный канал: потомок -> родитель. */
    int up[2];
    if (pipe(up) != 0) { perror("pipe"); return 1; }

    const char *msg = "SCAN 47 TARGETS";

    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* --- потомок: читает из down, отвечает в up --- */
        close(down[1]);                /* не пишем в down */
        close(up[0]);                  /* не читаем из up */

        char buf[64];
        ssize_t n = read(down[0], buf, sizeof buf - 1);
        buf[(n > 0) ? n : 0] = '\0';

        /* Считаем длину задания и отвечаем родителю. */
        char reply[64];
        int len = snprintf(reply, sizeof reply, "OK:%zd", n);
        write(up[1], reply, (size_t)len);

        close(down[0]);
        close(up[1]);
        _exit((buf[0] == 'S') ? 0 : 1);
    }

    /* --- родитель: пишет в down, читает из up --- */
    close(down[0]);                    /* не читаем из down */
    close(up[1]);                      /* не пишем в up */

    write(down[1], msg, strlen(msg));
    close(down[1]);                    /* ВАЖНО: закрыли -> потомок увидит EOF */

    char ans[64];
    ssize_t n = read(up[0], ans, sizeof ans - 1);
    ans[(n > 0) ? n : 0] = '\0';
    close(up[0]);

    int status = 0;
    waitpid(pid, &status, 0);

    printf("[parent] отправлено потомку:      \"%s\"\n", msg);
    printf("[child ] ответ через обратный канал: \"%s\"\n", ans);
    printf("[parent] потомок прочитал %d байт:   %s\n",
           (int)strlen(msg), atoi(ans + 3) == (int)strlen(msg) ? "да" : "нет");
    printf("[parent] код возврата потомка:       %d\n\n", WEXITSTATUS(status));

    /* ---------- 2. Конвейер: echo ... | tr a-z A-Z ---------- */
    printf("--- конвейер (как \"|\" в оболочке) ---\n");
    fflush(stdout);

    int pfd[2];
    if (pipe(pfd) != 0) { perror("pipe"); return 1; }

    pid_t p1 = fork();
    if (p1 == 0) {
        /* левая часть: stdout -> в канал */
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);
        execl("/bin/echo", "echo", "stealth agent online", (char *)NULL);
        _exit(127);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        /* правая часть: stdin <- из канала */
        close(pfd[1]);
        dup2(pfd[0], STDIN_FILENO);
        close(pfd[0]);
        execl("/usr/bin/tr", "tr", "a-z", "A-Z", (char *)NULL);
        execl("/bin/tr", "tr", "a-z", "A-Z", (char *)NULL);   /* запасной путь */
        _exit(127);
    }

    /* Родитель обязан закрыть ОБА конца, иначе tr не дождётся EOF. */
    close(pfd[0]);
    close(pfd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("(строка выше прошла через два процесса и канал)\n");
    return 0;
}
