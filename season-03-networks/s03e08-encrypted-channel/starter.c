/*
 * OPERATION MOONLIGHT — s03e08 "Шифрованный канал" (финал Season 3)
 * Каркас. Скопируй в artifacts/intercept.c и закрой TODO.
 *
 *   cp starter.c artifacts/intercept.c
 *   make test        # запустит: intercept ../channel.dat
 *
 * Концепт: собрать сезон — реассемблинг по seq (s03e07) + XOR-расшифровка (s01).
 * Формат сегмента: seq(uint32 BE) len(uint16 BE) data(len байт, XOR-шифр).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXBUF 4096
#define XOR_KEY 0x42

static uint16_t be16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "channel.dat";
    FILE *f = fopen(path, "rb");
    if (f == NULL) { perror("fopen"); return 1; }

    unsigned char stream[MAXBUF];
    int filled[MAXBUF];
    memset(stream, 0, sizeof stream);
    memset(filled, 0, sizeof filled);
    int maxend = 0, segs = 0;

    printf("=== ENCRYPTED CHANNEL INTERCEPT ===\n");

    /* TODO 1: реассемблинг по seq (как в s03e07): читай сегменты, клади по смещению,
       считай segs, maxend, filled[]. */
    uint8_t hdr[6];
    while (fread(hdr, 1, 6, f) == 6) {
        uint32_t seq = be32(hdr);
        uint16_t len = be16(hdr + 4);
        uint8_t data[512];
        if (len > sizeof data) break;
        if (fread(data, 1, len, f) != len) break;
        segs++;
        /* ... уложи data по seq ... */
        (void)seq;
    }
    fclose(f);

    int gaps = 0;
    /* TODO 2: посчитай gaps по filled[] < maxend. */
    printf("reassembled: %d segments, %d bytes, %d gap(s)\n", segs, maxend, gaps);
    if (gaps != 0) { printf("stream incomplete — cannot decrypt.\n"); return 1; }

    /* TODO 3: XOR-расшифровка stream[0..maxend) ключом XOR_KEY.
       Печать блока:
         "\n--- DECRYPTED MESSAGE ---\n"  "%.*s\n" (maxend, stream)  "--- END ---\n" */
    return 0;
}
