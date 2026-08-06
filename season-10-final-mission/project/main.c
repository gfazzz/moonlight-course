/*
 * OPERATION MOONLIGHT — SEASON PROJECT: moonlight_core
 *
 * Это НЕ ещё одна серия. Это программа, собранная из МОДУЛЕЙ, написанных
 * в предыдущих сезонах. Ни одна из используемых функций здесь не реализована
 * заново — они подключаются заголовками и линкуются как отдельные единицы
 * трансляции:
 *
 *   season-04/lib/ml_hash.c     — djb2, индекс корзины        (s04e03, s04e04)
 *   season-05/lib/ml_money.c    — суммы целыми, округление    (s05e02)
 *   season-05/lib/ml_time.c     — календарь и часовые пояса   (s05e04)
 *   season-05/lib/ml_text.c     — ширина текста, двойники     (s05e08)
 *   season-06/lib/ml_crc.c      — CRC-16/MODBUS               (s06e05)
 *   season-08/lib/ml_stats.c    — Уэлфорд, квантили, Тьюки    (s08e01, s08e02)
 *   season-09/lib/ml_kalman.c   — фильтр Калмана [x, v]       (s09e07)
 *
 * Сборка (см. Makefile) компилирует восемь .c из пяти сезонов и линкует их
 * в один исполняемый файл. Именно это и означает «собрать программу из ранее
 * написанных программ как модулей».
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml_hash.h"      /* Season 4 */
#include "ml_money.h"     /* Season 5 */
#include "ml_time.h"      /* Season 5 */
#include "ml_text.h"      /* Season 5 */
#include "ml_crc.h"       /* Season 6 */
#include "ml_stats.h"     /* Season 8 */
#include "ml_kalman.h"    /* Season 9 */

#define NFRAMES 60
#define NSRC     5

/* Пятый источник появился в сети три дня назад и представился как первый.
   Отличие в одном символе: вторая буква не латинская "a", а кириллическая
   "а" (U+0430). На экране они неразличимы — это s05e08 в бою. */
static const char *SOURCES[NSRC] = {
    "radar", "gps", "thermal", "uplink", "r" "\xD0\xB0" "dar"
};

