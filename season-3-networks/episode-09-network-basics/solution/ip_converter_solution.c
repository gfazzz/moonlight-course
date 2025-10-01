/*
 * ip_converter_solution.c
 * 
 * OPERATION MOONLIGHT — Episode 09: Network Basics
 * Промежуточная задача: IP Address Converter
 * 
 * Цель: Освоить преобразование IP-адресов между форматами
 * - String ↔ Binary ↔ Hex
 * - Host byte order ↔ Network byte order
 * 
 * Компиляция:
 *   gcc -Wall -Wextra -std=c11 -o ip_converter ip_converter_solution.c
 * 
 * Использование:
 *   ./ip_converter
 *   Enter IP address: 192.168.1.100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

// Цвета для вывода (опционально)
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RED     "\033[31m"

/**
 * Конвертировать и вывести информацию об IP-адресе
 */
void convert_and_display_ip(const char *ip_string) {
    struct in_addr addr;
    
    printf("\n");
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    printf(COLOR_CYAN "    IP ADDRESS CONVERTER\n" COLOR_RESET);
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    
    // 1. String → Binary (inet_pton)
    printf("\n" COLOR_YELLOW "Input:" COLOR_RESET "\n");
    printf("  String: %s\n", ip_string);
    
    if (inet_pton(AF_INET, ip_string, &addr) != 1) {
        printf(COLOR_RED "  ❌ Error: Invalid IP address!\n" COLOR_RESET);
        return;
    }
    
    printf(COLOR_GREEN "  ✓ Valid IPv4 address\n" COLOR_RESET);
    
    // 2. Binary → Hex (network byte order)
    printf("\n" COLOR_YELLOW "Binary Format:" COLOR_RESET "\n");
    printf("  Network byte order: 0x%08X\n", (unsigned int)addr.s_addr);
    
    // 3. Binary → Hex (host byte order)
    uint32_t host_order = ntohl(addr.s_addr);
    printf("  Host byte order:    0x%08X\n", host_order);
    
    // 4. Decimal representation
    printf("\n" COLOR_YELLOW "Decimal:" COLOR_RESET "\n");
    printf("  Unsigned 32-bit: %u\n", host_order);
    
    // 5. Octets (разбить на байты)
    printf("\n" COLOR_YELLOW "Octets:" COLOR_RESET "\n");
    uint8_t *octets = (uint8_t*)&host_order;
    printf("  [3] . [2] . [1] . [0]  (little-endian indexing)\n");
    printf("  %3u . %3u . %3u . %3u\n", 
           octets[3], octets[2], octets[1], octets[0]);
    
    // Альтернативный способ (без приведения типа)
    printf("\n  Вычисление через побитовые операции:\n");
    printf("  %3u . %3u . %3u . %3u\n",
           (host_order >> 24) & 0xFF,
           (host_order >> 16) & 0xFF,
           (host_order >> 8) & 0xFF,
           host_order & 0xFF);
    
    // 6. Binary representation
    printf("\n" COLOR_YELLOW "Binary:" COLOR_RESET "\n");
    printf("  ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (host_order >> i) & 1);
        if (i % 8 == 0 && i != 0) {
            printf(".");
        }
    }
    printf("\n");
    
    // 7. Byte order comparison
    printf("\n" COLOR_YELLOW "Byte Order Comparison:" COLOR_RESET "\n");
    printf("  Network (big-endian):    ");
    uint8_t *net_bytes = (uint8_t*)&addr.s_addr;
    for (int i = 0; i < 4; i++) {
        printf("%02X ", net_bytes[i]);
    }
    printf("\n");
    
    printf("  Host (your machine):     ");
    uint8_t *host_bytes = (uint8_t*)&host_order;
    for (int i = 0; i < 4; i++) {
        printf("%02X ", host_bytes[i]);
    }
    printf("\n");
    
    // 8. IP classification
    printf("\n" COLOR_YELLOW "Classification:" COLOR_RESET "\n");
    
    // Check private IP (RFC 1918)
    if ((host_order & 0xFF000000) == 0x0A000000) {
        printf("  Type: " COLOR_GREEN "Private" COLOR_RESET " (10.0.0.0/8 - RFC 1918)\n");
    } else if ((host_order & 0xFFF00000) == 0xAC100000) {
        printf("  Type: " COLOR_GREEN "Private" COLOR_RESET " (172.16.0.0/12 - RFC 1918)\n");
    } else if ((host_order & 0xFFFF0000) == 0xC0A80000) {
        printf("  Type: " COLOR_GREEN "Private" COLOR_RESET " (192.168.0.0/16 - RFC 1918)\n");
    } else if ((host_order & 0xFF000000) == 0x7F000000) {
        printf("  Type: " COLOR_CYAN "Loopback" COLOR_RESET " (127.0.0.0/8)\n");
    } else {
        printf("  Type: " COLOR_YELLOW "Public" COLOR_RESET " (routable on Internet)\n");
    }
    
    printf("\n" COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
}

/**
 * Интерактивный режим
 */
void interactive_mode(void) {
    char input[100];
    
    while (1) {
        printf("\n" COLOR_GREEN "Enter IP address (or 'quit' to exit): " COLOR_RESET);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Удалить \n
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        // Проверка на выход
        if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0 || strcmp(input, "exit") == 0) {
            printf(COLOR_CYAN "Goodbye! 🌙\n" COLOR_RESET);
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        convert_and_display_ip(input);
    }
}

/**
 * Режим с аргументами командной строки
 */
void process_arguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        convert_and_display_ip(argv[i]);
    }
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    printf(COLOR_CYAN);
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║  IP ADDRESS CONVERTER v1.0                    ║\n");
    printf("║  OPERATION MOONLIGHT — Episode 09             ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    
    if (argc > 1) {
        // Режим с аргументами: ./ip_converter 192.168.1.1 10.0.0.5
        process_arguments(argc, argv);
    } else {
        // Интерактивный режим
        interactive_mode();
    }
    
    return 0;
}

