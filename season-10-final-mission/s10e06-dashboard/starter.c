/*
 * OPERATION MOONLIGHT — s10e06 "Дашборд операции"
 * Каркас. Скопируй в artifacts/dashboard.c и реализуй.
 *
 *   cp starter.c artifacts/dashboard.c
 *   make test
 *
 * МАКСИМАЛЬНАЯ АВТОНОМИЯ. Требования — в mission.md, вид экрана — в tests/expected.txt.
 *
 * Два правила, которые здесь важнее рисования:
 *   1) МОДЕЛЬ (состояние операции) отделена от ПРЕДСТАВЛЕНИЯ (буфер кадра);
 *   2) экран показывает не только значения, но и их ВОЗРАСТ и ДОСТУПНОСТЬ.
 *      Устаревшие данные, выглядящие свежими, опаснее их отсутствия.
 *
 * Рендер идёт в буфер (приём из s09e06) — терминал не нужен.
 * Подписи ВНУТРИ экрана только ASCII: ячейка = один байт.
 */
#include <stdio.h>
#include <string.h>

#define W 62
#define H 18
#define STALE_SEC 30

typedef enum { SRC_OK, SRC_STALE, SRC_DOWN } SrcState;

typedef struct {
    const char *name;
    int   value;
    int   age_sec;
    int   alive;
} Source;

int main(void) {
    printf("=== дашборд операции ===\n");
    printf("экран %dx%d, порог устаревания %d с\n\n", W, H, STALE_SEC);
    return 0;
}
