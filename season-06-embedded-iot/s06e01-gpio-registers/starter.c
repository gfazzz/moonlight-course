/*
 * OPERATION MOONLIGHT — s06e01 "GPIO и регистры"
 * Каркас. Скопируй в artifacts/gpio.c и закрой TODO.
 *
 *   cp starter.c artifacts/gpio.c
 *   make test
 *
 * Концепт: регистр — байт в памяти, пин — бит. Управление железом = биты:
 *   set (|= 1<<n), clear (&= ~(1<<n)), toggle (^= 1<<n), read (>>n & 1).
 */
#include <stdio.h>
#include <stdint.h>

static void show(const char *name, uint8_t v) {
    printf("%-5s = 0b", name);
    for (int b = 7; b >= 0; b--) putchar((v >> b) & 1 ? '1' : '0');
    printf("  (0x%02X)\n", v);
}

enum { PIN_LED = 1, PIN_LOCK = 3, PIN_DOOR = 2, PIN_TAMPER = 6 };

int main(void) {
    uint8_t DDR  = 0x00;
    uint8_t PORT = 0x00;
    uint8_t PIN  = (1 << PIN_DOOR);

    printf("=== GPIO / hardware registers ===\n\n");

    /* TODO 1: DDR |= (1<<PIN_LED) | (1<<PIN_LOCK);
       печать "step 1: set LED(%d) and LOCK(%d) as outputs" и show("DDR", DDR). */

    /* TODO 2: door = (PIN>>PIN_DOOR)&1; tamper = (PIN>>PIN_TAMPER)&1;
       печать "\nstep 2: read inputs -> door=%d tamper=%d". */

    /* TODO 3: если door && !tamper: PORT |= (1<<PIN_LED); PORT |= (1<<PIN_LOCK);
       печать "\nstep 3: conditions OK -> LED on, LOCK energized"
       иначе "\nstep 3: unsafe -> stay locked". Затем show("PORT", PORT). */

    /* TODO 4: "\nstep 4: blink LED twice"; 4 раза: PORT ^= (1<<PIN_LED);
       печать "  blink %d: LED=%d". */

    /* TODO 5: PORT &= ~(1<<PIN_LOCK); "\nstep 5: de-energize LOCK"; show("PORT", PORT);
       lock_on = (PORT>>PIN_LOCK)&1;
       "\nresult: LOCK pin is %s -> hatch %s" (HIGH/LOW, OPEN/SECURED). */

    (void)PORT; (void)PIN;
    return 0;
}
