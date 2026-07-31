/*
 * OPERATION MOONLIGHT — s02e04 "Навигация по цепочке"
 * Эталонное решение: navigator.c
 *
 * Концепт серии: указатели на структуры и доступ через стрелку (p->field).
 *                Записи в файле лежат в произвольном порядке; порядок ЧТЕНИЯ
 *                задаёт цепочка полей next (как связный список).
 * Задача: пройти цепочку от offset 0 и собрать спрятанное сообщение.
 *
 * Формат записи (12 байт, little-endian):
 *   uint32 offset  — «логический» адрес записи
 *   uint32 value   — байт данных (символ)
 *   uint32 next    — offset следующей записи (0 = конец)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX 256

typedef struct {
    uint32_t offset;
    uint32_t value;
    uint32_t next;
} Entry;

/* Найти запись по её полю offset. Возвращает указатель на неё или NULL. */
static Entry *find(Entry *arr, int n, uint32_t off) {
    for (int i = 0; i < n; i++)
        if (arr[i].offset == off) return &arr[i];
    return NULL;
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "pointers.dat";
    FILE *f = fopen(path, "rb");
    if (f == NULL) { perror("fopen"); return 1; }

    Entry arr[MAX];
    int n = (int)fread(arr, sizeof(Entry), MAX, f);
    fclose(f);

    printf("=== POINTER NAVIGATOR ===\n");
    printf("Entries: %d | starting at offset 0x0000\n\n", n);

    char msg[MAX + 1];
    int m = 0;
    uint32_t target = 0;
    Entry *e;                       /* указатель на структуру */

    while ((e = find(arr, n, target)) != NULL) {
        char c = (e->value >= 32 && e->value < 127) ? (char)e->value : '.';
        printf("[0x%04X] value=0x%02X ('%c') -> next 0x%04X\n",
               e->offset, e->value, c, e->next);   /* доступ через стрелку p->field */
        msg[m++] = (char)e->value;
        if (e->next == 0) break;    /* конец цепочки */
        target = e->next;
    }
    msg[m] = '\0';

    printf("\nDecoded message: \"%s\"\n", msg);
    return 0;
}
