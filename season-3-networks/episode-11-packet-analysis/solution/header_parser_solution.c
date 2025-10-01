/*
 * header_parser.c
 * 
 * Промежуточная задача для Episode 11: Packet Analysis
 * 
 * Цель: Парсинг IP и TCP заголовков из сырых пакетов
 * 
 * Концепции:
 * - struct iphdr — IP заголовок
 * - struct tcphdr — TCP заголовок
 * - Network byte order (ntohs/ntohl)
 * - IP адреса (inet_ntop)
 * - TCP флаги (SYN, ACK, FIN, PSH, RST, URG)
 * 
 * Задача:
 * 1. Читать сырые пакеты из файла (hex dump)
 * 2. Парсить IP заголовок
 * 3. Парсить TCP заголовок
 * 4. Выводить информацию о пакете
 * 
 * Компиляция:
 *   gcc -Wall -Wextra -std=c11 header_parser_solution.c -o header_parser
 * 
 * Использование:
 *   ./header_parser <packet_file.bin>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

// Кроссплатформенная структура TCP заголовка
struct tcp_header {
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t res1:4;
    uint8_t doff:4;
    uint8_t fin:1;
    uint8_t syn:1;
    uint8_t rst:1;
    uint8_t psh:1;
    uint8_t ack:1;
    uint8_t urg:1;
    uint8_t res2:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t doff:4;
    uint8_t res1:4;
    uint8_t res2:2;
    uint8_t urg:1;
    uint8_t ack:1;
    uint8_t psh:1;
    uint8_t rst:1;
    uint8_t syn:1;
    uint8_t fin:1;
#endif
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};

// Флаги TCP
const char* get_tcp_flags(struct tcp_header *tcp) {
    static char flags[64];
    flags[0] = '\0';
    
    if (tcp->fin) strcat(flags, "FIN ");
    if (tcp->syn) strcat(flags, "SYN ");
    if (tcp->rst) strcat(flags, "RST ");
    if (tcp->psh) strcat(flags, "PSH ");
    if (tcp->ack) strcat(flags, "ACK ");
    if (tcp->urg) strcat(flags, "URG ");
    
    if (flags[0] == '\0') {
        return "NONE";
    }
    
    // Remove trailing space
    flags[strlen(flags) - 1] = '\0';
    return flags;
}

// Парсинг IP заголовка
void parse_ip_header(unsigned char *packet) {
    struct iphdr *ip = (struct iphdr*)packet;
    
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);
    
    printf("═══════════════════════════════════════\n");
    printf("    IP HEADER\n");
    printf("═══════════════════════════════════════\n\n");
    
    printf("Version:        %u\n", ip->version);
    printf("Header Length:  %u bytes (%u * 4)\n", ip->ihl * 4, ip->ihl);
    printf("Total Length:   %u bytes\n", ntohs(ip->tot_len));
    printf("Protocol:       %u ", ip->protocol);
    
    switch (ip->protocol) {
        case 1:  printf("(ICMP)\n"); break;
        case 6:  printf("(TCP)\n"); break;
        case 17: printf("(UDP)\n"); break;
        default: printf("(Unknown)\n"); break;
    }
    
    printf("TTL:            %u\n", ip->ttl);
    printf("Source IP:      %s\n", src_ip);
    printf("Dest IP:        %s\n", dst_ip);
    
    printf("\n");
}

// Парсинг TCP заголовка
void parse_tcp_header(unsigned char *packet) {
    struct iphdr *ip = (struct iphdr*)packet;
    int ip_header_len = ip->ihl * 4;
    
    struct tcp_header *tcp = (struct tcp_header*)(packet + ip_header_len);
    
    printf("═══════════════════════════════════════\n");
    printf("    TCP HEADER\n");
    printf("═══════════════════════════════════════\n\n");
    
    printf("Source Port:    %u\n", ntohs(tcp->source));
    printf("Dest Port:      %u\n", ntohs(tcp->dest));
    printf("Sequence:       %u (0x%08X)\n", ntohl(tcp->seq), ntohl(tcp->seq));
    printf("Ack Sequence:   %u (0x%08X)\n", ntohl(tcp->ack_seq), ntohl(tcp->ack_seq));
    printf("Header Length:  %u bytes (%u * 4)\n", tcp->doff * 4, tcp->doff);
    printf("Flags:          %s\n", get_tcp_flags(tcp));
    printf("Window Size:    %u\n", ntohs(tcp->window));
    printf("Checksum:       0x%04X\n", ntohs(tcp->check));
    
    // Payload
    int tcp_header_len = tcp->doff * 4;
    int total_header_len = ip_header_len + tcp_header_len;
    int payload_len = ntohs(ip->tot_len) - total_header_len;
    
    printf("Payload Length: %d bytes\n", payload_len);
    
    if (payload_len > 0) {
        unsigned char *payload = packet + total_header_len;
        
        printf("\nPayload (first %d bytes):\n", payload_len > 64 ? 64 : payload_len);
        printf("Hex: ");
        for (int i = 0; i < payload_len && i < 64; i++) {
            printf("%02X ", payload[i]);
            if ((i + 1) % 16 == 0) printf("\n     ");
        }
        printf("\n");
        
        printf("ASCII: ");
        for (int i = 0; i < payload_len && i < 64; i++) {
            if (payload[i] >= 32 && payload[i] <= 126) {
                printf("%c", payload[i]);
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    
    printf("\n");
}

// Чтение пакета из файла (бинарный формат)
int read_packet_from_file(const char *filename, unsigned char *buffer, int max_len) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return -1;
    }
    
    int bytes_read = fread(buffer, 1, max_len, f);
    fclose(f);
    
    return bytes_read;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <packet_file.bin>\n", argv[0]);
        fprintf(stderr, "Example: %s packet_sample.bin\n", argv[0]);
        return 1;
    }
    
    char *packet_file = argv[1];
    unsigned char packet_buffer[4096];
    
    printf("═══════════════════════════════════════\n");
    printf("    HEADER PARSER\n");
    printf("═══════════════════════════════════════\n\n");
    
    printf("Reading packet from: %s\n\n", packet_file);
    
    // Читаем пакет
    int packet_len = read_packet_from_file(packet_file, packet_buffer, sizeof(packet_buffer));
    
    if (packet_len < 0) {
        fprintf(stderr, "Failed to read packet file\n");
        return 1;
    }
    
    if (packet_len < 20) {
        fprintf(stderr, "Packet too small (%d bytes), need at least IP header (20 bytes)\n", packet_len);
        return 1;
    }
    
    printf("Packet size: %d bytes\n\n", packet_len);
    
    // Проверка IP версии
    struct iphdr *ip = (struct iphdr*)packet_buffer;
    if (ip->version != 4) {
        fprintf(stderr, "Not an IPv4 packet (version=%u)\n", ip->version);
        return 1;
    }
    
    // Парсим IP заголовок
    parse_ip_header(packet_buffer);
    
    // Парсим TCP заголовок (если это TCP)
    if (ip->protocol == 6) {  // TCP
        int ip_header_len = ip->ihl * 4;
        
        if (packet_len < ip_header_len + 20) {
            fprintf(stderr, "Packet too small for TCP header\n");
            return 1;
        }
        
        parse_tcp_header(packet_buffer);
    } else {
        printf("⚠️  Not a TCP packet (protocol=%u)\n", ip->protocol);
        printf("    Only TCP parsing is supported in this version.\n");
    }
    
    printf("═══════════════════════════════════════\n");
    printf("✅ Parsing complete!\n");
    printf("═══════════════════════════════════════\n");
    
    return 0;
}

