/*
 * MOONLIGHT Protocol - Episode 22 SOLUTION
 * Industrial Protocols: Wiegand & Modbus
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
// MAIN
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

int main(int argc, char *argv[]) {
    printf("MOONLIGHT - Episode 22: Industrial Protocols\n");
    printf("=============================================\n");
    
    if (argc > 1) {
        if (strcmp(argv[1], "--wiegand") == 0) {
            test_wiegand();
        } else if (strcmp(argv[1], "--modbus") == 0) {
            test_modbus();
        } else if (strcmp(argv[1], "--rfid") == 0) {
            test_rfid_emulator();
        } else {
            printf("Usage: %s [--wiegand|--modbus|--rfid]\n", argv[0]);
        }
    } else {
        test_wiegand();
        test_modbus();
        test_rfid_emulator();
    }
    
    return 0;
}
