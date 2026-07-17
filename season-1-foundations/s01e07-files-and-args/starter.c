/*
 * OPERATION MOONLIGHT — s01e07 "Файлы и аргументы"
 * Каркас. Скопируй в artifacts/decoder.c и закрой TODO.
 *
 *   cp starter.c artifacts/decoder.c
 *   make test        # запустит: decoder ../message.enc
 *
 * Концепт: argc/argv (аргументы командной строки) и fopen/fread/fclose.
 * Задача: ./decoder <file.enc> — прочитать байты из файла и расшифровать (XOR 0x42).
 */
#include <stdio.h>
#include <stdlib.h>

#define KEY 0x42
#define BUF_MAX 4096

int main(int argc, char **argv) {
    /* TODO 1: если argc < 2 — напечатай usage в stderr и верни 1.
       fprintf(stderr, "usage: %s <file.enc>\n", argv[0]); */

    /* TODO 2: FILE *f = fopen(argv[1], "rb");
       если f == NULL — perror("fopen"); return 1; */

    unsigned char buf[BUF_MAX];
    /* TODO 3: size_t n = fread(buf, 1, BUF_MAX, f);  затем fclose(f); */
    size_t n = 0;

    printf("=== MOONLIGHT DECODER — from file ===\n");
    printf("File: %s | %zu bytes\n\n", argv[1], n);

    /* TODO 4: цикл по n байтам — putchar(buf[i] ^ KEY); затем putchar('\n'); */

    return 0;
}
