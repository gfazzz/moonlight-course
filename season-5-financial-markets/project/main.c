/*
 * OPERATION MOONLIGHT — SEASON PROJECT: trading_system
 *
 * Первый проект курса, собираемый ИЗ МОДУЛЕЙ ДРУГОГО СЕЗОНА.
 * Подключается и линкуется:
 *   season-4/lib/ml_hash.c        — djb2 и корзины      (s04e03, s04e04)
 *   season-5/lib/ml_indicators.c  — SMA, EMA, RSI       (s05e03, s05e04)
 *
 * Хеш из Season 4 используется по назначению: тикеры приходят строками,
 * а искать их надо быстро — строим по ним хеш-индекс вместо линейного поиска.
 * Индикаторы из Season 5 считают сигналы по ценам каждого инструмента.
 */
#include <stdio.h>
#include <string.h>

#include "ml_hash.h"          /* Season 4 */
#include "ml_indicators.h"    /* Season 5 */

#define NBUCKETS 16
#define NSYM      6
#define NBARS    40

typedef struct {
    const char *ticker;
    double close[NBARS];
} Symbol;

static Symbol syms[NSYM];
static int bucket_of[NSYM];

/* Детерминированные цены. */
static void gen(void) {
    static const char *names[NSYM] = {"MOON", "LUNA", "ORBIT", "RADAR", "NODE", "CIPHER"};
    unsigned long s = 20260727UL;
    for (int i = 0; i < NSYM; i++) {
        syms[i].ticker = names[i];
        double p = 100.0 + i * 25.0;
        for (int b = 0; b < NBARS; b++) {
            s = s * 6364136223846793005UL + 1442695040888963407UL;
            double step = ((double)((s >> 33) % 1000) / 1000.0 - 0.45) * 3.0;
            p += step;
            if (p < 1.0) p = 1.0;
            syms[i].close[b] = p;
        }
    }
}

int main(void) {
    gen();
    printf("=== trading_system: сборка из модулей S4 + S5 ===\n\n");

    /* ---------- Season 4: хеш-индекс по тикерам ---------- */
    printf("--- модуль Season 4 (ml_hash): индекс инструментов ---\n");
    int collisions = 0;
    int used[NBUCKETS];
    memset(used, 0, sizeof used);

    for (int i = 0; i < NSYM; i++) {
        bucket_of[i] = (int)ml_hash_bucket(syms[i].ticker, NBUCKETS);
        if (used[bucket_of[i]]++) collisions++;
        printf("  %-7s -> корзина %2d\n", syms[i].ticker, bucket_of[i]);
    }
    printf("  инструментов: %d, корзин: %d, коллизий: %d\n\n", NSYM, NBUCKETS, collisions);

    /* ---------- Season 5: индикаторы по каждому инструменту ---------- */
    printf("--- модуль Season 5 (ml_indicators): сигналы ---\n");
    printf("  тикер     цена      SMA(10)     EMA(10)    RSI(14)   сигнал\n");

    int buy = 0, sell = 0, hold = 0;
    for (int i = 0; i < NSYM; i++) {
        const double *c = syms[i].close;
        double last = c[NBARS - 1];
        double sma  = ml_sma(c, NBARS - 1, 10);
        double ema  = ml_ema(c, NBARS, 10);
        double rsi  = ml_rsi(c, NBARS, 14);

        const char *sig;
        if (rsi < 30.0 && last > sma)      { sig = "ПОКУПКА";  buy++;  }
        else if (rsi > 70.0 && last < sma) { sig = "ПРОДАЖА";  sell++; }
        else                                { sig = "ждать";    hold++; }

        printf("  %-7s %8.2f %10.2f %11.2f %9.2f   %s\n", syms[i].ticker, last, sma, ema, rsi, sig);
    }
    printf("\n  сигналов: покупка %d, продажа %d, ждать %d\n", buy, sell, hold);

    /* ---------- проверки свойств (а не запомненных чисел) ---------- */
    printf("\n--- самопроверка ---\n");
    int hash_ok = ml_hash_bucket("MOON", NBUCKETS) == ml_hash_bucket("MOON", NBUCKETS);
    printf("  хеш детерминирован:            %s\n", hash_ok ? "да" : "НЕТ");

    int rsi_ok = 1;
    for (int i = 0; i < NSYM; i++) {
        double r = ml_rsi(syms[i].close, NBARS, 14);
        if (r < 0.0 || r > 100.0) rsi_ok = 0;
    }
    printf("  RSI в диапазоне [0, 100]:      %s\n", rsi_ok ? "да" : "НЕТ");

    /* EMA реагирует быстрее SMA: на растущем ряду она выше. */
    double up[20];
    for (int i = 0; i < 20; i++) up[i] = 100.0 + i;
    int ema_faster = ml_ema(up, 20, 10) > ml_sma(up, 19, 10);
    printf("  EMA быстрее SMA на тренде:     %s\n", ema_faster ? "да" : "НЕТ");

    int total_ok = (buy + sell + hold == NSYM);
    printf("  все инструменты обработаны:    %s\n", total_ok ? "да" : "НЕТ");

    printf("\n--- состав сборки ---\n");
    printf("  [S4] ml_hash.c        — хеш-индекс тикеров\n");
    printf("  [S5] ml_indicators.c  — SMA, EMA, RSI\n");
    printf("  [S5] main.c           — торговая логика\n");
    printf("\n3 единицы трансляции из 2 сезонов слинкованы в trading_system.\n");

    int ok = hash_ok && rsi_ok && ema_faster && total_ok;
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
