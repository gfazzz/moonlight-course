/*
 * OPERATION MOONLIGHT — s03e06 "Контрольная сумма"
 * Эталонное решение: checksum.c
 *
 * Концепт серии: Internet checksum (RFC 1071) — 16-битная сумма с переносом
 *                и дополнением до единицы. Так IP/TCP/UDP проверяют целостность.
 * Задача: посчитать checksum IP-заголовка, вставить, проверить; показать, что
 *         порча любого байта делает пакет невалидным.
 */
#include <stdio.h>
#include <stdint.h>

/* Сумма 16-битных слов (big-endian) со сверткой переносов, затем ~ (дополнение). */
static uint16_t inet_checksum(const uint8_t *buf, int len) {
    uint32_t sum = 0;
    for (int i = 0; i + 1 < len; i += 2)
        sum += (uint32_t)((buf[i] << 8) | buf[i + 1]);
    if (len & 1) sum += (uint32_t)(buf[len - 1] << 8);   /* нечётный хвост */
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16); /* свернуть переносы */
    return (uint16_t)~sum;
}

int main(void) {
    /* IP-заголовок с обнулённым полем checksum (байты 10–11). */
    uint8_t ip[20] = {
        0x45, 0x00, 0x00, 0x28, 0x1C, 0x46, 0x40, 0x00,
        0x40, 0x06, 0x00, 0x00, 0xCB, 0x00, 0x71, 0x2A,
        0xC0, 0xA8, 0x01, 0x64
    };

    printf("=== INTERNET CHECKSUM ===\n");

    uint16_t cs = inet_checksum(ip, 20);
    printf("computed checksum = 0x%04X\n", cs);

    ip[10] = (uint8_t)(cs >> 8);   /* вставили checksum в заголовок */
    ip[11] = (uint8_t)(cs & 0xFF);

    /* Проверка: сумма по всему заголовку (с checksum) должна дать 0. */
    uint16_t v = inet_checksum(ip, 20);
    printf("verify (checksum inserted): 0x%04X -> %s\n", v, (v == 0) ? "valid" : "INVALID");

    /* Порча одного байта адреса делает пакет невалидным. */
    ip[12] ^= 0xFF;
    uint16_t v2 = inet_checksum(ip, 20);
    printf("after corrupting src IP:    0x%04X -> %s\n", v2, (v2 == 0) ? "valid" : "INVALID");
    return 0;
}
