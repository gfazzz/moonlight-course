/*
 * OPERATION MOONLIGHT — s07e03 "Демоны"
 * Каркас. Скопируй в artifacts/daemon.c и закрой TODO.
 *
 *   cp starter.c artifacts/daemon.c
 *   make test
 *
 * Ритуал: fork -> setsid -> fork -> chdir("/") -> umask(0) -> fds в /dev/null.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define REPORT "/tmp/moonlight_s07e03_report.txt"

/* TODO 1: daemonize() -> 1 если мы демон, 0 если промежуточный процесс, -1 ошибка.
     fflush(stdout);
     pid=fork(); pid<0 -> -1; pid>0 -> return 0;      // родитель уходит
     setsid() < 0 -> _exit(1);                         // новая сессия, без TTY
     pid=fork(); pid<0 -> _exit(1); pid>0 -> _exit(0); // лидер сессии уходит
     chdir("/"); umask(0);
     fd=open("/dev/null", O_RDWR); dup2 в STDIN/STDOUT/STDERR; close(fd) если > 2;
     return 1. */
static int daemonize(void);

int main(void) {
    unlink(REPORT);

    printf("=== демон: жизнь без терминала ===\n\n");
    fflush(stdout);

    int role = daemonize();
    if (role < 0) { perror("fork"); return 1; }

    if (role == 1) {
        /* TODO 2: код демона.
           getcwd(cwd, ...); is_session_leader = (getsid(0) == getpid());
           has_no_tty = (open("/dev/tty", O_RDWR) < 0);
           printf("ЭТА СТРОКА НЕ ДОЛЖНА ПОЯВИТЬСЯ\n"); fflush(stdout);  // уйдёт в /dev/null
           записать в REPORT (fopen "w"):
             "[daemon] рабочий каталог = %s\n"
             "[daemon] лидер сессии (после 2-го fork): да|нет\n"
             "[daemon] управляющий терминал отсутствует: да|нет\n"
             "[daemon] отчёт записан, ухожу в фон\n"
           затем _exit(0). */
    }

    /* TODO 3: управляющий процесс: wait(NULL);
       дождаться появления REPORT (до 200 попыток по 10 мс), вывести его построчно,
       удалить файл, затем напечатать:
         "\n[control] вывод демона в stdout виден здесь: нет (ушёл в /dev/null)"
         "[control] демон пережил уход своего родителя: да" */

    (void)daemonize;
    return 0;
}

static int daemonize(void) { return -1; /* TODO 1 */ }
