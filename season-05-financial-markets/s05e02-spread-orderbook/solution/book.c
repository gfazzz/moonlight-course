/*
 * OPERATION MOONLIGHT — s05e02 "Спред и стакан"
 * Эталонное решение: book.c
 *
 * Концепт серии: биржевой стакан (order book) — заявки на покупку (bid) и
 *                продажу (ask). Лучший bid и лучший ask задают спред — стоимость
 *                мгновенного входа. Дисбаланс объёмов намекает на давление.
 * Задача: прочитать стакан, найти best bid/ask, спред, mid-price и imbalance.
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
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%7[^,],%lf,%ld", side, &price, &size) != 3) continue;
        if (strcmp(side, "bid") == 0 && nb < MAX_LEVELS) { bids[nb].price = price; bids[nb].size = size; nb++; }
        else if (strcmp(side, "ask") == 0 && na < MAX_LEVELS) { asks[na].price = price; asks[na].size = size; na++; }
    }
    fclose(f);

    printf("=== ORDER BOOK ===\n");
    printf("bids: %d levels, asks: %d levels\n\n", nb, na);
    if (nb == 0 || na == 0) { printf("empty side\n"); return 0; }

    /* Best bid — самая ВЫСОКАЯ цена покупки; best ask — самая НИЗКАЯ цена продажи. */
    double best_bid = bids[0].price;
    double best_ask = asks[0].price;
    for (int i = 1; i < nb; i++) if (bids[i].price > best_bid) best_bid = bids[i].price;
    for (int i = 1; i < na; i++) if (asks[i].price < best_ask) best_ask = asks[i].price;

    double spread = best_ask - best_bid;
    double mid = (best_bid + best_ask) / 2.0;
    double spread_bps = spread / mid * 10000.0;      /* спред в базисных пунктах */

    printf("best bid: %.2f\n", best_bid);
    printf("best ask: %.2f\n", best_ask);
    printf("spread:   %.2f (%.1f bps)\n", spread, spread_bps);
    printf("mid:      %.3f\n", mid);

    /* Дисбаланс: (объём bid - объём ask) / (сумма). > 0 -> давление вверх. */
    long vb = 0, va = 0;
    for (int i = 0; i < nb; i++) vb += bids[i].size;
    for (int i = 0; i < na; i++) va += asks[i].size;
    double imbalance = (double)(vb - va) / (double)(vb + va);

    printf("\nbid volume: %ld, ask volume: %ld\n", vb, va);
    printf("imbalance: %+.3f (%s pressure)\n", imbalance,
           imbalance > 0.05 ? "buy" : (imbalance < -0.05 ? "sell" : "balanced"));
    return 0;
}
