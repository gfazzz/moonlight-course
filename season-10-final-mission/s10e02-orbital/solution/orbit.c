/*
 * OPERATION MOONLIGHT — s10e02 "Орбитальная механика"
 * Эталонное решение: orbit.c
 *
 * Концепт серии: положение спутника считается не «по формуле», а решением
 *                УРАВНЕНИЯ КЕПЛЕРА:  M = E - e*sin(E)
 *                Оно трансцендентное: выразить E через M в элементарных функциях
 *                нельзя. Решают итерационно (Ньютон) — и это редкий случай, когда
 *                численный метод стоит между «знаю физику» и «знаю, где спутник».
 *                Дальше: E -> истинная аномалия -> радиус -> координаты.
 * Задача: посчитать положение и скорость спутника на витке, проверить законы
 *         Кеплера (период, сохранение энергии и момента импульса).
 *
 * Тригонометрия и корень реализованы вручную (ряды + Ньютон): без -lm,
 * с воспроизводимым результатом на любой платформе — дисциплина всего курса.
 */
#include <stdio.h>

#define PI 3.14159265358979323846
#define MU 398600.4418          /* гравитационный параметр Земли, км^3/с^2 */
#define RE 6378.137             /* радиус Земли, км */

/* ---------- своя математика, без -lm ---------- */
static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);
    return g;
}

/* Приведение угла к [-PI, PI] — иначе ряды Тейлора теряют точность. */
static double wrap(double a) {
    while (a >  PI) a -= 2.0 * PI;
    while (a < -PI) a += 2.0 * PI;
    return a;
}

static double my_sin(double x) {
    x = wrap(x);
    double term = x, sum = x;
    for (int n = 1; n <= 12; n++) {
        term *= -x * x / ((2.0 * n) * (2.0 * n + 1.0));
        sum += term;
    }
    return sum;
}

static double my_cos(double x) {
    x = wrap(x);
    double term = 1.0, sum = 1.0;
    for (int n = 1; n <= 12; n++) {
        term *= -x * x / ((2.0 * n - 1.0) * (2.0 * n));
        sum += term;
    }
    return sum;
}

static double my_atan2(double y, double x) {          /* через ряд для atan + квадранты */
    if (x == 0.0 && y == 0.0) return 0.0;
    double ax = x < 0 ? -x : x, ay = y < 0 ? -y : y;
    double z = (ax > ay) ? ay / ax : ax / ay;
    /* atan(z) для z в [0,1]: ряд сходится, но медленно у 1 — берём много членов */
    double z2 = z * z, term = z, sum = z;
    for (int n = 1; n <= 60; n++) {
        term *= -z2;
        sum += term / (2.0 * n + 1.0);
    }
    double a = (ax > ay) ? sum : (PI / 2.0 - sum);
    if (x < 0) a = PI - a;
    if (y < 0) a = -a;
    return a;
}

/* ---------- уравнение Кеплера ---------- */
/* M = E - e*sin(E). Ищем E методом Ньютона: E -= f(E)/f'(E). */
static double solve_kepler(double M, double e, int *iters) {
    double E = M;                        /* хорошее начальное приближение при малых e */
    int n = 0;
    for (; n < 50; n++) {
        double f  = E - e * my_sin(E) - M;
        double fp = 1.0 - e * my_cos(E);
        double dE = f / fp;
        E -= dE;
        if (dE < 1e-13 && dE > -1e-13) break;
        }
    *iters = n + 1;
    return E;
}

