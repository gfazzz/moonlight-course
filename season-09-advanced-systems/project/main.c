/*
 * OPERATION MOONLIGHT — SEASON PROJECT: advanced_systems
 *
 * Собирается из модулей ТРЁХ сезонов:
 *   season-04/lib/ml_hash.c    — ключи записей из имён      (s04e03)
 *   season-06/lib/ml_crc.c     — целостность входных кадров (s06e05)
 *   season-09/lib/ml_btree.c   — индекс B-дерева            (s09e04)
 *   season-09/lib/ml_kalman.c  — оценка при шуме            (s09e07)
 *
 * Подсистема хранения и сопровождения: принимает кадры, индексирует записи,
 * фильтрует шумные измерения.
 */
#include <stdio.h>
#include <string.h>

#include "ml_hash.h"     /* Season 4 */
#include "ml_crc.h"      /* Season 6 */
#include "ml_btree.h"    /* Season 9 */
#include "ml_kalman.h"   /* Season 9 */

#define NREC 150

static MlBtree tree;

int main(void) {
    printf("=== advanced_systems: сборка из модулей S4 + S6 + S9 ===\n\n");

    /* --- Season 6: приём кадров с проверкой целостности --- */
    printf("--- модуль Season 6 (ml_crc): приём записей ---\n");
    ml_bt_init(&tree);

    unsigned long s = 20260727UL;
    int accepted = 0, rejected = 0;
    for (int i = 0; i < NREC; i++) {
        s = s * 6364136223846793005UL + 1442695040888963407UL;

        unsigned char frame[6];
        frame[0] = 0x21;
        frame[1] = (unsigned char)(i & 0xFF);
        frame[2] = (unsigned char)((s >> 20) & 0xFF);
        frame[3] = (unsigned char)((s >> 28) & 0xFF);
        unsigned short crc = ml_crc16_modbus(frame, 4);
        frame[4] = (unsigned char)(crc & 0xFF);
        frame[5] = (unsigned char)(crc >> 8);

        if (i % 13 == 0) frame[2] ^= 0xFF;              /* каждый 13-й портим */

        if (ml_crc16_frame_ok(frame, 6)) {
            int key = (int)((s >> 16) % 1000);          /* ключ записи */
            ml_bt_insert(&tree, key, 1000L + i);        /* Season 9: индекс */
            accepted++;
        } else rejected++;
    }
    printf("  подано %d, принято %d, отброшено %d, баланс: %s\n\n",
           NREC, accepted, rejected,
           accepted + rejected == NREC ? "сходится" : "НЕ СХОДИТСЯ");

    /* --- Season 9: индекс --- */
    printf("--- модуль Season 9 (ml_btree): индекс ---\n");
    printf("  записей в индексе: %d, страниц: %d, высота: %d\n",
           accepted, tree.used, ml_bt_height(&tree));

    /* Season 4: имена превращаются в ключи через хеш. */
    static const char *names[4] = {"track.alpha", "track.bravo", "track.charlie", "track.delta"};
    printf("\n--- модуль Season 4 (ml_hash): поиск по имени ---\n");
    for (int i = 0; i < 4; i++) {
        int key = (int)(ml_hash_djb2(names[i]) % 1000);
        long val = -1;
        tree.page_reads = 0;
        int found = ml_bt_search(&tree, key, &val);
        printf("  %-14s -> ключ %3d: %s, страниц прочитано %ld\n",
               names[i], key, found ? "найден" : "нет в индексе", tree.page_reads);
    }

    /* --- Season 9: фильтр Калмана --- */
    printf("\n--- модуль Season 9 (ml_kalman): сопровождение ---\n");
    MlKalman k;
    ml_kalman_init(&k, 500.0, 0.05, 25.0);
    double true_x = 50.0, true_v = 8.0, sum_meas = 0.0, sum_est = 0.0;
    for (int i = 0; i < 25; i++) {
        s = s * 6364136223846793005UL + 1442695040888963407UL;
        true_x += true_v;
        double noise = ((double)((s >> 33) % 1000) / 1000.0 - 0.5) * 10.0;
        double z = true_x + noise;
        ml_kalman_step(&k, z, 1.0);
        double me = z - true_x;                 if (me < 0) me = -me;
        double ee = ml_kalman_pos(&k) - true_x; if (ee < 0) ee = -ee;
        sum_meas += me; sum_est += ee;
    }
    printf("  ошибка измерения %.3f, ошибка оценки %.3f\n", sum_meas / 25.0, sum_est / 25.0);
    printf("  скорость: оценка %.2f, истина %.1f\n", ml_kalman_vel(&k), true_v);
    printf("  фильтр точнее измерения: %s\n\n", sum_est < sum_meas ? "да" : "НЕТ");

    printf("--- состав сборки ---\n");
    printf("  [S4] ml_hash.c   — ключи из имён\n");
    printf("  [S6] ml_crc.c    — целостность кадров\n");
    printf("  [S9] ml_btree.c  — индекс\n");
    printf("  [S9] ml_kalman.c — оценка при шуме\n");
    printf("  [S9] main.c      — подсистема\n");
    printf("\n5 единиц трансляции из 3 сезонов слинкованы в advanced_systems.\n");

    int ok = (accepted + rejected == NREC) && (ml_bt_height(&tree) >= 2) && (sum_est < sum_meas);
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
