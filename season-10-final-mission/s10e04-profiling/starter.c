/*
 * OPERATION MOONLIGHT — s10e04 "Профилирование и оптимизация"
 * Каркас. Скопируй в artifacts/profile.c и реализуй.
 *
 *   cp starter.c artifacts/profile.c
 *   make test
 *
 * МАКСИМАЛЬНАЯ АВТОНОМИЯ. Требования — в mission.md, вывод — в tests/expected.txt.
 *
 * Дисциплина оптимизации (ей и посвящена серия):
 *   1) измерить;  2) найти доминирующую часть;  3) оптимизировать ТОЛЬКО её;
 *   4) измерить снова;  5) убедиться, что результат НЕ ИЗМЕНИЛСЯ.
 *
 * Метрика — счётчик операций (глобальный `ops`), а не время: время
 * невоспроизводимо, а операции одинаковы на любой машине.
 */
#include <stdio.h>
#include <string.h>

#define NREC   4000
#define NKEYS  64

static long ops;

typedef struct { int key; long value; } Rec;
static Rec data[NREC];
static char names[NKEYS][16];

static void gen(void) {
    unsigned long s = 20260727UL;
    for (int i = 0; i < NKEYS; i++) snprintf(names[i], sizeof names[0], "src_%02d", i);
    for (int i = 0; i < NREC; i++) {
        s = s * 6364136223846793005UL + 1442695040888963407UL;
        data[i].key = (int)((s >> 33) % NKEYS);
        data[i].value = (long)((s >> 20) % 1000);
    }
}

int main(void) {
    gen();
    printf("=== профилирование конвейера телеметрии ===\n");
    printf("записей: %d, источников: %d\n\n", NREC, NKEYS);
    (void)ops;
    return 0;
}
