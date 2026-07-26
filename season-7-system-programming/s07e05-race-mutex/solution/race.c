/*
 * OPERATION MOONLIGHT — s07e05 "Гонки и мьютексы"
 * Эталонное решение: race.c
 *
 * Концепт серии: если несколько потоков пишут в ОДНУ переменную без синхронизации —
 *                это data race. `counter++` не атомарен: это чтение, инкремент и
 *                запись, между которыми поток могут прервать. Часть обновлений
 *                теряется. Лечится взаимным исключением (mutex).
 * Задача: воспроизвести гонку, затем починить её мьютексом.
 *
 * ДЕТЕРМИНИЗМ: точное «сколько потерялось» непредсказуемо, поэтому в stdout идут
 * только ИНВАРИАНТЫ (они верны всегда), а плавающие числа — в stderr, для глаз.
 */
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 4
#define NITER    200000
#define EXPECTED ((long)NTHREADS * NITER)

static long counter_race = 0;      /* без защиты */
static long counter_safe = 0;      /* под мьютексом */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Гонка: read-modify-write без синхронизации.
   Промежуточная переменная делает окно между чтением и записью явным. */
static void *worker_race(void *arg) {
    (void)arg;
    for (int i = 0; i < NITER; i++) {
        long tmp = counter_race;   /* 1) прочитали  */
        counter_race = tmp + 1;    /* 2) записали   — между 1 и 2 могут вклиниться */
    }
    return NULL;
}

/* Исправление: критическая секция под мьютексом. */
static void *worker_safe(void *arg) {
    (void)arg;
    for (int i = 0; i < NITER; i++) {
        pthread_mutex_lock(&lock);
        long tmp = counter_safe;
        counter_safe = tmp + 1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

static void run_all(void *(*fn)(void *)) {
    pthread_t th[NTHREADS];
    for (int t = 0; t < NTHREADS; t++) pthread_create(&th[t], NULL, fn, NULL);
    for (int t = 0; t < NTHREADS; t++) pthread_join(th[t], NULL);
}

int main(void) {
    printf("=== data race и мьютекс ===\n");
    printf("потоков: %d, инкрементов на поток: %d, ожидаемый итог: %ld\n\n",
           NTHREADS, NITER, EXPECTED);

    run_all(worker_race);
    run_all(worker_safe);

    /* Плавающие значения — в stderr: они меняются от запуска к запуску. */
    fprintf(stderr, "[stderr] без мьютекса получилось: %ld (потеряно %ld)\n",
            counter_race, EXPECTED - counter_race);
    fprintf(stderr, "[stderr] с мьютексом получилось:  %ld\n", counter_safe);

    /* В stdout — только инварианты, они верны при любом планировщике. */
    printf("[race ] итог не больше ожидаемого (обновления могут теряться): %s\n",
           counter_race <= EXPECTED ? "да" : "НЕТ");
    printf("[race ] итог гарантированно верен:                             %s\n",
           "нет — зависит от планировщика");
    printf("[mutex] итог в точности равен ожидаемому:                      %s\n",
           counter_safe == EXPECTED ? "да" : "НЕТ");
    printf("[mutex] результат воспроизводим между запусками:               %s\n", "да");

    printf("\nвывод: counter++ — это три операции, а не одна.\n");
    printf("посмотреть реальные числа: make run 2>&1 (они в stderr и каждый раз разные)\n");
    return 0;
}
