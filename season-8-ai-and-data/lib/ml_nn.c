#include "ml_nn.h"

double ml_exp(double x) {
    if (x < -30.0) return 0.0;
    if (x > 30.0) x = 30.0;
    int k = 0;
    while (x > 0.5 || x < -0.5) { x /= 2.0; k++; }
    double term = 1.0, sum = 1.0;
    for (int i = 1; i <= 16; i++) { term *= x / i; sum += term; }
    while (k-- > 0) sum *= sum;
    return sum;
}

double ml_sigmoid(double z) { return 1.0 / (1.0 + ml_exp(-z)); }

void ml_nn_init(MlNet *n, unsigned long seed) {
    unsigned long s = seed;
    for (int i = 0; i < ML_NN_IN; i++)
        for (int j = 0; j < ML_NN_HID; j++) {
            s = s * 1103515245UL + 12345UL;
            n->w1[i][j] = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 2.0;
        }
    for (int j = 0; j < ML_NN_HID; j++) {
        s = s * 1103515245UL + 12345UL;
        n->b1[j] = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 0.5;
        s = s * 1103515245UL + 12345UL;
        n->w2[j] = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 2.0;
    }
    s = s * 1103515245UL + 12345UL;
    n->b2 = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 0.5;
}

double ml_nn_forward(const MlNet *n, const double *x, double *hidden) {
    double h[ML_NN_HID];
    for (int j = 0; j < ML_NN_HID; j++) {
        double z = n->b1[j];
        for (int i = 0; i < ML_NN_IN; i++) z += n->w1[i][j] * x[i];
        h[j] = ml_sigmoid(z);
        if (hidden) hidden[j] = h[j];
    }
    double z = n->b2;
    for (int j = 0; j < ML_NN_HID; j++) z += n->w2[j] * h[j];
    return ml_sigmoid(z);
}

double ml_nn_train_step(MlNet *n, const double *x, double target, double lr) {
    double h[ML_NN_HID];
    double out = ml_nn_forward(n, x, h);
    double d_out = out - target;                  /* градиент кросс-энтропии по z */

    double d_h[ML_NN_HID];
    for (int j = 0; j < ML_NN_HID; j++)
        d_h[j] = d_out * n->w2[j] * h[j] * (1.0 - h[j]);   /* цепное правило */

    for (int j = 0; j < ML_NN_HID; j++) n->w2[j] -= lr * d_out * h[j];
    n->b2 -= lr * d_out;
    for (int j = 0; j < ML_NN_HID; j++) {
        for (int i = 0; i < ML_NN_IN; i++) n->w1[i][j] -= lr * d_h[j] * x[i];
        n->b1[j] -= lr * d_h[j];
    }
    return out;
}
