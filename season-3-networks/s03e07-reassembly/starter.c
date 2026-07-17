/*
 * OPERATION MOONLIGHT — s03e07 "Реассемблинг потока"
 * Каркас. Скопируй в artifacts/reassemble.c и закрой TODO.
 *
 *   cp starter.c artifacts/reassemble.c
 *   make test        # запустит: reassemble ../segments.dat
 *
 * Концепт: seq = смещение байта в потоке. Клади сегмент на его смещение —
 *          порядок прихода не важен, важен seq.
 * Формат сегмента: seq(uint32 BE) len(uint16 BE) data(len байт).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXBUF 4096

static uint16_t be16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "segments.dat";
    FILE *f = fopen(path, "rb");
    if (f == NULL) { perror("fopen"); return 1; }

    char stream[MAXBUF];
    int filled[MAXBUF];
    memset(stream, 0, sizeof stream);
    memset(filled, 0, sizeof filled);
    int maxend = 0, segs = 0;

    printf("=== STREAM REASSEMBLER ===\n");

    uint8_t hdr[6];
    while (fread(hdr, 1, 6, f) == 6) {
        uint32_t seq = be32(hdr);
        uint16_t len = be16(hdr + 4);
        uint8_t data[512];
        if (len > sizeof data) break;
        if (fread(data, 1, len, f) != len) break;
        segs++;
        printf("segment seq=%-3u len=%u (arrival order)\n", seq, len);

        /* TODO 1: положи каждый байт data[i] на смещение seq+i:
             stream[seq+i]=data[i]; filled[seq+i]=1; (в пределах MAXBUF)
           TODO 2: обнови maxend = max(maxend, seq+len). */
    }
    fclose(f);

    /* TODO 3: посчитай пропуски gaps (позиции < maxend без filled). */
    int gaps = 0;

    printf("\n%d segments, %d bytes, %d gap(s)\n", segs, maxend, gaps);
    printf("reassembled: \"%.*s\"\n", maxend, stream);
    return 0;
}
