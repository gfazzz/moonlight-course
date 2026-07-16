/*
 * OPERATION MOONLIGHT — s01e03 "Паттерны во времени"
 * Эталонное решение: pattern_finder.c
 *
 * Концепт серии: циклы — for/while, накопление, поиск максимума/моды,
 *                прерывание аномалий. Читаем расписание, ищем период и сбой.
 *
 * Вход (stdin): по одной метке времени (UNIX ts) на строку.
 * Период = самый частый интервал между соседними метками.
 * Аномалия = интервал, не равный периоду (сбой расписания).
 */

#include <stdio.h>

#define MAX_TS 1024

int main(void) {
    long ts[MAX_TS];
    int n = 0;

    /* while: читаем метки, пока не кончатся (scanf вернёт 1 при успехе). */
    while (n < MAX_TS && scanf("%ld", &ts[n]) == 1) {
        n++;
    }

    printf("=== PATTERN FINDER v2.0 ===\n");
    printf("Reading schedule...\n\n");

    if (n < 2) {
        printf("Недостаточно меток (%d): период не определить.\n", n);
        return 0;
    }

    int intervals = n - 1;
    printf("Timestamps: %d\n", n);
    printf("Intervals:  %d\n\n", intervals);

    /* Мода интервалов: для каждого интервала считаем, сколько раз он встречается,
       и запоминаем самый частый. Вложенный цикл — O(n^2), но n мал. */
    long period = ts[1] - ts[0];
    int best_count = 0;
    for (int i = 0; i + 1 < n; i++) {
        long gap = ts[i + 1] - ts[i];
        int count = 0;
        for (int j = 0; j + 1 < n; j++) {
            if (ts[j + 1] - ts[j] == gap) count++;
        }
        if (count > best_count) { best_count = count; period = gap; }
    }

    printf("Dominant period: %ld s (occurs %dx)\n", period, best_count);

    /* Аномалии: интервалы, отличные от периода. */
    int anomalies = 0;
    for (int i = 0; i + 1 < n; i++) {
        long gap = ts[i + 1] - ts[i];
        if (gap != period) anomalies++;
    }
    printf("Anomalies (interval != period): %d\n", anomalies);
    for (int i = 0; i + 1 < n; i++) {
        long gap = ts[i + 1] - ts[i];
        if (gap != period) printf("  at interval %d: gap %ld s\n", i, gap);
    }

    printf("\nThe schedule breaks %d time(s). That break is the real event.\n", anomalies);
    return 0;
}
