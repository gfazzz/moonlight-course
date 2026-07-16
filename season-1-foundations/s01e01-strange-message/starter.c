/*
 * OPERATION MOONLIGHT — s01e01 "Странное сообщение"
 * Каркас задачи. Скопируй в artifacts/decoder.c и закрой TODO.
 *
 *   cp starter.c artifacts/decoder.c
 *   # правишь artifacts/decoder.c, затем:
 *   make test
 *
 * Один концепт: данные — это байты, XOR — обратимый ключ (A ^ K ^ K = A).
 * Одна задача: расшифровать перехват от V. ключом 0x42.
 */

#include <stdio.h>

#define XOR_KEY 0x42

int main(void) {
    /* Перехват от V., 75 байт. Не трогай — это входные данные. */
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

    /* TODO 1: длина массива через sizeof (без магических чисел). */
    /* int length = ... ; */

    printf("=== MOONLIGHT DECODER v2.0 ===\n");
    printf("Intercept from V. — decrypting...\n\n");
    /* TODO 2: напечатай строку "Length: <N> bytes | Key: 0x42" + пустая строка.
       Подсказка по формату: printf("Length: %d bytes | Key: 0x%02X\n\n", length, XOR_KEY); */

    /* TODO 3: массив под расшифровку. Помни про +1 байт для '\0'. */
    /* unsigned char decrypted[length + 1]; */

    /* TODO 4: в цикле XOR каждого байта ключом; попутно копи checksum (сумму байтов). */
    /* unsigned char checksum = 0; */
    /* for (int i = 0; i < length; i++) { ... } */

    /* TODO 5: поставь '\0' в конец, чтобы это стало строкой C. */

    /* TODO 6: выведи блок сообщения:
         --- MESSAGE ---
         <текст>
         --- END ---
       и строку "Checksum: 0x%02X". */

    return 0;
}
