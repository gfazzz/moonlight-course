/*
 * OPERATION MOONLIGHT — s05e01 "OHLCV-парсер"
 * Каркас. Скопируй в artifacts/parse.c и закрой TODO.
 *
 *   cp starter.c artifacts/parse.c
 *   make test        # запустит: parse ../data/prices.csv
 *
 * Концепт: разобрать рыночные CSV-свечи в структуры за один проход.
 * CSV: timestamp,open,high,low,close,volume
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

    /* TODO 1: пропусти строку заголовка (первый fgets). */

    /* TODO 2: читай строки; sscanf(line, "%ld,%lf,%lf,%lf,%lf,%ld", ...) == 6;
       сохраняй разобранную свечу в bars[n++]. */

    fclose(f);

    printf("=== OHLCV PARSER ===\n");
    printf("parsed %d bars\n\n", n);
    if (n == 0) return 0;

    /* TODO 3: сводка — total_vol, min low, max high, avg_vol = total/n.
       Печать "price range: %.2f .. %.2f" и "total volume: %ld  (avg %.0f per bar)". */

    /* TODO 4: аномалии — объём > 3x среднего:
       change = (close-open)/open*100;
       "bar %d: vol %ld (%.1fx), close %+.2f%%"; посчитай flagged.
       В конце: "flagged %d suspicious bar(s)". */
    (void)bars;
    return 0;
}
