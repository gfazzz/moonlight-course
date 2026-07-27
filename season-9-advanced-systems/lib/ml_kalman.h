/*
 * OPERATION MOONLIGHT — библиотека Season 9: оценивание при шуме
 * Извлечено из s09e07 (фильтр Калмана, модель [положение, скорость]).
 */
#ifndef ML_KALMAN_H
#define ML_KALMAN_H

/* Состояние [x, v] и ковариация 2x2 поэлементно.
   Внедиагональные элементы обязательны: без них скорость не выучивается. */
typedef struct {
    double x, v;
    double p00, p01, p10, p11;
    double q;      /* шум процесса */
    double r;      /* шум измерения */
} MlKalman;

void   ml_kalman_init(MlKalman *k, double p0, double q, double r);
/* Один шаг: предсказание на dt, затем коррекция измерением z (координата). */
void   ml_kalman_step(MlKalman *k, double z, double dt);
double ml_kalman_pos(const MlKalman *k);
double ml_kalman_vel(const MlKalman *k);

#endif /* ML_KALMAN_H */
