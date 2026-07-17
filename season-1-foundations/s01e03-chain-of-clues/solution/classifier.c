/*
 * OPERATION MOONLIGHT — s01e02 "Цепочка зацепок"
 * Эталонное решение: classifier.c
 *
 * Концепт серии: ветвление — if/else, switch, операторы сравнения и логики.
 * Задача: разобрать цепочку зацепок (data.dat), отделить настоящие от подделок.
 *
 * Формат записи: <тип> <v1> <v2> <timestamp>
 *   C lat lon ts   — координата: lat в [-90,90], lon в [-180,180]
 *   T hh mm ts     — время: hh в [0,23], mm в [0,59]
 *   I id 0 ts      — идентификатор: id > 0
 * Подделка Призрака: координата с широтой 99.99 (вне диапазона).
 */

#include <stdio.h>

int main(void) {
    printf("=== DATA CLASSIFIER v2.0 ===\n");
    printf("Pulling the chain of clues...\n\n");

    char type;
    int coords = 0, times = 0, ids = 0;
    int valid = 0, invalid = 0, total = 0;

    while (scanf(" %c", &type) == 1) {
        total++;

        switch (type) {
            case 'C': {
                double lat, lon; long ts;
                if (scanf("%lf %lf %ld", &lat, &lon, &ts) != 3) { printf("[BAD] COORD  parse error\n"); invalid++; break; }
                coords++;
                if (lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon <= 180.0) {
                    printf("[OK ] COORD  %.4f, %.4f\n", lat, lon);
                    valid++;
                } else {
                    printf("[BAD] COORD  %.4f, %.4f  (out of range%s)\n",
                           lat, lon, (lat > 90.0 ? " — planted marker" : ""));
                    invalid++;
                }
                break;
            }
            case 'T': {
                int hh, mm; long ts;
                if (scanf("%d %d %ld", &hh, &mm, &ts) != 3) { printf("[BAD] TIME   parse error\n"); invalid++; break; }
                times++;
                if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) {
                    printf("[OK ] TIME   %02d:%02d\n", hh, mm);
                    valid++;
                } else {
                    printf("[BAD] TIME   %02d:%02d  (out of range)\n", hh, mm);
                    invalid++;
                }
                break;
            }
            case 'I': {
                int id, dummy; long ts;
                if (scanf("%d %d %ld", &id, &dummy, &ts) != 3) { printf("[BAD] ID     parse error\n"); invalid++; break; }
                ids++;
                if (id > 0) {
                    printf("[OK ] ID     %d\n", id);
                    valid++;
                } else {
                    printf("[BAD] ID     %d  (must be positive)\n", id);
                    invalid++;
                }
                break;
            }
            default:
                printf("[BAD] UNKNOWN type '%c'\n", type);
                invalid++;
                while (getchar() != '\n' && !feof(stdin)) { /* skip rest of line */ }
                break;
        }
    }

    printf("\n=== STATS ===\n");
    printf("Records: %d | valid: %d  invalid: %d\n", total, valid, invalid);
    printf("Coords: %d  Time: %d  ID: %d\n", coords, times, ids);
    if (invalid > 0)
        printf("\nWARNING: %d planted/corrupt record(s). Ghost was right.\n", invalid);
    printf("\n=== CLASSIFICATION COMPLETE ===\n");
    return 0;
}
