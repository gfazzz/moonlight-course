/*
 * OPERATION MOONLIGHT — s06e08 "MAVLink / дроны" (финал Season 6)
 * Каркас. Скопируй в artifacts/mavlink.c и закрой TODO.
 *
 *   cp starter.c artifacts/mavlink.c
 *   make test
 *
 * Кадр v1: 0xFE | LEN | SEQ | SYSID | COMPID | MSGID | payload | CK_A CK_B.
 * Checksum X.25/MCRF4XX по LEN..payload + CRC_EXTRA (magic не входит).
 */
#include <stdio.h>
#include <stdint.h>

#define MAGIC     0xFE
#define MSGID     33
#define CRC_EXTRA 104
#define PLEN      13      /* lat(4) lon(4) alt(4) batt(1), little-endian */

/* TODO 1: crc_acc(data, *crc) — шаг X.25:
     tmp = data ^ (uint8_t)(*crc & 0xFF);
     tmp ^= (uint8_t)(tmp << 4);
     *crc = (uint16_t)((*crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4)). */
static void crc_acc(uint8_t data, uint16_t *crc);

/* TODO 2: frame_crc(f) — crc=0xFFFF; crc_acc по f[1..6+PLEN-1]; затем crc_acc(CRC_EXTRA); вернуть crc. */
static uint16_t frame_crc(const uint8_t *f);

static void put_i32(uint8_t *p, int32_t v) {
    p[0] = v & 0xFF; p[1] = (v >> 8) & 0xFF; p[2] = (v >> 16) & 0xFF; p[3] = (v >> 24) & 0xFF;
}
static int32_t get_i32(const uint8_t *p) {
    return (int32_t)((uint32_t)p[0] | (uint32_t)p[1] << 8 |
                     (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24);
}

static int encode(uint8_t *f, uint8_t seq, int32_t lat, int32_t lon, int32_t alt, uint8_t batt) {
    f[0] = MAGIC; f[1] = PLEN; f[2] = seq; f[3] = 1; f[4] = 200; f[5] = MSGID;
    put_i32(&f[6], lat); put_i32(&f[10], lon); put_i32(&f[14], alt); f[18] = batt;
    uint16_t crc = frame_crc(f);
    f[6 + PLEN] = crc & 0xFF; f[6 + PLEN + 1] = crc >> 8;
    return 6 + PLEN + 2;
}

static void decode(const char *label, const uint8_t *f, int len) {
    printf("%s (%d bytes):", label, len);
    for (int i = 0; i < len; i++) printf(" %02X", f[i]);
    printf("\n");

    /* TODO 3: если f[0]!=MAGIC -> "  bad magic -> dropped\n\n", return.
       want=frame_crc(f); got=f[6+PLEN] | f[6+PLEN+1]<<8;
       если want!=got -> "  checksum FAIL -> frame rejected\n\n", return.
       иначе извлечь lat/lon/alt (get_i32), batt=f[18]; печать:
         "  seq=%u sysid=%u msgid=%u  checksum ok"
         "  lat=%.4f lon=%.4f alt=%.1f m  battery=%u%%\n" (lat/1e7, lon/1e7, alt/1000.0). */
    (void)f; (void)len;
}

int main(void) {
    printf("=== MAVLink telemetry decode (Season 6 finale) ===\n\n");

    uint8_t pkt[32];
    int len = encode(pkt, 7, 548486000, 830974000, 120000, 47);
    decode("drone GPS", pkt, len);

    uint8_t bad[32];
    for (int i = 0; i < len; i++) bad[i] = pkt[i];
    bad[8] ^= 0xFF;
    decode("drone GPS (corrupt)", bad, len);

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

static void crc_acc(uint8_t data, uint16_t *crc) { (void)data; (void)crc; /* TODO 1 */ }
static uint16_t frame_crc(const uint8_t *f) { (void)f; return 0; /* TODO 2 */ }
