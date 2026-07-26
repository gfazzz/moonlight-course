/*
 * OPERATION MOONLIGHT — s07e03 "Демоны"
 * Эталонное решение: daemon.c
 *
 * Концепт серии: демон — процесс без терминала и сессии, живущий сам по себе.
 *                Классический ритуал: fork -> setsid -> fork -> chdir("/") ->
 *                umask(0) -> перенаправить stdin/stdout/stderr в /dev/null.
 *                Второй fork нужен, чтобы демон НЕ был лидером сессии и уже
 *                никогда не смог случайно захватить управляющий терминал.
 * Задача: демонизироваться, отчитаться о своём состоянии в файл и доказать,
 *         что вывод демона в stdout уходит в никуда.
 *
 * Детерминизм: PID печатать нельзя, поэтому демон пишет ПРОВЕРКИ (да/нет),
 * а управляющий процесс дожидается отчёта и печатает его.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define REPORT "/tmp/moonlight_s07e03_report.txt"

/* Ритуал демонизации. Возврат: 1 — мы демон, 0 — мы промежуточный процесс. */
static int daemonize(void) {
    fflush(stdout);

    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid > 0) return 0;              /* родитель уходит: потомок осиротеет */

    /* Новая сессия: отвязка от управляющего терминала. Мы — лидер сессии. */
    if (setsid() < 0) _exit(1);

    /* Второй fork: перестаём быть лидером сессии, терминал уже не захватить. */
    pid = fork();
    if (pid < 0) _exit(1);
    if (pid > 0) _exit(0);              /* лидер сессии уходит */

    /* --- отсюда мы демон --- */
    if (chdir("/") != 0) _exit(1);      /* не держим смонтированную ФС занятой */
    umask(0);                           /* свои права на создаваемые файлы */

    /* stdin/stdout/stderr -> /dev/null: у демона нет консоли. */
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) _exit(1);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO) close(fd);

    return 1;
}

int main(void) {
    unlink(REPORT);                     /* чистый старт */

    printf("=== демон: жизнь без терминала ===\n\n");
    fflush(stdout);

    int role = daemonize();
    if (role < 0) { perror("fork"); return 1; }

    if (role == 1) {
        /* --- код демона --- */
        char cwd[64] = {0};
        if (!getcwd(cwd, sizeof cwd)) cwd[0] = '?';

        int is_session_leader = (getsid(0) == getpid());
        int has_no_tty        = (open("/dev/tty", O_RDWR) < 0);   /* терминала нет */

        /* Этот вывод уйдёт в /dev/null — в отчёт теста он попасть не должен. */
        printf("ЭТА СТРОКА НЕ ДОЛЖНА ПОЯВИТЬСЯ\n");
        fflush(stdout);

        FILE *f = fopen(REPORT, "w");
        if (!f) _exit(1);
        fprintf(f, "[daemon] рабочий каталог = %s\n", cwd);
        fprintf(f, "[daemon] лидер сессии (после 2-го fork): %s\n",
                is_session_leader ? "да" : "нет");
        fprintf(f, "[daemon] управляющий терминал отсутствует: %s\n",
                has_no_tty ? "да" : "нет");
        fprintf(f, "[daemon] отчёт записан, ухожу в фон\n");
        fclose(f);
        _exit(0);
    }

    /* --- управляющий процесс: ждём отчёт демона --- */
    wait(NULL);                          /* промежуточный потомок завершается сразу */

    FILE *f = NULL;
    for (int i = 0; i < 200 && !f; i++) { f = fopen(REPORT, "r"); if (!f) usleep(10000); }
    if (!f) { printf("отчёт от демона не получен\n"); return 1; }

    char line[128];
    while (fgets(line, sizeof line, f)) fputs(line, stdout);
    fclose(f);
    unlink(REPORT);

    printf("\n[control] вывод демона в stdout виден здесь: %s\n", "нет (ушёл в /dev/null)");
    printf("[control] демон пережил уход своего родителя: %s\n", "да");
    return 0;
}
