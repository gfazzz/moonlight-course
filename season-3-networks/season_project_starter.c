/*
 * Season 3: Season Project — network_interceptor
 * 
 * ЗАДАНИЕ: Собрать универсальный инструмент сетевого анализа
 *          из компонентов Episodes 09-12
 * 
 * КОМПОНЕНТЫ:
 * - ip_tools (Episode 09)
 * - tcp_client (Episode 10)
 * - packet_sniffer (Episode 11)
 * - session_reconstructor (Episode 12)
 * 
 * АВТОР: [ВАШ ПОЗЫВНОЙ]
 * ДАТА: [ДАТА]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// TODO: Раскомментировать после создания модулей!
// #include "season-project/ip_tools.h"
// #include "season-project/tcp_client.h"
// #include "season-project/packet_sniffer.h"
// #include "season-project/session_reconstructor.h"

/*
 * Главная программа network_interceptor
 * 
 * ВОЗМОЖНОСТИ:
 * 1. Конвертация IP адресов (Episode 09)
 * 2. Декодирование GPS из IP (Episode 09)
 * 3. TCP подключение с challenge-response (Episode 10)
 * 4. Перехват пакетов raw sockets (Episode 11)
 * 5. Восстановление TCP сессий (Episode 12)
 * 6. XOR расшифровка (Episode 01 + 12)
 */

void print_banner(void) {
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║    NETWORK INTERCEPTOR v1.0               ║\n");
    printf("║    Operation MOONLIGHT - Season 3         ║\n");
    printf("║    Agent: [ВАШ ПОЗЫВНОЙ]                  ║\n");
    printf("╚═══════════════════════════════════════════╝\n\n");
}

void print_help(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n\n", prog_name);
    printf("OPTIONS:\n");
    printf("  --mode <mode>       Operation mode:\n");
    printf("                        'ip'      - IP conversion demo\n");
    printf("                        'tcp'     - TCP client demo\n");
    printf("                        'sniff'   - Packet sniffer\n");
    printf("                        'session' - Session reconstructor\n");
    printf("                        'full'    - Full pipeline\n");
    printf("  --host <ip>         Target host IP\n");
    printf("  --port <port>       Target port\n");
    printf("  --key <key>         Auth key (for TCP)\n");
    printf("  --filter <port>     Filter port (for sniffer)\n");
    printf("  --help              Show this help\n\n");
    printf("EXAMPLES:\n");
    printf("  %s --mode ip --host 203.0.113.42\n", prog_name);
    printf("  %s --mode tcp --host 203.0.113.42 --port 31337 --key MOONLIGHT2024\n", prog_name);
    printf("  sudo %s --mode sniff --filter 9999\n", prog_name);
    printf("  sudo %s --mode full\n", prog_name);
}

/*
 * MODE 1: IP Conversion Demo (Episode 09)
 */
void demo_ip_conversion(const char *ip_str) {
    printf("\n[MODE: IP Conversion]\n");
    printf("═══════════════════════════════════════════\n\n");
    
    // TODO: Реализовать с использованием ip_tools
    printf("IP String: %s\n", ip_str);
    
    // TODO: Конвертация string → uint32_t
    // uint32_t ip = ip_string_to_int(ip_str);
    // printf("IP Uint32: 0x%08X\n", ip);
    
    // TODO: Декодирование GPS
    // double lat, lon;
    // ip_to_gps(ip, &lat, &lon);
    // printf("GPS: %.4f°N %.4f°E\n", lat, lon);
    
    printf("\n✅ IP conversion complete!\n");
}

/*
 * MODE 2: TCP Client Demo (Episode 10)
 */
void demo_tcp_client(const char *host, uint16_t port, const char *key) {
    printf("\n[MODE: TCP Client]\n");
    printf("═══════════════════════════════════════════\n\n");
    
    printf("Target: %s:%u\n", host, port);
    printf("Auth key: %s\n\n", key);
    
    // TODO: Реализовать с использованием tcp_client
    
    // TODO: ШАГ 1 — Подключение
    // printf("[1/3] Connecting...\n");
    // int sockfd = tcp_connect(host, port);
    // if (sockfd < 0) {
    //     printf("❌ Connection failed!\n");
    //     return;
    // }
    // printf("✅ Connected!\n\n");
    
    // TODO: ШАГ 2 — Challenge-Response
    // printf("[2/3] Authenticating...\n");
    // if (challenge_response(sockfd, key) < 0) {
    //     printf("❌ Auth failed!\n");
    //     tcp_close(sockfd);
    //     return;
    // }
    // printf("✅ Authenticated!\n\n");
    
    // TODO: ШАГ 3 — Получение сообщения
    // printf("[3/3] Receiving message...\n");
    // char buffer[1024];
    // tcp_recv_safe(sockfd, buffer, sizeof(buffer), 10);
    // printf("Message: %s\n", buffer);
    
    // TODO: Закрыть соединение
    // tcp_close(sockfd);
    
    printf("\n✅ TCP client demo complete!\n");
}

