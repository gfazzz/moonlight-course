/*
 * OPERATION MOONLIGHT — s07e02 "exec: замена образа процесса"
 * Эталонное решение: exec.c
 *
 * Концепт серии: fork() делает копию, а exec() ЗАМЕНЯЕТ образ процесса новой
 *                программой — PID сохраняется, код/данные/стек затираются.
 *                При успехе exec НЕ ВОЗВРАЩАЕТСЯ: возврат = всегда ошибка.
 *                Связка fork+exec — то, как в UNIX запускается вообще всё.
 * Задача: запустить внешнюю программу, снять её код возврата, обработать
 *         неудачный exec и различить «вышел сам» / «убит сигналом».
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

/* Запустить программу и вернуть её код возврата (или -1, если запуск не удался). */
static int run(char *const argv[]) {
    fflush(stdout);                 /* буфер не должен попасть в потомка */
    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid == 0) {
        execv(argv[0], argv);
        /* Сюда попадаем ТОЛЬКО если exec провалился. */
        _exit(127);                 /* соглашение оболочки: 127 = команда не найдена */
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);   /* соглашение sh */
    return -1;
}

int main(void) {
    printf("=== exec(): процесс меняет программу ===\n\n");

    /* 1. Запуск обычной команды: потомок печатает сам, своим кодом. */
    char *echo_argv[] = {(char *)"/bin/echo", (char *)"[child ] я — уже другая программа", NULL};
    int rc = run(echo_argv);
    printf("[parent] /bin/echo вернул код: %d\n\n", rc);

    /* 2. Код возврата команды: sh -c 'exit 3'. */
    char *sh_argv[] = {(char *)"/bin/sh", (char *)"-c", (char *)"exit 3", NULL};
    rc = run(sh_argv);
    printf("[parent] sh -c 'exit 3' вернул код: %d\n\n", rc);

    /* 3. Неудачный exec: программы не существует -> потомок выходит с 127. */
    char *bad_argv[] = {(char *)"/nonexistent/tracker", NULL};
    rc = run(bad_argv);
    printf("[parent] несуществующая программа -> код: %d (127 = не найдена)\n\n", rc);

    /* 4. exec НЕ возвращается при успехе — докажем в отдельном потомке. */
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        char *true_argv[] = {(char *)"/bin/sh", (char *)"-c", (char *)"exit 0", NULL};
        execv(true_argv[0], true_argv);
        _exit(42);                  /* достижимо только при провале exec */
    }
    int status = 0;
    waitpid(pid, &status, 0);
    printf("[parent] код после успешного exec равен 42? %s\n",
           WEXITSTATUS(status) == 42 ? "да" : "нет (exec не вернулся — так и должно быть)");

    /* 5. Провал exec — это ошибка с errno, а не аварийное завершение. */
    errno = 0;
    execv("/nonexistent/tracker", bad_argv);
    printf("[parent] execv на несуществующий путь вернул -1: %s\n", "да");
    printf("[parent] errno == ENOENT: %s\n", errno == ENOENT ? "да" : "нет");
    printf("[parent] процесс жив после провала exec: %s\n", "да");

    return 0;
}
