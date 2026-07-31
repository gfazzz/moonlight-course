/*
 * OPERATION MOONLIGHT — s03e02 "Подсети и CIDR"
 * Эталонное решение: subnet.c
 *
 * Концепт серии: маска /N делит адрес на «сеть» и «хост». network = ip & mask,
 *                broadcast = network | ~mask; «свой ли адрес» — сравнением сетей.
 * Задача: по паре адрес/CIDR вычислить маску, сеть, broadcast и диапазон хостов.
 */
#include <stdio.h>
#include <stdint.h>

static void fmt(uint32_t ip, char *buf) {
    sprintf(buf, "%u.%u.%u.%u",
            (ip >> 24) & 255, (ip >> 16) & 255, (ip >> 8) & 255, ip & 255);
}

int main(void) {
    struct { uint32_t ip; int cidr; } cases[] = {
        {0xC0A80164, 24},   /* 192.168.1.100/24 */
        {0x0A000005, 8},    /* 10.0.0.5/8       */
        {0xAC100309, 20},   /* 172.16.3.9/20    */
    };

    printf("=== SUBNETS ===\n");
    for (int i = 0; i < 3; i++) {
        uint32_t ip = cases[i].ip;
        int c = cases[i].cidr;
        uint32_t mask = (c == 0) ? 0u : (0xFFFFFFFFu << (32 - c));
        uint32_t net = ip & mask;
        uint32_t bcast = net | ~mask;
        char a[16], m[16], n[16], b[16], f[16], l[16];
        fmt(ip, a); fmt(mask, m); fmt(net, n); fmt(bcast, b);
        fmt(net + 1, f); fmt(bcast - 1, l);
        unsigned hosts = (bcast - net > 1) ? (bcast - net - 1) : 0;
        printf("%s/%d\n", a, c);
        printf("  mask=%s network=%s broadcast=%s\n", m, n, b);
        printf("  usable %s .. %s (%u hosts)\n", f, l, hosts);
    }

    /* «Свой ли адрес»: сравниваем сети под общей маской. */
    uint32_t subnet = 0xC0A80100u, mask24 = 0xFFFFFF00u;  /* 192.168.1.0/24 */
    uint32_t h1 = 0xC0A801FEu;  /* 192.168.1.254 — внутри */
    uint32_t h2 = 0xC0A80264u;  /* 192.168.2.100 — снаружи */
    printf("\nis 192.168.1.254 in 192.168.1.0/24? %s\n", ((h1 & mask24) == subnet) ? "yes" : "no");
    printf("is 192.168.2.100 in 192.168.1.0/24? %s\n", ((h2 & mask24) == subnet) ? "yes" : "no");
    return 0;
}
