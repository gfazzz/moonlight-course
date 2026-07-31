/*
 * OPERATION MOONLIGHT — s06e07 "IP-камеры: детект движения"
 * Эталонное решение: motion.c
 *
 * Концепт серии: видеопоток камеры (RTSP) — это кадры, а кадр — 2D-массив пикселей.
 *                Движение находят РАЗНОСТЬЮ кадров: |f2 - f1| по каждому пикселю,
 *                порог отсекает шум сенсора, а разбиение на зоны говорит, ГДЕ движение.
 * Задача: сравнить два кадра, построить карту изменений, найти зоны с движением
 *         и «слепые» зоны (где патруль не появляется).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define W 8
#define H 8
#define THRESH 40      /* порог: меньше — шум сенсора, игнорируем */
#define MIN_PIX 3      /* сколько изменённых пикселей считаем движением */

static uint8_t f1[H][W], f2[H][W];

static void build_frames(void) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) { f1[y][x] = 30; f2[y][x] = 30; }

    /* Подпороговый шум во втором кадре (diff < THRESH — должен отсеяться). */
    f2[3][0] = 38; f2[4][7] = 37; f2[0][7] = 39;

    /* «Охранник» — яркий блок 2x2. В кадре 1 слева-сверху, в кадре 2 справа-снизу. */
    f1[0][0] = f1[0][1] = f1[1][0] = f1[1][1] = 200;
    f2[6][6] = f2[6][7] = f2[7][6] = f2[7][7] = 200;
}

int main(void) {
    build_frames();
    printf("=== camera motion detection (%dx%d, thresh %d) ===\n\n", W, H, THRESH);

    int zone[2][2] = {{0, 0}, {0, 0}};   /* счётчики по квадрантам */
    int total = 0;

    printf("change map ('#' = motion):\n");
    for (int y = 0; y < H; y++) {
        printf("  ");
        for (int x = 0; x < W; x++) {
            int d = abs((int)f2[y][x] - (int)f1[y][x]);
            int changed = d > THRESH;
            putchar(changed ? '#' : '.');
            if (changed) { zone[y / (H / 2)][x / (W / 2)]++; total++; }
        }
        putchar('\n');
    }

    printf("\nchanged pixels: %d -> motion %s\n",
           total, total >= MIN_PIX ? "DETECTED" : "none");

    const char *qn[2][2] = {{"top-left", "top-right"}, {"bottom-left", "bottom-right"}};
    printf("\nper-zone:\n");
    for (int qy = 0; qy < 2; qy++)
        for (int qx = 0; qx < 2; qx++)
            printf("  %-13s: %d %s\n", qn[qy][qx], zone[qy][qx],
                   zone[qy][qx] > 0 ? "(motion)" : "(BLIND — no patrol)");

    return 0;
}
