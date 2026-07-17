/*
 * OPERATION MOONLIGHT — s01e07 "Файлы и аргументы"
 * Эталонное решение: decoder.c
 *
 * Концепт серии: аргументы командной строки (argc/argv) и файловый ввод
 *                (fopen/fread/fclose). Данные больше не «зашиты» в код —
 *                декодер читает их из файла, имя которого даёт пользователь.
 * Задача: ./decoder <file.enc> — прочитать зашифрованные байты, расшифровать.
 */
#include <stdio.h>
#include <stdlib.h>

#define KEY 0x42
#define BUF_MAX 4096

int main(int argc, char **argv) {
    /* argv[0] — имя программы; argv[1] — первый аргумент (имя файла). */
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file.enc>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");   /* "rb" — бинарное чтение */
    if (f == NULL) {
        perror("fopen");              /* печатает причину: No such file и т.п. */
        return 1;
    }

    unsigned char buf[BUF_MAX];
    size_t n = fread(buf, 1, BUF_MAX, f);  /* сколько байт реально прочитали */
    fclose(f);

    printf("=== MOONLIGHT DECODER — from file ===\n");
    printf("File: %s | %zu bytes\n\n", argv[1], n);

    for (size_t i = 0; i < n; i++) putchar(buf[i] ^ KEY);
    putchar('\n');
    return 0;
}
