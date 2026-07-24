/*
 * OPERATION MOONLIGHT — s06e03 "I2C/SPI"
 * Каркас. Скопируй в artifacts/i2c.c и закрой TODO.
 *
 *   cp starter.c artifacts/i2c.c
 *   make test
 *
 * Концепт: на шине много устройств; у каждого адрес и карта регистров.
 *   транзакция = адрес -> регистр -> чтение/запись; нет устройства -> NACK.
 */
#include <stdio.h>
#include <stdint.h>

#define SENSOR_ADDR 0x76
#define CHIP_ID     0x58

static uint8_t reg[256];
static uint8_t readonly[256];

static void dev_init(void) {
    reg[0xD0] = CHIP_ID; readonly[0xD0] = 1;
    reg[0xF3] = 0x00;
    reg[0xF4] = 0x00;
    reg[0xFA] = 0x0A; reg[0xFB] = 0x2B;
}

/* TODO 1: i2c_read(addr, r, *out): addr!=SENSOR_ADDR -> return -1; *out=reg[r]; return 0. */

/* TODO 2: i2c_write(addr, r, val): addr!=SENSOR_ADDR -> -1; readonly[r] -> -2;
   reg[r]=val; если r==0xF4 && val!=0: reg[0xF3] |= (1<<0); return 0. */

int main(void) {
    dev_init();
    printf("=== I2C sensor probe (bus addr 0x%02X) ===\n\n", SENSOR_ADDR);

    uint8_t v;

    /* TODO 3: read 0xD0 -> "read WHO_AM_I (0xD0) -> 0x%02X : sensor OK|unknown device". */

    /* TODO 4: read с адреса 0x08 -> "read from 0x08       -> ACK|NACK (no device)". */

    /* TODO 5: write 0xF4=0x27 -> "write CTRL_MEAS 0x27 -> ACK|NACK". */

    /* TODO 6: read 0xF3 -> "read STATUS (0xF3)   -> 0x%02X (data_ready=%d)" (v&1). */

    /* TODO 7: read 0xFA (msb) и 0xFB (lsb); raw=(msb<<8)|lsb;
       "read RAW (0xFA:0xFB) -> 0x%04X = %u -> %.2f units" (raw, raw, raw/100.0). */

    /* TODO 8: write 0xD0=0x00 -> "write to 0xD0 (RO)   -> NACK (read-only)|ACK". */

    (void)v;
    return 0;
}
