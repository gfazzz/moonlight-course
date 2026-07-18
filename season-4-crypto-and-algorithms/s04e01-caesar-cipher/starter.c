/*
 * OPERATION MOONLIGHT — s04e01 "Шифр Цезаря"
 * Каркас. Скопируй в artifacts/caesar.c и закрой TODO.
 *
 *   cp starter.c artifacts/caesar.c
 *   make test
 *
 * Концепт: подстановочный шифр со сдвигом; ключей всего 25 -> вскрывается перебором.
 */
#include <stdio.h>
#include <string.h>

/* TODO 1: caesar(in, out, shift) — сдвинуть буквы на shift, не-буквы оставить.
   Подсказка: для 'A'..'Z': out[i] = 'A' + (((c-'A'+shift) % 26) + 26) % 26;
   аналогично для 'a'..'z'. Не забудь out[i] = '\0' в конце. */

int main(void) {
    const char *plain = "USB CONTAINS THREE FILES TRUST EVA";
    char enc[128], dec[128];

    printf("=== CAESAR CIPHER ===\n");

    /* TODO 2: caesar(plain, enc, 7); печать (формат — см. tests/expected.txt):
         "plain:  %s\n" "shift:  +7\n" "cipher: %s\n\n" */

    /* TODO 3: caesar(enc, dec, -7); печать "decrypt with -7: %s\n\n" */

    printf("--- brute force (all 25 shifts) ---\n");
    /* TODO 4: цикл s = 1..25: caesar(enc, t, -s); печать "%2d: %s\n" */

    (void)enc; (void)dec;
    return 0;
}
