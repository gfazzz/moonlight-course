/*
 * OPERATION MOONLIGHT — s10e08 "Финальная операция" (ФИНАЛ КУРСА)
 * Каркас. Скопируй в artifacts/operation.c и проведи операцию.
 *
 *   cp starter.c artifacts/operation.c
 *   make test
 *
 * Новых тем здесь нет. Это ПРИЁМКА: каждая фаза проверяет навык одного сезона
 * измеримым критерием. Что именно проверять — решаешь ты; требования и
 * ожидаемый вывод — в mission.md и tests/expected.txt.
 *
 * Два правила финала:
 *   1) фаза засчитывается по КРИТЕРИЮ, а не по факту «код отработал»;
 *   2) программа возвращает 0 только если пройдены ВСЕ фазы.
 *
 * Совет: проверяй СВОЙСТВА (детерминированность, самопроверку суммы, равенство
 * последовательного и параллельного счёта), а не заученные константы —
 * это и надёжнее, и честнее.
 */
#include <stdio.h>
#include <string.h>

#define PI 3.14159265358979323846

static int phases_total = 0, phases_passed = 0;

static void phase(const char *season, const char *name, int ok, const char *detail) {
    phases_total++;
    if (ok) phases_passed++;
    printf("  [%s] %-4s %s — %s\n", ok ? "OK" : "!!", season, name, detail);
}

int main(void) {
    printf("=== OPERATION MOONLIGHT: финальная операция ===\n\n");
    (void)phase;
    printf("\n--- приёмка ---\n");
    printf("  фаз пройдено: %d из %d\n", phases_passed, phases_total);
    return phases_passed == phases_total ? 0 : 1;
}
