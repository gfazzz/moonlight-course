/*
 * OPERATION MOONLIGHT — s04e01 "Шифр Цезаря"
 * Эталонное решение: caesar.c
 *
 * Концепт серии: подстановочный шифр со сдвигом. Ключ — одно число (1..25),
 *                поэтому пространство ключей крошечное и вскрывается перебором.
 * Задача: зашифровать/расшифровать сдвигом и взломать перехват брутфорсом.
 */
#include <stdio.h>
#include <string.h>

/* Сдвиг букв на shift позиций по алфавиту; не-буквы не трогаем.
   ((x % 26) + 26) % 26 — корректный модуль и для отрицательных сдвигов. */
static void caesar(const char *in, char *out, int shift) {
    size_t i;
    for (i = 0; in[i] != '\0'; i++) {
        char c = in[i];
        if (c >= 'A' && c <= 'Z')      out[i] = (char)('A' + (((c - 'A' + shift) % 26) + 26) % 26);
        else if (c >= 'a' && c <= 'z') out[i] = (char)('a' + (((c - 'a' + shift) % 26) + 26) % 26);
        else                            out[i] = c;
    }
    out[i] = '\0';
}

int main(void) {
    const char *plain = "USB CONTAINS THREE FILES TRUST EVA";
    char enc[128], dec[128];

    printf("=== CAESAR CIPHER ===\n");

    caesar(plain, enc, 7);                 /* шифруем сдвигом +7 */
    printf("plain:  %s\n", plain);
    printf("shift:  +7\n");
    printf("cipher: %s\n\n", enc);

    caesar(enc, dec, -7);                  /* тем же ключом обратно */
    printf("decrypt with -7: %s\n\n", dec);

    /* Взлом: ключей всего 25 — перебираем все. */
    printf("--- brute force (all 25 shifts) ---\n");
    for (int s = 1; s < 26; s++) {
        char t[128];
        caesar(enc, t, -s);
        printf("%2d: %s\n", s, t);
    }
    return 0;
}
