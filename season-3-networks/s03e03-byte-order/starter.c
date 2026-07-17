/*
 * OPERATION MOONLIGHT — s03e03 "Порядок байтов"
 * Каркас. Скопируй в artifacts/byteorder.c и закрой TODO.
 *
 *   cp starter.c artifacts/byteorder.c
 *   make test
 *
 * Концепт: сеть = big-endian; x86 = little-endian. htons/htonl -> сеть, ntohs/ntohl -> хост.
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

    /* TODO 1: определи endianness хоста через probe (первый байт uint32_t=1)
       и напечатай "host is little-endian\n\n" или "big". */
    uint32_t probe = 1; (void)probe;

    uint16_t port = 443;
    /* TODO 2: nport = htons(port);
       напечатай (формат — см. tests/expected.txt):
         "port %u (0x%04X):\n"
         "  in memory (host):     " + hexbytes(&port,2)
         "  htons -> on the wire: " + hexbytes(&nport,2)
         "  wire value read as host = %u (wrong); ntohs = %u (right)\n", nport, ntohs(nport) */

    uint32_t ip = 0xCB00712A;
    /* TODO 3: nip = htonl(ip); напечатай его байты в памяти и «на проводе». */
    (void)ip;
    return 0;
}
