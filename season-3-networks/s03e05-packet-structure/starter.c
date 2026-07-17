/*
 * OPERATION MOONLIGHT — s03e05 "Структура пакета"
 * Каркас. Скопируй в artifacts/dissect.c и закрой TODO.
 *
 *   cp starter.c artifacts/dissect.c
 *   make test
 *
 * Концепт: пакет — слои по смещениям. IPv4 (20 байт), затем TCP. Поля — big-endian.
 * Читай многобайтовые поля побайтово (be16/be32) — переносимо и без проблем с выравниванием.
 */
#include <stdio.h>
#include <stdint.h>

/* TODO 1: be16(p) -> (p[0]<<8)|p[1];  be32(p) -> 4 байта big-endian. */

int main(void) {
    uint8_t pkt[] = {
        0x45, 0x00, 0x00, 0x28,  0x1C, 0x46, 0x40, 0x00,
        0x40, 0x06, 0x00, 0x00,  0xCB, 0x00, 0x71, 0x2A,
        0xC0, 0xA8, 0x01, 0x64,
        0x7A, 0x69, 0x01, 0xBB,  0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00,  0x50, 0x02, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00
    };
    int len = (int)sizeof(pkt);

    /* TODO 2: IP-заголовок:
       version = pkt[0] >> 4; ihl = pkt[0] & 0x0F; ihl_bytes = ihl*4;
       total_len = be16(pkt+2); ttl = pkt[8]; proto = pkt[9].
       Печать (см. tests/expected.txt): строка "IP: ..." и "    src=... dst=..." (pkt[12..19]). */

    /* TODO 3: TCP-заголовок (tcp = pkt + ihl_bytes):
       src_port = be16(tcp+0); dst_port = be16(tcp+2); seq = be32(tcp+4);
       data_off = tcp[12] >> 4; flags = tcp[13].
       Флаг SYN = 0x02, ACK = 0x10. Печать "TCP: ..." и "payload: <total_len - headers> bytes". */

    /* TODO 4: если src_port == 31337 — вывести подпись «Призрака». */
    (void)len;
    return 0;
}
