/*
 * OPERATION MOONLIGHT — s06e05 "Modbus RTU"
 * Эталонное решение: modbus.c
 *
 * Концепт серии: промышленные контроллеры (PLC) общаются по Modbus RTU. Кадр:
 *                адрес + функция + данные + CRC-16. CRC (полином 0xA001) защищает
 *                целостность куда надёжнее бита чётности из s06e02/s06e04.
 * Задача: собрать запрос "read holding registers", проверить CRC ответа,
 *         извлечь значения регистров, отбраковать повреждённый кадр.
 *
 * CRC-16/MODBUS: init 0xFFFF, отражённый полином 0xA001, дописывается младшим байтом.
 */
#include <stdio.h>
#include <stdint.h>

#define FUNC_READ_HOLDING 0x03

static uint16_t crc16(const uint8_t *buf, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int b = 0; b < 8; b++)
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }
    return crc;
}

/* Дописать CRC к кадру длины len (младший байт первым). Возврат — новая длина. */
static int append_crc(uint8_t *buf, int len) {
    uint16_t c = crc16(buf, len);
    buf[len]     = (uint8_t)(c & 0xFF);        /* low */
    buf[len + 1] = (uint8_t)(c >> 8);          /* high */
    return len + 2;
}

/* Проверка CRC всего кадра (последние 2 байта — контрольная сумма). */
static int crc_ok(const uint8_t *buf, int len) {
    uint16_t calc = crc16(buf, len - 2);
    uint16_t got  = (uint16_t)(buf[len - 2] | (buf[len - 1] << 8));
    return calc == got;
}

static void dump(const char *label, const uint8_t *buf, int len) {
    printf("%s (%d bytes):", label, len);
    for (int i = 0; i < len; i++) printf(" %02X", buf[i]);
    putchar('\n');
}

int main(void) {
    printf("=== Modbus RTU ===\n\n");

    /* 1. Запрос: slave 0x11, читать 3 holding-регистра с адреса 0x006B. */
    uint8_t req[8] = {0x11, FUNC_READ_HOLDING, 0x00, 0x6B, 0x00, 0x03};
    int rlen = append_crc(req, 6);
    dump("request ", req, rlen);
    printf("  slave=0x%02X func=0x%02X start=0x%04X count=%u\n\n",
           req[0], req[1], (req[2] << 8) | req[3], (req[4] << 8) | req[5]);

    /* 2. Ответ от PLC: 3 регистра (VENT=555, PUMP=0, GATE=100). */
    uint8_t resp[16] = {0x11, FUNC_READ_HOLDING, 0x06,
                        0x02, 0x2B,   /* 0x022B = 555 */
                        0x00, 0x00,   /* 0x0000 = 0   */
                        0x00, 0x64};  /* 0x0064 = 100 */
    int elen = append_crc(resp, 9);
    dump("response", resp, elen);

    if (crc_ok(resp, elen)) {
        int nregs = resp[2] / 2;
        printf("  CRC ok -> %d registers:\n", nregs);
        const char *name[] = {"VENT", "PUMP", "GATE"};
        for (int i = 0; i < nregs; i++) {
            uint16_t val = (uint16_t)((resp[3 + i * 2] << 8) | resp[4 + i * 2]);
            printf("    reg[%d] %-4s = %u\n", i, name[i], val);
        }
    } else {
        printf("  CRC FAIL -> frame rejected\n");
    }
    putchar('\n');

    /* 3. Тот же ответ с одним искажённым байтом -> CRC не сойдётся. */
    uint8_t bad[16];
    for (int i = 0; i < elen; i++) bad[i] = resp[i];
    bad[4] ^= 0xFF;                            /* шум в линии повредил регистр */
    dump("corrupt ", bad, elen);
    printf("  CRC %s\n", crc_ok(bad, elen) ? "ok (!?)" : "FAIL -> frame rejected");

    return 0;
}
