/*
 * OPERATION MOONLIGHT — s06e04 "Wiegand / СКУД"
 * Каркас. Скопируй в artifacts/wiegand.c и закрой TODO.
 *
 *   cp starter.c artifacts/wiegand.c
 *   make test
 *
 * Формат Wiegand-26 (бит 25 первым, бит 0 последним):
 *   [25]=even по битам 24..13, [24..17]=facility, [16..1]=card, [0]=odd по битам 12..1.
 */
#include <stdio.h>
#include <stdint.h>

#define AUTH_FACILITY 42

/* TODO 1: xor_bits(v, hi, lo) — XOR всех бит v в диапазоне [lo..hi]. */
static int xor_bits(uint32_t v, int hi, int lo);

static uint32_t encode(uint8_t fac, uint16_t card) {
    uint32_t code = ((uint32_t)fac << 17) | ((uint32_t)card << 1);
    int lead  =  xor_bits(code, 24, 13);
    int trail = !xor_bits(code, 12, 1);
    code |= ((uint32_t)lead << 25) | (uint32_t)trail;
    return code;
}

static void decode(const char *label, uint32_t code) {
    /* TODO 2: извлеки lead=(code>>25)&1, fac=(code>>17)&0xFF,
       card=(code>>1)&0xFFFF, trail=code&1. */

    /* TODO 3: lead_ok = (lead == xor_bits(code,24,13));
              trail_ok = (trail == !xor_bits(code,12,1)). */

    /* TODO 4: печать (формат — см. tests/expected.txt):
         "%s: raw=0x%07X" (code & 0x3FFFFFF)
         "  facility=%u  card=%u"
         "  parity: leading(even)=ok|BAD trailing(odd)=ok|BAD"
       решение:
         !lead_ok||!trail_ok -> "  -> DENY (corrupt card)\n"
         fac!=AUTH_FACILITY   -> "  -> DENY (facility %u not authorized)\n"
         иначе                -> "  -> GRANT\n". */
    (void)label; (void)code;
}

int main(void) {
    printf("=== Wiegand-26 access control (authorized facility %d) ===\n\n", AUTH_FACILITY);

    uint32_t good = encode(42, 1337);
    decode("badge A (valid)", good);

    uint32_t tampered = good ^ (1u << 5);
    decode("badge B (tampered)", tampered);

    uint32_t foreign = encode(99, 5);
    decode("badge C (cloned)", foreign);

    return 0;
}

static int xor_bits(uint32_t v, int hi, int lo) { (void)v;(void)hi;(void)lo; return 0; /* TODO 1 */ }
