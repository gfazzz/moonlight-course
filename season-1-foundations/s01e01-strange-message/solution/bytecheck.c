/*
 * OPERATION MOONLIGHT — s01e01 "Первый байт"
 * Эталонное решение: bytecheck.c
 *
 * Концепт серии: типы (unsigned char), hex, оператор XOR на ОДНОМ байте
 *                и его самообратимость (A ^ K ^ K = A).
 * Задача: проверить ключ на первом байте перехвата, не декодируя всё сообщение.
 */
#include <stdio.h>

#define KEY 0x42

int main(void) {
    unsigned char first = 0x0F;      /* первый байт перехвата от V. */
    unsigned char dec  = first ^ KEY; /* расшифровали один байт */
    unsigned char back = dec ^ KEY;   /* тем же ключом — вернули исходный */

    printf("=== MOONLIGHT — byte check ===\n");
    printf("First intercepted byte: 0x%02X\n", first);
    printf("Key: 0x%02X\n\n", KEY);

    printf("0x%02X ^ 0x%02X = 0x%02X  ('%c')\n", first, KEY, dec, dec);
    printf("0x%02X ^ 0x%02X = 0x%02X  (back to source)\n\n", dec, KEY, back);

    printf("Key 0x%02X works. The message is real. Decode the rest in s01e02.\n", KEY);
    return 0;
}
