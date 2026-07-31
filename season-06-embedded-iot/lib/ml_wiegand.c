#include "ml_wiegand.h"

static int xor_bits(uint32_t v, int hi, int lo) {
    int p = 0;
    for (int i = lo; i <= hi; i++) p ^= (v >> i) & 1;
    return p;
}

uint32_t ml_wiegand_encode(uint8_t facility, uint16_t card) {
    uint32_t code = ((uint32_t)facility << 17) | ((uint32_t)card << 1);
    int lead  =  xor_bits(code, 24, 13);      /* even по старшей половине */
    int trail = !xor_bits(code, 12, 1);       /* odd по младшей */
    return code | ((uint32_t)lead << 25) | (uint32_t)trail;
}

int ml_wiegand_decode(uint32_t code, uint8_t *facility, uint16_t *card) {
    int lead  = (code >> 25) & 1;
    int trail =  code        & 1;
    if (lead  !=  xor_bits(code, 24, 13)) return 0;
    if (trail != !xor_bits(code, 12, 1))  return 0;
    if (facility) *facility = (uint8_t)((code >> 17) & 0xFF);
    if (card)     *card     = (uint16_t)((code >> 1) & 0xFFFF);
    return 1;
}
