/*
 * OPERATION MOONLIGHT — SEASON PROJECT: crypto_toolkit
 *
 * Собирается из модулей ДВУХ сезонов:
 *   season-1/lib/ml_crypto.c  — XOR-шифр и контрольная сумма  (s01e06, s01e08)
 *   season-4/lib/ml_hash.c    — djb2 и корзины                (s04e03, s04e04)
 *
 * Инструмент: шифрует сообщения, проверяет целостность и ведёт хеш-реестр ключей.
 */
#include <stdio.h>
#include <string.h>

#include "ml_crypto.h"   /* Season 1 */
#include "ml_hash.h"     /* Season 4 */

#define NMSG 4
#define NBUCKETS 8

int main(void) {
    printf("=== crypto_toolkit: сборка из модулей S1 + S4 ===\n\n");

    static const char *msgs[NMSG] = {
        "MOONLIGHT ACTIVE", "TARGET ACQUIRED", "ABORT MISSION", "SAFE HOUSE 7"
    };
    const unsigned char key = 0x5A;

    printf("--- модуль Season 1 (ml_crypto): шифрование и целостность ---\n");
    int roundtrip_ok = 1, checksum_ok = 1;

    for (int i = 0; i < NMSG; i++) {
        unsigned char buf[64];
        size_t len = strlen(msgs[i]);
        memcpy(buf, msgs[i], len);

        unsigned char before = ml_checksum(buf, len);
        ml_xor_crypt(buf, len, key);                 /* зашифровали */
        unsigned char enc_sum = ml_checksum(buf, len);
        ml_xor_crypt(buf, len, key);                 /* и обратно */
        unsigned char after = ml_checksum(buf, len);

        int same = (memcmp(buf, msgs[i], len) == 0);
        if (!same) roundtrip_ok = 0;
        if (before != after) checksum_ok = 0;

        printf("  %-18s сумма %3u -> шифр %3u -> %3u   %s\n",
               msgs[i], before, enc_sum, after, same ? "восстановлено" : "ОШИБКА");
    }
    printf("  XOR самообратим:            %s\n", roundtrip_ok ? "да" : "НЕТ");
    printf("  контрольная сумма стабильна: %s\n\n", checksum_ok ? "да" : "НЕТ");

    printf("--- модуль Season 4 (ml_hash): реестр ключей ---\n");
    static const char *keys[NMSG] = {"key.alpha", "key.bravo", "key.charlie", "key.delta"};
    int used[NBUCKETS];
    memset(used, 0, sizeof used);
    int collisions = 0;

    for (int i = 0; i < NMSG; i++) {
        size_t b = ml_hash_bucket(keys[i], NBUCKETS);
        if (used[b]++) collisions++;
        printf("  %-12s djb2=%-16lu корзина %zu\n", keys[i], ml_hash_djb2(keys[i]), b);
    }
    printf("  ключей: %d, корзин: %d, коллизий: %d\n\n", NMSG, NBUCKETS, collisions);

    printf("--- самопроверка ---\n");
    int hash_det = ml_hash_djb2("key.alpha") == ml_hash_djb2("key.alpha");
    int hash_dis = ml_hash_djb2("key.alpha") != ml_hash_djb2("key.alpah");
    printf("  хеш детерминирован:          %s\n", hash_det ? "да" : "НЕТ");
    printf("  различает перестановку букв: %s\n", hash_dis ? "да" : "НЕТ");

    printf("\n--- состав сборки ---\n");
    printf("  [S1] ml_crypto.c — XOR и контрольная сумма\n");
    printf("  [S4] ml_hash.c   — djb2 и корзины\n");
    printf("  [S4] main.c      — инструмент\n");
    printf("\n3 единицы трансляции из 2 сезонов слинкованы в crypto_toolkit.\n");

    int ok = roundtrip_ok && checksum_ok && hash_det && hash_dis;
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
