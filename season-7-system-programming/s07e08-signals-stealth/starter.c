/*
 * OPERATION MOONLIGHT — s07e08 "Сигналы и stealth-финал" (финал Season 7)
 * Каркас. Скопируй в artifacts/signals.c и закрой TODO.
 *
 *   cp starter.c artifacts/signals.c
 *   make test
 *
 * Правило обработчика: только выставить volatile sig_atomic_t.
 * Никаких printf/malloc внутри — они не async-signal-safe.
 */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t got_term = 0;
static volatile sig_atomic_t got_usr1 = 0;
static volatile sig_atomic_t usr1_count = 0;

/* TODO 1: on_signal(sig) — если SIGTERM: got_term = 1;
   если SIGUSR1: got_usr1 = 1, usr1_count++. Больше ничего! */
static void on_signal(int sig);

/* TODO 2: install(sig, fn) через sigaction:
     memset(&sa,0,sizeof sa); sa.sa_handler = fn; sigemptyset(&sa.sa_mask);
     sa.sa_flags = SA_RESTART; return sigaction(sig, &sa, NULL). */
static int install(int sig, void (*fn)(int));

int main(void) {
    printf("=== сигналы: как система окликает процесс ===\n\n");

    /* TODO 3: установить обработчики на SIGTERM и SIGUSR1; при ошибке perror + return 1. */

    /* TODO 4: raise(SIGUSR1) дважды; печать:
         "[agent] SIGUSR1 получен:            да|нет"
         "[agent] обработчик сработал дважды: да|нет"   (usr1_count == 2) */

    /* TODO 5: raise(SIGTERM); печать "[agent] SIGTERM получен:            да|нет";
       затем цикл на 1000 итераций, который прерывается при got_term,
       и печать "[agent] цикл остановлен на итерации %d (чистое завершение)\n". */

    /* TODO 6: SIGKILL нельзя перехватить:
         pipe(ready); fork();
         потомок: close(ready[0]); signal(SIGTERM, SIG_IGN);
                  write(ready[1], "R", 1); close(ready[1]); for(;;) pause();
         родитель: close(ready[1]); read(ready[0], &c, 1); close(ready[0]);
                  kill(pid, SIGTERM); usleep(50000);
                  r = waitpid(pid, &status, WNOHANG);
                  "[child ] пережил SIGTERM (игнорирует):        да|нет"  (r == 0)
                  kill(pid, SIGKILL); waitpid(pid, &status, 0);
                  "[child ] завершён сигналом:                   да|нет"  (WIFSIGNALED)
                  "[child ] это был SIGKILL (перехват невозможен): да|нет" (WTERMSIG) */

    /* TODO 7: финальная сборка — напечатать "\n--- stealth_agent online ---",
       восемь строк "  [OK] ..." (по одной на серию сезона) и
       "\nагент невидим и управляем. Season 7 complete." */

    (void)on_signal; (void)install;
    return 0;
}

static void on_signal(int sig) { (void)sig; /* TODO 1 */ }
static int install(int sig, void (*fn)(int)) { (void)sig; (void)fn; return -1; /* TODO 2 */ }
