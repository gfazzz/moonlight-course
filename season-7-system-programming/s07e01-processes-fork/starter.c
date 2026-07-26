/*
 * OPERATION MOONLIGHT — s07e01 "Процессы и fork()"
 * Каркас. Скопируй в artifacts/fork.c и закрой TODO.
 *
 *   cp starter.c artifacts/fork.c
 *   make test
 *
 * Концепт: fork() возвращается ДВАЖДЫ — 0 в потомке, PID потомка в родителе.
 * ВАЖНО: не печатай сырые PID (они меняются каждый запуск) — печатай свойства.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILD_EXIT_CODE 7

int main(void) {
    printf("=== fork(): один процесс становится двумя ===\n\n");

    pid_t parent_pid = getpid();

    /* TODO 1: fflush(stdout) ДО fork() — иначе буфер скопируется и текст
       напечатается дважды. */

    /* TODO 2: rc = fork(); при rc < 0 -> perror("fork"), return 1. */

    /* TODO 3: ветка потомка (rc == 0):
         same_parent = (getppid() == parent_pid);
         "[child ] fork() вернул 0:            да"
         "[child ] getppid() == PID родителя:  да|нет"
         "[child ] выхожу с кодом %d"
         fflush(stdout); _exit(CHILD_EXIT_CODE);   // _exit, не exit! */

    /* TODO 4: ветка родителя:
         waited = waitpid(rc, &status, 0);
         "[parent] fork() вернул PID потомка > 0: да|нет"
         "[parent] waitpid() вернул того же:      да|нет"
         "[parent] потомок завершился нормально:  да|нет"   (WIFEXITED)
         "[parent] код возврата потомка:          %d"       (WEXITSTATUS) */

    /* TODO 5: доказать раздельность памяти:
         int x = 100; fflush(stdout); rc2 = fork();
         в потомке: x += 1; _exit((x == 101) ? 0 : 1);
         в родителе: waitpid(rc2, &status, 0);
         "\n[parent] потомок менял x, у родителя x = %d (память раздельна)"
         "[parent] потомок подтвердил свою копию: да|нет" */

    (void)parent_pid;
    return 0;
}
