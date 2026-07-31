/* Реализация модуля хеширования Season 4. */
#include "ml_hash.h"

unsigned long ml_hash_djb2(const char *s) {
    unsigned long h = 5381UL;
    for (; *s; s++) h = h * 33UL + (unsigned char)*s;
    return h;
}

size_t ml_hash_bucket(const char *s, size_t n) {
    return (size_t)(ml_hash_djb2(s) % (unsigned long)n);
}
