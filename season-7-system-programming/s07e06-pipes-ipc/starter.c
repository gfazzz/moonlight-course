/*
 * OPERATION MOONLIGHT — s07e06 "Каналы (IPC)"
 * Каркас. Скопируй в artifacts/pipes.c и закрой TODO.
 *
 *   cp starter.c artifacts/pipes.c
 *   make test
 *
 * ГЛАВНОЕ ПРАВИЛО: каждая сторона закрывает НЕиспользуемый конец канала.
 * Иначе читатель не увидит EOF и программа зависнет.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("=== pipe(): связь между процессами ===\n\n");

    /* TODO 1: создать два канала: down[2] (родитель->потомок) и up[2] (обратно).
       pipe(down), pipe(up); при ошибке perror и выход. */

    const char *msg = "SCAN 47 TARGETS";

    /* TODO 2: fflush(stdout); pid = fork(). */

    /* TODO 3: потомок (pid == 0):
         close(down[1]); close(up[0]);
         n = read(down[0], buf, sizeof buf - 1); buf[n>0?n:0] = '\0';
         len = snprintf(reply, ..., "OK:%zd", n);  write(up[1], reply, len);
         close(down[0]); close(up[1]);
         _exit((buf[0] == 'S') ? 0 : 1); */

    /* TODO 4: родитель:
         close(down[0]); close(up[1]);
         write(down[1], msg, strlen(msg));
         close(down[1]);              // без этого потомок не увидит EOF
         n = read(up[0], ans, ...); ans[n>0?n:0]='\0'; close(up[0]);
         waitpid(pid, &status, 0);
       печать:
         "[parent] отправлено потомку:      \"%s\""
         "[child ] ответ через обратный канал: \"%s\""
         "[parent] потомок прочитал %d байт:   да|нет"   (сравни atoi(ans+3) с strlen(msg))
         "[parent] код возврата потомка:       %d\n"     (WEXITSTATUS) */

    /* TODO 5: конвейер "echo ... | tr a-z A-Z":
         печать "--- конвейер (как \"|\" в оболочке) ---"; fflush(stdout);
         pipe(pfd);
         p1 = fork(): close(pfd[0]); dup2(pfd[1], STDOUT_FILENO); close(pfd[1]);
                      execl("/bin/echo", "echo", "stealth agent online", NULL); _exit(127);
         p2 = fork(): close(pfd[1]); dup2(pfd[0], STDIN_FILENO); close(pfd[0]);
                      execl("/usr/bin/tr", "tr", "a-z", "A-Z", NULL);
                      execl("/bin/tr", "tr", "a-z", "A-Z", NULL); _exit(127);
         родитель: close(pfd[0]); close(pfd[1]); waitpid(p1); waitpid(p2);
         печать "(строка выше прошла через два процесса и канал)". */

    (void)msg;
    return 0;
}
