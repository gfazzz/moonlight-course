/*
 * OPERATION MOONLIGHT — s10e05 "Защита входных данных"
 * Эталонное решение: harden.c
 *
 * Концепт серии: конвейер быстрый (s10e04), но принимает пакеты ИЗ ЭФИРА.
 *                Всё, что пришло снаружи, — враждебно, пока не доказано обратное.
 *                Четыре классические дыры, каждая из которых валила реальные системы:
 *                  1) длина из пакета больше буфера      -> переполнение буфера;
 *                  2) арифметика над длинами             -> целочисленное переполнение;
 *                  3) строка без нуля                     -> чтение за границей;
 *                  4) индекс/смещение из пакета           -> выход за массив.
 *                Защита строится на ОДНОМ принципе: проверять ДО использования,
 *                а не после, и сравнивать так, чтобы сама проверка не переполнилась.
 * Задача: разбор недоверенного пакета, устойчивый ко всем четырём атакам.
 *
 * Проверяется санитайзерами: тест собирает и с -fsanitize=address,undefined.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAGIC_HI   0x4D      /* 'M' */
#define MAGIC_LO   0x4C      /* 'L' — MoonLight */
#define MAXPAYLOAD 64
#define NCHANNELS  8

/* Формат пакета: [magic:2][channel:1][len:1][payload:len][crc:1] */
typedef struct {
    const unsigned char *raw;
    size_t size;
} Packet;

static long channel_totals[NCHANNELS];

/* Разбор с проверками. Возврат: 0 — принят, иначе код отказа. */
static int parse_packet(Packet p, char *out, size_t out_cap, const char **why) {
    /* 1. Пакет обязан вмещать заголовок. Проверяем ДО любого обращения. */
    if (p.size < 5) { *why = "слишком короткий пакет"; return 1; }

    if (p.raw[0] != MAGIC_HI || p.raw[1] != MAGIC_LO) { *why = "неверная сигнатура"; return 2; }

    unsigned char channel = p.raw[2];
    unsigned char len     = p.raw[3];

    /* 2. Канал — индекс в массив. Из пакета пришло произвольное число. */
    if (channel >= NCHANNELS) { *why = "канал вне диапазона"; return 3; }

    /* 3. Заявленная длина обязана совпадать с реальным размером.
          Сравнение написано так, чтобы НЕ переполниться: не (len + 5 > size),
          а (size - 5 < len) — вычитание безопасно, size >= 5 уже проверено. */
    if (p.size - 5 < (size_t)len) { *why = "длина больше пакета"; return 4; }

    /* 4. И обязана влезать в буфер получателя (с местом под завершающий ноль). */
    if ((size_t)len >= out_cap) { *why = "длина больше буфера"; return 5; }

    memcpy(out, p.raw + 4, len);
    out[len] = '\0';                 /* завершаем строку САМИ, не доверяя источнику */

    unsigned char crc = 0;
    for (size_t i = 0; i < (size_t)len + 4; i++) crc ^= p.raw[i];
    if (crc != p.raw[4 + len]) { *why = "контрольная сумма не сошлась"; return 6; }

    channel_totals[channel] += len;
    *why = "принят";
    return 0;
}

/* Собрать корректный пакет (для проверки положительного случая). */
static size_t build(unsigned char *buf, unsigned char ch, const char *text) {
    size_t len = strlen(text);
    buf[0] = MAGIC_HI; buf[1] = MAGIC_LO; buf[2] = ch; buf[3] = (unsigned char)len;
    memcpy(buf + 4, text, len);
    unsigned char crc = 0;
    for (size_t i = 0; i < len + 4; i++) crc ^= buf[i];
    buf[4 + len] = crc;
    return len + 5;
}

static void check(const char *title, Packet p) {
    char out[MAXPAYLOAD];
    const char *why = "";
    int rc = parse_packet(p, out, sizeof out, &why);
    printf("  %s\n      -> %s (%s)\n", title, rc == 0 ? "ПРИНЯТ" : "отвергнут", why);
}

int main(void) {
    unsigned char buf[128];
    printf("=== защита разбора недоверенного пакета ===\n");
    printf("формат: [magic:2][channel:1][len:1][payload][crc:1], буфер %d байт\n\n", MAXPAYLOAD);

    printf("--- корректные пакеты ---\n");
    size_t n = build(buf, 3, "TELEMETRY OK");
    check("нормальный пакет", (Packet){buf, n});

    n = build(buf, 0, "");
    check("пустая полезная нагрузка", (Packet){buf, n});

    printf("\n--- атаки ---\n");

    /* Атака 1: длина больше, чем реально пришло. */
    n = build(buf, 1, "short");
    buf[3] = 200;
    check("длина 200 при пакете в 10 байт", (Packet){buf, n});

    /* Атака 2: длина больше буфера получателя. */
    unsigned char big[300];
    char longtext[200];
    memset(longtext, 'A', sizeof longtext - 1);
    longtext[sizeof longtext - 1] = '\0';
    size_t bn = build(big, 2, longtext);
    check("полезная нагрузка 199 байт в буфер 64", (Packet){big, bn});

    /* Атака 3: номер канала вне массива. */
    n = build(buf, 3, "data");
    buf[2] = 250;
    check("канал 250 при 8 каналах", (Packet){buf, n});

    /* Атака 4: обрезанный пакет — заголовок не помещается. */
    n = build(buf, 4, "data");
    check("обрезанный пакет (3 байта)", (Packet){buf, 3});

    /* Атака 5: подделка сигнатуры. */
    n = build(buf, 5, "data");
    buf[0] = 0x00;
    check("неверная сигнатура", (Packet){buf, n});

    /* Атака 6: испорченная контрольная сумма. */
    n = build(buf, 6, "payload");
    buf[n - 1] ^= 0xFF;
    check("испорченная контрольная сумма", (Packet){buf, n});

    /* Атака 7: пакет без завершающего нуля в данных. */
    n = build(buf, 7, "no-nul-here");
    check("данные без нуля внутри (ноль дописываем сами)", (Packet){buf, n});

    printf("\n--- накопленная статистика по каналам ---\n");
    long total = 0;
    for (int i = 0; i < NCHANNELS; i++) {
        total += channel_totals[i];
        if (channel_totals[i]) printf("  канал %d: %ld байт\n", i, channel_totals[i]);
    }
    printf("  всего принято: %ld байт\n", total);

    printf("\nпринцип: проверять ДО использования и так, чтобы сама проверка\n");
    printf("не могла переполниться. Ни один отказ не должен ронять систему.\n");
    return 0;
}
