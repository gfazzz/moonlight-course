/*
 * OPERATION MOONLIGHT — s08e03 "ГПСЧ и перемешивание Кнута"
 * Эталонное решение: shuffle.c
 *
 * Концепт серии: «перемешать случайно» — не то же самое, что «перемешать честно».
 *                Наивный цикл со swap на случайный индекс из ВСЕГО массива даёт
 *                СМЕЩЁННОЕ распределение перестановок. Правильный алгоритм —
 *                Фишера–Йетса в форме Кнута: на шаге i меняем местами с индексом
 *                из [0, i]. Вторая ловушка — modulo bias: rand() % n неравномерен,
 *                если диапазон не делится на n нацело. Лечится отбраковкой.
 * Задача: измерить обе ошибки на числах и реализовать честные версии.
 *
 * Долг из Season 5: там мы отложили «честный ГПСЧ и перемешивание Кнута» до S8.
 */
#include <stdio.h>
#include <string.h>

#define TRIALS 60000
#define M      3          /* перемешиваем 3 элемента: 6 перестановок */
#define PERMS  6

/* Детерминированный ГПСЧ (LCG), общий для всех экспериментов. */
static unsigned long rng_state = 424242UL;
static void rng_seed(unsigned long s) { rng_state = s; }
static unsigned long rng_next(void) {          /* 31 значащий бит */
    rng_state = rng_state * 1103515245UL + 12345UL;
    return (rng_state >> 16) & 0x7FFFFFFF;
}

/* ЧЕСТНЫЙ выбор: отбраковка «хвоста», который не делится на n нацело. */
static int below_fair(int n) {
    unsigned long range = 0x80000000UL;             /* rng_next() ∈ [0, 2^31) */
    unsigned long limit = range - (range % (unsigned long)n);  /* кратная n граница */
    unsigned long r;
    do { r = rng_next(); } while (r >= limit);      /* хвост отбрасываем */
    return (int)(r % (unsigned long)n);
}

/* Индекс перестановки трёх элементов (0..5) для подсчёта частот. */
static int perm_index(const int *a) {
    static const int table[PERMS][M] = {
        {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
    };
    for (int p = 0; p < PERMS; p++)
        if (a[0] == table[p][0] && a[1] == table[p][1] && a[2] == table[p][2]) return p;
    return -1;
}

static void swap(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; }

/* НАИВНОЕ тасование: меняем i с любым индексом всего массива. */
static void shuffle_naive(int *a, int n) {
    for (int i = 0; i < n; i++) swap(a, i, below_fair(n));
}

/* Фишер–Йетс (Кнут): идём с конца, меняем i с индексом из [0, i]. */
static void shuffle_knuth(int *a, int n) {
    for (int i = n - 1; i > 0; i--) swap(a, i, below_fair(i + 1));
}

static void measure(const char *title, void (*fn)(int *, int), unsigned long seed) {
    int counts[PERMS] = {0};
    rng_seed(seed);
    for (int t = 0; t < TRIALS; t++) {
        int a[M] = {0, 1, 2};
        fn(a, M);
        counts[perm_index(a)]++;
    }
    int expect = TRIALS / PERMS;
    int worst = 0;
    printf("%s\n", title);
    for (int p = 0; p < PERMS; p++) {
        int dev = counts[p] - expect;
        if (dev < 0) dev = -dev;
        if (dev > worst) worst = dev;
        printf("  перестановка %d: %6d  (отклонение %+5d)\n", p, counts[p], counts[p] - expect);
    }
    printf("  ожидалось по %d; максимальное отклонение: %d (%.2f%%)\n\n",
           expect, worst, 100.0 * worst / expect);
}

int main(void) {
    printf("=== честный ГПСЧ и перемешивание Кнута ===\n");
    printf("испытаний: %d, элементов: %d, перестановок: %d\n\n", TRIALS, M, PERMS);

    measure("--- наивное тасование (swap с любым индексом) ---", shuffle_naive, 424242UL);
    measure("--- Фишер–Йетс / Кнут (swap с индексом из [0, i]) ---", shuffle_knuth, 424242UL);

    /* --- Вторая ловушка: modulo bias ---
       На диапазоне 2^31 остаток по модулю 3 смещён лишь на ~1e-9 — незаметно.
       Но эффект РЕАЛЕН и растёт, когда диапазон источника мал (или n к нему близко).
       Возьмём источник [0,10) и три корзины: 10 % 3 = 1, поэтому нулевая корзина
       получает лишний исход — 4/10 против 3/10. */
    printf("--- modulo bias: источник [0,10), 3 корзины ---\n");
    const int N = 3, RANGE = 10, DRAWS = 120000;

    int hist_bad[3] = {0}, hist_ok[3] = {0};

    rng_seed(777);
    for (int i = 0; i < DRAWS; i++) {
        int v = (int)(rng_next() % (unsigned long)RANGE);   /* источник [0,10) */
        hist_bad[v % N]++;                                   /* остаток — смещён */
    }
    rng_seed(777);
    for (int i = 0; i < DRAWS; i++) {
        int v;
        do { v = (int)(rng_next() % (unsigned long)RANGE); } while (v >= 9); /* отбраковка */
        hist_ok[v % N]++;
    }

    int exp_each = DRAWS / N;
    printf("  ожидалось по %d в каждой корзине\n", exp_each);
    for (int i = 0; i < N; i++)
        printf("    корзина %d: остаток %6d (%+5.1f%%), отбраковка %6d (%+4.1f%%)\n",
               i, hist_bad[i], 100.0 * (hist_bad[i] - exp_each) / exp_each,
               hist_ok[i], 100.0 * (hist_ok[i] - exp_each) / exp_each);
    printf("  корзина 0 получает лишний исход (10 %% 3 = 1) — это и есть modulo bias\n\n");

    printf("вывод: и тасование, и выбор индекса надо делать честно —\n");
    printf("       иначе смещение попадёт в разбиение выборки и в модель.\n");
    return 0;
}
