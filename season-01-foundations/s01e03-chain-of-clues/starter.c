/*
 * OPERATION MOONLIGHT — s01e02 "Цепочка зацепок"
 * Каркас задачи. Скопируй в artifacts/classifier.c и закрой TODO.
 *
 *   cp starter.c artifacts/classifier.c
 *   make test
 *
 * Концепт: ветвление — if/else, switch, сравнение и логика (&&, ||).
 * Задача: прочитать записи, определить тип, проверить диапазоны,
 *         пометить подделки (например, широта 99.99 — маркер Призрака).
 *
 * Формат: <тип> <v1> <v2> <timestamp>
 *   C lat lon ts | T hh mm ts | I id 0 ts
 */

#include <stdio.h>

int main(void) {
    printf("=== DATA CLASSIFIER v2.0 ===\n");
    printf("Pulling the chain of clues...\n\n");

    char type;
    int coords = 0, times = 0, ids = 0;
    int valid = 0, invalid = 0, total = 0;

    /* Читаем тип записи в цикле. " %c" пропускает пробелы/переводы строк. */
    while (scanf(" %c", &type) == 1) {
        total++;

        /* TODO: switch(type) по 'C' / 'T' / 'I', иначе default.
         *
         * case 'C': scanf("%lf %lf %ld", &lat, &lon, &ts);
         *           valid, если lat в [-90,90] И lon в [-180,180];
         *           иначе [BAD] (широта > 90 — "planted marker").
         * case 'T': scanf("%d %d %ld", &hh, &mm, &ts);
         *           valid, если hh в [0,23] И mm в [0,59].
         * case 'I': scanf("%d %d %ld", &id, &dummy, &ts);
         *           valid, если id > 0.
         * default:  [BAD] UNKNOWN; пропусти остаток строки:
         *           while (getchar() != '\n' && !feof(stdin)) { }
         *
         * Не забудь считать coords/times/ids и valid/invalid.
         * Формат строки вывода — как в tests/expected.txt:
         *   printf("[OK ] COORD  %.4f, %.4f\n", lat, lon);
         *   printf("[BAD] TIME   %02d:%02d  (out of range)\n", hh, mm);
         */
    }

    printf("\n=== STATS ===\n");
    printf("Records: %d | valid: %d  invalid: %d\n", total, valid, invalid);
    printf("Coords: %d  Time: %d  ID: %d\n", coords, times, ids);
    /* TODO: если invalid > 0 — предупреждение про подделки. */
    printf("\n=== CLASSIFICATION COMPLETE ===\n");
    return 0;
}
