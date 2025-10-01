/**
 * Episode 10: Socket Programming - SOLUTION
 * MOONLIGHT TCP Client
 * 
 * Complete implementation with:
 * - TCP socket connection
 * - Send/receive commands
 * - XOR encryption support
 * - Interactive command loop
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define XOR_KEY 0x42  // Simple XOR key from Episode 01

// XOR encryption/decryption
void xor_crypt(unsigned char *data, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// Create and connect TCP socket
int connect_to_server(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Convert IP address
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        return -1;
    }
    
    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }
    
    printf("✓ Connected to %s:%d\n\n", ip, port);
    return sockfd;
}

// Send command to server
int send_command(int sockfd, const char *command) {
    ssize_t sent = send(sockfd, command, strlen(command), 0);
    if (sent < 0) {
        perror("Send failed");
        return -1;
    }
    return 0;
}

// Receive response from server
int receive_response(int sockfd, char *buffer, size_t buffer_size) {
    memset(buffer, 0, buffer_size);
    ssize_t received = recv(sockfd, buffer, buffer_size - 1, 0);
    
    if (received < 0) {
        perror("Receive failed");
        return -1;
    } else if (received == 0) {
        printf("Server closed connection\n");
        return 0;
    }
    
    buffer[received] = '\0';
    return received;
}

// Print help menu
void print_help() {
    printf("Available commands:\n");
    printf("  HELLO       - Get server greeting\n");
    printf("  GET_KEY     - Request encryption key\n");
    printf("  SEND <msg>  - Send encrypted message\n");
    printf("  QUIT        - Close connection\n");
    printf("  help        - Show this menu\n");
    printf("\n");
}

// Interactive command loop
void interactive_loop(int sockfd) {
    char command[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    
    print_help();
    
    while (1) {
        printf("moonlight> ");
        fflush(stdout);
        
        // Read command from user
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        command[strcspn(command, "\n")] = 0;
        
        // Skip empty commands
        if (strlen(command) == 0) {
            continue;
        }
        
        // Handle local commands
        if (strcmp(command, "help") == 0) {
            print_help();
            continue;
        }
        
        if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            strcpy(command, "QUIT");
        }
        
        // Send command to server
        if (send_command(sockfd, command) < 0) {
            break;
        }
        
        // Handle SEND command with encryption
        if (strncmp(command, "SEND ", 5) == 0) {
            // Extract message
            char *message = command + 5;
            unsigned char encrypted[BUFFER_SIZE];
            strcpy((char *)encrypted, message);
            
            // Encrypt message
            xor_crypt(encrypted, strlen((char *)encrypted), XOR_KEY);
            
            printf("Encrypted message (hex): ");
            for (size_t i = 0; i < strlen(message); i++) {
                printf("%02x ", encrypted[i]);
            }
            printf("\n");
        }
        
        // Receive response
        int received = receive_response(sockfd, response, sizeof(response));
        if (received <= 0) {
            break;
        }
        
        printf("Server: %s\n\n", response);
        
        // Exit on QUIT
        if (strcmp(command, "QUIT") == 0) {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        fprintf(stderr, "\nExample:\n");
        fprintf(stderr, "  %s 127.0.0.1 8080\n", argv[0]);
        return 1;
    }
    
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    
    printf("═══════════════════════════════════════\n");
    printf("  MOONLIGHT Client v1.0\n");
    printf("═══════════════════════════════════════\n\n");
    
    printf("Connecting to %s:%d...\n", ip, port);
    
    // Connect to server
    int sockfd = connect_to_server(ip, port);
    if (sockfd < 0) {
        return 1;
    }
    
    // Interactive command loop
    interactive_loop(sockfd);
    
    // Close socket
    close(sockfd);
    printf("\nConnection closed.\n");
    
    return 0;
}

