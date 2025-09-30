/**
 * Episode 09: Network Basics - SOLUTION
 * Network address analyzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAX_LINE 256
#define INET_ADDRSTRLEN 16

typedef struct {
    struct in_addr ip;
    uint16_t port;
} Address;

typedef struct {
    int total;
    int private_count;
    int public_count;
    int loopback_count;
    int wellknown_ports;
    int registered_ports;
    int dynamic_ports;
} Statistics;

int parse_address(const char *str, Address *addr) {
    char ip_str[INET_ADDRSTRLEN];
    int port;
    
    // Parse IP:PORT
    if (sscanf(str, "%15[^:]:%d", ip_str, &port) != 2) {
        return -1;
    }
    
    // Validate port range
    if (port < 0 || port > 65535) {
        return -1;
    }
    
    // Parse IP address
    if (inet_pton(AF_INET, ip_str, &addr->ip) != 1) {
        return -1;
    }
    
    addr->port = (uint16_t)port;
    return 0;
}

int is_private_ip(struct in_addr addr) {
    uint32_t ip = ntohl(addr.s_addr);
    
    // 10.0.0.0/8
    if ((ip & 0xFF000000) == 0x0A000000) return 1;
    
    // 172.16.0.0/12
    if ((ip & 0xFFF00000) == 0xAC100000) return 1;
    
    // 192.168.0.0/16
    if ((ip & 0xFFFF0000) == 0xC0A80000) return 1;
    
    return 0;
}

int is_loopback_ip(struct in_addr addr) {
    uint32_t ip = ntohl(addr.s_addr);
    // 127.0.0.0/8
    return (ip & 0xFF000000) == 0x7F000000;
}

const char* get_port_category(uint16_t port) {
    if (port <= 1023) return "Well-Known";
    if (port <= 49151) return "Registered";
    return "Dynamic";
}

const char* get_service_name(uint16_t port) {
    switch (port) {
        case 20: return "FTP-DATA";
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 67: return "DHCP-Server";
        case 68: return "DHCP-Client";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 587: return "SMTP-TLS";
        case 993: return "IMAPS";
        case 995: return "POP3S";
        case 3306: return "MySQL";
        case 5432: return "PostgreSQL";
        case 8080: return "HTTP-Proxy";
        default: return NULL;
    }
}

void calculate_network(struct in_addr addr, char *network_str) {
    uint32_t ip = ntohl(addr.s_addr);
    uint32_t mask, network;
    int prefix;
    
    if ((ip & 0xFF000000) == 0x0A000000) {
        mask = 0xFF000000;  // /8
        prefix = 8;
    } else if ((ip & 0xFFF00000) == 0xAC100000) {
        mask = 0xFFF00000;  // /12
        prefix = 12;
    } else if ((ip & 0xFFFF0000) == 0xC0A80000) {
        mask = 0xFFFF0000;  // /16
        prefix = 16;
    } else {
        sprintf(network_str, "N/A");
        return;
    }
    
    network = htonl(ip & mask);
    struct in_addr net_addr = { .s_addr = network };
    
    char net_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &net_addr, net_str, INET_ADDRSTRLEN);
    sprintf(network_str, "%s/%d", net_str, prefix);
}

uint32_t ip_to_hex(struct in_addr addr) {
    return ntohl(addr.s_addr);
}

const char* get_ip_type(struct in_addr addr) {
    if (is_loopback_ip(addr)) return "Loopback";
    if (is_private_ip(addr)) return "Private (RFC 1918)";
    return "Public";
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <addresses.txt>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    printf("=== NETWORK ANALYZER ===\n");
    printf("Analyzing %s...\n\n", filename);
    
    Statistics stats = {0};
    char line[MAX_LINE];
    int index = 1;
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') continue;
        
        Address addr;
        if (parse_address(line, &addr) != 0) {
            printf("[%d] %s\n", index++, line);
            printf("    ERROR: Invalid address format\n\n");
            continue;
        }
        
        stats.total++;
        
        // Print address info
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.ip, ip_str, INET_ADDRSTRLEN);
        
        printf("[%d] %s:%u\n", index++, ip_str, addr.port);
        
        // IP type
        const char *ip_type = get_ip_type(addr.ip);
        printf("    Type: %s\n", ip_type);
        
        if (is_private_ip(addr.ip)) {
            stats.private_count++;
            char network[32];
            calculate_network(addr.ip, network);
            printf("    Network: %s\n", network);
        } else if (is_loopback_ip(addr.ip)) {
            stats.loopback_count++;
        } else {
            stats.public_count++;
        }
        
        // Port info
        const char *category = get_port_category(addr.port);
        const char *service = get_service_name(addr.port);
        
        printf("    Port: %u (%s", addr.port, category);
        if (service) {
            printf(" - %s", service);
        }
        printf(")\n");
        
        if (addr.port <= 1023) stats.wellknown_ports++;
        else if (addr.port <= 49151) stats.registered_ports++;
        else stats.dynamic_ports++;
        
        // Hex representation
        printf("    Hex: 0x%08X\n", ip_to_hex(addr.ip));
        
        printf("\n");
    }
    
    fclose(file);
    
    // Print statistics
    printf("Statistics:\n");
    printf("- Total addresses: %d\n", stats.total);
    
    if (stats.total > 0) {
        printf("- Private: %d (%.0f%%)\n", 
               stats.private_count, 
               100.0 * stats.private_count / stats.total);
        printf("- Public: %d (%.0f%%)\n", 
               stats.public_count, 
               100.0 * stats.public_count / stats.total);
        printf("- Loopback: %d (%.0f%%)\n", 
               stats.loopback_count, 
               100.0 * stats.loopback_count / stats.total);
    }
    
    printf("- Well-known ports: %d\n", stats.wellknown_ports);
    printf("- Registered ports: %d\n", stats.registered_ports);
    printf("- Dynamic ports: %d\n", stats.dynamic_ports);

    return 0;
}
