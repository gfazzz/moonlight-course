/*
 * OPERATION MOONLIGHT — s07e08 "Сигналы и stealth-финал" (финал Season 7)
 * Эталонное решение: signals.c
 *
 * Концепт серии: сигнал — асинхронное уведомление от ядра или другого процесса.
 *                Обработчик прерывает поток выполнения в произвольной точке,
 *                поэтому в нём МОЖНО крайне мало: по сути — выставить флаг
 *                volatile sig_atomic_t, а решать уже в основном цикле.
 *                SIGKILL и SIGSTOP перехватить нельзя — это гарантия ядра.
 * Задача: корректно обработать SIGTERM/SIGUSR1, завершиться чисто, доказать
 *         неперехватываемость SIGKILL и собрать stealth_agent.
 *
 * Финал собирает сезон: процессы, exec, демоны, потоки, мьютексы, каналы, mmap.
 */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

/* Единственное, что безопасно менять из обработчика. */
static volatile sig_atomic_t got_term = 0;
static volatile sig_atomic_t got_usr1 = 0;
static volatile sig_atomic_t usr1_count = 0;

static void on_signal(int sig) {
    if (sig == SIGTERM) got_term = 1;
    if (sig == SIGUSR1) { got_usr1 = 1; usr1_count++; }
    /* Никаких printf/malloc здесь: они не async-signal-safe. */
}

/* sigaction вместо signal(): переносимое и предсказуемое поведение. */
static int install(int sig, void (*fn)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = fn;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;          /* прерванные syscall'ы перезапускать */
    return sigaction(sig, &sa, NULL);
}

int main(void) {
    printf("=== сигналы: как система окликает процесс ===\n\n");

    if (install(SIGTERM, on_signal) != 0 || install(SIGUSR1, on_signal) != 0) {
        perror("sigaction");
        return 1;
    }

    /* 1. Пользовательский сигнал самому себе: raise доставляет синхронно. */
    raise(SIGUSR1);
    raise(SIGUSR1);
    printf("[agent] SIGUSR1 получен:            %s\n", got_usr1 ? "да" : "нет");
    printf("[agent] обработчик сработал дважды: %s\n", usr1_count == 2 ? "да" : "нет");

    /* 2. Просьба завершиться: флаг ставится, работа останавливается штатно. */
    raise(SIGTERM);
    printf("[agent] SIGTERM получен:            %s\n", got_term ? "да" : "нет");

    int processed = 0;
    for (int i = 0; i < 1000; i++) {
        if (got_term) break;           /* решение принимает основной цикл, не обработчик */
        processed++;
    }
    printf("[agent] цикл остановлен на итерации %d (чистое завершение)\n\n", processed);

    /* 3. SIGKILL перехватить нельзя. Потомок игнорирует SIGTERM и ждёт. */
    int ready[2];
    if (pipe(ready) != 0) { perror("pipe"); return 1; }

    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        close(ready[0]);
        signal(SIGTERM, SIG_IGN);      /* демонстративно игнорируем */
        write(ready[1], "R", 1);       /* сообщаем: готов */
        close(ready[1]);
        for (;;) pause();              /* ждём сигнал, который нас всё же убьёт */
    }

    close(ready[1]);
    char c;
    read(ready[0], &c, 1);             /* барьер: потомок точно установил игнор */
    close(ready[0]);

    kill(pid, SIGTERM);                /* проигнорирован */
    usleep(50000);
    int status = 0;
    pid_t r = waitpid(pid, &status, WNOHANG);
    printf("[child ] пережил SIGTERM (игнорирует):        %s\n", r == 0 ? "да" : "нет");

    kill(pid, SIGKILL);                /* а вот это не обсуждается */
    waitpid(pid, &status, 0);
    printf("[child ] завершён сигналом:                   %s\n",
           WIFSIGNALED(status) ? "да" : "нет");
    printf("[child ] это был SIGKILL (перехват невозможен): %s\n",
           WTERMSIG(status) == SIGKILL ? "да" : "нет");

    /* 4. Финальная сборка сезона. */
    printf("\n--- stealth_agent online ---\n");
    const char *sub[] = {
        "процессы, fork (e01)", "запуск программ, exec (e02)",
        "уход в фон, демон (e03)", "потоки, pthread (e04)",
        "мьютексы, гонки (e05)",  "каналы, IPC (e06)",
        "разделяемая память, mmap (e07)", "сигналы, чистый выход (e08)"
    };
    for (int i = 0; i < 8; i++) printf("  [OK] %s\n", sub[i]);
    printf("\nагент невидим и управляем. Season 7 complete.\n");
    return 0;
}
