/*
 * OPERATION MOONLIGHT — s01e01 "Первый байт"
 * Каркас. Скопируй в artifacts/bytecheck.c и закрой TODO.
 *
 *   cp starter.c artifacts/bytecheck.c
 *   make test
 *
 * Концепт: типы (unsigned char), hex, оператор XOR на одном байте.
 * Задача: расшифровать ПЕРВЫЙ байт перехвата (0x0F) ключом 0x42 и показать,
 *         что тот же ключ возвращает исходный байт (самообратимость).
 */
#include <stdio.h>

#define KEY 0x42

int main(void) {
    unsigned char first = 0x0F;   /* первый байт перехвата от V. */

    /* TODO 1: dec = first XOR KEY  (оператор ^). Тип — unsigned char. */
    /* TODO 2: back = dec XOR KEY   (тем же ключом обратно). */

    printf("=== MOONLIGHT — byte check ===\n");
    printf("First intercepted byte: 0x%02X\n", first);
    printf("Key: 0x%02X\n\n", KEY);

    /* TODO 3: напечатай две строки (формат — см. tests/expected.txt):
       printf("0x%02X ^ 0x%02X = 0x%02X  ('%c')\n", first, KEY, dec, dec);
       printf("0x%02X ^ 0x%02X = 0x%02X  (back to source)\n\n", dec, KEY, back); */

    printf("Key 0x%02X works. The message is real. Decode the rest in s01e02.\n", KEY);
    return 0;
}
