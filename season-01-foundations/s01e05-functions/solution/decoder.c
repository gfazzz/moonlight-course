/*
 * OPERATION MOONLIGHT — s01e05 "Функции"
 * Эталонное решение: decoder.c (один файл, но логика — в функциях)
 *
 * Концепт серии: функция как контракт — параметры, возврат, декомпозиция.
 *                Та же задача декода, но XOR и checksum вынесены в ФУНКЦИИ,
 *                а не скопированы в main. Одна функция — много вызовов.
 * (Разбиение на модули .h/.c — уже в s01e06.)
 */
#include <stdio.h>

#define KEY 0x42

/* Преобразовать буфер XOR-ключом на месте. Самообратимо. */
void xor_crypt(unsigned char *buf, int len, unsigned char key) {
    for (int i = 0; i < len; i++) buf[i] ^= key;
}

/* Аддитивная контрольная сумма буфера. */
unsigned char checksum(const unsigned char *buf, int len) {
    unsigned char sum = 0;
    for (int i = 0; i < len; i++) sum = (unsigned char)(sum + buf[i]);
    return sum;
}

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

    xor_crypt(msg, len, KEY);                 /* вызов 1: расшифровка */
    unsigned char sum = checksum(msg, len);   /* вызов функции checksum */

    for (int i = 0; i < len; i++) putchar(msg[i]);
    printf("\n\nChecksum: 0x%02X\n", sum);

    /* Тот же xor_crypt() снова — доказательство переиспользования: без копипаста. */
    xor_crypt(msg, len, KEY);                 /* вызов 2: обратно в шифр */
    printf("Re-encrypted via same xor_crypt(); first byte: 0x%02X\n", msg[0]);
    return 0;
}
