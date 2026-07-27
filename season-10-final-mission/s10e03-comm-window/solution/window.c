/*
 * OPERATION MOONLIGHT — s10e03 "Окно связи"
 * Эталонное решение: window.c
 *
 * Концепт серии: знать положение спутника (s10e02) мало. Связь возможна, только
 *                пока аппарат ПОДНЯЛСЯ НАД ГОРИЗОНТОМ станции выше минимального
 *                угла места. Отсюда три расчёта:
 *                  - угол места (elevation) из геометрии «станция — спутник»;
 *                  - границы окна: моменты пересечения порога (поиск делением
 *                    отрезка пополам — надёжнее, чем шаг сетки);
 *                  - доплеровский сдвиг: частота «плывёт», приёмник обязан
 *                    подстраиваться, иначе канал развалится.
 * Задача: найти окна связи за виток, их длительность и сдвиг частоты.
 *
 * Модель упрощена до плоской задачи (станция и орбита в одной плоскости) —
 * этого достаточно, чтобы получить верную качественную картину. Без -lm.
 */
#include <stdio.h>

#define PI 3.14159265358979323846
#define MU 398600.4418
#define RE 6378.137
#define C_LIGHT 299792.458        /* км/с */

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);
    return g;
}
static double wrap(double a) {
    while (a >  PI) a -= 2.0 * PI;
    while (a < -PI) a += 2.0 * PI;
    return a;
}
static double my_sin(double x) {
    x = wrap(x);
    double t = x, s = x;
    for (int n = 1; n <= 12; n++) { t *= -x * x / ((2.0*n) * (2.0*n + 1.0)); s += t; }
    return s;
}
static double my_cos(double x) {
    x = wrap(x);
    double t = 1.0, s = 1.0;
    for (int n = 1; n <= 12; n++) { t *= -x * x / ((2.0*n - 1.0) * (2.0*n)); s += t; }
    return s;
}
static double my_asin(double z) {                 /* через ряд arcsin, |z| <= 1 */
    if (z >  1.0) z =  1.0;
    if (z < -1.0) z = -1.0;
    double term = z, sum = z, z2 = z * z;
    for (int n = 1; n <= 80; n++) {
        term *= z2 * (2.0*n - 1.0) * (2.0*n - 1.0) / ((2.0*n) * (2.0*n + 1.0));
        sum += term;
    }
    return sum;
}

/* Орбита (круговая — для окна связи эксцентриситет не принципиален). */
static const double a_orb = 7000.0;
static double period(void) { return 2.0 * PI * my_sqrt(a_orb * a_orb * a_orb / MU); }

/*
 * Угол места станции на спутник в плоской модели.
 * Станция на поверхности (радиус RE), спутник на радиусе r, угловое расстояние
 * между ними — theta. Тогда, разложив по осям «локальная вертикаль / горизонт»:
 *   вверх   = r*cos(theta) - RE
 *   вдоль   = r*sin(theta)
 *   elevation = atan2(вверх, вдоль)
 * Мы считаем через дальность и arcsin — так короче и устойчивее.
 */
static double elevation(double theta) {
    double r = a_orb;
    double up    = r * my_cos(theta) - RE;
    double along = r * my_sin(theta);
    double d = my_sqrt(up * up + along * along);      /* наклонная дальность */
    if (d <= 0.0) return -PI / 2.0;
    return my_asin(up / d);
}

static double slant_range(double theta) {
    double r = a_orb;
    double up    = r * my_cos(theta) - RE;
    double along = r * my_sin(theta);
    return my_sqrt(up * up + along * along);
}

/* Угловое положение спутника относительно станции в момент t. */
static double theta_at(double t) {
    double n = 2.0 * PI / period();
    return wrap(n * t - PI);        /* в t=0 спутник на противоположной стороне */
}

