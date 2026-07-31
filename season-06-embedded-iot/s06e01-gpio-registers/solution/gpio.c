/*
 * OPERATION MOONLIGHT — s06e01 "GPIO и регистры"
 * Эталонное решение: gpio.c
 *
 * Концепт серии: аппаратный регистр — это просто байт в памяти, а каждый пин —
 *                отдельный бит. Управление железом = битовые операции:
 *                set (|= 1<<n), clear (&= ~(1<<n)), toggle (^= 1<<n), read (>>n & 1).
 * Задача: сконфигурировать порт, зажечь/переключить выводы, прочитать входы,
 *         снять реле замка люка по условию датчиков.
 *
 * Модель — AVR-подобная: DDR (направление), PORT (выход), PIN (вход).
 */
#include <stdio.h>
#include <stdint.h>

/* Печать байта в двоичном виде: биты 7..0. */
static void show(const char *name, uint8_t v) {
    printf("%-5s = 0b", name);
    for (int b = 7; b >= 0; b--) putchar((v >> b) & 1 ? '1' : '0');
    printf("  (0x%02X)\n", v);
}

/* Именование выводов люка (по схеме Дмитрия). */
enum { PIN_LED = 1, PIN_LOCK = 3, PIN_DOOR = 2, PIN_TAMPER = 6 };

int main(void) {
    uint8_t DDR  = 0x00;               /* всё на вход по умолчанию */
    uint8_t PORT = 0x00;               /* выходные защёлки сброшены */
    uint8_t PIN  = (1 << PIN_DOOR);    /* датчик двери замкнут (люк закрыт), tamper=0 */

    printf("=== GPIO / hardware registers ===\n\n");

    /* Шаг 1: пины LED и LOCK — на выход. */
    DDR |= (1 << PIN_LED) | (1 << PIN_LOCK);
    printf("step 1: set LED(%d) and LOCK(%d) as outputs\n", PIN_LED, PIN_LOCK);
    show("DDR", DDR);

    /* Шаг 2: читаем входы. */
    int door   = (PIN >> PIN_DOOR) & 1;
    int tamper = (PIN >> PIN_TAMPER) & 1;
    printf("\nstep 2: read inputs -> door=%d tamper=%d\n", door, tamper);

    /* Шаг 3: если люк на месте и нет вскрытия — включаем индикатор и снимаем замок. */
    if (door && !tamper) {
        PORT |= (1 << PIN_LED);        /* индикатор ON */
        PORT |= (1 << PIN_LOCK);       /* реле замка: снять блокировку */
        printf("\nstep 3: conditions OK -> LED on, LOCK energized\n");
    } else {
        printf("\nstep 3: unsafe -> stay locked\n");
    }
    show("PORT", PORT);

    /* Шаг 4: мигнуть индикатором дважды (toggle). */
    printf("\nstep 4: blink LED twice\n");
    for (int i = 0; i < 4; i++) {
        PORT ^= (1 << PIN_LED);
        printf("  blink %d: LED=%d\n", i, (PORT >> PIN_LED) & 1);
    }

    /* Шаг 5: снять напряжение с замка (clear), проверить состояние. */
    PORT &= ~(1 << PIN_LOCK);
    printf("\nstep 5: de-energize LOCK\n");
    show("PORT", PORT);

    int lock_on = (PORT >> PIN_LOCK) & 1;
    printf("\nresult: LOCK pin is %s -> hatch %s\n",
           lock_on ? "HIGH" : "LOW", lock_on ? "OPEN" : "SECURED");
    return 0;
}
