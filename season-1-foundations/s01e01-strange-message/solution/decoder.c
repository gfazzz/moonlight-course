/*
 * OPERATION MOONLIGHT — s01e01 "Странное сообщение"
 * Эталонное решение: decoder.c
 *
 * Открывай ПОСЛЕ честной попытки. Смысл курса — в самой попытке.
 * Концепт серии: данные — это байты; XOR — обратимый ключ (A ^ K ^ K = A).
 */

#include <stdio.h>

#define XOR_KEY 0x42

int main(void) {
    /* Перехват от V. Те же 75 байт, что пришли в 23:47 на старый канал. */
    unsigned char encrypted[] = {
        0x0F, 0x0D, 0x0D, 0x0C, 0x0E, 0x0B, 0x05, 0x0A,
        0x16, 0x62, 0x23, 0x21, 0x36, 0x2B, 0x34, 0x23,
        0x36, 0x27, 0x26, 0x6C, 0x48, 0x16, 0x2A, 0x27,
        0x3B, 0x62, 0x29, 0x2C, 0x2D, 0x35, 0x6C, 0x48,
        0x04, 0x2B, 0x2E, 0x27, 0x31, 0x62, 0x23, 0x36,
        0x62, 0x2D, 0x2E, 0x26, 0x62, 0x32, 0x2E, 0x23,
        0x21, 0x27, 0x6C, 0x48, 0x16, 0x30, 0x37, 0x31,
        0x36, 0x62, 0x2D, 0x2C, 0x2E, 0x3B, 0x62, 0x21,
        0x2D, 0x26, 0x27, 0x6C, 0x48, 0xA0, 0xC2, 0xD6,
        0x62, 0x14, 0x6C
    };

    /* sizeof(массив)/sizeof(элемент) — длина в элементах, без "магических чисел". */
    int length = (int)(sizeof(encrypted) / sizeof(encrypted[0]));

    printf("=== MOONLIGHT DECODER v2.0 ===\n");
    printf("Intercept from V. — decrypting...\n\n");
    printf("Length: %d bytes | Key: 0x%02X\n\n", length, XOR_KEY);

    /* +1 байт под '\0': строка в C заканчивается нулевым терминатором. */
    unsigned char decrypted[length + 1];

    unsigned char checksum = 0;
    for (int i = 0; i < length; i++) {
        decrypted[i] = encrypted[i] ^ XOR_KEY; /* тот же ключ отменяет XOR */
        checksum = (unsigned char)(checksum + decrypted[i]);
    }
    decrypted[length] = '\0';

    printf("--- MESSAGE ---\n");
    printf("%s\n", decrypted);
    printf("--- END ---\n\n");

    /* LUNA сверяет целостность: простая аддитивная контрольная сумма. */
    printf("Checksum: 0x%02X\n", checksum);

    return 0;
}
