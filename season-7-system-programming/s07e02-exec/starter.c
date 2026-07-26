/*
 * OPERATION MOONLIGHT — s07e02 "exec: замена образа процесса"
 * Каркас. Скопируй в artifacts/exec.c и закрой TODO.
 *
 *   cp starter.c artifacts/exec.c
 *   make test
 *
 * Концепт: fork() копирует, exec() ЗАМЕНЯЕТ образ. При успехе exec не возвращается.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

/* TODO 1: run(argv) — fork + execv в потомке, waitpid в родителе.
     fflush(stdout); pid=fork(); pid<0 -> return -1;
     в потомке: execv(argv[0], argv); _exit(127);   // 127 = команда не найдена
     в родителе: waitpid(pid,&status,0);
       WIFEXITED  -> return WEXITSTATUS(status);
       WIFSIGNALED-> return 128 + WTERMSIG(status);
       иначе      -> return -1. */
static int run(char *const argv[]);

int main(void) {
    printf("=== exec(): процесс меняет программу ===\n\n");

    /* TODO 2: запустить /bin/echo с аргументом "[child ] я — уже другая программа";
       затем "[parent] /bin/echo вернул код: %d\n\n". */

    /* TODO 3: запустить /bin/sh -c "exit 3";
       "[parent] sh -c 'exit 3' вернул код: %d\n\n". */

    /* TODO 4: запустить "/nonexistent/tracker";
       "[parent] несуществующая программа -> код: %d (127 = не найдена)\n\n". */

    /* TODO 5: доказать, что exec не возвращается при успехе:
       fork; в потомке execv(/bin/sh -c "exit 0"); затем _exit(42);
       в родителе waitpid и печать
       "[parent] код после успешного exec равен 42? да|нет (exec не вернулся — так и должно быть)". */

    /* TODO 6: errno=0; execv("/nonexistent/tracker", ...) прямо в родителе; после него:
       "[parent] execv на несуществующий путь вернул -1: да"
       "[parent] errno == ENOENT: да|нет"
       "[parent] процесс жив после провала exec: да". */

    (void)run;
    return 0;
}

static int run(char *const argv[]) { (void)argv; return -1; /* TODO 1 */ }
