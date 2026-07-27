/*
 * OPERATION MOONLIGHT — s10e03 "Окно связи"
 * Каркас. Скопируй в artifacts/window.c и реализуй.
 *
 *   cp starter.c artifacts/window.c
 *   make test
 *
 * МАКСИМАЛЬНАЯ АВТОНОМИЯ: сигнатур нет. Физика и критерии — в mission.md,
 * формат вывода — в tests/expected.txt.
 *
 * Понадобится (без -lm): my_sqrt, my_sin, my_cos, my_asin.
 *
 * Суть:
 *   угол места считается из геометрии «станция (радиус RE) — спутник (радиус r)»
 *   при угловом расстоянии theta:
 *       up    = r*cos(theta) - RE          (вверх по местной вертикали)
 *       along = r*sin(theta)               (вдоль горизонта)
 *       d     = sqrt(up^2 + along^2)       (наклонная дальность)
 *       el    = asin(up / d)
 *
 *   границы окна — моменты, когда el пересекает порог: ищи делением отрезка пополам;
 *   доплеровский сдвиг: vr = d(дальность)/dt (численно), сдвиг = -vr/c * f0.
 */
#include <stdio.h>

#define PI 3.14159265358979323846
#define MU 398600.4418
#define RE 6378.137
#define C_LIGHT 299792.458

int main(void) {
    printf("=== окно связи со спутником ===\n");
    return 0;
}
