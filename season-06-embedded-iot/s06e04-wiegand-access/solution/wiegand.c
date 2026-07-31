/*
 * OPERATION MOONLIGHT — s06e04 "Wiegand / СКУД"
 * Эталонное решение: wiegand.c
 *
 * Концепт серии: считыватель карт шлёт контроллеру 26 бит. Внутри — код объекта
 *                (facility), номер карты и ДВА бита чётности (ведущий even по старшей
 *                половине, замыкающий odd по младшей). Контроллер декодирует и решает.
 * Задача: разобрать Wiegand-26 по битам, проверить обе чётности, принять решение
 *         о доступе по списку разрешённых facility.
 *
 * Формат 26 бит (бит 25 передаётся первым, бит 0 — последним):
 *   [25]      ведущая чётность (even) по битам 24..13
 *   [24..17]  facility code (8 бит)
 *   [16..1]   card number (16 бит)
 *   [0]       замыкающая чётность (odd) по битам 12..1
 */
#include <stdio.h>
#include <stdint.h>

#define AUTH_FACILITY 42     /* разрешённый код объекта (НИИ, корпус B) */

/* XOR всех бит value в диапазоне [lo..hi] включительно. */
static int xor_bits(uint32_t v, int hi, int lo) {
    int p = 0;
    for (int i = lo; i <= hi; i++) p ^= (v >> i) & 1;
    return p;
}

/* Считыватель: собрать корректный 26-битный кадр из facility и номера карты. */
static uint32_t encode(uint8_t fac, uint16_t card) {
    uint32_t code = ((uint32_t)fac << 17) | ((uint32_t)card << 1);
    int lead  =  xor_bits(code, 24, 13);   /* even: бит = XOR старших 12 бит данных */
    int trail = !xor_bits(code, 12, 1);    /* odd:  инверсия XOR младших 12 бит */
    code |= ((uint32_t)lead << 25) | (uint32_t)trail;
    return code;
}

static void decode(const char *label, uint32_t code) {
    int lead      = (code >> 25) & 1;
    uint8_t fac   = (code >> 17) & 0xFF;
    uint16_t card = (code >> 1)  & 0xFFFF;
    int trail     =  code        & 1;

    int lead_ok  = (lead  ==  xor_bits(code, 24, 13));
    int trail_ok = (trail == !xor_bits(code, 12, 1));

    printf("%s: raw=0x%07X\n", label, code & 0x3FFFFFF);
    printf("  facility=%u  card=%u\n", fac, card);
    printf("  parity: leading(even)=%s trailing(odd)=%s\n",
           lead_ok ? "ok" : "BAD", trail_ok ? "ok" : "BAD");

    if (!lead_ok || !trail_ok) {
        printf("  -> DENY (corrupt card)\n\n");
    } else if (fac != AUTH_FACILITY) {
        printf("  -> DENY (facility %u not authorized)\n\n", fac);
    } else {
        printf("  -> GRANT\n\n");
    }
}

int main(void) {
    printf("=== Wiegand-26 access control (authorized facility %d) ===\n\n", AUTH_FACILITY);

    /* 1. Легальный пропуск сотрудника НИИ. */
    uint32_t good = encode(42, 1337);
    decode("badge A (valid)", good);

    /* 2. Тот же кадр с одним перевёрнутым битом данных -> чётность не сойдётся. */
    uint32_t tampered = good ^ (1u << 5);
    decode("badge B (tampered)", tampered);

    /* 3. Клон с верной чётностью, но чужой facility -> не в списке доступа. */
    uint32_t foreign = encode(99, 5);
    decode("badge C (cloned)", foreign);

    return 0;
}
