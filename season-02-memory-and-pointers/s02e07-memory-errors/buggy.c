/*
 * OPERATION MOONLIGHT — s02e07 "Ошибки памяти"
 * УЧЕБНЫЙ ФАЙЛ (не для сдачи): четыре классические ошибки памяти.
 * Изучи каждую, потом напиши корректную версию в artifacts/safe.c.
 *
 * НЕ компилируй это как решение — здесь намеренные баги.
 * Хочешь увидеть, как их ловит инструмент:
 *   gcc -fsanitize=address -g buggy.c -o buggy && ./buggy
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    memcpy(p, s, n);
    return p;
}

int main(void) {
    /* БАГ 1 — УТЕЧКА (leak): выделили, но не освободили. */
    char *a = dup_str("viktor");
    printf("%s\n", a);
    /* free(a);  <- забыли: память потеряна */

    /* БАГ 2 — USE-AFTER-FREE: используем после free. */
    char *b = dup_str("luna");
    free(b);
    printf("%s\n", b);          /* b уже освобождён — читаем чужое */

    /* БАГ 3 — DOUBLE-FREE: освобождаем дважды. */
    char *c = dup_str("ghost");
    free(c);
    free(c);                    /* второй free портит кучу */

    /* БАГ 4 — ВЫХОД ЗА ГРАНИЦЫ (buffer overflow): пишем мимо блока. */
    char *d = malloc(4);
    strcpy(d, "too long");      /* 9 байт в блок на 4 — переполнение */
    free(d);

    return 0;
}
