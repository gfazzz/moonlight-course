/*
 * OPERATION MOONLIGHT — s03e07 "Реассемблинг потока"
 * Эталонное решение: reassemble.c
 *
 * Концепт серии: TCP — поток байтов, нарезанный на сегменты с номерами (seq =
 *                байтовое смещение). Сегменты приходят вперемешку; собрать сообщение
 *                — значит положить каждый на своё смещение (порядок из seq, не из прихода).
 * Задача: прочитать сегменты из файла и восстановить исходный поток.
 *
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

        /* Кладём каждый байт на его смещение seq+i — порядок прихода не важен. */
        for (int i = 0; i < len && (int)(seq + i) < MAXBUF; i++) {
            stream[seq + i] = (char)data[i];
            filled[seq + i] = 1;
        }
        if ((int)(seq + len) > maxend) maxend = (int)(seq + len);
    }
    fclose(f);

    /* Пропуски (недостающие сегменты) — дыры в потоке. */
    int gaps = 0;
    for (int i = 0; i < maxend; i++) if (!filled[i]) gaps++;

    printf("\n%d segments, %d bytes, %d gap(s)\n", segs, maxend, gaps);
    printf("reassembled: \"%.*s\"\n", maxend, stream);
    return 0;
}
