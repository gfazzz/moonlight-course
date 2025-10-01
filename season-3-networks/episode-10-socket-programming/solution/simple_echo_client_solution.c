/*
 * simple_echo_client.c
 * 
 * Промежуточная задача для Episode 10: Socket Programming
 * 
 * Цель: Создать простой TCP-клиент для подключения к echo-серверу
 * 
 * Концепции:
 * - socket() — создание сокета
 * - connect() — подключение к серверу
 * - send() — отправка данных
 * - recv() — получение данных
 * - close() — закрытие соединения
 * 
 * Задача:
 * 1. Подключиться к echo-серверу (127.0.0.1:7777 для тестов)
 * 2. Отправить строку "HELLO MOONLIGHT"
 * 3. Получить echo (тот же текст обратно)
 * 4. Вывести результат
 * 5. Закрыть соединение
 * 
 * Компиляция:
 *   gcc -Wall -Wextra -std=c11 simple_echo_client_solution.c -o simple_echo_client
 * 
 * Использование:
 *   ./simple_echo_client <server_ip> <port>
 *   ./simple_echo_client 127.0.0.1 7777
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define DEFAULT_MESSAGE "HELLO MOONLIGHT"

int main(int argc, char *argv[]) {
    // Аргументы командной строки
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        fprintf(stderr, "Example: %s 127.0.0.1 7777\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    if (server_port <= 0 || server_port > 65535) {
        fprintf(stderr, "Invalid port: %d (must be 1-65535)\n", server_port);
        return 1;
    }

    printf("═══════════════════════════════════════\n");
    printf("    SIMPLE ECHO CLIENT\n");
    printf("═══════════════════════════════════════\n\n");

    printf("Target: %s:%d\n", server_ip, server_port);
    printf("Message: %s\n\n", DEFAULT_MESSAGE);

    // ────────────────────────────────────────
    // 1. Создание сокета
    // ────────────────────────────────────────
    printf("[1/5] Creating socket...\n");
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        return 1;
    }
    printf("  ✓ Socket created (fd=%d)\n\n", sockfd);

    // ────────────────────────────────────────
    // 2. Настройка адреса сервера
    // ────────────────────────────────────────
    printf("[2/5] Setting up server address...\n");
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);  // Network byte order!
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1) {
        fprintf(stderr, "  ✗ Invalid IP address: %s\n", server_ip);
        close(sockfd);
        return 1;
    }
    
    printf("  ✓ Address: %s:%d\n", server_ip, server_port);
    printf("  ✓ Network byte order: 0x%04X\n\n", ntohs(server_addr.sin_port));

    // ────────────────────────────────────────
    // 3. Подключение к серверу
    // ────────────────────────────────────────
    printf("[3/5] Connecting to server...\n");
    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("  ✗ connect() failed");
        close(sockfd);
        return 1;
    }
    
    printf("  ✓ Connected!\n");
    printf("  ✓ TCP handshake complete (SYN → SYN-ACK → ACK)\n\n");

    // ────────────────────────────────────────
    // 4. Отправка сообщения
    // ────────────────────────────────────────
    printf("[4/5] Sending message...\n");
    
    const char *message = DEFAULT_MESSAGE;
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    
    if (bytes_sent < 0) {
        perror("  ✗ send() failed");
        close(sockfd);
        return 1;
    }
    
    printf("  ✓ Sent: \"%s\" (%zd bytes)\n\n", message, bytes_sent);

    // ────────────────────────────────────────
    // 5. Получение ответа
    // ────────────────────────────────────────
    printf("[5/5] Receiving echo...\n");
    
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received < 0) {
        perror("  ✗ recv() failed");
        close(sockfd);
        return 1;
    }
    
    if (bytes_received == 0) {
        printf("  ⚠️  Connection closed by server\n");
        close(sockfd);
        return 1;
    }
    
    buffer[bytes_received] = '\0';  // Null-terminate
    
    printf("  ✓ Received: \"%s\" (%zd bytes)\n\n", buffer, bytes_received);

    // ────────────────────────────────────────
    // Проверка echo
    // ────────────────────────────────────────
    printf("═══════════════════════════════════════\n");
    printf("    RESULT\n");
    printf("═══════════════════════════════════════\n\n");
    
    if (strcmp(message, buffer) == 0) {
        printf("✅ Echo successful!\n");
        printf("   Sent:     \"%s\"\n", message);
        printf("   Received: \"%s\"\n", buffer);
        printf("   Match: ✓\n");
    } else {
        printf("❌ Echo failed!\n");
        printf("   Sent:     \"%s\"\n", message);
        printf("   Received: \"%s\"\n", buffer);
        printf("   Match: ✗\n");
    }

    // ────────────────────────────────────────
    // Закрытие соединения
    // ────────────────────────────────────────
    close(sockfd);
    printf("\n🔒 Connection closed.\n");

    return 0;
}

