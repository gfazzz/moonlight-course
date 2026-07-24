/*
 * OPERATION MOONLIGHT — s06e05 "Modbus RTU"
 * Каркас. Скопируй в artifacts/modbus.c и закрой TODO.
 *
 *   cp starter.c artifacts/modbus.c
 *   make test
 *
 * Кадр: адрес + функция + данные + CRC-16. CRC-16/MODBUS: init 0xFFFF,
 * полином 0xA001 (отражённый), дописывается младшим байтом первым.
 */
#include <stdio.h>
#include <stdint.h>

#define FUNC_READ_HOLDING 0x03

/* TODO 1: crc16(buf, len): crc=0xFFFF; для каждого байта: crc^=buf[i];
   8 раз: crc = (crc&1) ? (crc>>1)^0xA001 : (crc>>1). Вернуть crc. */
static uint16_t crc16(const uint8_t *buf, int len);

/* TODO 2: append_crc(buf, len): c=crc16(buf,len); buf[len]=c&0xFF; buf[len+1]=c>>8; return len+2. */
static int append_crc(uint8_t *buf, int len);

/* TODO 3: crc_ok(buf, len): сравнить crc16(buf, len-2) с (buf[len-2] | buf[len-1]<<8). */
static int crc_ok(const uint8_t *buf, int len);

static void dump(const char *label, const uint8_t *buf, int len) {
    printf("%s (%d bytes):", label, len);
    for (int i = 0; i < len; i++) printf(" %02X", buf[i]);
    putchar('\n');
}

int main(void) {
    printf("=== Modbus RTU ===\n\n");

    uint8_t req[8] = {0x11, FUNC_READ_HOLDING, 0x00, 0x6B, 0x00, 0x03};
    int rlen = append_crc(req, 6);
    dump("request ", req, rlen);
    printf("  slave=0x%02X func=0x%02X start=0x%04X count=%u\n\n",
           req[0], req[1], (req[2] << 8) | req[3], (req[4] << 8) | req[5]);

    uint8_t resp[16] = {0x11, FUNC_READ_HOLDING, 0x06,
                        0x02, 0x2B, 0x00, 0x00, 0x00, 0x64};
    int elen = append_crc(resp, 9);
    dump("response", resp, elen);

    /* TODO 4: если crc_ok(resp,elen): nregs=resp[2]/2; "  CRC ok -> %d registers:";
       имена {"VENT","PUMP","GATE"}; для каждого: val=(resp[3+i*2]<<8)|resp[4+i*2];
       "    reg[%d] %-4s = %u". Иначе "  CRC FAIL -> frame rejected". */

    uint8_t bad[16];
    for (int i = 0; i < elen; i++) bad[i] = resp[i];
    bad[4] ^= 0xFF;
    dump("corrupt ", bad, elen);
    /* TODO 5: "  CRC ok (!?)" или "  CRC FAIL -> frame rejected" по crc_ok(bad,elen). */

    return 0;
}

static uint16_t crc16(const uint8_t *buf, int len) { (void)buf;(void)len; return 0; /* TODO 1 */ }
static int append_crc(uint8_t *buf, int len) { (void)buf; return len; /* TODO 2 */ }
static int crc_ok(const uint8_t *buf, int len) { (void)buf;(void)len; return 0; /* TODO 3 */ }
