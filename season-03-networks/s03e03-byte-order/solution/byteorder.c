/*
 * OPERATION MOONLIGHT — s03e03 "Порядок байтов"
 * Эталонное решение: byteorder.c
 *
 * Концепт серии: сеть использует big-endian ("сетевой порядок"), а x86 —
 *                little-endian. htons/htonl переводят host->network, ntohs/ntohl
 *                обратно. Прочитаешь байты с пакета «как есть» — получишь мусор.
 * Вывод детерминирован на x86-64 (little-endian) — платформе автотеста.
 */
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

static void hexbytes(const void *p, int n) {
    const unsigned char *b = p;
    for (int i = 0; i < n; i++) printf("%02X ", b[i]);
}

int main(void) {
    printf("=== BYTE ORDER ===\n");

    uint32_t probe = 1;
    printf("host is %s-endian\n\n",
           (*(unsigned char *)&probe == 1) ? "little" : "big");

    uint16_t port = 443;                 /* HTTPS */
    uint16_t nport = htons(port);        /* host -> network */
    printf("port %u (0x%04X):\n", port, port);
    printf("  in memory (host):     "); hexbytes(&port, 2);  printf("\n");
    printf("  htons -> on the wire: "); hexbytes(&nport, 2); printf("\n");
    printf("  wire value read as host = %u (wrong); ntohs = %u (right)\n",
           nport, ntohs(nport));

    uint32_t ip = 0xCB00712A;            /* 203.0.113.42 */
    uint32_t nip = htonl(ip);
    printf("\nip 0x%08X (203.0.113.42):\n", ip);
    printf("  in memory (host):     "); hexbytes(&ip, 4);  printf("\n");
    printf("  htonl -> on the wire: "); hexbytes(&nip, 4); printf("\n");
    return 0;
}
