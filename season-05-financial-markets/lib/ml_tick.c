/*
 * OPERATION MOONLIGHT — библиотека Season 5: бинарный тиковый формат
 * Реализация ml_tick.h. Извлечено из s05e06.
 */
#include "ml_tick.h"
#include <string.h>

void ml_put_u16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)(v >> 8);
}

void ml_put_u32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

void ml_put_u64(uint8_t *p, uint64_t v) {
    for (int i = 0; i < 8; i++) p[i] = (uint8_t)(v >> (8 * i));
}

/* Знаковое кладём через беззнаковое: преобразование определено стандартом,
   а сдвиг вправо отрицательного числа — нет. */
void ml_put_i32(uint8_t *p, int32_t v) { ml_put_u32(p, (uint32_t)v); }

/* Приведение к нужному типу ДО сдвига: p[3] — это uint8_t, он продвинется
   до int, и сдвиг на 24 у знакового типа даст переполнение. */
uint16_t ml_get_u16(const uint8_t *p) {
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

uint32_t ml_get_u32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

uint64_t ml_get_u64(const uint8_t *p) {
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) v |= (uint64_t)p[i] << (8 * i);
    return v;
}

int32_t ml_get_i32(const uint8_t *p) { return (int32_t)ml_get_u32(p); }

uint32_t ml_zigzag(int32_t v)    { return ((uint32_t)v << 1) ^ (uint32_t)(v >> 31); }
int32_t  ml_unzigzag(uint32_t v) { return (int32_t)((v >> 1) ^ (~(v & 1) + 1)); }

size_t ml_tick_write_header(uint8_t *p, uint16_t version, uint32_t count) {
    memcpy(p, ML_TICK_MAGIC, 4);
    ml_put_u16(p + 4, version);
    ml_put_u16(p + 6, (uint16_t)(version == 1 ? ML_TICK_WIRE_V1 : ML_TICK_WIRE_V2));
    ml_put_u32(p + 8, count);
    ml_put_u32(p + 12, 0);          /* запас: поле можно добавить не сдвигая данные */
    return ML_TICK_HDR_BYTES;
}

int ml_tick_read_header(const uint8_t *p, ml_tick_header *h) {
    if (!h || memcmp(p, ML_TICK_MAGIC, 4) != 0) return 0;
    h->version    = ml_get_u16(p + 4);
    h->wire_bytes = ml_get_u16(p + 6);
    h->count      = ml_get_u32(p + 8);
    return 1;
}

size_t ml_tick_write(uint8_t *p, const ml_tick *t, uint16_t version) {
    ml_put_u32(p + 0,  t->ts);
    ml_put_i32(p + 4,  t->price);
    ml_put_u32(p + 8,  t->qty);
    ml_put_u16(p + 12, t->symbol_id);
    p[14] = t->flags;
    if (version == 1) return ML_TICK_WIRE_V1;
    p[15] = t->route_id;
    return ML_TICK_WIRE_V2;
}

size_t ml_tick_read(const uint8_t *p, ml_tick *t, uint16_t version) {
    t->ts        = ml_get_u32(p + 0);
    t->price     = ml_get_i32(p + 4);
    t->qty       = ml_get_u32(p + 8);
    t->symbol_id = ml_get_u16(p + 12);
    t->flags     = p[14];
    if (version == 1) { t->route_id = 0; return ML_TICK_WIRE_V1; }
    t->route_id = p[15];
    return ML_TICK_WIRE_V2;
}

int ml_tick_equal(const ml_tick *a, const ml_tick *b) {
    return a->ts == b->ts && a->price == b->price && a->qty == b->qty
        && a->symbol_id == b->symbol_id && a->flags == b->flags
        && a->route_id == b->route_id;
}

size_t ml_tick_encode_delta(uint8_t *out, size_t outcap, const ml_tick *t, int n) {
    if (n <= 0 || outcap < 8) return 0;
    size_t k = 0;
    ml_put_u32(out + k, t[0].ts);    k += 4;    /* опорная точка целиком */
    ml_put_i32(out + k, t[0].price); k += 4;

    for (int i = 1; i < n; i++) {
        if (k + 2 > outcap) return 0;
        uint32_t dt = t[i].ts - t[i - 1].ts;
        uint32_t dp = ml_zigzag(t[i].price - t[i - 1].price);
        if (dt > 0xFF || dp > 0xFF) return 0;   /* шаг не влез: честный отказ */
        out[k++] = (uint8_t)dt;
        out[k++] = (uint8_t)dp;
    }
    return k;
}

int ml_tick_decode_delta(const uint8_t *in, size_t inlen, ml_tick *t, int n) {
    if (n <= 0 || inlen < 8 || inlen < 8 + (size_t)(n - 1) * 2) return 0;
    size_t k = 0;
    t[0].ts    = ml_get_u32(in + k); k += 4;
    t[0].price = ml_get_i32(in + k); k += 4;

    for (int i = 1; i < n; i++) {
        t[i].ts    = t[i - 1].ts + in[k++];
        t[i].price = t[i - 1].price + ml_unzigzag(in[k++]);
    }
    return 1;
}
