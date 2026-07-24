/*
 * OPERATION MOONLIGHT — s06e08 "MAVLink / дроны" (финал Season 6)
 * Эталонное решение: mavlink.c
 *
 * Концепт серии: дроны периметра шлют телеметрию по MAVLink. Кадр v1:
 *                0xFE | LEN | SEQ | SYSID | COMPID | MSGID | payload | CK_A CK_B.
 *                Целостность держит X.25-контрольная сумма (CRC-16/MCRF4XX) с
 *                добавочным байтом CRC_EXTRA на каждый тип сообщения.
 * Задача: декодировать пакет телеметрии, проверить контрольную сумму,
 *         извлечь координаты/высоту/заряд; отбраковать повреждённый кадр.
 *
 * Финал собирает сезон: биты, кадры, шины, CRC, поля — всё вместе (iot_controller).
 */
#include <stdio.h>
#include <stdint.h>

#define MAGIC     0xFE
#define MSGID     33      /* GLOBAL_POSITION_INT-подобное сообщение */
#define CRC_EXTRA 104     /* фиксированный для этого MSGID */
#define PLEN      13      /* payload: lat(4) lon(4) alt(4) batt(1), little-endian */

/* Шаг X.25/MCRF4XX — как в референсной реализации MAVLink. */
static void crc_acc(uint8_t data, uint16_t *crc) {
    uint8_t tmp = data ^ (uint8_t)(*crc & 0xFF);
    tmp ^= (uint8_t)(tmp << 4);
    *crc = (uint16_t)((*crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4));
}

/* Контрольная сумма кадра: по LEN..payload + CRC_EXTRA (magic не входит). */
static uint16_t frame_crc(const uint8_t *f) {
    uint16_t crc = 0xFFFF;
    for (int i = 1; i < 6 + PLEN; i++) crc_acc(f[i], &crc);  /* LEN,SEQ,SYS,COMP,MSGID,payload */
    crc_acc(CRC_EXTRA, &crc);
    return crc;
}

static void put_i32(uint8_t *p, int32_t v) {   /* little-endian */
    p[0] = v & 0xFF; p[1] = (v >> 8) & 0xFF; p[2] = (v >> 16) & 0xFF; p[3] = (v >> 24) & 0xFF;
}
static int32_t get_i32(const uint8_t *p) {
    return (int32_t)((uint32_t)p[0] | (uint32_t)p[1] << 8 |
                     (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24);
}

/* Дрон: собрать кадр телеметрии. Возврат — длина. */
static int encode(uint8_t *f, uint8_t seq, int32_t lat, int32_t lon, int32_t alt, uint8_t batt) {
    f[0] = MAGIC; f[1] = PLEN; f[2] = seq; f[3] = 1; f[4] = 200; f[5] = MSGID;
    put_i32(&f[6], lat); put_i32(&f[10], lon); put_i32(&f[14], alt); f[18] = batt;
    uint16_t crc = frame_crc(f);
    f[6 + PLEN]     = crc & 0xFF;    /* CK_A */
    f[6 + PLEN + 1] = crc >> 8;      /* CK_B */
    return 6 + PLEN + 2;
}

static void decode(const char *label, const uint8_t *f, int len) {
    printf("%s (%d bytes):", label, len);
    for (int i = 0; i < len; i++) printf(" %02X", f[i]);
    printf("\n");

    if (f[0] != MAGIC) { printf("  bad magic -> dropped\n\n"); return; }

    uint16_t want = frame_crc(f);
    uint16_t got  = (uint16_t)(f[6 + PLEN] | (f[6 + PLEN + 1] << 8));
    if (want != got) { printf("  checksum FAIL -> frame rejected\n\n"); return; }

    int32_t lat = get_i32(&f[6]), lon = get_i32(&f[10]), alt = get_i32(&f[14]);
    uint8_t batt = f[18];
    printf("  seq=%u sysid=%u msgid=%u  checksum ok\n", f[2], f[3], f[5]);
    printf("  lat=%.4f lon=%.4f alt=%.1f m  battery=%u%%\n\n",
           lat / 1e7, lon / 1e7, alt / 1000.0, batt);
}

int main(void) {
    printf("=== MAVLink telemetry decode (Season 6 finale) ===\n\n");

    /* Дрон периметра над Академгородком (координаты из брифинга сезона). */
    uint8_t pkt[32];
    int len = encode(pkt, 7, 548486000, 830974000, 120000, 47);
    decode("drone GPS", pkt, len);

    /* Тот же кадр с искажённым байтом payload -> контрольная сумма не сойдётся. */
    uint8_t bad[32];
    for (int i = 0; i < len; i++) bad[i] = pkt[i];
    bad[8] ^= 0xFF;
    decode("drone GPS (corrupt)", bad, len);

    /* Интеграция: iot_controller собирает всё железо сезона. */
    printf("--- iot_controller online ---\n");
    const char *sub[] = {
        "GPIO lock (e01)", "UART link (e02)", "I2C sensor (e03)",
        "Wiegand SCUD (e04)", "Modbus PLC (e05)", "sensor FSM (e06)",
        "camera motion (e07)", "drone telemetry (e08)"
    };
    for (int i = 0; i < 8; i++) printf("  [OK] %s\n", sub[i]);
    printf("\nperimeter mapped. Season 6 complete.\n");
    return 0;
}