/*
 * MODE 3: Packet Sniffer Demo (Episode 11)
 */
void demo_packet_sniffer(uint16_t filter_port) {
    printf("\n[MODE: Packet Sniffer]\n");
    printf("═══════════════════════════════════════════\n\n");
    
    printf("Filter port: %u\n", filter_port);
    printf("⚠️  Requires root privileges!\n\n");
    
    // TODO: Реализовать с использованием packet_sniffer
    
    // TODO: ШАГ 1 — Создать raw socket
    // printf("[1/3] Creating raw socket...\n");
    // int raw_fd = create_raw_socket(filter_port);
    // if (raw_fd < 0) {
    //     printf("❌ Failed! (are you root?)\n");
    //     return;
    // }
    // printf("✅ Raw socket created!\n\n");
    
    // TODO: ШАГ 2 — Capture packets
    // printf("[2/3] Capturing packets (Ctrl+C to stop)...\n");
    // Packet packets[100];
    // int count = 0;
    // 
    // while (count < 10) {  // Capture 10 packets
    //     if (capture_packet(raw_fd, &packets[count]) == 0) {
    //         count++;
    //         printf("  Packet %d captured\n", count);
    //     }
    // }
    
    // TODO: ШАГ 3 — Analyze packets
    // printf("\n[3/3] Analyzing packets...\n");
    // for (int i = 0; i < count; i++) {
    //     printf("\nPacket %d:\n", i + 1);
    //     hex_dump(packets[i].payload, packets[i].payload_len);
    // }
    
    // close(raw_fd);
    
    printf("\n✅ Packet sniffer demo complete!\n");
}

/*
 * MODE 4: Session Reconstructor Demo (Episode 12)
 */
void demo_session_reconstructor(void) {
    printf("\n[MODE: Session Reconstructor]\n");
    printf("═══════════════════════════════════════════\n\n");
    
    // TODO: Реализовать с использованием session_reconstructor
    
    // TODO: ШАГ 1 — Создать тестовые пакеты (неупорядоченные)
    // Packet packets[4];
    // packets[0].seq = 1065; strcpy(packets[0].payload, "Coordinates...");
    // packets[1].seq = 1000; strcpy(packets[1].payload, "KEY:0x42\n...");
    // packets[2].seq = 1165; strcpy(packets[2].payload, "7558°N...");
    // packets[3].seq = 1015; strcpy(packets[3].payload, "Поздравляю...");
    // int count = 4;
    
    // TODO: ШАГ 2 — Сортировка
    // printf("[1/4] Sorting packets by SEQ...\n");
    // packet_sorter(packets, count);
    // printf("✅ Sorted!\n\n");
    
    // TODO: ШАГ 3 — Gap detection
    // printf("[2/4] Checking for gaps...\n");
    // if (detect_gaps(packets, count) == 0) {
    //     printf("✅ No gaps!\n\n");
    // }
    
    // TODO: ШАГ 4 — Reassembly
    // printf("[3/4] Reassembling stream...\n");
    // char stream[10000];
    // int stream_len = reassemble_stream(packets, count, stream);
    // printf("✅ Stream size: %d bytes\n\n", stream_len);
    
    // TODO: ШАГ 5 — Decryption
    // printf("[4/4] Decrypting XOR...\n");
    // uint8_t key = extract_key(stream);
    // xor_decrypt(stream, stream_len, key);
    // printf("✅ Decrypted!\n");
    // printf("\nMessage:\n%s\n", stream);
    
    printf("\n✅ Session reconstructor demo complete!\n");
}

/*
 * MODE 5: Full Pipeline (все компоненты вместе!)
 */
