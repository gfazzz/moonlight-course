/*
 * OPERATION MOONLIGHT — s04e03 "Хеш-функции"
 * Эталонное решение: hashes.c
 *
 * Концепт серии: хеш — короткий «отпечаток» данных фиксированной длины.
 *                Здесь важен честный вывод: djb2 и FNV-1a — НЕкриптографические.
 *                Их лавинный эффект зависит от того, ГДЕ изменился вход:
 *                правка последнего байта у djb2 меняет всего пару младших битов.
 * Задача: реализовать обе функции, измерить лавину и увидеть коллизии по корзинам.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* djb2 (Бернштейн): h = h*33 + c, старт 5381. */
static uint32_t djb2(const char *s) {
    uint32_t h = 5381;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        h = ((h << 5) + h) + *p;
    return h;
}

/* FNV-1a: XOR с байтом, затем умножение на простое. */
static uint32_t fnv1a(const char *s) {
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static int popcount32(uint32_t x) {
    int c = 0;
    while (x) { c += (int)(x & 1u); x >>= 1; }
    return c;
}

/* Насколько сильно разошлись хеши двух строк (в битах из 32). */
static void avalanche(const char *what, const char *a, const char *b) {
    uint32_t d1 = djb2(a) ^ djb2(b);
    uint32_t d2 = fnv1a(a) ^ fnv1a(b);
    printf("%-18s djb2: %2d/32 bits   fnv1a: %2d/32 bits\n",
           what, popcount32(d1), popcount32(d2));
}

int main(void) {
    const char *words[] = {"MOONLIGHT", "VIKTOR", "EVA", "GHOST"};
    int n = 4;

    printf("=== HASH FUNCTIONS ===\n");
    printf("%-12s %-12s %s\n", "string", "djb2", "fnv1a");
    for (int i = 0; i < n; i++)
        printf("%-12s 0x%08X   0x%08X\n", words[i], djb2(words[i]), fnv1a(words[i]));

    /* Лавина зависит от позиции изменения — это ключевой вывод серии. */
    printf("\n--- avalanche: 1 changed character ---\n");
    avalanche("last  char T->U", "MOONLIGHT", "MOONLIGHU");
    avalanche("first char M->N", "MOONLIGHT", "NOONLIGHT");

    printf("\ndjb2 barely reacts to a last-byte change: h = h*33 + c\n");
    printf("leaves the final byte in the low bits — weak avalanche.\n");

    /* Коллизии: хеш большой, корзин мало — совпадения неизбежны. */
    printf("\n--- buckets in a 16-slot table (djb2 %% 16) ---\n");
    for (int i = 0; i < n; i++)
        printf("%-12s bucket %2u\n", words[i], djb2(words[i]) % 16u);
    return 0;
}
