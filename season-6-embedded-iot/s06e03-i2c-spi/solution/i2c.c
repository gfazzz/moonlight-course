/*
 * OPERATION MOONLIGHT — s06e03 "I2C/SPI"
 * Эталонное решение: i2c.c
 *
 * Концепт серии: на шине много устройств. У каждого — адрес и карта регистров.
 *                Транзакция: выбрать устройство (адрес) -> указать регистр ->
 *                прочитать/записать. Нет устройства — шина отвечает NACK.
 * Задача: опросить датчик по I2C — проверить его ID, запустить измерение,
 *         прочитать статус и сырое значение (два байта -> одно число).
 *
 * SPI устроен иначе (выбор устройства линией CS, полный дуплекс) — но модель
 * «адрес регистра -> данные» та же; разбор различий в theory.md.
 */
#include <stdio.h>
#include <stdint.h>

#define SENSOR_ADDR 0x76      /* адрес датчика на шине (7-битный) */
#define CHIP_ID     0x58      /* ожидаемый ID в регистре 0xD0 */

/* Модель одного устройства: карта из 256 регистров + маска «только чтение». */
static uint8_t reg[256];
static uint8_t readonly[256];

static void dev_init(void) {
    reg[0xD0] = CHIP_ID; readonly[0xD0] = 1;   /* chip id, read-only */
    reg[0xF3] = 0x00;                          /* status */
    reg[0xF4] = 0x00;                          /* ctrl_meas */
    reg[0xFA] = 0x0A; reg[0xFB] = 0x2B;        /* сырое значение: 0x0A2B */
}

/* Чтение регистра. rc: 0 = ACK (ок), -1 = NACK (не тот адрес). */
static int i2c_read(uint8_t addr, uint8_t r, uint8_t *out) {
    if (addr != SENSOR_ADDR) return -1;        /* устройство не откликнулось */
    *out = reg[r];
    return 0;
}

/* Запись регистра. rc: 0 = ACK, -1 = NACK (адрес), -2 = регистр только для чтения. */
static int i2c_write(uint8_t addr, uint8_t r, uint8_t val) {
    if (addr != SENSOR_ADDR) return -1;
    if (readonly[r]) return -2;
    reg[r] = val;
    /* Запуск измерения выставляет бит готовности данных в статусе. */
    if (r == 0xF4 && val != 0) reg[0xF3] |= (1 << 0);
    return 0;
}

int main(void) {
    dev_init();
    printf("=== I2C sensor probe (bus addr 0x%02X) ===\n\n", SENSOR_ADDR);

    uint8_t v;

    /* 1. Проверка ID: убеждаемся, что на шине именно наш датчик. */
    i2c_read(SENSOR_ADDR, 0xD0, &v);
    printf("read WHO_AM_I (0xD0) -> 0x%02X : %s\n",
           v, v == CHIP_ID ? "sensor OK" : "unknown device");

    /* 2. Обращение по неверному адресу -> NACK. */
    int rc = i2c_read(0x08, 0xD0, &v);
    printf("read from 0x08       -> %s\n", rc == 0 ? "ACK" : "NACK (no device)");

    /* 3. Запуск измерения: пишем ctrl_meas. */
    rc = i2c_write(SENSOR_ADDR, 0xF4, 0x27);
    printf("write CTRL_MEAS 0x27 -> %s\n", rc == 0 ? "ACK" : "NACK");

    /* 4. Статус: бит0 = данные готовы. */
    i2c_read(SENSOR_ADDR, 0xF3, &v);
    printf("read STATUS (0xF3)   -> 0x%02X (data_ready=%d)\n", v, v & 1);

    /* 5. Сырое значение: два байта, старший первым -> 16 бит. */
    uint8_t msb, lsb;
    i2c_read(SENSOR_ADDR, 0xFA, &msb);
    i2c_read(SENSOR_ADDR, 0xFB, &lsb);
    uint16_t raw = (uint16_t)((msb << 8) | lsb);
    printf("read RAW (0xFA:0xFB) -> 0x%04X = %u -> %.2f units\n",
           raw, raw, raw / 100.0);

    /* 6. Попытка записи в read-only регистр -> NACK. */
    rc = i2c_write(SENSOR_ADDR, 0xD0, 0x00);
    printf("write to 0xD0 (RO)   -> %s\n", rc == -2 ? "NACK (read-only)" : "ACK");

    return 0;
}
