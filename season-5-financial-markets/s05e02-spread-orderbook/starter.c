/*
 * OPERATION MOONLIGHT — s05e02 "Спред и стакан"
 * Каркас. Скопируй в artifacts/book.c и закрой TODO.
 *
 *   cp starter.c artifacts/book.c
 *   make test        # запустит: book ../data/book.csv
 *
 * Концепт: стакан = bid (покупка) + ask (продажа). best bid — максимальная цена
 *          покупки, best ask — минимальная цена продажи. spread = ask - bid.
 * CSV: side,price,size  (side = bid|ask)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVELS 256

typedef struct { double price; long size; } Level;

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "book.csv";
    FILE *f = fopen(path, "r");
    if (f == NULL) { perror("fopen"); return 1; }

    Level bids[MAX_LEVELS], asks[MAX_LEVELS];
    int nb = 0, na = 0;
    char line[128], side[8];
    double price; long size;

    if (!fgets(line, sizeof line, f)) { fclose(f); return 1; }   /* заголовок */
    /* TODO 1: читай строки; sscanf(line, "%7[^,],%lf,%ld", side, &price, &size) == 3;
       по side клади в bids[] или asks[]. */
    (void)price; (void)size;
    fclose(f);

    printf("=== ORDER BOOK ===\n");
    printf("bids: %d levels, asks: %d levels\n\n", nb, na);
    if (nb == 0 || na == 0) { printf("empty side\n"); return 0; }

    /* TODO 2: best_bid = максимум bids[].price; best_ask = минимум asks[].price. */

    /* TODO 3: spread = best_ask - best_bid; mid = (bid+ask)/2;
       spread_bps = spread/mid*10000. Печать:
         "best bid: %.2f", "best ask: %.2f",
         "spread:   %.2f (%.1f bps)", "mid:      %.3f". */

    /* TODO 4: vb, va — суммы size; imbalance = (vb-va)/(vb+va).
         "bid volume: %ld, ask volume: %ld"
         "imbalance: %+.3f (buy|sell|balanced pressure)" — порог ±0.05. */

    (void)bids; (void)asks;
    return 0;
}