/* Расходы операции: строками, как их присылает снабжение. */
static const char *COSTS[] = {
    "12480.00", "3305.15", "990.25", "20030.00", "734.80", "6120.40"
};
#define NCOSTS ((int)(sizeof COSTS / sizeof COSTS[0]))

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
    printf("=== moonlight_core: сборка из модулей пяти сезонов ===\n\n");

    /* ---------- Season 4: хеш как идентификатор источника ---------- */
    printf("--- модуль Season 4: идентификация источников (ml_hash) ---\n");
    for (int i = 0; i < NSRC; i++) {
        /* Добивка по ЗНАКОМЕСТАМ, а не по байтам: "%-8s" разъехался бы
           на пятом источнике, потому что считает байты (s05e08). */
        printf("  ");
        ml_pad_cols(SOURCES[i], 9);
        printf("djb2=%lu -> корзина %zu из 8\n",
               ml_hash_djb2(SOURCES[i]), ml_hash_bucket(SOURCES[i], 8));
    }

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

    /* ---------- Season 5: время, деньги, текст ---------- */
    printf("--- модули Season 5: окно связи (ml_time) ---\n");

    /* Расписание сеанса задано МЕСТНЫМ временем наземной станции.
       Ночь на 3 ноября: местные часы возвращаются с 02:00 на 01:00,
       и метка 01:30 соответствует двум разным моментам. */
    ml_civil open_c  = { 2024, 11, 3, 1, 30, 0 };
    ml_civil close_c = { 2024, 11, 3, 1, 50, 0 };

    ml_time_t o_early, o_late, c_early, c_late;
    int o_kind = ml_utc_from_local(&ML_TZ_NEW_YORK, ml_secs_from_civil(open_c),  &o_early, &o_late);
    int c_kind = ml_utc_from_local(&ML_TZ_NEW_YORK, ml_secs_from_civil(close_c), &c_early, &c_late);

    char ts[40];
    printf("  открытие 01:30 местного — вариантов: %d\n", o_kind == ML_LT_AMBIGUOUS ? 2 : 1);
    ml_format(o_early, ts, sizeof ts); printf("    ранний: %s UTC\n", ts);
    ml_format(o_late,  ts, sizeof ts); printf("    поздний: %s UTC\n", ts);

    /* Длительность считается в моментах. По настенным часам сеанс,
       начатый до перевода стрелок и закрытый после, даёт минус (s05e04). */
    long wall = (long)((ml_secs_from_civil(close_c) - ml_secs_from_civil(open_c)) / 60);
    long real_min = (long)((c_late - o_early) / 60);
    printf("  по настенным часам: %ld мин\n", wall);
    printf("  по шкале UTC (сеанс через перевод стрелок): %ld мин\n", real_min);
    int time_ok = (o_kind == ML_LT_AMBIGUOUS) && (c_kind == ML_LT_AMBIGUOUS)
               && (real_min != wall);
    printf("  неоднозначность обнаружена, а не выбрана молча: %s\n\n",
           time_ok ? "да" : "НЕТ");

    printf("--- модули Season 5: сведение расходов (ml_money) ---\n");
    ml_money_t total = 0, fee_total = 0;
    int money_ok = 1;
    for (int i = 0; i < NCOSTS; i++) {
        ml_money_t v;
        if (!ml_money_parse(COSTS[i], 100, &v)) { money_ok = 0; continue; }
        /* Комиссия банковским округлением: усечение дало бы односторонний
           сдвиг, и итог перестал бы сходиться (s05e02). */
        ml_money_t fee = ml_money_mul_ppm(v, 1500, ML_ROUND_HALF_EVEN);
        if (!ml_money_add(total, v, &total))         money_ok = 0;
        if (!ml_money_add(fee_total, fee, &fee_total)) money_ok = 0;
    }
    char m[32];
    ml_money_str(total, 100, m, sizeof m);      printf("  расходы:  %s\n", m);
    ml_money_str(fee_total, 100, m, sizeof m);  printf("  комиссия: %s\n", m);

    /* Дробление счёта на четыре доли не должно терять ни цента. */
    ml_money_t parts[4], sum_parts = 0;
    ml_money_split(total, 4, parts);
    for (int i = 0; i < 4; i++) sum_parts += parts[i];
    printf("  разделено на 4 доли без потерь: %s\n",
           sum_parts == total ? "да" : "НЕТ");
    money_ok = money_ok && (sum_parts == total);
    printf("  суммы сходятся до цента: %s\n\n", money_ok ? "да" : "НЕТ");

    printf("--- модули Season 5: проверка имён источников (ml_text) ---\n");
    int spoofed = -1, mixed = 0;
    for (int i = 0; i < NSRC; i++)
        if (ml_suspicious_scripts(SOURCES[i])) mixed++;
    for (int i = 0; i < NSRC && spoofed < 0; i++)
        for (int j = i + 1; j < NSRC; j++)
            if (ml_looks_same(SOURCES[i], SOURCES[j])) { spoofed = j; break; }

    printf("  имён со смешанным алфавитом: %d\n", mixed);
    if (spoofed >= 0) {
        printf("  источник [%d] выглядит как [0], но это другой источник\n", spoofed);
        printf("    байт: %zu против %zu, знакомест: %zu против %zu\n",
               strlen(SOURCES[spoofed]), strlen(SOURCES[0]),
               ml_len_columns(SOURCES[spoofed]), ml_len_columns(SOURCES[0]));
        printf("    хеши различаются: %s\n",
               ml_hash_djb2(SOURCES[spoofed]) != ml_hash_djb2(SOURCES[0]) ? "да" : "нет");
    }
    /* Два модуля отвечают на два разных вопроса: ml_hash — «это одна
       строка?», ml_text — «они выглядят одинаково?». Подмена источника
       проходит первый вопрос и проваливает второй. */
    int text_ok = (spoofed > 0) && (mixed == 1);
    printf("  подменённый источник найден: %s\n\n", text_ok ? "да" : "НЕТ");

    /* ---------- сводка сборки ---------- */
    printf("--- состав сборки ---\n");
    printf("  [S4] ml_hash.c    — идентификация источников\n");
    printf("  [S5] ml_money.c   — сведение расходов до цента\n");
    printf("  [S5] ml_time.c    — окно связи через перевод стрелок\n");
    printf("  [S5] ml_text.c    — ширина колонок и подмена имён\n");
    printf("  [S6] ml_crc.c     — контроль целостности кадров\n");
    printf("  [S8] ml_stats.c   — статистика и выбросы\n");
    printf("  [S9] ml_kalman.c  — оценка при шуме\n");
    printf("  [S10] main.c      — интеграция\n");
    printf("\n8 единиц трансляции из 5 сезонов слинкованы в moonlight_core.\n");

    int ok = hash_stable && (accepted + rejected == NFRAMES) && (sum_est < sum_meas)
          && time_ok && money_ok && text_ok;
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
