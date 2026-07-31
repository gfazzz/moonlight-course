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

/* Путь отчёта задаётся в main() и включает PID запуска: два экземпляра
   на одной машине иначе дерутся за один файл. Рядом — временное имя,
   через которое отчёт публикуется атомарно (см. TODO 2). */
static char REPORT[64];
static char REPORT_TMP[80];

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
    snprintf(REPORT,     sizeof REPORT,     "/tmp/moonlight_s07e03_report.%d.txt", (int)getpid());
    snprintf(REPORT_TMP, sizeof REPORT_TMP, "%s.part", REPORT);
    unlink(REPORT); unlink(REPORT_TMP);

    printf("=== демон: жизнь без терминала ===\n\n");
    fflush(stdout);

    int role = daemonize();
    if (role < 0) { perror("fork"); return 1; }

    if (role == 1) {
        /* TODO 2: код демона.
           getcwd(cwd, ...); is_session_leader = (getsid(0) == getpid());
           has_no_tty = (open("/dev/tty", O_RDWR) < 0);
           printf("ЭТА СТРОКА НЕ ДОЛЖНА ПОЯВИТЬСЯ\n"); fflush(stdout);  // уйдёт в /dev/null
           записать отчёт в REPORT_TMP (fopen "w"):
             "[daemon] рабочий каталог = %s\n"
             "[daemon] лидер сессии (после 2-го fork): да|нет\n"
             "[daemon] управляющий терминал отсутствует: да|нет\n"
             "[daemon] отчёт записан, ухожу в фон\n"
           закрыть файл и опубликовать: rename(REPORT_TMP, REPORT);
           затем _exit(0).

           ПОЧЕМУ ЧЕРЕЗ ВРЕМЕННОЕ ИМЯ. Управляющий процесс ждёт ПОЯВЛЕНИЯ
           файла. Если писать сразу в REPORT, то fopen(..., "w") создаст его
           пустым задолго до того, как строки окажутся внутри, — и читатель,
           заглянувший в это окно, получит половину отчёта, не узнав об этом.
           rename() в пределах одной файловой системы неделим: читатель видит
           либо отсутствие файла, либо готовый файл целиком. */
    }

    /* TODO 3: управляющий процесс: wait(NULL);
       дождаться появления REPORT (до 500 попыток по 10 мс), вывести его построчно,
       удалить файл, затем напечатать:
         "\n[control] вывод демона в stdout виден здесь: нет (ушёл в /dev/null)"
         "[control] демон пережил уход своего родителя: да" */

    (void)daemonize;
    return 0;
}

static int daemonize(void) { return -1; /* TODO 1 */ }
