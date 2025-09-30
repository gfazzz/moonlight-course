/**
 * Episode 11: Packet Analysis
 * Raw packet sniffer
 * 
 * ⚠️ Requires root/sudo!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

// TODO: Implement packet capture and analysis

int main() {
    printf("MOONLIGHT Packet Sniffer\n");
    printf("⚠️  Requires root privileges!\n\n");
    
    // TODO:
    // 1. Create raw socket
    // 2. Loop: receive packets
    // 3. Parse IP header
    // 4. Parse TCP/UDP header
    // 5. Display info + hex dump
    
    return 0;
}
