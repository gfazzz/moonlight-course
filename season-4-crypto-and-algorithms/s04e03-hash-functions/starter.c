/*
 * OPERATION MOONLIGHT — s04e03 "Хеш-функции"
 * Каркас. Скопируй в artifacts/hashes.c и закрой TODO.
 *
 *   cp starter.c artifacts/hashes.c
 *   make test
 *
 * Концепт: хеш — короткий отпечаток данных. djb2 и FNV-1a — НЕкриптографические:
 *          сила лавины зависит от того, где изменился вход.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* TODO 1: djb2(s) — h = 5381; для каждого байта: h = ((h<<5)+h) + c;  вернуть h. */

/* TODO 2: fnv1a(s) — h = 2166136261u; для каждого байта: h ^= c; h *= 16777619u. */

/* TODO 3: popcount32(x) — сколько единичных битов в x. */

/* TODO 4: avalanche(what, a, b) — посчитать djb2(a)^djb2(b) и fnv1a(a)^fnv1a(b),
   напечатать: "%-18s djb2: %2d/32 bits   fnv1a: %2d/32 bits\n" */

int main(void) {
    const char *words[] = {"MOONLIGHT", "VIKTOR", "EVA", "GHOST"};
    int n = 4;

    printf("=== HASH FUNCTIONS ===\n");
    printf("%-12s %-12s %s\n", "string", "djb2", "fnv1a");
    /* TODO 5: таблица хешей: "%-12s 0x%08X   0x%08X\n" */

    printf("\n--- avalanche: 1 changed character ---\n");
    /* TODO 6: avalanche("last  char T->U", "MOONLIGHT", "MOONLIGHU");
               avalanche("first char M->N", "MOONLIGHT", "NOONLIGHT"); */

    printf("\ndjb2 barely reacts to a last-byte change: h = h*33 + c\n");
    printf("leaves the final byte in the low bits — weak avalanche.\n");

    printf("\n--- buckets in a 16-slot table (djb2 %% 16) ---\n");
    /* TODO 7: "%-12s bucket %2u\n" для каждого слова: djb2(word) % 16 */

    (void)words; (void)n;
    return 0;
}
