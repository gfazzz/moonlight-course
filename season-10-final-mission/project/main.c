/*
 * OPERATION MOONLIGHT — SEASON PROJECT: moonlight_core
 *
 * Это НЕ ещё одна серия. Это программа, собранная из МОДУЛЕЙ, написанных
 * в предыдущих сезонах. Ни одна из используемых функций здесь не реализована
 * заново — они подключаются заголовками и линкуются как отдельные единицы
 * трансляции:
 *
 *   season-4/lib/ml_hash.c     — djb2, индекс корзины        (s04e03, s04e04)
 *   season-6/lib/ml_crc.c      — CRC-16/MODBUS               (s06e05)
 *   season-8/lib/ml_stats.c    — Уэлфорд, квантили, Тьюки    (s08e01, s08e02)
 *   season-9/lib/ml_kalman.c   — фильтр Калмана [x, v]       (s09e07)
 *
 * Сборка (см. Makefile) компилирует пять .c из четырёх сезонов и линкует их
 * в один исполняемый файл. Именно это и означает «собрать программу из ранее
 * написанных программ как модулей».
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml_hash.h"      /* Season 4 */
#include "ml_crc.h"       /* Season 6 */
#include "ml_stats.h"     /* Season 8 */
#include "ml_kalman.h"    /* Season 9 */

#define NFRAMES 60
#define NSRC     4

static const char *SOURCES[NSRC] = {"radar", "gps", "thermal", "uplink"};

/* Детерминированный источник кадров телеметрии. */
static unsigned long rs = 20260727UL;
static unsigned long rnd(void) {
    rs = rs * 6364136223846793005UL + 1442695040888963407UL;
    return (rs >> 33) & 0xFFFFFF;
}

static int cmp_d(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

int main(void) {
    printf("=== moonlight_core: сборка из модулей четырёх сезонов ===\n\n");

    /* ---------- Season 4: хеш как идентификатор источника ---------- */
    printf("--- модуль Season 4: идентификация источников (ml_hash) ---\n");
    for (int i = 0; i < NSRC; i++)
        printf("  %-8s djb2=%lu -> корзина %zu из 8\n",
               SOURCES[i], ml_hash_djb2(SOURCES[i]), ml_hash_bucket(SOURCES[i], 8));

    /* Хеш детерминирован — свойство, а не запомненное число. */
    int hash_stable = (ml_hash_djb2("radar") == ml_hash_djb2("radar")) &&
                      (ml_hash_djb2("radar") != ml_hash_djb2("radas"));
    printf("  детерминирован и различает похожие имена: %s\n\n", hash_stable ? "да" : "НЕТ");

    /* ---------- Season 6: приём кадров с проверкой CRC ---------- */
    printf("--- модуль Season 6: контроль целостности (ml_crc) ---\n");
    MlWelford w;
    ml_welford_init(&w);                       /* модуль Season 8 */

    double samples[NFRAMES];
    int accepted = 0, rejected = 0;

    for (int i = 0; i < NFRAMES; i++) {
        unsigned char frame[8];
        frame[0] = 0x11;
        frame[1] = 0x03;
        frame[2] = (unsigned char)(i & 0xFF);
        frame[3] = (unsigned char)(rnd() & 0xFF);
        frame[4] = (unsigned char)(rnd() & 0xFF);
        frame[5] = (unsigned char)(rnd() & 0xFF);

        unsigned short crc = ml_crc16_modbus(frame, 6);   /* Season 6 */
        frame[6] = (unsigned char)(crc & 0xFF);
        frame[7] = (unsigned char)(crc >> 8);

        if (i % 11 == 0) frame[3] ^= 0xFF;      /* каждый 11-й кадр портим */

        if (ml_crc16_frame_ok(frame, 8)) {      /* Season 6 */
            double value = 1000.0 + (double)((frame[3] << 8) | frame[4]) / 655.36;
            samples[accepted] = value;
            ml_welford_push(&w, value);          /* Season 8 */
            accepted++;
        } else {
            rejected++;
        }
    }
    printf("  кадров подано: %d, принято: %d, отвергнуто по CRC: %d\n", NFRAMES, accepted, rejected);
    printf("  баланс сходится: %s\n\n", accepted + rejected == NFRAMES ? "да" : "НЕТ");

    /* ---------- Season 8: статистика и выбросы ---------- */
    printf("--- модуль Season 8: устойчивая статистика (ml_stats) ---\n");
    printf("  среднее (Уэлфорд): %.4f\n", ml_welford_mean(&w));
    printf("  дисперсия:         %.4f\n", ml_welford_var(&w));
    printf("  диапазон:          %.4f .. %.4f\n", w.min, w.max);

    double sorted[NFRAMES];
    memcpy(sorted, samples, sizeof(double) * (size_t)accepted);
    qsort(sorted, (size_t)accepted, sizeof(double), cmp_d);

    double med = ml_quantile_sorted(sorted, (size_t)accepted, 0.5);
    double lo, hi;
    ml_tukey_bounds(sorted, (size_t)accepted, &lo, &hi);

    int outliers = 0;
    for (int i = 0; i < accepted; i++) if (samples[i] < lo || samples[i] > hi) outliers++;

    printf("  медиана:           %.4f\n", med);
    printf("  границы Тьюки:     [%.4f, %.4f]\n", lo, hi);
    printf("  выбросов найдено:  %d\n\n", outliers);

    /* ---------- Season 9: сопровождение цели ---------- */
    printf("--- модуль Season 9: фильтрация шума (ml_kalman) ---\n");
    MlKalman k;
    ml_kalman_init(&k, 500.0, 0.05, 25.0);

    double true_x = 100.0, true_v = 12.0;
    double sum_meas = 0.0, sum_est = 0.0;
    for (int i = 0; i < 30; i++) {
        true_x += true_v;
        double noise = ((double)(rnd() % 1000) / 1000.0 - 0.5) * 10.0;
        double z = true_x + noise;
        ml_kalman_step(&k, z, 1.0);

        double me = z - true_x;                if (me < 0) me = -me;
        double ee = ml_kalman_pos(&k) - true_x; if (ee < 0) ee = -ee;
        sum_meas += me;
        sum_est  += ee;
    }
    printf("  средняя ошибка измерения: %.3f\n", sum_meas / 30.0);
    printf("  средняя ошибка оценки:    %.3f\n", sum_est / 30.0);
    printf("  оценённая скорость:       %.3f (истинная %.1f)\n", ml_kalman_vel(&k), true_v);
    printf("  фильтр точнее измерения:  %s\n\n", sum_est < sum_meas ? "да" : "НЕТ");

    /* ---------- сводка сборки ---------- */
    printf("--- состав сборки ---\n");
    printf("  [S4] ml_hash.c    — идентификация источников\n");
    printf("  [S6] ml_crc.c     — контроль целостности кадров\n");
    printf("  [S8] ml_stats.c   — статистика и выбросы\n");
    printf("  [S9] ml_kalman.c  — оценка при шуме\n");
    printf("  [S10] main.c      — интеграция\n");
    printf("\n5 единиц трансляции из 4 сезонов слинкованы в moonlight_core.\n");

    int ok = hash_stable && (accepted + rejected == NFRAMES) && (sum_est < sum_meas);
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
