/*
 * OPERATION MOONLIGHT — s10e01 "Архитектура moonlight_core"
 * Каркас. Скопируй в artifacts/core.c и построй систему.
 *
 *   cp starter.c artifacts/core.c
 *   make test
 *
 * SEASON 10: МАКСИМАЛЬНАЯ АВТОНОМИЯ.
 * Здесь нет ни пошаговых TODO, ни готовых сигнатур сверх необходимого.
 * Есть брифинг (mission.md), критерии приёмки и ожидаемый вывод
 * (tests/expected.txt). Как устроить внутренности — решаешь ты.
 *
 * Требуемое поведение — коротко:
 *   1) реестр модулей: имя, слой, зависимости, функция init;
 *   2) проверка правила слоёв (зависеть можно только вниз или на свой уровень);
 *   3) топологическая сортировка -> порядок инициализации + детект циклов;
 *   4) запуск: модуль стартует, только если все его зависимости готовы.
 *
 * Слои (снизу вверх): platform -> data -> logic -> ui.
 */
#include <stdio.h>
#include <string.h>

#define MAXMOD 16
#define MAXDEP 4

typedef enum { LAYER_PLATFORM, LAYER_DATA, LAYER_LOGIC, LAYER_UI } Layer;
static const char *LAYER_NAME[] = {"platform", "data", "logic", "ui"};

/* Три сценария, которые обязана отработать твоя система:
 *   1. корректная система из 7 модулей (см. mission.md) — запускаются все;
 *   2. storage зависит от dashboard — нарушение слоёв + цикл;
 *   3. storage не инициализируется — analytics и dashboard не стартуют.
 */

int main(void) {
    printf("=== moonlight_core: архитектура системы ===\n\n");
    (void)LAYER_NAME;
    return 0;
}
