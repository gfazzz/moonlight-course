/*
 * OPERATION MOONLIGHT — s05e08 "Арбитраж и pump & dump" (финал Season 5)
 * Каркас. Скопируй в artifacts/arbitrage.c и закрой TODO.
 *
 *   cp starter.c artifacts/arbitrage.c
 *   make test
 *
 * Концепт: арбитраж (один актив, разные цены на биржах, с учётом комиссий) +
 *          детект pump&dump (разгон объёмом -> сброс).
 */
#include <stdio.h>

#define NEX 4
#define NBARS 12
#define FEE_PCT 0.30

typedef struct { const char *name; double bid, ask; } Quote;

int main(void) {
    Quote ex[NEX] = {
        {"Binance", 1.0200, 1.0250},
        {"Kraken",  1.0480, 1.0520},
        {"Uniswap", 1.0310, 1.0360},
        {"OKX",     1.0270, 1.0300}
    };

    printf("=== CRYPTO ARBITRAGE & PUMP-DUMP ===\n");
    printf("token MOON across %d exchanges\n\n", NEX);

    /* TODO 1: buy = биржа с МИНИМАЛЬНЫМ ask; sell = биржа с МАКСИМАЛЬНЫМ bid.
       gross_pct = (sell.bid - buy.ask)/buy.ask*100; net_pct = gross_pct - 2*FEE_PCT.
       Печать (формат — см. tests/expected.txt):
         "buy  on %-8s @ ask %.4f", "sell on %-8s @ bid %.4f",
         "gross %.2f%%, net after fees %.2f%% -> ARBITRAGE|no profit". */

    double price[NBARS] = {1.00,1.01,1.00,1.02,1.35,1.62,1.58,1.20,0.85,0.70,0.68,0.69};
    long   vol[NBARS]   = {12000,11000,13000,12500,95000,180000,60000,140000,210000,90000,20000,18000};

    /* TODO 2: base_vol = среднее объёма ПЕРВЫХ 4 баров (спокойный период). */

    printf("--- pump & dump scan ---\n");
    /* TODO 3: найди PUMP (ch > 20%% и vol > 3*base_vol) и следующий за ним DUMP (ch < -20%%).
       Печать "PUMP  at bar %d: ..." и "DUMP  at bar %d: ...".
       Если оба найдены: реальный пик (max price после пампа) и дно (min после пика):
         "SCHEME CONFIRMED: peak %.4f (bar %d), bottom %.4f (bar %d) -> %.0f%% wipeout". */

    (void)price; (void)vol;
    return 0;
}
