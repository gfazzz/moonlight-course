/**
 * Episode 09: Network Basics
 * Network address analyzer
 * 
 * TODO: Implement network analysis functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

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

/**
 * Parse "IP:PORT" string
 * 
 * @param str Input string
 * @param addr Output address structure
 * @return 0 on success, -1 on error
 */
int parse_address(const char *str, Address *addr) {
    // TODO: Parse IP and port from "IP:PORT" format
    // Hint: use sscanf or strtok
    // Validate IP with inet_pton
    
    return -1;  // placeholder
}

/**
 * Check if IP is private (RFC 1918)
 * 
 * @param addr IP address
 * @return 1 if private, 0 otherwise
 */
int is_private_ip(struct in_addr addr) {
    // TODO: Check for:
    // 10.0.0.0/8
    // 172.16.0.0/12
    // 192.168.0.0/16
    
    return 0;  // placeholder
}

/**
 * Check if IP is loopback
 * 
 * @param addr IP address
 * @return 1 if loopback, 0 otherwise
 */
int is_loopback_ip(struct in_addr addr) {
    // TODO: Check for 127.0.0.0/8
    
    return 0;  // placeholder
}

/**
 * Get port category
 * 
 * @param port Port number
 * @return Category string
 */
const char* get_port_category(uint16_t port) {
    // TODO:
    // 0-1023: Well-Known
    // 1024-49151: Registered
    // 49152-65535: Dynamic
    
    return "Unknown";  // placeholder
}

/**
 * Get service name for well-known port
 * 
 * @param port Port number
 * @return Service name or "Unknown"
 */
const char* get_service_name(uint16_t port) {
    // TODO: Return service name for common ports
    // 20=FTP-DATA, 21=FTP, 22=SSH, 23=Telnet
    // 25=SMTP, 53=DNS, 80=HTTP, 443=HTTPS, etc.
    
    return "Unknown";  // placeholder
}

/**
 * Calculate network address for private IP
 * 
 * @param addr IP address
 * @param network_str Output string (e.g., "192.168.1.0/24")
 */
void calculate_network(struct in_addr addr, char *network_str) {
    // TODO: Calculate network based on private IP type
    // 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16
    
    sprintf(network_str, "N/A");  // placeholder
}

/**
 * Convert IP to hex
 * 
 * @param addr IP address
 * @return Hex value (host byte order)
 */
uint32_t ip_to_hex(struct in_addr addr) {
    // TODO: Convert to hex (use ntohl)
    
    return 0;  // placeholder
}

/**
 * Get IP type as string
 */
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
    
    // TODO: Implement the main logic:
    // 1. Open file
    // 2. Read each line
    // 3. Parse address
    // 4. Analyze and print details
    // 5. Update statistics
    // 6. Print final statistics

    printf("=== NETWORK ANALYZER ===\n");
    printf("Analyzing %s...\n\n", filename);
    
    // Your code here
    
    printf("\nStatistics:\n");
    printf("- Total addresses: 0\n");
    printf("- Private: 0 (0%%)\n");
    printf("- Public: 0 (0%%)\n");
    printf("- Loopback: 0 (0%%)\n");
    printf("- Well-known ports: 0\n");
    printf("- Registered ports: 0\n");
    printf("- Dynamic ports: 0\n");

    return 0;
}
