/*
 * OPERATION MOONLIGHT — s10e04 "Профилирование и оптимизация"
 * Эталонное решение: profile.c
 *
 * Концепт серии: окно связи — 8.74 минуты (s10e03). Если конвейер не успевает,
 *                данные теряются. Но оптимизировать «по интуиции» бессмысленно:
 *                узкое место почти всегда не там, где кажется.
 *                Дисциплина: ИЗМЕРИТЬ -> найти горячий путь -> оптимизировать
 *                ТОЛЬКО его -> измерить снова -> проверить, что результат не изменился.
 * Задача: профилировать конвейер обработки телеметрии и ускорить его,
 *         не сломав корректность.
 *
 * Метрика — СЧЁТЧИКИ ОПЕРАЦИЙ, а не время: время не воспроизводимо (об этом же
 * говорилось в S7 и S8). Считаем обращения к памяти и сравнения — они одинаковы
 * на любой машине и показывают именно алгоритмическую разницу.
 */
#include <stdio.h>
#include <string.h>

#define NREC   4000
#define NKEYS  64

static long ops;                 /* счётчик «единиц работы» */

typedef struct {
    int  key;                    /* идентификатор источника */
    long value;
} Rec;

static Rec data[NREC];
static char names[NKEYS][16];

static void gen(void) {
    unsigned long s = 20260727UL;
    for (int i = 0; i < NKEYS; i++) snprintf(names[i], sizeof names[0], "src_%02d", i);
    for (int i = 0; i < NREC; i++) {
        s = s * 6364136223846793005UL + 1442695040888963407UL;
        data[i].key = (int)((s >> 33) % NKEYS);
        data[i].value = (long)((s >> 20) % 1000);
    }
}

/* ---------- ВЕРСИЯ 1: наивный конвейер ---------- */

/* Поиск имени источника линейным перебором — вызывается для КАЖДОЙ записи. */
static const char *lookup_linear(int key) {
    for (int i = 0; i < NKEYS; i++) {
        ops++;
        if (i == key) return names[i];
    }
    return "?";
}

/* Суммирование с поиском группы линейным перебором. */
static long pipeline_naive(long *checksum) {
    long sums[NKEYS];
    int  keys[NKEYS];
    int  ngroups = 0;
    long total = 0;

    for (int i = 0; i < NREC; i++) {
        const char *nm = lookup_linear(data[i].key);   /* поиск имени: O(NKEYS) */
        total += (long)strlen(nm);
        ops++;

        int g = -1;
        for (int j = 0; j < ngroups; j++) {            /* поиск группы: O(ngroups) */
            ops++;
            if (keys[j] == data[i].key) { g = j; break; }
        }
        if (g < 0) { g = ngroups++; keys[g] = data[i].key; sums[g] = 0; }
        sums[g] += data[i].value;
    }

    long acc = 0;
    for (int j = 0; j < ngroups; j++) acc += sums[j];
    *checksum = acc;
    return total;
}

/* ---------- ВЕРСИЯ 2: оптимизирован горячий путь ---------- */

/* Ключ и есть индекс — поиск не нужен вовсе: O(1). */
static const char *lookup_direct(int key) {
    ops++;
    return names[key];
}

static long pipeline_fast(long *checksum) {
    long sums[NKEYS];
    int  seen[NKEYS];
    memset(sums, 0, sizeof sums);
    memset(seen, 0, sizeof seen);

    /* Длины имён считаем ОДИН раз, а не для каждой записи. */
    int len_cache[NKEYS];
    for (int i = 0; i < NKEYS; i++) { len_cache[i] = (int)strlen(names[i]); ops++; }

    long total = 0;
    for (int i = 0; i < NREC; i++) {
        int k = data[i].key;
        (void)lookup_direct(k);
        total += len_cache[k];                          /* без strlen в цикле */
        ops++;
        sums[k] += data[i].value;                       /* прямая адресация вместо поиска */
        seen[k] = 1;
    }

    long acc = 0;
    for (int j = 0; j < NKEYS; j++) if (seen[j]) acc += sums[j];
    *checksum = acc;
    return total;
}

int main(void) {
    gen();

    printf("=== профилирование конвейера телеметрии ===\n");
    printf("записей: %d, источников: %d\n\n", NREC, NKEYS);

    /* --- замер до оптимизации --- */
    ops = 0;
    long sum1 = 0;
    long total1 = pipeline_naive(&sum1);
    long ops_naive = ops;

    /* --- замер после --- */
    ops = 0;
    long sum2 = 0;
    long total2 = pipeline_fast(&sum2);
    long ops_fast = ops;

    printf("--- корректность (главное условие оптимизации) ---\n");
    printf("сумма значений:   наивно %ld, быстро %ld -> %s\n",
           sum1, sum2, sum1 == sum2 ? "совпало" : "РАСХОЖДЕНИЕ");
    printf("сумма длин имён:  наивно %ld, быстро %ld -> %s\n\n",
           total1, total2, total1 == total2 ? "совпало" : "РАСХОЖДЕНИЕ");

    printf("--- профиль ---\n");
    printf("операций до оптимизации:    %ld\n", ops_naive);
    printf("операций после оптимизации: %ld\n", ops_fast);
    printf("ускорение: %.1f раза\n\n", (double)ops_naive / (double)ops_fast);

    /* --- что именно было горячим --- */
    printf("--- разбор горячего пути ---\n");
    long useful     = NREC;                          /* собственно полезная работа */
    long searching  = ops_naive - useful;            /* оба линейных поиска */
    printf("полезная работа (суммирование):   %ld операций (%.1f%%)\n",
           useful, 100.0 * useful / ops_naive);
    printf("линейные поиски (имя + группа):   %ld операций (%.1f%%)\n",
           searching, 100.0 * searching / ops_naive);
    printf("оба поиска заменены прямой адресацией -> O(1)\n\n");

    /* --- закон Амдала: ускорять холодный путь бесполезно --- */
    printf("--- закон Амдала ---\n");
    double hot = (double)searching / (double)ops_naive;
    printf("доля горячего пути: %.1f%%\n", 100.0 * hot);
    printf("ускорить ХОЛОДНОЕ в 10 раз -> общее ускорение всего %.2f раза\n",
           1.0 / (hot + (1.0 - hot) / 10.0));
    printf("ускорить ГОРЯЧЕЕ  в 10 раз -> общее ускорение %.2f раза\n",
           1.0 / (hot / 10.0 + (1.0 - hot)));
    printf("предел при бесконечном ускорении горячего: %.1f раза\n", 1.0 / (1.0 - hot));
    printf("вывод: оптимизировать имеет смысл только то, что доминирует.\n");

    return 0;
}
