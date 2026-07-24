/*
 * OPERATION MOONLIGHT — s05e01 "OHLCV-парсер"
 * Эталонное решение: parse.c
 *
 * Концепт серии: рыночные данные приходят как CSV (Open/High/Low/Close/Volume).
 *                Разобрать их в структуры — первый шаг любого анализа. В C это
 *                быстро (один проход, без аллокаций на строку), и это его смысл в FinTech.
 * Задача: распарсить свечи, посчитать сводку и отметить аномальные объёмы.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_BARS 4096

typedef struct {
    long   ts;
    double open, high, low, close;
    long   volume;
} Bar;

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "prices.csv";
    FILE *f = fopen(path, "r");
    if (f == NULL) { perror("fopen"); return 1; }

    Bar bars[MAX_BARS];
    int n = 0;
    char line[256];

    /* Пропускаем строку заголовка. */
    if (!fgets(line, sizeof line, f)) { fclose(f); fprintf(stderr, "empty file\n"); return 1; }

    while (n < MAX_BARS && fgets(line, sizeof line, f)) {
        Bar b;
        /* CSV: ts,open,high,low,close,volume — sscanf возвращает число полей. */
        if (sscanf(line, "%ld,%lf,%lf,%lf,%lf,%ld",
                   &b.ts, &b.open, &b.high, &b.low, &b.close, &b.volume) == 6) {
            bars[n++] = b;
        }
    }
    fclose(f);

    printf("=== OHLCV PARSER ===\n");
    printf("parsed %d bars\n\n", n);

    if (n == 0) return 0;

    /* Сводка: суммарный объём, диапазон цен, средний объём. */
    long   total_vol = 0;
    double hi = bars[0].high, lo = bars[0].low;
    for (int i = 0; i < n; i++) {
        total_vol += bars[i].volume;
        if (bars[i].high > hi) hi = bars[i].high;
        if (bars[i].low  < lo) lo = bars[i].low;
    }
    double avg_vol = (double)total_vol / n;

    printf("price range: %.2f .. %.2f\n", lo, hi);
    printf("total volume: %ld  (avg %.0f per bar)\n", total_vol, avg_vol);

    /* Аномалия объёма: > 3x среднего — типичный маркер манипуляции. */
    printf("\n--- volume anomalies (> 3x average) ---\n");
    int flagged = 0;
    for (int i = 0; i < n; i++) {
        if ((double)bars[i].volume > 3.0 * avg_vol) {
            double change = (bars[i].close - bars[i].open) / bars[i].open * 100.0;
            printf("bar %d: vol %ld (%.1fx), close %+.2f%%\n",
                   i, bars[i].volume, bars[i].volume / avg_vol, change);
            flagged++;
        }
    }
    printf("flagged %d suspicious bar(s)\n", flagged);
    return 0;
}
