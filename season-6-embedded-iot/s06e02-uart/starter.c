/*
 * OPERATION MOONLIGHT — s06e02 "UART"
 * Каркас. Скопируй в artifacts/uart.c и закрой TODO.
 *
 *   cp starter.c artifacts/uart.c
 *   make test
 *
 * Кадр (16-битное слово): бит0=start(0), биты1..8=данные, бит9=parity(even), бит10=stop(1).
 */
#include <stdio.h>
#include <stdint.h>

/* TODO 1: even_parity(b) — XOR всех 8 бит байта (0/1). */
static int even_parity(uint8_t b);

static uint16_t encode(uint8_t b) {
    return (uint16_t)((0u << 0) | ((uint16_t)b << 1) |
                      ((uint16_t)even_parity(b) << 9) | (1u << 10));
}

int main(void) {
    const char *msg = "OPEN 7";
    uint16_t wire[8];
    int n = 0;
    for (const char *p = msg; *p; p++) wire[n++] = encode((uint8_t)*p);
    wire[n++] = encode('*') ^ (1u << 9);
    wire[n++] = encode('Z') & ~(1u << 10);

    printf("=== UART frame decode ===\n");
    printf("wire: %d frames (start + 8 data + even parity + stop)\n\n", n);

    char out[16];
    int oi = 0, ok = 0, err = 0;
    for (int i = 0; i < n; i++) {
        uint16_t f = wire[i];
        /* TODO 2: извлеки start=f&1, data=(f>>1)&0xFF, parity=(f>>9)&1, stop=(f>>10)&1. */

        /* TODO 3: если start!=0 -> "frame %d: FRAMING ERROR (start=1)", err++, continue;
                   если stop!=1  -> "frame %d: FRAMING ERROR (stop=0)",  err++, continue;
                   want=even_parity(data); если parity!=want ->
                     "frame %d: data=0x%02X PARITY ERROR (got %d, want %d)", err++, continue;
                   иначе "frame %d: data=0x%02X '%c' parity ok stop ok",
                     out[oi++]=data, ok++. */
    }
    out[oi] = '\0';

    printf("\nmessage: \"%s\"\n", out);
    printf("frames: %d ok, %d errors\n", ok, err);
    return 0;
}

static int even_parity(uint8_t b) { (void)b; return 0; /* TODO 1 */ }
