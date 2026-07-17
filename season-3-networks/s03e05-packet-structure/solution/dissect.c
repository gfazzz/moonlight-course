/*
 * OPERATION MOONLIGHT — s03e05 "Структура пакета"
 * Эталонное решение: dissect.c
 *
 * Концепт серии: пакет — это слои по смещениям. IPv4-заголовок (20 байт),
 *                затем TCP-заголовок. Все многобайтовые поля — big-endian.
 * Задача: разобрать захваченный пакет по полям (адреса, порты, seq, флаги).
 *
 * Читаем поля побайтово (big-endian) — без приведения указателей: и переносимо,
 * и без проблем с выравниванием.
 */
#include <stdio.h>
#include <stdint.h>

static uint16_t be16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

int main(void) {
    /* Захваченный пакет: SYN 203.0.113.42:31337 -> 192.168.1.100:443. */
    uint8_t pkt[] = {
        /* --- IPv4 (20 байт) --- */
        0x45, 0x00, 0x00, 0x28,   /* ver/ihl, dscp, total_len=40 */
        0x1C, 0x46, 0x40, 0x00,   /* id, flags/frag */
        0x40, 0x06, 0x00, 0x00,   /* ttl=64, proto=6(TCP), checksum */
        0xCB, 0x00, 0x71, 0x2A,   /* src 203.0.113.42 */
        0xC0, 0xA8, 0x01, 0x64,   /* dst 192.168.1.100 */
        /* --- TCP (20 байт) --- */
        0x7A, 0x69, 0x01, 0xBB,   /* src_port=31337, dst_port=443 */
        0x00, 0x00, 0x00, 0x01,   /* seq=1 */
        0x00, 0x00, 0x00, 0x00,   /* ack=0 */
        0x50, 0x02, 0xFF, 0xFF,   /* data_off=5, flags=SYN(0x02), window */
        0x00, 0x00, 0x00, 0x00    /* checksum, urgent */
    };
    int len = (int)sizeof(pkt);

    /* --- IP-заголовок --- */
    int version = pkt[0] >> 4;
    int ihl = pkt[0] & 0x0F;
    int ihl_bytes = ihl * 4;
    int total_len = be16(pkt + 2);
    int ttl = pkt[8];
    int proto = pkt[9];

    printf("=== PACKET DISSECTOR (%d bytes) ===\n", len);
    printf("IP: version=%d ihl=%d (%d bytes) total_len=%d ttl=%d proto=%d (%s)\n",
           version, ihl, ihl_bytes, total_len, ttl, proto, (proto == 6) ? "TCP" : "?");
    printf("    src=%u.%u.%u.%u dst=%u.%u.%u.%u\n",
           pkt[12], pkt[13], pkt[14], pkt[15], pkt[16], pkt[17], pkt[18], pkt[19]);

    /* --- TCP-заголовок (начинается на ihl_bytes) --- */
    const uint8_t *tcp = pkt + ihl_bytes;
    int src_port = be16(tcp + 0);
    int dst_port = be16(tcp + 2);
    uint32_t seq = be32(tcp + 4);
    int data_off = tcp[12] >> 4;
    uint8_t flags = tcp[13];

    char fbuf[32] = {0};
    if (flags & 0x02) snprintf(fbuf, sizeof fbuf, "SYN");
    else if (flags & 0x10) snprintf(fbuf, sizeof fbuf, "ACK");
    else snprintf(fbuf, sizeof fbuf, "0x%02X", flags);

    printf("TCP: src_port=%d dst_port=%d seq=%u data_off=%d flags=%s\n",
           src_port, dst_port, seq, data_off, fbuf);

    int header_bytes = ihl_bytes + data_off * 4;
    printf("payload: %d bytes\n", total_len - header_bytes);

    if (src_port == 31337)
        printf("\n[!] source port 31337 — the Ghost's signature.\n");
    return 0;
}
