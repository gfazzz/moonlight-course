/**
 * Episode 11: Packet Analysis - SOLUTION
 * Raw packet sniffer with protocol parsing
 * 
 * ⚠️ Requires root/sudo privileges!
 * 
 * Features:
 * - Raw socket packet capture
 * - IP header parsing
 * - TCP/UDP header parsing
 * - Hex dump of payload
 * - Packet statistics
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <signal.h>

// Statistics counters
typedef struct {
    unsigned long total_packets;
    unsigned long tcp_packets;
    unsigned long udp_packets;
    unsigned long icmp_packets;
    unsigned long other_packets;
    unsigned long total_bytes;
} PacketStats;

PacketStats stats = {0};
int running = 1;

// Signal handler for graceful exit
void signal_handler(int sig) {
    running = 0;
}

// Print hex dump of data
void print_hex_dump(unsigned char *data, int size, int max_bytes) {
    int bytes_to_print = (size < max_bytes) ? size : max_bytes;
    
    printf("    Payload (%d bytes):\n", size);
    printf("    ");
    
    for (int i = 0; i < bytes_to_print; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n    ");
        }
    }
    
    if (size > max_bytes) {
        printf("... (truncated)");
    }
    
    printf("\n");
}

// Parse and print IP header
void parse_ip_header(unsigned char *buffer, int size) {
    struct iphdr *ip = (struct iphdr *)buffer;
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;
    
    printf("\n[+] IP Header:\n");
    printf("    Version: %d\n", ip->version);
    printf("    Header Length: %d bytes\n", ip->ihl * 4);
    printf("    Total Length: %d bytes\n", ntohs(ip->tot_len));
    printf("    Protocol: %d ", ip->protocol);
    
    switch (ip->protocol) {
        case IPPROTO_TCP:
            printf("(TCP)\n");
            stats.tcp_packets++;
            break;
        case IPPROTO_UDP:
            printf("(UDP)\n");
            stats.udp_packets++;
            break;
        case IPPROTO_ICMP:
            printf("(ICMP)\n");
            stats.icmp_packets++;
            break;
        default:
            printf("(Other)\n");
            stats.other_packets++;
            break;
    }
    
    printf("    Source IP: %s\n", inet_ntoa(source.sin_addr));
    printf("    Dest IP: %s\n", inet_ntoa(dest.sin_addr));
}

// Parse and print TCP header
void parse_tcp_header(unsigned char *buffer, int size) {
    struct iphdr *ip = (struct iphdr *)buffer;
    unsigned short ip_header_len = ip->ihl * 4;
    
    struct tcphdr *tcp = (struct tcphdr *)(buffer + ip_header_len);
    
    printf("\n[+] TCP Header:\n");
    printf("    Source Port: %d\n", ntohs(tcp->source));
    printf("    Dest Port: %d\n", ntohs(tcp->dest));
    printf("    Sequence: %u\n", ntohl(tcp->seq));
    printf("    Ack Sequence: %u\n", ntohl(tcp->ack_seq));
    printf("    Flags: ");
    if (tcp->syn) printf("SYN ");
    if (tcp->ack) printf("ACK ");
    if (tcp->fin) printf("FIN ");
    if (tcp->rst) printf("RST ");
    if (tcp->psh) printf("PSH ");
    printf("\n");
    
    // Print payload
    int header_size = ip_header_len + tcp->doff * 4;
    int data_size = size - header_size;
    
    if (data_size > 0) {
        unsigned char *data = buffer + header_size;
        print_hex_dump(data, data_size, 64);
    }
}

// Parse and print UDP header
void parse_udp_header(unsigned char *buffer, int size) {
    struct iphdr *ip = (struct iphdr *)buffer;
    unsigned short ip_header_len = ip->ihl * 4;
    
    struct udphdr *udp = (struct udphdr *)(buffer + ip_header_len);
    
    printf("\n[+] UDP Header:\n");
    printf("    Source Port: %d\n", ntohs(udp->source));
    printf("    Dest Port: %d\n", ntohs(udp->dest));
    printf("    Length: %d\n", ntohs(udp->len));
    
    // Print payload
    int header_size = ip_header_len + sizeof(struct udphdr);
    int data_size = size - header_size;
    
    if (data_size > 0) {
        unsigned char *data = buffer + header_size;
        print_hex_dump(data, data_size, 64);
    }
}

// Print statistics
void print_statistics() {
    printf("\n");
    printf("═══════════════════════════════════════\n");
    printf("  Packet Capture Statistics\n");
    printf("═══════════════════════════════════════\n");
    printf("Total packets: %lu\n", stats.total_packets);
    printf("  TCP:  %lu\n", stats.tcp_packets);
    printf("  UDP:  %lu\n", stats.udp_packets);
    printf("  ICMP: %lu\n", stats.icmp_packets);
    printf("  Other: %lu\n", stats.other_packets);
    printf("Total bytes: %lu\n", stats.total_bytes);
    printf("═══════════════════════════════════════\n");
}

int main() {
    printf("═══════════════════════════════════════\n");
    printf("  MOONLIGHT Packet Sniffer\n");
    printf("═══════════════════════════════════════\n");
    printf("⚠️  Requires root privileges!\n");
    printf("Press Ctrl+C to stop...\n\n");
    
    // Check if running as root
    if (getuid() != 0) {
        fprintf(stderr, "Error: This program requires root privileges.\n");
        fprintf(stderr, "Please run with: sudo %s\n", "packet_sniffer");
        return 1;
    }
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    
    // Create raw socket for IP protocol
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        fprintf(stderr, "Note: Raw sockets require root privileges\n");
        return 1;
    }
    
    // Also capture UDP packets
    int sockfd_udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd_udp < 0) {
        perror("UDP socket creation failed");
        close(sockfd);
        return 1;
    }
    
    printf("✓ Raw sockets created successfully\n");
    printf("✓ Starting packet capture...\n\n");
    
    unsigned char buffer[65536];
    
    // Main capture loop
    while (running) {
        struct sockaddr saddr;
        socklen_t saddr_len = sizeof(saddr);
        
        // Try to receive from TCP socket
        int data_size = recvfrom(sockfd, buffer, sizeof(buffer), MSG_DONTWAIT, 
                                 &saddr, &saddr_len);
        
        if (data_size < 0) {
            // Try UDP socket
            data_size = recvfrom(sockfd_udp, buffer, sizeof(buffer), MSG_DONTWAIT, 
                                &saddr, &saddr_len);
        }
        
        if (data_size > 0) {
            stats.total_packets++;
            stats.total_bytes += data_size;
            
            printf("════════════════════════════════════════════════════\n");
            printf("Packet #%lu (Size: %d bytes)\n", stats.total_packets, data_size);
            
            // Parse IP header
            struct iphdr *ip = (struct iphdr *)buffer;
            parse_ip_header(buffer, data_size);
            
            // Parse protocol-specific headers
            switch (ip->protocol) {
                case IPPROTO_TCP:
                    parse_tcp_header(buffer, data_size);
                    break;
                case IPPROTO_UDP:
                    parse_udp_header(buffer, data_size);
                    break;
                case IPPROTO_ICMP:
                    printf("\n[+] ICMP Packet\n");
                    break;
            }
            
            printf("════════════════════════════════════════════════════\n");
        } else {
            usleep(10000);  // Sleep 10ms if no packets
        }
    }
    
    // Cleanup
    close(sockfd);
    close(sockfd_udp);
    
    // Print final statistics
    print_statistics();
    
    return 0;
}

