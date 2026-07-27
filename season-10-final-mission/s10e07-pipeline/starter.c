/*
 * OPERATION MOONLIGHT — s10e07 "Сквозной конвейер"
 * Каркас. Скопируй в artifacts/pipeline.c и собери конвейер.
 *
 *   cp starter.c artifacts/pipeline.c
 *   make test
 *
 * МАКСИМАЛЬНАЯ АВТОНОМИЯ. Требования — в mission.md, вывод — в tests/expected.txt.
 *
 * Конвейер: приём -> валидация -> журнал -> хранение -> агрегация -> сводка.
 *
 * Главное требование — СВЕДЕНИЕ БАЛАНСА:
 *     принято + отвергнуто == подано на вход
 * Если равенство не выполняется, значит запись исчезла молча. Это худшая
 * из возможных ошибок интеграции: система выглядит работающей, а данные теряются.
 *
 * Второе требование — конвейер не падает: переполнение хранилища и битые
 * пакеты обрабатываются как штатные исходы, обработка продолжается.
 */
#include <stdio.h>
#include <string.h>

#define NPACK    40
#define NCHAN     4
#define CAPACITY 24        /* намеренно меньше входного потока */

typedef enum {
    ST_ACCEPTED, ST_BAD_MAGIC, ST_BAD_LEN, ST_BAD_CHAN, ST_BAD_CRC, ST_NO_SPACE
} Outcome;

typedef struct { unsigned char magic, chan, len, crc; long value; } Packet;

/* Источник дан готовым: каждый 7-й пакет содержит дефект. */
static void gen(Packet *p, int i) {
    unsigned long s = 20260727UL + (unsigned long)i * 2654435761UL;
    s = s * 6364136223846793005UL + 1442695040888963407UL;
    p->magic = 0x4D;
    p->chan  = (unsigned char)((s >> 20) % NCHAN);
    p->len   = (unsigned char)(4 + (s >> 28) % 8);
    p->value = (long)((s >> 33) % 500);
    p->crc   = (unsigned char)(p->magic ^ p->chan ^ p->len);
    if (i % 7 == 0) {
        switch ((i / 7) % 4) {
            case 0: p->magic = 0x00;  break;
            case 1: p->len   = 200;   break;
            case 2: p->chan  = 99;    break;
            default: p->crc ^= 0xFF;  break;
        }
    }
}

int main(void) {
    printf("=== сквозной конвейер moonlight_core ===\n");
    printf("пакетов на входе: %d, вместимость хранилища: %d, каналов: %d\n\n",
           NPACK, CAPACITY, NCHAN);
    (void)gen;
    return 0;
}
