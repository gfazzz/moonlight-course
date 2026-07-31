/*
 * OPERATION MOONLIGHT — s05e08 "Арбитраж и pump & dump" (финал Season 5)
 * Эталонное решение: arbitrage.c
 *
 * Концепт серии: живые деньги схемы текут через крипто-биржи. Две классические
 *                картины: арбитраж (один актив, разные цены на биржах) и
 *                pump & dump (искусственный разгон объёмом, затем сброс).
 * Задача: найти арбитраж с учётом комиссий и детектировать pump&dump.
 *
 * Собирает сезон: разбор данных (e01), аномалии объёма, проценты, пороги.
 */
#include <stdio.h>

#define NEX 4
#define NBARS 12
#define FEE_PCT 0.30      /* комиссия за сделку, % (в одну сторону) */

typedef struct { const char *name; double bid, ask; } Quote;

int main(void) {
    /* Часть 1: котировки токена MOON на четырёх площадках. */
    Quote ex[NEX] = {
        {"Binance", 1.0200, 1.0250},
        {"Kraken",  1.0480, 1.0520},
        {"Uniswap", 1.0310, 1.0360},
        {"OKX",     1.0270, 1.0300}
    };

    printf("=== CRYPTO ARBITRAGE & PUMP-DUMP ===\n");
    printf("token MOON across %d exchanges\n\n", NEX);

    /* Арбитраж: купить там, где ask МИНИМАЛЬНЫЙ; продать там, где bid МАКСИМАЛЬНЫЙ. */
    int buy = 0, sell = 0;
    for (int i = 1; i < NEX; i++) {
        if (ex[i].ask < ex[buy].ask)  buy = i;
        if (ex[i].bid > ex[sell].bid) sell = i;
    }
    double gross_pct = (ex[sell].bid - ex[buy].ask) / ex[buy].ask * 100.0;
    double net_pct = gross_pct - 2.0 * FEE_PCT;      /* комиссия с обеих сторон */

    printf("buy  on %-8s @ ask %.4f\n", ex[buy].name, ex[buy].ask);
    printf("sell on %-8s @ bid %.4f\n", ex[sell].name, ex[sell].bid);
    printf("gross %.2f%%, net after fees %.2f%% -> %s\n\n",
           gross_pct, net_pct, net_pct > 0 ? "ARBITRAGE" : "no profit");

    /* Часть 2: pump & dump по ряду цена/объём. */
    double price[NBARS] = {1.00,1.01,1.00,1.02,1.35,1.62,1.58,1.20,0.85,0.70,0.68,0.69};
    long   vol[NBARS]   = {12000,11000,13000,12500,95000,180000,60000,140000,210000,90000,20000,18000};

    /* Базовый объём — среднее «спокойного» периода (первые 4 бара), а не всего ряда:
       иначе объёмы самого пампа раздувают среднее и порог не срабатывает. */
    long base = 0;
    for (int i = 0; i < 4; i++) base += vol[i];
    double base_vol = (double)base / 4.0;

    printf("--- pump & dump scan (baseline vol %.0f) ---\n", base_vol);
    int pump = -1, dump = -1;
    for (int i = 1; i < NBARS; i++) {
        double ch = (price[i] - price[i - 1]) / price[i - 1] * 100.0;
        if (ch > 20.0 && (double)vol[i] > 3.0 * base_vol && pump < 0) {
            pump = i;
            printf("PUMP  at bar %d: %+.1f%% on volume %ld (%.1fx baseline)\n",
                   i, ch, vol[i], vol[i] / base_vol);
        }
        if (pump >= 0 && i > pump && ch < -20.0 && dump < 0) {
            dump = i;
            printf("DUMP  at bar %d: %+.1f%% (%.4f)\n", i, ch, price[i]);
        }
    }

    if (pump >= 0 && dump >= 0) {
        /* Реальный масштаб: пик цены и дно ПОСЛЕ пика. */
        int peak_i = pump;
        for (int i = pump; i < NBARS; i++) if (price[i] > price[peak_i]) peak_i = i;
        int bot_i = peak_i;
        for (int i = peak_i; i < NBARS; i++) if (price[i] < price[bot_i]) bot_i = i;
        printf("\nSCHEME CONFIRMED: peak %.4f (bar %d), bottom %.4f (bar %d) -> %.0f%% wipeout\n",
               price[peak_i], peak_i, price[bot_i], bot_i,
               (price[bot_i] - price[peak_i]) / price[peak_i] * 100.0);
    } else {
        printf("\nno pump&dump pattern\n");
    }
    return 0;
}
