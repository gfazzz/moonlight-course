/*
 * OPERATION MOONLIGHT — s03e08 "Шифрованный канал" (финал Season 3)
 * Эталонное решение: intercept.c
 *
 * Концепт серии: собрать сезон воедино — перехваченный TCP-поток пришёл
 *                вперемешку (s03e07) И зашифрован XOR (s01). Реассемблируем по seq,
 *                затем расшифровываем ключом 0x42 — и читаем разговор.
 * Задача: из зашифрованных сегментов восстановить открытое сообщение.
 *
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

    /* 1) Реассемблинг по seq (как в s03e07). */
    uint8_t hdr[6];
    while (fread(hdr, 1, 6, f) == 6) {
        uint32_t seq = be32(hdr);
        uint16_t len = be16(hdr + 4);
        uint8_t data[512];
        if (len > sizeof data) break;
        if (fread(data, 1, len, f) != len) break;
        segs++;
        for (int i = 0; i < len && (int)(seq + i) < MAXBUF; i++) {
            stream[seq + i] = data[i];
            filled[seq + i] = 1;
        }
        if ((int)(seq + len) > maxend) maxend = (int)(seq + len);
    }
    fclose(f);

    int gaps = 0;
    for (int i = 0; i < maxend; i++) if (!filled[i]) gaps++;
    printf("reassembled: %d segments, %d bytes, %d gap(s)\n", segs, maxend, gaps);

    if (gaps != 0) { printf("stream incomplete — cannot decrypt.\n"); return 1; }

    /* 2) Расшифровка XOR (как в s01). */
    for (int i = 0; i < maxend; i++) stream[i] ^= XOR_KEY;

    printf("\n--- DECRYPTED MESSAGE ---\n");
    printf("%.*s\n", maxend, stream);
    printf("--- END ---\n");
    return 0;
}