/* Поиск момента пересечения порога делением отрезка пополам. */
static double bisect_cross(double t_lo, double t_hi, double thresh) {
    for (int i = 0; i < 60; i++) {
        double mid = 0.5 * (t_lo + t_hi);
        double e_lo = elevation(theta_at(t_lo)) - thresh;
        double e_md = elevation(theta_at(mid))  - thresh;
        if ((e_lo < 0) == (e_md < 0)) t_lo = mid; else t_hi = mid;
    }
    return 0.5 * (t_lo + t_hi);
}

int main(void) {
    const double T = period();
    const double min_el = 10.0 * PI / 180.0;      /* порог видимости: 10 градусов */
    const double f0 = 2200.0;                     /* несущая, МГц (диапазон S) */

    printf("=== окно связи со спутником ===\n");
    printf("орбита: круговая, r = %.0f км, период %.1f мин\n", a_orb, T / 60.0);
    printf("порог видимости: %.0f градусов, несущая %.0f МГц\n\n", min_el * 180.0 / PI, f0);

    /* --- 1. Профиль угла места за виток --- */
    printf("--- профиль прохода (шаг 1 мин) ---\n");
    printf(" t, мин   угол места   дальность, км   видимость\n");
    const int STEPS = 98;
    int visible_samples = 0;
    for (int i = 0; i <= STEPS; i++) {
        double t = T * i / STEPS;
        double el = elevation(theta_at(t));
        int vis = (el >= min_el);
        if (vis) visible_samples++;
        if (i % 7 == 0 || (vis && i % 2 == 0))
            printf("%7.1f %11.2f %14.1f   %s\n",
                   t / 60.0, el * 180.0 / PI, slant_range(theta_at(t)), vis ? "ДА" : "нет");
    }

    /* --- 2. Границы окна: ищем пересечения порога --- */
    printf("\n--- границы окна связи ---\n");
    double t_rise = -1.0, t_set = -1.0;
    double prev = elevation(theta_at(0.0)) - min_el;
    for (int i = 1; i <= 2000; i++) {
        double t = T * i / 2000.0;
        double cur = elevation(theta_at(t)) - min_el;
        if (prev < 0 && cur >= 0) t_rise = bisect_cross(T * (i - 1) / 2000.0, t, min_el);
        if (prev >= 0 && cur < 0) t_set  = bisect_cross(T * (i - 1) / 2000.0, t, min_el);
        prev = cur;
    }

    if (t_rise >= 0 && t_set >= 0) {
        double dur = t_set - t_rise;
        printf("восход:  %.2f мин\n", t_rise / 60.0);
        printf("заход:   %.2f мин\n", t_set / 60.0);
        printf("длительность окна: %.1f с (%.2f мин)\n", dur, dur / 60.0);
        printf("доля витка в зоне видимости: %.1f%%\n", 100.0 * dur / T);

        /* Максимальный угол места — в середине прохода. */
        double t_mid = 0.5 * (t_rise + t_set);
        printf("максимальный угол места: %.2f градусов (на %.2f мин)\n",
               elevation(theta_at(t_mid)) * 180.0 / PI, t_mid / 60.0);
        printf("минимальная дальность: %.1f км\n", slant_range(theta_at(t_mid)));
    } else {
        printf("окно связи не найдено\n");
    }

    /* --- 3. Доплеровский сдвиг --- */
    printf("\n--- доплеровский сдвиг ---\n");
    printf(" t, мин   радиальная скорость, км/с   сдвиг, кГц\n");
    double dt = 1.0;                                   /* численная производная дальности */
    for (int i = 0; i <= 6; i++) {
        double t = t_rise + (t_set - t_rise) * i / 6.0;
        double d1 = slant_range(theta_at(t - dt));
        double d2 = slant_range(theta_at(t + dt));
        double vr = (d2 - d1) / (2.0 * dt);             /* > 0 — удаляется */
        double shift = -vr / C_LIGHT * f0 * 1000.0;     /* кГц */
        printf("%7.2f %25.4f %12.2f\n", t / 60.0, vr, shift);
    }

    printf("\nсдвиг меняет знак в середине прохода: спутник перестаёт приближаться\n");
    printf("и начинает удаляться. Приёмник обязан подстраивать частоту непрерывно.\n");
    return 0;
}
