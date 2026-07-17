/*
 * OPERATION MOONLIGHT — s01e05 "Функции"
 * Каркас. Скопируй в artifacts/decoder.c и закрой TODO.
 *
 *   cp starter.c artifacts/decoder.c
 *   make test
 *
 * Концепт: функции — параметры, возврат, декомпозиция. Вынеси XOR и checksum
 *          из main в отдельные функции и вызывай их (без копипаста).
 */
#include <stdio.h>

#define KEY 0x42

/* TODO 1: определи функцию
 *   void xor_crypt(unsigned char *buf, int len, unsigned char key)
 * — XOR каждого байта buf[i] ключом key, на месте. */

/* TODO 2: определи функцию
 *   unsigned char checksum(const unsigned char *buf, int len)
 * — верни аддитивную сумму байтов. */

int main(void) {
    unsigned char msg[] = {
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
    int len = (int)(sizeof(msg) / sizeof(msg[0]));

    printf("=== MOONLIGHT DECODER — functions ===\n\n");

    /* TODO 3: xor_crypt(msg, len, KEY); затем sum = checksum(msg, len); */
    /* TODO 4: выведи байты (putchar в цикле), затем "\n\nChecksum: 0x%02X\n". */
    /* TODO 5: вызови xor_crypt(...) ещё раз (обратно в шифр) и напечатай:
       "Re-encrypted via same xor_crypt(); first byte: 0x%02X\n", msg[0] */

    (void)len;  /* убери, когда используешь len */
    return 0;
}
