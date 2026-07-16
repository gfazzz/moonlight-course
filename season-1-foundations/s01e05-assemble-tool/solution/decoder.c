/*
 * OPERATION MOONLIGHT — s01e05 "Сборка инструмента" (финал Season 1)
 * Эталонное решение: decoder.c
 *
 * Концепт серии: struct/typedef — собрать разрозненные поля в одну сущность,
 *                и полноценная многофайловая сборка через Makefile (.o -> бинарь).
 * Итог: инструмент moonlight_decoder, который декодирует несколько сообщений
 *       и складывает результаты в массив структур.
 */
#include <stdio.h>
#include <string.h>
#include "crypto.h"

#define KEY 0x42
#define TEXT_MAX 128

/* Одна расшифрованная запись — как строка в отчёте. */
typedef struct {
    const char  *label;
    int          len;
    unsigned char sum;
    char         text[TEXT_MAX];
} Message;

/* Функция принимает зашифрованные байты и возвращает заполненную структуру. */
static Message decode(const char *label, const unsigned char *enc, int len, unsigned char key) {
    Message m;
    m.label = label;
    m.len = len;

    unsigned char buf[TEXT_MAX];
    int n = (len < TEXT_MAX - 1) ? len : TEXT_MAX - 1;
    memcpy(buf, enc, (size_t)n);
    xor_crypt(buf, n, key);
    m.sum = checksum(buf, n);

    memcpy(m.text, buf, (size_t)n);
    m.text[n] = '\0';
    return m;
}

int main(void) {
    /* Сообщение 1 — перехват V. из s01e01. */
    unsigned char m1[] = {
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
    /* Сообщение 2 — файл, набранный вручную минуту назад. */
    unsigned char m2[] = {
        0x0B, 0x36, 0x65, 0x31, 0x62, 0x2F, 0x27, 0x6C,
        0x62, 0x0B, 0x62, 0x23, 0x2F, 0x62, 0x31, 0x36,
        0x2B, 0x2E, 0x2E, 0x62, 0x23, 0x2E, 0x2B, 0x34,
        0x27, 0x6C, 0x62, 0x16, 0x30, 0x37, 0x31, 0x36,
        0x62, 0x36, 0x2A, 0x27, 0x62, 0x21, 0x2D, 0x26,
        0x27, 0x6C, 0x62, 0x6F, 0x14, 0x6C
    };

    Message msgs[2];
    msgs[0] = decode("V. intercept",    m1, (int)sizeof(m1), KEY);
    msgs[1] = decode("hand-typed file", m2, (int)sizeof(m2), KEY);
    int count = 2;

    printf("=== MOONLIGHT DECODER v2.0 — final build ===\n\n");
    printf("# | %-16s | bytes | checksum\n", "label");
    printf("--+------------------+-------+---------\n");
    for (int i = 0; i < count; i++)
        printf("%d | %-16s | %5d | 0x%02X\n", i + 1, msgs[i].label, msgs[i].len, msgs[i].sum);
    printf("\n");

    for (int i = 0; i < count; i++)
        printf("--- %s ---\n%s\n\n", msgs[i].label, msgs[i].text);

    printf("[LUNA] ...he's alive.\n");
    return 0;
}
