/*
 * OPERATION MOONLIGHT — s08e03 "ГПСЧ и перемешивание Кнута"
 * Каркас. Скопируй в artifacts/shuffle.c и закрой TODO.
 *
 *   cp starter.c artifacts/shuffle.c
 *   make test
 *
 * Две ловушки: (1) наивное тасование смещает распределение перестановок;
 *              (2) остаток от деления смещает выбор индекса (modulo bias).
 */
#include <stdio.h>
#include <string.h>

#define TRIALS 60000
#define M      3
#define PERMS  6

static unsigned long rng_state = 424242UL;
static void rng_seed(unsigned long s) { rng_state = s; }
static unsigned long rng_next(void) {
    rng_state = rng_state * 1103515245UL + 12345UL;
    return (rng_state >> 16) & 0x7FFFFFFF;
}

/* TODO 1: below_fair(n) — честный выбор из [0, n) с отбраковкой:
     range = 0x80000000UL; limit = range - (range % n);
     do { r = rng_next(); } while (r >= limit); return r % n. */
static int below_fair(int n);

static int perm_index(const int *a) {
    static const int table[PERMS][M] = {
        {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
    };
    for (int p = 0; p < PERMS; p++)
        if (a[0] == table[p][0] && a[1] == table[p][1] && a[2] == table[p][2]) return p;
    return -1;
}

static void swap(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; }

/* TODO 2: shuffle_naive — для i от 0 до n-1: swap(a, i, below_fair(n)).
   (Это НЕПРАВИЛЬНЫЙ алгоритм — он нужен, чтобы измерить смещение.) */
static void shuffle_naive(int *a, int n);

/* TODO 3: shuffle_knuth — для i от n-1 вниз до 1: swap(a, i, below_fair(i + 1)). */
static void shuffle_knuth(int *a, int n);

/* TODO 4: measure(title, fn, seed) — TRIALS раз перемешать {0,1,2},
   считать частоты perm_index; печать (см. tests/expected.txt):
     "%s"
     "  перестановка %d: %6d  (отклонение %+5d)"
     "  ожидалось по %d; максимальное отклонение: %d (%.2f%%)\n" */
static void measure(const char *title, void (*fn)(int *, int), unsigned long seed);

int main(void) {
    printf("=== честный ГПСЧ и перемешивание Кнута ===\n");
    printf("испытаний: %d, элементов: %d, перестановок: %d\n\n", TRIALS, M, PERMS);

    /* TODO 5: measure для наивного и для Кнута (сид 424242 обоим):
         "--- наивное тасование (swap с любым индексом) ---"
         "--- Фишер–Йетс / Кнут (swap с индексом из [0, i]) ---" */

    /* TODO 6: modulo bias на малом диапазоне:
         RANGE = 10, N = 3, DRAWS = 120000;
         смещённо:  v = rng_next() % RANGE; hist_bad[v % N]++;
         честно:    do { v = rng_next() % RANGE; } while (v >= 9); hist_ok[v % N]++;
       печать:
         "--- modulo bias: источник [0,10), 3 корзины ---"
         "  ожидалось по %d в каждой корзине"
         "    корзина %d: остаток %6d (%+5.1f%%), отбраковка %6d (%+4.1f%%)"
         "  корзина 0 получает лишний исход (10 %% 3 = 1) — это и есть modulo bias\n"
       и финальный вывод (две строки, см. expected.txt). */

    (void)below_fair; (void)shuffle_naive; (void)shuffle_knuth; (void)measure; (void)perm_index;
    return 0;
}

static int below_fair(int n) { (void)n; return 0; /* TODO 1 */ }
static void shuffle_naive(int *a, int n) { (void)a; (void)n; /* TODO 2 */ }
static void shuffle_knuth(int *a, int n) { (void)a; (void)n; /* TODO 3 */ }
static void measure(const char *t, void (*fn)(int *, int), unsigned long s) {
    (void)t; (void)fn; (void)s; /* TODO 4 */
}