void demo_full_pipeline(void) {
    printf("\n[MODE: Full Pipeline]\n");
    printf("═══════════════════════════════════════════\n\n");
    printf("⚠️  Requires root privileges!\n\n");
    
    // TODO: Реализовать полный pipeline:
    
    // 1. IP конвертация + GPS
    printf("[1/6] IP → GPS Decoding...\n");
    // demo_ip_conversion("203.0.113.42");
    
    // 2. TCP подключение
    printf("\n[2/6] TCP Connection...\n");
    // demo_tcp_client("203.0.113.42", 31337, "MOONLIGHT2024");
    
    // 3. Packet capture
    printf("\n[3/6] Packet Sniffing...\n");
    // Capture packets on port 4433 (Episode 12)
    
    // 4. Session reconstruction
    printf("\n[4/6] Session Reconstruction...\n");
    // Sort, detect gaps, reassemble
    
    // 5. XOR decryption
    printf("\n[5/6] XOR Decryption...\n");
    // Extract key, decrypt
    
    // 6. Результат
    printf("\n[6/6] Final Result...\n");
    printf("✅ Coordinates: 55.7558°N 37.6173°E\n");
    printf("✅ Location: Красная площадь, Спасская башня\n");
    printf("✅ Season 3 COMPLETE!\n");
    
    printf("\n═══════════════════════════════════════════\n");
    printf("    SEASON 4: CRYPTO & ALGORITHMS\n");
    printf("            UNLOCKED!\n");
    printf("═══════════════════════════════════════════\n");
}

/*
 * MAIN
 */
int main(int argc, char *argv[]) {
    print_banner();
    
    // Парсинг аргументов
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }
    
    const char *mode = NULL;
    const char *host = "203.0.113.42";  // Default
    uint16_t port = 31337;               // Default
    const char *key = "MOONLIGHT2024";  // Default
    uint16_t filter_port = 9999;        // Default
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            mode = argv[++i];
        } else if (strcmp(argv[i], "--host") == 0 && i + 1 < argc) {
            host = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--key") == 0 && i + 1 < argc) {
            key = argv[++i];
        } else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
            filter_port = atoi(argv[++i]);
        }
    }
    
    if (!mode) {
        printf("❌ Error: --mode required!\n\n");
        print_help(argv[0]);
        return 1;
    }
    
    // Выполнение режима
    if (strcmp(mode, "ip") == 0) {
        demo_ip_conversion(host);
    } else if (strcmp(mode, "tcp") == 0) {
        demo_tcp_client(host, port, key);
    } else if (strcmp(mode, "sniff") == 0) {
        demo_packet_sniffer(filter_port);
    } else if (strcmp(mode, "session") == 0) {
        demo_session_reconstructor();
    } else if (strcmp(mode, "full") == 0) {
        demo_full_pipeline();
    } else {
        printf("❌ Unknown mode: %s\n\n", mode);
        print_help(argv[0]);
        return 1;
    }
    
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║  Season Project: network_interceptor      ║\n");
    printf("║  Status: [TODO - Implement modules!]      ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    
    return 0;
}

/*
 * ИНСТРУКЦИЯ ДЛЯ СТУДЕНТА:
 * 
 * 1. Создай папку season-project/
 * 
 * 2. Извлеки код из Episodes 09-12:
 *    - Episode 09 → ip_tools.h + ip_tools.c
 *    - Episode 10 → tcp_client.h + tcp_client.c
 *    - Episode 11 → packet_sniffer.h + packet_sniffer.c
 *    - Episode 12 → session_reconstructor.h + session_reconstructor.c
 * 
 * 3. Раскомментируй #include выше
 * 
 * 4. Раскомментируй TODO в demo функциях
 * 
 * 5. Обнови Makefile:
 *    - Раскомментируй PROJECT_SRC
 *    - Добавь правило season_project
 * 
 * 6. Скомпилируй:
 *    make season_project
 * 
 * 7. Запусти:
 *    ./network_interceptor --mode ip --host 203.0.113.42
 *    ./network_interceptor --mode tcp --host 203.0.113.42 --port 31337
 *    sudo ./network_interceptor --mode sniff --filter 9999
 *    sudo ./network_interceptor --mode full
 * 
 * 8. Проверь утечки:
 *    valgrind --leak-check=full ./network_interceptor --mode ip --host 127.0.0.1
 * 
 * КРИТЕРИИ УСПЕХА:
 * ✅ Компилируется без warnings
 * ✅ Valgrind без утечек
 * ✅ Все 5 режимов работают
 * ✅ Модульная архитектура (4 компонента)
 * ✅ Код переиспользуемый (для Season 4-10!)
 * 
 * УДАЧИ! 🚀
 */

