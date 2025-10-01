/**
 * Episode 12: Encrypted Communications - SOLUTION
 * Communication Interceptor - FINAL MISSION Season 3
 * 
 * Integrates:
 * - Packet capture (Episode 11)
 * - TCP session reconstruction
 * - XOR decryption (Episode 01)
 * - Stream analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX_PACKETS 10000
#define BUFFER_SIZE 65536

// TCP Packet structure
typedef struct {
    uint32_t seq_num;
    uint32_t data_len;
    unsigned char data[2048];
    struct in_addr src_ip;
    struct in_addr dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
} TCPPacket;

TCPPacket captured_packets[MAX_PACKETS];
int packet_count = 0;
int running = 1;

// Signal handler
void signal_handler(int sig) {
    running = 0;
}

// XOR decryption from Episode 01
void xor_decrypt(unsigned char *data, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// Frequency analysis to find XOR key
unsigned char find_xor_key(unsigned char *data, size_t len) {
    int freq[256] = {0};
    
    // Try all possible keys
    for (int key = 0; key < 256; key++) {
        int space_count = 0;
        int printable_count = 0;
        
        for (size_t i = 0; i < len && i < 1000; i++) {
            unsigned char c = data[i] ^ key;
            if (c == ' ') space_count++;
            if (c >= 32 && c <= 126) printable_count++;
        }
        
        freq[key] = printable_count * 100 + space_count * 10;
    }
    
    // Find best key
    int best_key = 0;
    int best_score = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > best_score) {
            best_score = freq[i];
            best_key = i;
        }
    }
    
    return (unsigned char)best_key;
}

// Capture TCP packet
int capture_tcp_packet(int sockfd, TCPPacket *pkt) {
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);
    
    int data_size = recvfrom(sockfd, buffer, sizeof(buffer), 0, &saddr, &saddr_len);
    if (data_size < 0) {
        return -1;
    }
    
    // Parse IP header
    struct iphdr *ip = (struct iphdr *)buffer;
    if (ip->protocol != IPPROTO_TCP) {
        return 0;  // Not TCP
    }
    
    unsigned short ip_header_len = ip->ihl * 4;
    struct tcphdr *tcp = (struct tcphdr *)(buffer + ip_header_len);
    
    // Store packet info
    pkt->src_ip.s_addr = ip->saddr;
    pkt->dst_ip.s_addr = ip->daddr;
    pkt->src_port = ntohs(tcp->source);
    pkt->dst_port = ntohs(tcp->dest);
    pkt->seq_num = ntohl(tcp->seq);
    
    // Extract payload
    int header_size = ip_header_len + tcp->doff * 4;
    pkt->data_len = data_size - header_size;
    
    if (pkt->data_len > 0 && pkt->data_len < sizeof(pkt->data)) {
        memcpy(pkt->data, buffer + header_size, pkt->data_len);
        return 1;  // Valid TCP packet with data
    }
    
    return 0;  // No payload
}

// Filter packets by IP and port
int filter_packet(TCPPacket *pkt, const char *target_ip, int target_port) {
    char src_ip_str[INET_ADDRSTRLEN];
    char dst_ip_str[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &pkt->src_ip, src_ip_str, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &pkt->dst_ip, dst_ip_str, INET_ADDRSTRLEN);
    
    // Check if packet matches target
    if (target_ip) {
        if (strcmp(src_ip_str, target_ip) != 0 && strcmp(dst_ip_str, target_ip) != 0) {
            return 0;
        }
    }
    
    if (target_port > 0) {
        if (pkt->src_port != target_port && pkt->dst_port != target_port) {
            return 0;
        }
    }
    
    return 1;
}

// Compare function for qsort
int compare_packets(const void *a, const void *b) {
    TCPPacket *pkt_a = (TCPPacket *)a;
    TCPPacket *pkt_b = (TCPPacket *)b;
    
    if (pkt_a->seq_num < pkt_b->seq_num) return -1;
    if (pkt_a->seq_num > pkt_b->seq_num) return 1;
    return 0;
}

// Reconstruct TCP stream
unsigned char* reconstruct_stream(TCPPacket *packets, int count, size_t *total_len) {
    // Sort packets by sequence number
    qsort(packets, count, sizeof(TCPPacket), compare_packets);
    
    // Calculate total length
    *total_len = 0;
    for (int i = 0; i < count; i++) {
        *total_len += packets[i].data_len;
    }
    
    // Allocate buffer
    unsigned char *stream = malloc(*total_len);
    if (!stream) {
        return NULL;
    }
    
    // Concatenate payloads
    size_t offset = 0;
    for (int i = 0; i < count; i++) {
        memcpy(stream + offset, packets[i].data, packets[i].data_len);
        offset += packets[i].data_len;
    }
    
    return stream;
}

// Save stream to file
void save_stream(const char *filename, unsigned char *data, size_t len) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to save stream");
        return;
    }
    
    fwrite(data, 1, len, f);
    fclose(f);
    
    printf("✓ Stream saved to %s (%zu bytes)\n", filename, len);
}

int main(int argc, char *argv[]) {
    printf("═══════════════════════════════════════════════\n");
    printf("  MOONLIGHT COMMUNICATION INTERCEPTOR\n");
    printf("  Final Mission: Season 3\n");
    printf("═══════════════════════════════════════════════\n");
    printf("⚠️  Requires root privileges!\n\n");
    
    // Check root
    if (getuid() != 0) {
        fprintf(stderr, "Error: This program requires root privileges.\n");
        fprintf(stderr, "Please run with: sudo %s\n", argv[0]);
        return 1;
    }
    
    // Parse arguments (optional filtering)
    const char *target_ip = NULL;
    int target_port = 0;
    
    if (argc >= 2) {
        target_ip = argv[1];
    }
    if (argc >= 3) {
        target_port = atoi(argv[2]);
    }
    
    if (target_ip) {
        printf("Filtering: IP=%s", target_ip);
        if (target_port) {
            printf(", Port=%d", target_port);
        }
        printf("\n");
    }
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    
    // Create raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    printf("✓ Raw socket created\n");
    printf("✓ Starting packet capture... (Press Ctrl+C to stop)\n\n");
    
    // Capture loop
    while (running && packet_count < MAX_PACKETS) {
        TCPPacket pkt;
        int result = capture_tcp_packet(sockfd, &pkt);
        
        if (result > 0) {
            // Filter packet if needed
            if (!target_ip || filter_packet(&pkt, target_ip, target_port)) {
                memcpy(&captured_packets[packet_count], &pkt, sizeof(TCPPacket));
                packet_count++;
                
                char src_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &pkt.src_ip, src_ip, INET_ADDRSTRLEN);
                
                printf("[%d] %s:%d -> seq=%u len=%u\n", 
                       packet_count, src_ip, pkt.src_port, 
                       pkt.seq_num, pkt.data_len);
            }
        }
    }
    
    close(sockfd);
    
    printf("\n═══════════════════════════════════════════════\n");
    printf("Capture complete: %d packets captured\n", packet_count);
    printf("═══════════════════════════════════════════════\n\n");
    
    if (packet_count == 0) {
        printf("No packets captured. Exiting.\n");
        return 0;
    }
    
    // Reconstruct TCP stream
    printf("Reconstructing TCP stream...\n");
    size_t stream_len;
    unsigned char *stream = reconstruct_stream(captured_packets, packet_count, &stream_len);
    
    if (!stream) {
        fprintf(stderr, "Failed to reconstruct stream\n");
        return 1;
    }
    
    printf("✓ Stream reconstructed: %zu bytes\n", stream_len);
    
    // Save raw stream
    save_stream("artifacts/stream.dat", stream, stream_len);
    
    // Try to decrypt with frequency analysis
    printf("\nAttempting decryption with frequency analysis...\n");
    unsigned char key = find_xor_key(stream, stream_len);
    printf("✓ Detected XOR key: 0x%02x (%d)\n", key, key);
    
    // Decrypt
    unsigned char *decrypted = malloc(stream_len);
    memcpy(decrypted, stream, stream_len);
    xor_decrypt(decrypted, stream_len, key);
    
    // Save decrypted stream
    save_stream("artifacts/decrypted.txt", decrypted, stream_len);
    
    // Display first 500 bytes
    printf("\nDecrypted message (first 500 bytes):\n");
    printf("─────────────────────────────────────────────────\n");
    for (size_t i = 0; i < stream_len && i < 500; i++) {
        if (decrypted[i] >= 32 && decrypted[i] <= 126) {
            putchar(decrypted[i]);
        } else if (decrypted[i] == '\n') {
            putchar('\n');
        } else {
            putchar('.');
        }
    }
    printf("\n─────────────────────────────────────────────────\n");
    
    // Cleanup
    free(stream);
    free(decrypted);
    
    printf("\n✓ Mission complete!\n");
    printf("Check artifacts/ directory for full results.\n");
    
    return 0;
}

