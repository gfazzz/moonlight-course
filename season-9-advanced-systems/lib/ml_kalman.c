/* Реализация фильтра Калмана Season 9. */
#include "ml_kalman.h"

void ml_kalman_init(MlKalman *k, double p0, double q, double r) {
    k->x = 0.0; k->v = 0.0;
    k->p00 = p0; k->p01 = 0.0; k->p10 = 0.0; k->p11 = p0;
    k->q = q; k->r = r;
}

void ml_kalman_step(MlKalman *k, double z, double dt) {
    /* Предсказание: x = F x, P = F P F^T + Q */
    k->x += k->v * dt;
    double n00 = k->p00 + dt * (k->p10 + k->p01) + dt * dt * k->p11;
    double n01 = k->p01 + dt * k->p11;
    double n10 = k->p10 + dt * k->p11;
    double n11 = k->p11;
    k->p00 = n00 + k->q; k->p01 = n01; k->p10 = n10; k->p11 = n11 + k->q;

    /* Коррекция: измеряется только положение, H = [1 0] */
    double s  = k->p00 + k->r;
    double k0 = k->p00 / s;
    double k1 = k->p10 / s;
    double y  = z - k->x;
    k->x += k0 * y;
    k->v += k1 * y;

    double u00 = (1.0 - k0) * k->p00;
    double u01 = (1.0 - k0) * k->p01;
    double u10 = k->p10 - k1 * k->p00;
    double u11 = k->p11 - k1 * k->p01;
    k->p00 = u00; k->p01 = u01; k->p10 = u10; k->p11 = u11;
}

double ml_kalman_pos(const MlKalman *k) { return k->x; }
double ml_kalman_vel(const MlKalman *k) { return k->v; }
