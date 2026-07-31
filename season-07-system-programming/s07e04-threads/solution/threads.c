/*
 * OPERATION MOONLIGHT — s07e04 "Потоки (pthread)"
 * Эталонное решение: threads.c
 *
 * Концепт серии: потоки живут ВНУТРИ одного процесса и делят адресное пространство.
 *                Это делает их дешёвыми и опасными одновременно. Здесь — «неопасный»
 *                случай: задача разбита на НЕПЕРЕСЕКАЮЩИЕСЯ куски, синхронизация не
 *                нужна вовсе. Ключевой урок: параллельный счёт может быть ПОБИТОВО
 *                воспроизводимым — если поток случайных чисел зависит от НОМЕРА
 *                задачи, а не от порядка выполнения.
 *
 * Долг из Season 5: там Monte Carlo был секвенциальным, а параллельный обещан
 * «после Season 7». Вот он.
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

/* Поток случайных чисел, ПРИВЯЗАННЫЙ К НОМЕРУ ТРАЕКТОРИИ.
   Поэтому результат не зависит от того, кто и в каком порядке её считал. */
static unsigned long seed_for(int path) {
    return 12345UL + (unsigned long)path * 2654435761UL;   /* множитель Кнута */
}
static double uniform(unsigned long *s) {
    *s = *s * 1103515245UL + 12345UL;
    return (double)((*s >> 16) & 0x7FFF) / 32768.0;
}
static double normal(unsigned long *s) {          /* N(0,1) по ЦПТ, без -lm */
    double acc = 0.0;
    for (int i = 0; i < 12; i++) acc += uniform(s);
    return acc - 6.0;
}

/* Одна траектория: детерминирована своим номером. */
static double simulate(int path) {
    unsigned long s = seed_for(path);
    double value = START;
    for (int d = 0; d < HORIZON; d++) value *= (1.0 + DAILY_SIG * normal(&s));
    return value - START;
}

typedef struct { int lo, hi; } Range;             /* полуинтервал [lo, hi) */

static void *worker(void *arg) {
    Range *r = (Range *)arg;
    for (int p = r->lo; p < r->hi; p++) pnl_par[p] = simulate(p);
    return NULL;
}

int main(void) {
    printf("=== pthread: параллельный Monte Carlo (долг из Season 5) ===\n");
    printf("траекторий: %d, горизонт: %d дней, потоков: %d\n\n", NSIM, HORIZON, NTHREADS);

    /* 1. Последовательно — как в s05e07. */
    for (int p = 0; p < NSIM; p++) pnl_seq[p] = simulate(p);
    printf("[seq] посчитано траекторий: %d\n", NSIM);

    /* 2. Параллельно: делим диапазон на непересекающиеся куски. */
    pthread_t th[NTHREADS];
    Range rng[NTHREADS];
    int chunk = NSIM / NTHREADS;

    for (int t = 0; t < NTHREADS; t++) {
        rng[t].lo = t * chunk;
        rng[t].hi = (t == NTHREADS - 1) ? NSIM : (t + 1) * chunk;
        if (pthread_create(&th[t], NULL, worker, &rng[t]) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            return 1;
        }
    }
    for (int t = 0; t < NTHREADS; t++) pthread_join(th[t], NULL);

    printf("[par] потоков запущено и собрано: %d\n", NTHREADS);
    for (int t = 0; t < NTHREADS; t++)
        printf("      поток %d считал траектории [%5d, %5d)\n", t, rng[t].lo, rng[t].hi);

    /* 3. Главная проверка: результаты совпали ПОБИТОВО. */
    int identical = (memcmp(pnl_seq, pnl_par, sizeof pnl_seq) == 0);
    printf("\nрезультаты seq и par совпадают побитово: %s\n", identical ? "да" : "НЕТ");
    printf("почему: поток ГПСЧ привязан к номеру траектории, а не к порядку счёта\n");

    /* 4. Риск-метрика поверх параллельного результата (как в Season 5). */
    double worst = pnl_par[0], best = pnl_par[0];
    int losses = 0;
    for (int p = 0; p < NSIM; p++) {
        if (pnl_par[p] < worst) worst = pnl_par[p];
        if (pnl_par[p] > best)  best  = pnl_par[p];
        if (pnl_par[p] < 0) losses++;
    }
    printf("\nworst case: $%.0f\n", worst);
    printf("best case:  $%+.0f\n", best);
    printf("убыточных сценариев: %d из %d\n", losses, NSIM);

    return 0;
}
