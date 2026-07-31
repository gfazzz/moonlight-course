/*
 * OPERATION MOONLIGHT — библиотека Season 6: контроль целостности
 * Извлечено из s06e05 (Modbus RTU).
 */
#ifndef ML_CRC_H
#define ML_CRC_H

#include <stddef.h>

/* CRC-16/MODBUS: init 0xFFFF, отражённый полином 0xA001. */
unsigned short ml_crc16_modbus(const unsigned char *buf, size_t len);

/* Проверка кадра, у которого последние два байта — CRC (младший первым). */
int ml_crc16_frame_ok(const unsigned char *frame, size_t len);

#endif /* ML_CRC_H */
