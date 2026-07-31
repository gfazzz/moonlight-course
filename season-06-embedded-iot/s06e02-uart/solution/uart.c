/*
 * OPERATION MOONLIGHT — s06e02 "UART"
 * Эталонное решение: uart.c
 *
 * Концепт серии: по одному проводу байт едет последовательно, кадром:
 *                start(0) + 8 бит данных (LSB первым) + бит чётности + stop(1).
 *                Приёмник проверяет чётность и стоп-бит, собирает сообщение.
 * Задача: декодировать «провод» из 11-битных кадров, поймать ошибки, собрать текст.
 *
 * Кадр упакован в 16-битное слово:
 *   бит 0     = start (0)
 *   биты 1..8 = данные (байт)
 *   бит 9     = parity (even: XOR битов данных)
 *   бит 10    = stop (1)
 */
#include <stdio.h>
#include <stdint.h>

/* Бит чётности (even): XOR всех 8 бит — столько единиц, чтобы их общее число было чётным. */
static int even_parity(uint8_t b) {
    int p = 0;
    for (int i = 0; i < 8; i++) p ^= (b >> i) & 1;
    return p;
}

/* Передатчик (контроллер): собрать корректный кадр из байта. */
static uint16_t encode(uint8_t b) {
    return (uint16_t)((0u << 0) | ((uint16_t)b << 1) |
                      ((uint16_t)even_parity(b) << 9) | (1u << 10));
}

int main(void) {
    /* «Провод»: сообщение "OPEN 7" + два диагностических кадра с дефектами. */
    const char *msg = "OPEN 7";
    uint16_t wire[8];
    int n = 0;
    for (const char *p = msg; *p; p++) wire[n++] = encode((uint8_t)*p);
    wire[n++] = encode('*') ^ (1u << 9);   /* испорчен бит чётности */
    wire[n++] = encode('Z') & ~(1u << 10); /* стоп-бит сброшен в 0 */

    printf("=== UART frame decode ===\n");
    printf("wire: %d frames (start + 8 data + even parity + stop)\n\n", n);

    char out[16];
    int oi = 0, ok = 0, err = 0;
    for (int i = 0; i < n; i++) {
        uint16_t f = wire[i];
        int start  = f & 1;
        uint8_t data = (uint8_t)((f >> 1) & 0xFF);
        int parity = (f >> 9) & 1;
        int stop   = (f >> 10) & 1;

        if (start != 0) { printf("frame %d: FRAMING ERROR (start=1)\n", i); err++; continue; }
        if (stop != 1)  { printf("frame %d: FRAMING ERROR (stop=0)\n", i);  err++; continue; }

        int want = even_parity(data);
        if (parity != want) {
            printf("frame %d: data=0x%02X PARITY ERROR (got %d, want %d)\n",
                   i, data, parity, want);
            err++;
            continue;
        }
        printf("frame %d: data=0x%02X '%c' parity ok stop ok\n", i, data, data);
        out[oi++] = (char)data;
        ok++;
    }
    out[oi] = '\0';

    printf("\nmessage: \"%s\"\n", out);
    printf("frames: %d ok, %d errors\n", ok, err);
    return 0;
}