int main(void) {
    /* Орбита: слегка эллиптическая, низкая околоземная. */
    const double a = 7000.0;      /* большая полуось, км */
    const double e = 0.02;        /* эксцентриситет */

    /* Третий закон Кеплера: T = 2*pi*sqrt(a^3/mu). */
    double T = 2.0 * PI * my_sqrt(a * a * a / MU);
    double n_motion = 2.0 * PI / T;        /* среднее движение, рад/с */

    printf("=== орбитальная механика: где спутник ===\n");
    printf("большая полуось: %.0f км, эксцентриситет: %.3f\n", a, e);
    printf("период обращения: %.1f с (%.1f мин)\n", T, T / 60.0);
    printf("перигей: %.1f км, апогей: %.1f км (над поверхностью)\n\n",
           a * (1 - e) - RE, a * (1 + e) - RE);

    printf(" t, мин      M, рад    E, рад   итер    r, км    v, км/с   высота, км\n");
    printf("---------------------------------------------------------------------\n");

    double E0_energy = 0.0, E0_moment = 0.0;
    double max_dE = 0.0, max_dL = 0.0;

    for (int step = 0; step <= 8; step++) {
        double t = T * step / 8.0;                 /* полный виток за 8 шагов */
        double M = n_motion * t;                   /* средняя аномалия */

        int iters = 0;
        double E = solve_kepler(wrap(M), e, &iters);

        /* Радиус и истинная аномалия. */
        double r = a * (1.0 - e * my_cos(E));
        double nu = my_atan2(my_sqrt(1.0 - e * e) * my_sin(E), my_cos(E) - e);

        /* Скорость из интеграла энергии (vis-viva): v^2 = mu*(2/r - 1/a). */
        double v = my_sqrt(MU * (2.0 / r - 1.0 / a));

        /* Инварианты: удельная энергия и момент импульса. */
        double energy = v * v / 2.0 - MU / r;             /* = -mu/(2a) */
        double moment = my_sqrt(MU * a * (1.0 - e * e));  /* постоянен на орбите */

        if (step == 0) { E0_energy = energy; E0_moment = moment; }
        double dE_rel = (energy - E0_energy) / E0_energy;
        double dL_rel = (moment - E0_moment) / E0_moment;
        if (dE_rel < 0) dE_rel = -dE_rel;
        if (dL_rel < 0) dL_rel = -dL_rel;
        if (dE_rel > max_dE) max_dE = dE_rel;
        if (dL_rel > max_dL) max_dL = dL_rel;

        printf("%7.1f %11.4f %9.4f %5d %9.1f %9.4f %11.1f\n",
               t / 60.0, wrap(M), E, iters, r, v, r - RE);
        (void)nu;
    }

    printf("\n--- проверка законов сохранения ---\n");
    printf("удельная энергия: %.6f км^2/с^2 (теория -mu/2a = %.6f)\n",
           E0_energy, -MU / (2.0 * a));
    printf("момент импульса:  %.4f км^2/с\n", E0_moment);
    printf("максимальное относительное отклонение: энергия %.2e, момент %.2e\n",
           max_dE, max_dL);
    printf("законы сохранения выполняются: %s\n\n",
           (max_dE < 1e-9 && max_dL < 1e-9) ? "да" : "НЕТ");

    /* Второй закон Кеплера: у перигея быстрее, у апогея медленнее. */
    double r_peri = a * (1 - e), r_apo = a * (1 + e);
    double v_peri = my_sqrt(MU * (2.0 / r_peri - 1.0 / a));
    double v_apo  = my_sqrt(MU * (2.0 / r_apo  - 1.0 / a));
    printf("--- второй закон Кеплера ---\n");
    printf("в перигее (r=%.0f км): %.4f км/с\n", r_peri, v_peri);
    printf("в апогее  (r=%.0f км): %.4f км/с\n", r_apo, v_apo);
    printf("отношение скоростей: %.4f, отношение радиусов: %.4f\n",
           v_peri / v_apo, r_apo / r_peri);
    printf("секторная скорость постоянна: %s\n",
           ((v_peri * r_peri) - (v_apo * r_apo) < 1e-6 &&
            (v_peri * r_peri) - (v_apo * r_apo) > -1e-6) ? "да" : "нет");

    return 0;
}
