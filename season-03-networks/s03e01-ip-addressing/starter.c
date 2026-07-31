/*
 * OPERATION MOONLIGHT — s03e01 "IP-адресация"
 * Каркас. Скопируй в artifacts/ipinfo.c и закрой TODO.
 *
 *   cp starter.c artifacts/ipinfo.c
 *   make test        # запустит: ipinfo ../addresses.txt
 *
 * Концепт: IPv4 — 32-битное число из 4 октетов. dotted-quad -> uint32, hex, класс.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* TODO 1: parse_ipv4("a.b.c.d", &out) — sscanf 4 октета, проверь диапазон 0..255,
   собери uint32 = a<<24 | b<<16 | c<<8 | d. Верни -1 при ошибке. */

/* TODO 2: ip_class(ip) — 127.x = "loopback"; 10.x, 172.16-31.x, 192.168.x = "private";
   иначе "public". */

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
        /* TODO 3: разобрать line; если невалиден — "%-18s INVALID\n"; иначе
           "%-18s uint32=%-10u hex=0x%08X  %s\n" (адрес, число, hex, класс). */
    }
    fclose(f);

    printf("\n%d addresses, %d valid\n", total, valid);
    return 0;
}
