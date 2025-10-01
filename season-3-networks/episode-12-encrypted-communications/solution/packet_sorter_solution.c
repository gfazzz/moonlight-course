/*
 * packet_sorter.c
 * Промежуточная задача для Episode 12: Session Reconstruction
 * Цель: Сортировка TCP пакетов по sequence number
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t seq;
    char payload[256];
    int payload_len;
} Packet;

int compare_packets(const void *a, const void *b) {
    Packet *pa = (Packet*)a;
    Packet *pb = (Packet*)b;
    return (pa->seq > pb->seq) - (pa->seq < pb->seq);
}

int main() {
    printf("═══════════════════════════════════════\n");
    printf("    PACKET SORTER\n");
    printf("═══════════════════════════════════════\n\n");

    // Sample packets (out of order)
    Packet packets[] = {
        {1065, "Packet 3", 8},
        {1000, "Packet 1", 8},
        {1165, "Packet 4", 8},
        {1015, "Packet 2", 8}
    };
    int packet_count = 4;

    printf("Before sorting:\n");
    for (int i = 0; i < packet_count; i++) {
        printf("  SEQ=%u: %s\n", packets[i].seq, packets[i].payload);
    }

    // Sort by sequence number
    qsort(packets, packet_count, sizeof(Packet), compare_packets);

    printf("\nAfter sorting:\n");
    for (int i = 0; i < packet_count; i++) {
        printf("  SEQ=%u: %s\n", packets[i].seq, packets[i].payload);
    }

    // Verify ordering
    int gaps = 0;
    uint32_t expected_seq = packets[0].seq + packets[0].payload_len;
    for (int i = 1; i < packet_count; i++) {
        if (packets[i].seq != expected_seq) {
            printf("\n❌ Gap detected: expected %u, got %u\n", expected_seq, packets[i].seq);
            gaps++;
        }
        expected_seq = packets[i].seq + packets[i].payload_len;
    }

    if (gaps == 0) {
        printf("\n✅ All packets in order, no gaps!\n");
    }

    return 0;
}

