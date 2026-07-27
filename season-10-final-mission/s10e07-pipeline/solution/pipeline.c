/*
 * OPERATION MOONLIGHT — s10e07 "Сквозной конвейер"
 * Эталонное решение: pipeline.c
 *
 * Концепт серии: подсистемы по отдельности работают (e01-e06). Интеграция —
 *                отдельная инженерная задача, и ломается она не внутри модулей,
 *                а НА СТЫКАХ:
 *                  - у каждого этапа своя реакция на плохой вход;
 *                  - ошибка не должна «теряться» между стадиями;
 *                  - сквозная целостность проверяется от входа до выхода;
 *                  - при сбое этапа конвейер обязан продолжать работу с
 *                    остальными записями, а не падать целиком.
 * Задача: собрать конвейер приём -> валидация -> хранение -> агрегация -> вывод
 *         и доказать, что ни одна запись не потерялась незаметно.
 *
 * Главный инструмент проверки — БАЛАНС: принято = обработано + отвергнуто.
 * Если баланс не сходится, где-то запись исчезла молча. Это худший вид ошибки.
 */
#include <stdio.h>
#include <string.h>

#define NPACK   40
#define NCHAN   4
#define CAPACITY 24        /* вместимость хранилища: намеренно меньше потока */

/* ---------- стадии конвейера ---------- */
typedef enum {
    ST_ACCEPTED, ST_BAD_MAGIC, ST_BAD_LEN, ST_BAD_CHAN, ST_BAD_CRC, ST_NO_SPACE
} Outcome;

static const char *OUTCOME[] = {
    "принято", "сигнатура", "длина", "канал", "контрольная сумма", "нет места"
};

typedef struct { unsigned char magic, chan, len, crc; long value; } Packet;

/* Хранилище (упрощённая модель из s09e04/e05: записи + журнал). */
static struct { unsigned char chan; long value; } store[CAPACITY];
static int stored = 0;
static long journal_writes = 0;

/* Агрегация по каналам (s08). */
static long chan_sum[NCHAN];
static int  chan_cnt[NCHAN];

static int counters[6];

/* Источник пакетов: детерминированный, с заданной долей дефектов. */
static void gen(Packet *p, int i) {
    unsigned long s = 20260727UL + (unsigned long)i * 2654435761UL;
    s = s * 6364136223846793005UL + 1442695040888963407UL;

    p->magic = 0x4D;
    p->chan  = (unsigned char)((s >> 20) % NCHAN);
    p->len   = (unsigned char)(4 + (s >> 28) % 8);
    p->value = (long)((s >> 33) % 500);
    p->crc   = (unsigned char)(p->magic ^ p->chan ^ p->len);

    /* Каждый 7-й пакет — с дефектом (разного рода). */
    if (i % 7 == 0) {
        switch ((i / 7) % 4) {
            case 0: p->magic = 0x00;        break;   /* битая сигнатура */
            case 1: p->len   = 200;         break;   /* невозможная длина */
            case 2: p->chan  = 99;          break;   /* канал вне диапазона */
            default: p->crc  ^= 0xFF;       break;   /* испорченная сумма */
        }
    }
}

/* Одна запись проходит все стадии. Возвращает исход — он ВСЕГДА учитывается. */
static Outcome process(const Packet *p) {
    if (p->magic != 0x4D)                       return ST_BAD_MAGIC;
    if (p->len < 4 || p->len > 64)              return ST_BAD_LEN;
    if (p->chan >= NCHAN)                       return ST_BAD_CHAN;
    if (p->crc != (p->magic ^ p->chan ^ p->len)) return ST_BAD_CRC;

    if (stored >= CAPACITY)                     return ST_NO_SPACE;

    /* Журнал впереди данных (s09e05): сначала намерение, потом запись. */
    journal_writes++;
    store[stored].chan  = p->chan;
    store[stored].value = p->value;
    stored++;

    chan_sum[p->chan] += p->value;
    chan_cnt[p->chan]++;
    return ST_ACCEPTED;
}

int main(void) {
    printf("=== сквозной конвейер moonlight_core ===\n");
    printf("пакетов на входе: %d, вместимость хранилища: %d, каналов: %d\n\n",
           NPACK, CAPACITY, NCHAN);

    printf("--- прохождение конвейера ---\n");
    for (int i = 0; i < NPACK; i++) {
        Packet p;
        gen(&p, i);
        Outcome o = process(&p);
        counters[o]++;
        if (i < 12 || o == ST_NO_SPACE)
            printf("  пакет %2d: канал %3d, длина %3d -> %s\n",
                   i, p.chan, p.len, OUTCOME[o]);
    }
    printf("  ...\n\n");

    /* ---------- сведение баланса: главная проверка интеграции ---------- */
    printf("--- баланс ---\n");
    int rejected = 0;
    for (int i = 1; i < 6; i++) rejected += counters[i];
    int accepted = counters[ST_ACCEPTED];

    for (int i = 0; i < 6; i++)
        if (counters[i]) printf("  %-18s %3d\n", OUTCOME[i], counters[i]);

    printf("  ------------------------\n");
    printf("  принято + отвергнуто = %d + %d = %d\n", accepted, rejected, accepted + rejected);
    printf("  подано на вход:        %d\n", NPACK);
    printf("  баланс сходится: %s\n\n",
           accepted + rejected == NPACK ? "да" : "НЕТ — записи теряются молча");

    /* ---------- сквозная целостность ---------- */
    printf("--- сквозная целостность ---\n");
    long store_total = 0;
    for (int i = 0; i < stored; i++) store_total += store[i].value;

    long agg_total = 0;
    int  agg_count = 0;
    for (int c = 0; c < NCHAN; c++) { agg_total += chan_sum[c]; agg_count += chan_cnt[c]; }

    printf("  записей в хранилище:      %d\n", stored);
    printf("  записей в агрегатах:      %d\n", agg_count);
    printf("  сумма в хранилище:        %ld\n", store_total);
    printf("  сумма в агрегатах:        %ld\n", agg_total);
    printf("  записей журнала:          %ld\n", journal_writes);
    printf("  хранилище == агрегаты:    %s\n",
           (stored == agg_count && store_total == agg_total) ? "да" : "НЕТ");
    printf("  журнал == записи:         %s\n\n",
           journal_writes == stored ? "да" : "НЕТ");

    /* ---------- поведение при переполнении ---------- */
    printf("--- деградация ---\n");
    printf("  хранилище заполнено: %d из %d\n", stored, CAPACITY);
    printf("  отвергнуто из-за нехватки места: %d\n", counters[ST_NO_SPACE]);
    printf("  конвейер продолжил работу после переполнения: %s\n\n",
           counters[ST_NO_SPACE] > 0 ? "да" : "переполнения не было");

    /* ---------- сводка по каналам ---------- */
    printf("--- агрегаты по каналам ---\n");
    for (int c = 0; c < NCHAN; c++)
        printf("  канал %d: записей %2d, сумма %5ld, среднее %6.1f\n",
               c, chan_cnt[c], chan_sum[c],
               chan_cnt[c] ? (double)chan_sum[c] / chan_cnt[c] : 0.0);

    printf("\nвывод: интеграция проверяется балансом. Если принято + отвергнуто\n");
    printf("       не равно поданному — запись исчезла молча, и это худшая из ошибок.\n");
    return 0;
}
