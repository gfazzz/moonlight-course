/*
 * OPERATION MOONLIGHT — s03e01 "IP-адресация"
 * Эталонное решение: ipinfo.c
 *
 * Концепт серии: IPv4-адрес — это 32-битное число, записанное как четыре октета.
 *                Разбор dotted-quad -> uint32, hex, классификация (private/public/loopback).
 * Задача: прочитать адреса из файла (argv) и для каждого вывести число, hex, класс.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Разобрать "a.b.c.d" в uint32 (host order). -1, если формат/диапазон неверный. */
static int parse_ipv4(const char *s, uint32_t *out) {
    int a, b, c, d; char extra;
    if (sscanf(s, "%d.%d.%d.%d%c", &a, &b, &c, &d, &extra) != 4) return -1; /* ровно 4 октета */
    if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255) return -1;
    *out = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d;
    return 0;
}

static const char *ip_class(uint32_t ip) {
    uint8_t a = (uint8_t)(ip >> 24);
    uint8_t b = (uint8_t)(ip >> 16);
    if (a == 127) return "loopback";
    if (a == 10) return "private";
    if (a == 172 && b >= 16 && b <= 31) return "private";
    if (a == 192 && b == 168) return "private";
    return "public";
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "addresses.txt";
    FILE *f = fopen(path, "r");
    if (f == NULL) { perror("fopen"); return 1; }

    printf("=== IP INFO ===\n");
    int total = 0, valid = 0;
    char line[64];
    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == 0) continue;
        total++;
        uint32_t ip;
        if (parse_ipv4(line, &ip) != 0) { printf("%-18s INVALID\n", line); continue; }
        valid++;
        printf("%-18s uint32=%-10u hex=0x%08X  %s\n", line, ip, ip, ip_class(ip));
    }
    fclose(f);

    printf("\n%d addresses, %d valid\n", total, valid);
    return 0;
}
