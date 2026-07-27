/*
 * OPERATION MOONLIGHT — s09e07 "Радар и фильтр Калмана"
 * Каркас. Скопируй в artifacts/kalman.c и реализуй.
 *
 *   cp starter.c artifacts/kalman.c
 *   make test
 *
 * Требования — в mission.md, ожидаемые числа — в tests/expected.txt.
 *
 * Состояние: [x, v]. Модель постоянной скорости: F = [[1, dt], [0, 1]].
 * Измеряется ТОЛЬКО координата: H = [1, 0].
 *
 * Цикл фильтра:
 *   предсказание:  x = x + v*dt;   P = F P F^T + Q
 *   коррекция:     S = p00 + R;  K0 = p00/S;  K1 = p10/S
 *                  y = z - x;    x += K0*y;   v += K1*y
 *                  P = (I - K H) P
 *
 * Внимание: скалярной P недостаточно. Скорость выучивается ИМЕННО через
 * внедиагональные элементы p01/p10 — без них K1 всегда 0 и v не сойдётся.
 */
#include <stdio.h>

#define STEPS 30
#define DT    1.0

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 40; i++) g = 0.5 * (g + x / g);
    return g;
}

static unsigned long rs = 424242UL;
static double uniform(void) {
    rs = rs * 1103515245UL + 12345UL;
    return (double)((rs >> 16) & 0x7FFF) / 32768.0;
}
static double gauss(double sigma) {
    double acc = 0.0;
    for (int i = 0; i < 12; i++) acc += uniform();
    return (acc - 6.0) * sigma;
}

int main(void) {
    double true_x = 100.0, true_v = 12.0;
    const double R = 25.0;      /* дисперсия шума радара (sigma = 5 м) */
    const double Q = 0.05;      /* дисперсия шума процесса */

    double x = 0.0, v = 0.0;
    double p00 = 500.0, p01 = 0.0, p10 = 0.0, p11 = 500.0;

    printf("=== фильтр Калмана: сопровождение цели ===\n");
    printf("истина: старт %.0f м, скорость %.0f м/с; шум радара sigma=%.0f м\n",
           true_x, true_v, my_sqrt(R));
    printf("старт фильтра: x=%.0f v=%.0f, P=%.0f (полная неопределённость)\n\n", x, v, p00);

    printf("шаг  истина  измерение   оценка   скорость     K0     |изм-ист|  |оцен-ист|\n");
    printf("--------------------------------------------------------------------------\n");

    double sum_meas_err = 0.0, sum_est_err = 0.0;

    for (int k = 1; k <= STEPS; k++) {
        true_x += true_v * DT;
        double z = true_x + gauss(my_sqrt(R));

        /* TODO: предсказание и коррекция — см. формулы в шапке.
           Обнови x, v, p00, p01, p10, p11 и посчитай K0. */
        double K0 = 0.0;

        double meas_err = z - true_x; if (meas_err < 0) meas_err = -meas_err;
        double est_err  = x - true_x; if (est_err  < 0) est_err  = -est_err;
        sum_meas_err += meas_err;
        sum_est_err  += est_err;

        if (k <= 8 || k % 6 == 0)
            printf("%3d %8.1f %9.1f %9.1f %9.2f %8.3f %9.2f %10.2f\n",
                   k, true_x, z, x, v, K0, meas_err, est_err);
    }

    printf("\nсредняя ошибка измерения: %.2f м\n", sum_meas_err / STEPS);
    printf("средняя ошибка оценки:    %.2f м\n", sum_est_err / STEPS);
    printf("фильтр точнее сырого радара в %.1f раза\n", sum_meas_err / sum_est_err);
    printf("оценённая скорость: %.2f м/с (истинная %.0f)\n\n", v, true_v);

    printf("K0 падает от почти 1 до малых значений: чем увереннее фильтр,\n");
    printf("тем меньше он доверяет очередному измерению.\n");
    return 0;
}
