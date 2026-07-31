/*
 * OPERATION MOONLIGHT — s07e05 "Гонки и мьютексы"
 * Каркас. Скопируй в artifacts/race.c и закрой TODO.
 *
 *   cp starter.c artifacts/race.c
 *   make test
 *
 * ДЕТЕРМИНИЗМ: в stdout печатай только инварианты (верны всегда),
 * а плавающие значения — в stderr (они разные каждый запуск).
 */
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 4
#define NITER    200000
#define EXPECTED ((long)NTHREADS * NITER)

static long counter_race = 0;
static long counter_safe = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* TODO 1: worker_race — NITER раз: tmp = counter_race; counter_race = tmp + 1;
   (никакой синхронизации — это и есть гонка). */
static void *worker_race(void *arg);

/* TODO 2: worker_safe — то же, но вокруг чтения-записи
   pthread_mutex_lock(&lock) / pthread_mutex_unlock(&lock). */
static void *worker_safe(void *arg);

/* TODO 3: run_all(fn) — создать NTHREADS потоков с функцией fn и дождаться всех. */
static void run_all(void *(*fn)(void *));

int main(void) {
    printf("=== data race и мьютекс ===\n");
    printf("потоков: %d, инкрементов на поток: %d, ожидаемый итог: %ld\n\n",
           NTHREADS, NITER, EXPECTED);

    /* TODO 4: run_all(worker_race); run_all(worker_safe); */

    /* TODO 5: в stderr (fprintf(stderr, ...)):
         "[stderr] без мьютекса получилось: %ld (потеряно %ld)"
         "[stderr] с мьютексом получилось:  %ld" */

    /* TODO 6: в stdout — только инварианты:
         "[race ] итог не больше ожидаемого (обновления могут теряться): да|НЕТ"
         "[race ] итог гарантированно верен:                             нет — зависит от планировщика"
         "[mutex] итог в точности равен ожидаемому:                      да|НЕТ"
         "[mutex] результат воспроизводим между запусками:               да"
       затем:
         "\nвывод: counter++ — это три операции, а не одна."
         "посмотреть реальные числа: make run 2>&1 (они в stderr и каждый раз разные)" */

    (void)worker_race; (void)worker_safe; (void)run_all;
    return 0;
}

static void *worker_race(void *arg) { (void)arg; return NULL; /* TODO 1 */ }
static void *worker_safe(void *arg) { (void)arg; return NULL; /* TODO 2 */ }
static void run_all(void *(*fn)(void *)) { (void)fn; /* TODO 3 */ }
