/*
 * OPERATION MOONLIGHT — s03e06 "Контрольная сумма"
 * Каркас. Скопируй в artifacts/checksum.c и закрой TODO.
 *
 *   cp starter.c artifacts/checksum.c
 *   make test
 *
 * Концепт: Internet checksum (RFC 1071) — сумма 16-битных слов со сверткой
 *          переносов и дополнением (~). Проверка: сумма с checksum'ом даёт 0.
 */
#include <stdio.h>
#include <stdint.h>

/* TODO 1: inet_checksum(buf, len):
   uint32_t sum=0; по 16-битным словам big-endian: sum += (buf[i]<<8)|buf[i+1];
   нечётный хвост: sum += buf[len-1]<<8;
   свернуть переносы: while(sum>>16) sum=(sum&0xFFFF)+(sum>>16);
   вернуть (uint16_t)~sum. */

int main(void) {
    uint8_t ip[20] = {
        0x45, 0x00, 0x00, 0x28, 0x1C, 0x46, 0x40, 0x00,
        0x40, 0x06, 0x00, 0x00, 0xCB, 0x00, 0x71, 0x2A,
        0xC0, 0xA8, 0x01, 0x64
    };

    printf("=== INTERNET CHECKSUM ===\n");

    /* TODO 2: cs = inet_checksum(ip, 20); печать "computed checksum = 0x%04X\n". */
    /* TODO 3: вставь cs в ip[10], ip[11]; проверь: v = inet_checksum(ip,20) должно быть 0.
       Печать "verify (checksum inserted): 0x%04X -> valid|INVALID\n". */
    /* TODO 4: испорти ip[12] ^= 0xFF; снова проверь -> INVALID.
       Печать "after corrupting src IP:    0x%04X -> valid|INVALID\n". */
    (void)ip;
    return 0;
}
