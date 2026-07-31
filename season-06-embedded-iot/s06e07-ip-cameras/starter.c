/*
 * OPERATION MOONLIGHT — s06e07 "IP-камеры: детект движения"
 * Каркас. Скопируй в artifacts/motion.c и закрой TODO.
 *
 *   cp starter.c artifacts/motion.c
 *   make test
 *
 * Концепт: кадр — 2D-массив пикселей. Движение = |f2-f1| > порога;
 *   порог отсекает шум, зоны говорят, ГДЕ движение.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define W 8
#define H 8
#define THRESH 40
#define MIN_PIX 3

static uint8_t f1[H][W], f2[H][W];

static void build_frames(void) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) { f1[y][x] = 30; f2[y][x] = 30; }
    f2[3][0] = 38; f2[4][7] = 37; f2[0][7] = 39;            /* подпороговый шум */
    f1[0][0] = f1[0][1] = f1[1][0] = f1[1][1] = 200;        /* охранник в кадре 1 */
    f2[6][6] = f2[6][7] = f2[7][6] = f2[7][7] = 200;        /* охранник в кадре 2 */
}

int main(void) {
    build_frames();
    printf("=== camera motion detection (%dx%d, thresh %d) ===\n\n", W, H, THRESH);

    int zone[2][2] = {{0, 0}, {0, 0}};
    int total = 0;

    printf("change map ('#' = motion):\n");
    for (int y = 0; y < H; y++) {
        printf("  ");
        for (int x = 0; x < W; x++) {
            /* TODO 1: d = abs((int)f2[y][x] - (int)f1[y][x]); changed = d > THRESH;
               putchar('#'|'.'); если changed: zone[y/(H/2)][x/(W/2)]++, total++. */
        }
        putchar('\n');
    }

    /* TODO 2: "\nchanged pixels: %d -> motion DETECTED|none" (total, total>=MIN_PIX). */

    const char *qn[2][2] = {{"top-left", "top-right"}, {"bottom-left", "bottom-right"}};
    printf("\nper-zone:\n");
    /* TODO 3: по всем квадрантам:
       "  %-13s: %d (motion)|(BLIND — no patrol)" (qn, zone, zone>0). */

    (void)zone; (void)total; (void)qn;
    return 0;
}
