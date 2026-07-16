/*
 * OPERATION MOONLIGHT — s01e03 "Паттерны во времени"
 * Каркас задачи. Скопируй в artifacts/pattern_finder.c и закрой TODO.
 *
 *   cp starter.c artifacts/pattern_finder.c
 *   make test
 *
 * Концепт: циклы — for/while, накопление, поиск моды, обход аномалий.
 * Задача: по меткам времени найти период (самый частый интервал)
 *         и аномалии (интервалы != периода).
 */

#include <stdio.h>

#define MAX_TS 1024

int main(void) {
    long ts[MAX_TS];
    int n = 0;

    /* TODO 1: while-цикл — читать метки в ts[], пока scanf("%ld", ...) == 1
       и n < MAX_TS. Увеличивай n. */

    printf("=== PATTERN FINDER v2.0 ===\n");
    printf("Reading schedule...\n\n");

    if (n < 2) {
        printf("Недостаточно меток (%d): период не определить.\n", n);
        return 0;
    }

    int intervals = n - 1;
    printf("Timestamps: %d\n", n);
    printf("Intervals:  %d\n\n", intervals);

    /* TODO 2: период = мода интервалов ts[i+1]-ts[i].
       Вложенный цикл: для каждого интервала посчитай, сколько раз он встречается;
       запомни самый частый в period и best_count. */
    long period = ts[1] - ts[0];
    int best_count = 0;
    /* for (...) { for (...) { ... } } */

    printf("Dominant period: %ld s (occurs %dx)\n", period, best_count);

    /* TODO 3: посчитай и выведи аномалии (интервал != period), по одной на строку:
       printf("  at interval %d: gap %ld s\n", i, gap); */
    int anomalies = 0;

    printf("Anomalies (interval != period): %d\n", anomalies);
    /* TODO 3b: цикл вывода аномалий */

    printf("\nThe schedule breaks %d time(s). That break is the real event.\n", anomalies);
    return 0;
}
