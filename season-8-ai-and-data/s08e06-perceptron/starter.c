/*
 * OPERATION MOONLIGHT — s08e06 "Перцептрон"
 * Каркас. Скопируй в artifacts/perceptron.c и закрой TODO.
 *
 *   cp starter.c artifacts/perceptron.c
 *   make test
 *
 * Нейрон: p = sigmoid(w1*x1 + w2*x2 + b).
 * Градиент кросс-энтропии по z удивительно прост: (p - y).
 */
#include <stdio.h>

/* Свой exp: приведение аргумента + ряд Тейлора + возведения в квадрат. */
static double my_exp(double x) {
    if (x < -30.0) return 0.0;
    if (x > 30.0) x = 30.0;
    int k = 0;
    while (x > 0.5 || x < -0.5) { x /= 2.0; k++; }
    double term = 1.0, sum = 1.0;
    for (int i = 1; i <= 16; i++) { term *= x / i; sum += term; }
    while (k-- > 0) sum *= sum;
    return sum;
}

/* TODO 1: sigmoid(z) = 1 / (1 + my_exp(-z)). */
static double sigmoid(double z);

#define NS 4
static const double X[NS][2] = {{0,0}, {0,1}, {1,0}, {1,1}};

/* TODO 2: train(name, y, epochs, lr, verbose):
     старт: w1 = 0.3, w2 = -0.2, b = 0.1  (фиксированный — ради воспроизводимости)
     каждая эпоха: по всем примерам z = w1*x1 + w2*x2 + b; p = sigmoid(z);
                   err = p - y[i]; накопить dw1 += err*x1, dw2 += err*x2, db += err;
                   затем w1 -= lr*dw1/NS; w2 -= lr*dw2/NS; b -= lr*db/NS;
     в конце — оценка и печать (формат см. tests/expected.txt):
       "--- %s ---"
       "  вход (%d,%d): p = %.3f -> %d, ожидалось %d %s"   ("  <-- ошибка" если неверно)
       "  веса: w1 = %+.3f, w2 = %+.3f, b = %+.3f"
       "  верно: %d из %d (%.0f%%)\n" */
static void train(const char *name, const double *y, int epochs, double lr, int verbose);

int main(void) {
    printf("=== перцептрон: один нейрон ===\n");
    printf("нейрон: p = sigmoid(w1*x1 + w2*x2 + b), обучение 20000 эпох, lr = 0.5\n\n");

    static const double AND_Y[NS] = {0, 0, 0, 1};
    static const double OR_Y[NS]  = {0, 1, 1, 1};
    static const double XOR_Y[NS] = {0, 1, 1, 0};

    /* TODO 3: обучить на всех трёх наборах:
         "AND (линейно разделим)", "OR  (линейно разделим)", "XOR (НЕ разделим прямой)"
       затем вывод (две строки, см. expected.txt). */

    (void)AND_Y; (void)OR_Y; (void)XOR_Y; (void)train; (void)sigmoid; (void)X;
    return 0;
}

static double sigmoid(double z) { (void)z; return 0.0; /* TODO 1 */ }
static void train(const char *n, const double *y, int e, double lr, int v) {
    (void)n; (void)y; (void)e; (void)lr; (void)v; /* TODO 2 */
}
