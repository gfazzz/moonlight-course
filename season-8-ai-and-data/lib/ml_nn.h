/*
 * OPERATION MOONLIGHT — библиотека Season 8: нейросеть
 * Извлечено из s08e06 (перцептрон) и s08e07 (backprop).
 * exp реализован вручную — ради воспроизводимости и отсутствия -lm.
 */
#ifndef ML_NN_H
#define ML_NN_H

#define ML_NN_IN 3
#define ML_NN_HID 6

typedef struct {
    double w1[ML_NN_IN][ML_NN_HID], b1[ML_NN_HID];
    double w2[ML_NN_HID], b2;
} MlNet;

double ml_exp(double x);                 /* приведение аргумента + ряд Тейлора */
double ml_sigmoid(double z);

void   ml_nn_init(MlNet *n, unsigned long seed);
double ml_nn_forward(const MlNet *n, const double *x, double *hidden);
/* Один шаг обучения по примеру (backprop). Возврат — выход до обновления. */
double ml_nn_train_step(MlNet *n, const double *x, double target, double lr);

#endif /* ML_NN_H */
