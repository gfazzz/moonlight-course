/**
 * Episode 10: Socket Programming
 * MOONLIGHT TCP Client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// TODO: Implement TCP client functions

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }
    
    // TODO:
    // 1. Parse IP and PORT
    // 2. Create socket
    // 3. Connect to server
    // 4. Interactive loop: send/recv
    // 5. Close socket
    
    printf("MOONLIGHT Client v1.0\n");
    printf("Connecting to %s:%s...\n", argv[1], argv[2]);
    
    return 0;
}
