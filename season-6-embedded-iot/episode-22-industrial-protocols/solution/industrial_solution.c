/*
 * Episode 22: Industrial Protocols & Access Control - SOLUTION
 * Operation MOONLIGHT - Season 6
 * 
 * Mission: Reverse engineer RFID/NFC access control systems
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Wiegand 26-bit protocol (parse, generate, validate)
 * ✅ Modbus RTU (CRC16, read registers, validate frames)
 * ✅ RFID card emulator with card database
 * ✅ Access control system simulation
 * ✅ Authentication and authorization
 * ✅ Event logging and audit trail
 * ✅ Brute force attack detection
 * ✅ Statistics and reporting
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// ============================================================================
// WIEGAND PROTOCOL
// ============================================================================

typedef struct {
    uint8_t facility_code;
    uint16_t card_id;
    bool valid;
} WiegandCard;

WiegandCard parse_wiegand26(uint32_t raw_data) {
    WiegandCard card = {0};
    
    // Извлечь биты
    uint8_t p0 = (raw_data >> 25) & 0x01;
    uint8_t fc = (raw_data >> 17) & 0xFF;
    uint16_t id = (raw_data >> 1) & 0xFFFF;
    uint8_t p1 = raw_data & 0x01;
    
    // Проверка чётности P0 (even parity для первых 13 бит)
    uint32_t first_half = (raw_data >> 13) & 0x1FFF;
    int count0 = __builtin_popcount(first_half);
    bool p0_valid = ((count0 % 2) == 0);
    
    // Проверка P1 (odd parity для последних 13 бит)
    uint32_t second_half = raw_data & 0x1FFF;
    int count1 = __builtin_popcount(second_half);
    bool p1_valid = ((count1 % 2) == 1);
    
    card.facility_code = fc;
    card.card_id = id;
    card.valid = p0_valid && p1_valid;
    
    return card;
}

uint32_t generate_wiegand26(uint8_t fc, uint16_t card_id) {
    uint32_t data = 0;
    
    // Собрать средние биты (без чётности)
    data = ((uint32_t)fc << 17) | ((uint32_t)card_id << 1);
    
    // Вычислить P0 (even parity)
    uint32_t first_half = (data >> 13) & 0x1FFF;
    int count0 = __builtin_popcount(first_half);
    uint8_t p0 = (count0 % 2 == 0) ? 0 : 1;
    
    // Вычислить P1 (odd parity)
    uint32_t second_half = data & 0x1FFF;
    int count1 = __builtin_popcount(second_half);
    uint8_t p1 = (count1 % 2 == 1) ? 1 : 0;
    
    // Добавить биты чётности
    data |= ((uint32_t)p0 << 25);
    data |= p1;
    
    return data;
}

bool wiegand_is_valid(uint32_t raw_data) {
    WiegandCard card = parse_wiegand26(raw_data);
    return card.valid;
}

// ============================================================================
// MODBUS RTU PROTOCOL
// ============================================================================

uint16_t modbus_crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

int modbus_read_holding_registers(uint8_t *buf, uint8_t slave_id, 
                                   uint16_t start_addr, uint16_t count) {
    buf[0] = slave_id;
    buf[1] = 0x03;  // Read Holding Registers
    buf[2] = (start_addr >> 8) & 0xFF;
    buf[3] = start_addr & 0xFF;
    buf[4] = (count >> 8) & 0xFF;
    buf[5] = count & 0xFF;
    
    // Добавить CRC
    uint16_t crc = modbus_crc16(buf, 6);
    buf[6] = crc & 0xFF;
    buf[7] = (crc >> 8) & 0xFF;
    
    return 8;
}

bool modbus_check_crc(const uint8_t *frame, size_t len) {
    if (len < 4) return false;
    
    uint16_t received_crc = frame[len-2] | (frame[len-1] << 8);
    uint16_t calculated_crc = modbus_crc16(frame, len - 2);
    
    return received_crc == calculated_crc;
}

// ============================================================================
// RFID EMULATOR
// ============================================================================

#define MAX_CARDS 256

typedef struct {
    uint8_t fc;
    uint16_t id;
    uint32_t wiegand_code;
} Card;

typedef struct {
    Card cards[MAX_CARDS];
    int count;
} CardList;

int load_cards(const char *filename, CardList *list) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return -1;
    }
    
    list->count = 0;
    
    while (list->count < MAX_CARDS) {
        unsigned int fc, id;
        if (fscanf(f, "%u,%u", &fc, &id) == 2) {
            list->cards[list->count].fc = (uint8_t)fc;
            list->cards[list->count].id = (uint16_t)id;
            list->count++;
        } else {
            break;
        }
    }
    
    fclose(f);
    return list->count;
}

void generate_all_codes(CardList *list) {
    for (int i = 0; i < list->count; i++) {
        list->cards[i].wiegand_code = 
            generate_wiegand26(list->cards[i].fc, list->cards[i].id);
    }
}

void print_codes(const CardList *list) {
    printf("=== RFID CARDS ===\n");
    for (int i = 0; i < list->count; i++) {
        const Card *c = &list->cards[i];
        printf("Card %d: FC=%u, ID=%u, Wiegand=0x%08X\n", 
               i + 1, c->fc, c->id, c->wiegand_code);
    }
}

// ============================================================================
// ACCESS CONTROL SYSTEM (v2.0 Enhanced)
// ============================================================================

#include <time.h>

typedef struct {
    time_t timestamp;
    uint8_t fc;
    uint16_t card_id;
    bool granted;
    char reason[64];
} AccessLog;

typedef struct {
    Card *authorized_cards;
    int authorized_count;
    AccessLog *logs;
    int log_count;
    int log_capacity;
    bool door_locked;
    int failed_attempts;
    int successful_access;
} AccessControlSystem;

AccessControlSystem acs;

void acs_init(void) {
    acs.authorized_cards = NULL;
    acs.authorized_count = 0;
    acs.logs = malloc(1000 * sizeof(AccessLog));
    acs.log_count = 0;
    acs.log_capacity = 1000;
    acs.door_locked = true;
    acs.failed_attempts = 0;
    acs.successful_access = 0;
}

void acs_load_authorized(CardList *list) {
    acs.authorized_cards = malloc(list->count * sizeof(Card));
    memcpy(acs.authorized_cards, list->cards, list->count * sizeof(Card));
    acs.authorized_count = list->count;
}

bool acs_is_authorized(uint8_t fc, uint16_t id) {
    for (int i = 0; i < acs.authorized_count; i++) {
        if (acs.authorized_cards[i].fc == fc && 
            acs.authorized_cards[i].id == id) {
            return true;
        }
    }
    return false;
}

void acs_log_access(uint8_t fc, uint16_t id, bool granted, const char *reason) {
    if (acs.log_count >= acs.log_capacity) {
        acs.log_capacity *= 2;
        acs.logs = realloc(acs.logs, acs.log_capacity * sizeof(AccessLog));
    }
    
    AccessLog *log = &acs.logs[acs.log_count];
    log->timestamp = time(NULL);
    log->fc = fc;
    log->card_id = id;
    log->granted = granted;
    strncpy(log->reason, reason, sizeof(log->reason) - 1);
    log->reason[sizeof(log->reason) - 1] = '\0';
    
    acs.log_count++;
}

bool acs_try_access(uint32_t wiegand_code) {
    // Parse card
    WiegandCard card = parse_wiegand26(wiegand_code);
    
    if (!card.valid) {
        acs_log_access(0, 0, false, "Invalid Wiegand format");
        acs.failed_attempts++;
        return false;
    }
    
    // Check authorization
    bool authorized = acs_is_authorized(card.facility_code, card.card_id);
    
    if (authorized) {
        acs_log_access(card.facility_code, card.card_id, true, "Access granted");
        acs.door_locked = false;
        acs.successful_access++;
        printf("✅ ACCESS GRANTED: FC=%u, ID=%u\n", card.facility_code, card.card_id);
        return true;
    } else {
        acs_log_access(card.facility_code, card.card_id, false, "Unauthorized card");
        acs.failed_attempts++;
        printf("❌ ACCESS DENIED: FC=%u, ID=%u (unauthorized)\n", 
               card.facility_code, card.card_id);
        return false;
    }
}

void acs_print_logs(int last_n) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      ACCESS CONTROL LOG                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int start = (acs.log_count > last_n) ? acs.log_count - last_n : 0;
    
    for (int i = start; i < acs.log_count; i++) {
        AccessLog *log = &acs.logs[i];
        
        char time_str[64];
        struct tm *tm_info = localtime(&log->timestamp);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        
        const char *emoji = log->granted ? "✅" : "❌";
        
        printf("%s [%s] FC=%3u ID=%5u | %s\n",
               emoji, time_str, log->fc, log->card_id, log->reason);
    }
    
    printf("\n");
}

void acs_print_statistics(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    ACCESS CONTROL STATISTICS                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("📊 System Status:\n");
    printf("   Door: %s\n", acs.door_locked ? "🔒 LOCKED" : "🔓 UNLOCKED");
    printf("   Authorized Cards: %d\n", acs.authorized_count);
    printf("   Total Attempts: %d\n", acs.log_count);
    printf("\n");
    
    printf("📈 Access Statistics:\n");
    printf("   Successful: %d (%.1f%%)\n", 
           acs.successful_access, 
           (float)acs.successful_access / acs.log_count * 100);
    printf("   Failed: %d (%.1f%%)\n",
           acs.failed_attempts,
           (float)acs.failed_attempts / acs.log_count * 100);
    printf("\n");
    
    if (acs.failed_attempts > 10) {
        printf("⚠️  WARNING: High number of failed attempts detected!\n");
        printf("   Possible brute force attack in progress.\n\n");
    }
}

void acs_cleanup(void) {
    free(acs.authorized_cards);
    free(acs.logs);
}

// ============================================================================
// MAIN & TESTS
// ============================================================================

void test_wiegand() {
    printf("\n=== WIEGAND TEST ===\n");
    
    uint32_t code = generate_wiegand26(123, 45678);
    printf("Generated: FC=123, ID=45678 -> 0x%08X\n", code);
    
    WiegandCard card = parse_wiegand26(code);
    printf("Parsed: FC=%u, ID=%u, Valid=%s\n", 
           card.facility_code, card.card_id, card.valid ? "YES" : "NO");
    
    uint32_t known = 0x1A2B3C4;
    WiegandCard card2 = parse_wiegand26(known);
    printf("Known code 0x%08X: FC=%u, ID=%u, Valid=%s\n",
           known, card2.facility_code, card2.card_id, 
           card2.valid ? "YES" : "NO");
}

void test_modbus() {
    printf("\n=== MODBUS TEST ===\n");
    
    uint8_t test_data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A};
    uint16_t crc = modbus_crc16(test_data, sizeof(test_data));
    printf("CRC16 of request: 0x%04X (expected: 0xC5CD)\n", crc);
    
    uint8_t request[8];
    int len = modbus_read_holding_registers(request, 1, 0, 10);
    
    printf("Modbus request (%d bytes): ", len);
    for (int i = 0; i < len; i++) {
        printf("%02X ", request[i]);
    }
    printf("\n");
}

void test_rfid_emulator() {
    printf("\n=== RFID EMULATOR TEST ===\n");
    
    FILE *f = fopen("test_cards.txt", "w");
    if (f) {
        fprintf(f, "123,45678\n");
        fprintf(f, "123,45679\n");
        fprintf(f, "200,12345\n");
        fclose(f);
    }
    
    CardList list;
    int count = load_cards("test_cards.txt", &list);
    printf("Loaded %d cards\n", count);
    
    if (count > 0) {
        generate_all_codes(&list);
        print_codes(&list);
    }
}

void test_access_control() {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║            ACCESS CONTROL SYSTEM TEST (v2.0 Enhanced)            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    
    // Initialize ACS
    acs_init();
    
    // Load authorized cards
    CardList authorized;
    authorized.count = 3;
    authorized.cards[0].fc = 123;
    authorized.cards[0].id = 45678;
    authorized.cards[1].fc = 123;
    authorized.cards[1].id = 45679;
    authorized.cards[2].fc = 200;
    authorized.cards[2].id = 12345;
    
    generate_all_codes(&authorized);
    acs_load_authorized(&authorized);
    
    printf("\n✅ Access control system initialized\n");
    printf("   Authorized %d cards\n\n", authorized.count);
    
    // Test valid access
    printf("=== Testing Authorized Access ===\n");
    acs_try_access(authorized.cards[0].wiegand_code);
    acs_try_access(authorized.cards[2].wiegand_code);
    
    // Test unauthorized access
    printf("\n=== Testing Unauthorized Access ===\n");
    uint32_t unauthorized = generate_wiegand26(99, 99999);
    acs_try_access(unauthorized);
    
    // Simulate brute force
    printf("\n=== Simulating Brute Force Attack ===\n");
    for (int i = 0; i < 15; i++) {
        uint32_t fake = generate_wiegand26(100, 10000 + i);
        acs_try_access(fake);
    }
    
    // Print results
    acs_print_logs(20);
    acs_print_statistics();
    
    printf("🏆 Achievement Unlocked: \"Access Control Expert\"\n");
    printf("   You've mastered industrial access control systems.\n\n");
    
    // Cleanup
    acs_cleanup();
}

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║  Episode 22: Industrial Protocols & Access Control - SOLUTION    ║\n");
    printf("║                 Operation MOONLIGHT - Season 6                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n🔧 Location: Novosibirsk Akademgorodok, Russia\n");
    printf("🎯 Mission: Reverse engineer RFID/NFC access control systems\n");
    
    if (argc > 1) {
        if (strcmp(argv[1], "--wiegand") == 0) {
            test_wiegand();
        } else if (strcmp(argv[1], "--modbus") == 0) {
            test_modbus();
        } else if (strcmp(argv[1], "--rfid") == 0) {
            test_rfid_emulator();
        } else if (strcmp(argv[1], "--acs") == 0) {
            test_access_control();
        } else {
            printf("\nUsage: %s [--wiegand|--modbus|--rfid|--acs]\n", argv[0]);
            printf("\nOptions:\n");
            printf("  --wiegand   Test Wiegand 26-bit protocol\n");
            printf("  --modbus    Test Modbus RTU protocol\n");
            printf("  --rfid      Test RFID card emulator\n");
            printf("  --acs       Test Access Control System (full demo)\n");
            printf("\n  (no args)   Run all tests\n\n");
        }
    } else {
        test_wiegand();
        test_modbus();
        test_rfid_emulator();
        test_access_control();
    }
    
    printf("\n💡 Mission Complete!\n");
    printf("   You've successfully reverse engineered the access control system.\n\n");
    printf("➡️  Next: Episode 23 - IP Cameras & Video Surveillance\n");
    printf("    Analyze RTSP streams and find blind spots.\n\n");
    
    return 0;
}
