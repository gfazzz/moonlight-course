/*
 * OPERATION MOONLIGHT — s02e04 "Навигация по цепочке"
 * Каркас. Скопируй в artifacts/navigator.c и закрой TODO.
 *
 *   cp starter.c artifacts/navigator.c
 *   make test        # запустит: navigator ../pointers.dat
 *
 * Концепт: указатель на структуру, доступ через стрелку p->field.
 * Задача: пройти цепочку записей от offset 0 по полю next, собрать сообщение.
 *
 * Запись (12 байт): uint32 offset, uint32 value (символ), uint32 next (0=конец).
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

/* TODO 1: find(Entry *arr, int n, uint32_t off) — вернуть указатель на запись
   с arr[i].offset == off, иначе NULL. */

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

    /* TODO 2: цикл — пока find(...) находит запись e:
       - напечатай "[0x%04X] value=0x%02X ('%c') -> next 0x%04X\n"
         с e->offset, e->value, печатным символом, e->next;
       - msg[m++] = (char)e->value;
       - если e->next == 0 — break; иначе target = e->next;  */

    msg[m] = '\0';
    printf("\nDecoded message: \"%s\"\n", msg);
    return 0;
}
