/*
 * OPERATION MOONLIGHT — s07e04 "Потоки (pthread)"
 * Каркас. Скопируй в artifacts/threads.c и закрой TODO.
 *
 *   cp starter.c artifacts/threads.c
 *   make test
 *
 * Ключ: сид ГПСЧ привязан к НОМЕРУ траектории => параллельный результат
 * побитово совпадает с последовательным, как бы ни распределил работу планировщик.
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define NSIM      10000
#define HORIZON   20
#define NTHREADS  4
#define START     50000000.0
#define DAILY_SIG 0.013

static double pnl_seq[NSIM];
static double pnl_par[NSIM];

static unsigned long seed_for(int path) {
    return 12345UL + (unsigned long)path * 2654435761UL;
}
static double uniform(unsigned long *s) {
    *s = *s * 1103515245UL + 12345UL;
    return (double)((*s >> 16) & 0x7FFF) / 32768.0;
}
static double normal(unsigned long *s) {
    double acc = 0.0;
    for (int i = 0; i < 12; i++) acc += uniform(s);
    return acc - 6.0;
}

/* TODO 1: simulate(path) — s = seed_for(path); value = START;
   HORIZON раз: value *= (1.0 + DAILY_SIG * normal(&s)); вернуть value - START. */
static double simulate(int path);

typedef struct { int lo, hi; } Range;

/* TODO 2: worker(arg) — привести arg к Range*, заполнить pnl_par[p] = simulate(p)
   для p из [lo, hi); вернуть NULL. */
static void *worker(void *arg);

int main(void) {
    printf("=== pthread: параллельный Monte Carlo (долг из Season 5) ===\n");
    printf("траекторий: %d, горизонт: %d дней, потоков: %d\n\n", NSIM, HORIZON, NTHREADS);

    /* TODO 3: последовательно заполнить pnl_seq[p] = simulate(p);
       печать "[seq] посчитано траекторий: %d". */

    /* TODO 4: параллельно: chunk = NSIM/NTHREADS;
       для каждого t: rng[t].lo = t*chunk; rng[t].hi = (последний ? NSIM : (t+1)*chunk);
       pthread_create(&th[t], NULL, worker, &rng[t]);  затем pthread_join всех.
       печать "[par] потоков запущено и собрано: %d"
       и для каждого "      поток %d считал траектории [%5d, %5d)". */

    /* TODO 5: identical = (memcmp(pnl_seq, pnl_par, sizeof pnl_seq) == 0);
       "\nрезультаты seq и par совпадают побитово: да|НЕТ"
       "почему: поток ГПСЧ привязан к номеру траектории, а не к порядку счёта". */

    /* TODO 6: посчитать worst/best/losses по pnl_par и напечатать:
       "\nworst case: $%.0f", "best case:  $%+.0f",
       "убыточных сценариев: %d из %d". */

    (void)simulate; (void)worker;
    return 0;
}

static double simulate(int path) { (void)path; return 0.0; /* TODO 1 */ }
static void *worker(void *arg) { (void)arg; return NULL; /* TODO 2 */ }
