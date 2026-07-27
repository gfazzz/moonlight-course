/* Реализация модуля контроля целостности Season 6. */
#include "ml_crc.h"

unsigned short ml_crc16_modbus(const unsigned char *buf, size_t len) {
    unsigned short crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int b = 0; b < 8; b++)
            crc = (crc & 1) ? (unsigned short)((crc >> 1) ^ 0xA001) : (unsigned short)(crc >> 1);
    }
    return crc;
}

int ml_crc16_frame_ok(const unsigned char *frame, size_t len) {
    if (len < 3) return 0;
    unsigned short calc = ml_crc16_modbus(frame, len - 2);
    unsigned short got  = (unsigned short)(frame[len - 2] | (frame[len - 1] << 8));
    return calc == got;
}
