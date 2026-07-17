/*
 * OPERATION MOONLIGHT — s03e02 "Подсети и CIDR"
 * Каркас. Скопируй в artifacts/subnet.c и закрой TODO.
 *
 *   cp starter.c artifacts/subnet.c
 *   make test
 *
 * Концепт: маска /N делит адрес на сеть и хост.
 *   mask = 0xFFFFFFFF << (32 - N);  network = ip & mask;  broadcast = network | ~mask.
 */
#include <stdio.h>
#include <stdint.h>

static void fmt(uint32_t ip, char *buf) {
    sprintf(buf, "%u.%u.%u.%u",
            (ip >> 24) & 255, (ip >> 16) & 255, (ip >> 8) & 255, ip & 255);
}

int main(void) {
    struct { uint32_t ip; int cidr; } cases[] = {
        {0xC0A80164, 24}, {0x0A000005, 8}, {0xAC100309, 20},
    };

    printf("=== SUBNETS ===\n");
    for (int i = 0; i < 3; i++) {
        uint32_t ip = cases[i].ip; int c = cases[i].cidr;
        /* TODO 1: mask = (c==0)?0:(0xFFFFFFFFu << (32 - c));
           TODO 2: net = ip & mask;  bcast = net | ~mask;
           TODO 3: hosts = (bcast-net > 1) ? (bcast-net-1) : 0;
           TODO 4: печать (формат — см. tests/expected.txt):
             "<ip>/<c>"
             "  mask=<mask> network=<net> broadcast=<bcast>"
             "  usable <net+1> .. <bcast-1> (<hosts> hosts)"  */
        (void)ip; (void)c;
    }

    /* TODO 5: containment — 192.168.1.254 и 192.168.2.100 в 192.168.1.0/24 (mask /24).
       Печать "is <ip> in 192.168.1.0/24? yes|no" через сравнение (h & mask) == subnet. */
    return 0;
}
