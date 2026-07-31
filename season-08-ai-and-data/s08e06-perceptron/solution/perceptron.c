/*
 * OPERATION MOONLIGHT — s08e06 "Перцептрон"
 * Эталонное решение: perceptron.c
 *
 * Концепт серии: искусственный нейрон = взвешенная сумма входов + смещение,
 *                пропущенная через НЕЛИНЕЙНОСТЬ (сигмоиду). Выход — вероятность.
 *                Обучается тем же градиентным спуском (s08e05), но по функции
 *                потерь для классификации — бинарной кросс-энтропии.
 *                Исторический предел: один нейрон задаёт ОДНУ прямую, поэтому
 *                он решает AND и OR, но НЕ решает XOR (Минский и Паперт, 1969).
 * Задача: обучить нейрон на AND и на XOR и увидеть разницу своими глазами.
 *
 * exp() реализован сам — чтобы не тянуть -lm и держать вывод воспроизводимым.
 */
#include <stdio.h>

/* exp с приведением аргумента: x/2^k -> ряд Тейлора -> k возведений в квадрат. */
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

static double sigmoid(double z) { return 1.0 / (1.0 + my_exp(-z)); }

#define NS 4
static const double X[NS][2] = {{0,0}, {0,1}, {1,0}, {1,1}};

/* Обучение одного нейрона на четырёх примерах. */
static void train(const char *name, const double *y, int epochs, double lr, int verbose) {
    double w1 = 0.3, w2 = -0.2, b = 0.1;      /* фиксированный старт: воспроизводимо */

    for (int e = 0; e < epochs; e++) {
        double dw1 = 0, dw2 = 0, db = 0;
        for (int i = 0; i < NS; i++) {
            double z = w1 * X[i][0] + w2 * X[i][1] + b;
            double p = sigmoid(z);
            double err = p - y[i];            /* градиент кросс-энтропии по z — просто (p - y) */
            dw1 += err * X[i][0];
            dw2 += err * X[i][1];
            db  += err;
        }
        w1 -= lr * dw1 / NS;
        w2 -= lr * dw2 / NS;
        b  -= lr * db  / NS;
    }

    int correct = 0;
    printf("--- %s ---\n", name);
    for (int i = 0; i < NS; i++) {
        double p = sigmoid(w1 * X[i][0] + w2 * X[i][1] + b);
        int pred = (p >= 0.5) ? 1 : 0;
        if (pred == (int)y[i]) correct++;
        if (verbose)
            printf("  вход (%d,%d): p = %.3f -> %d, ожидалось %d %s\n",
                   (int)X[i][0], (int)X[i][1], p, pred, (int)y[i],
                   pred == (int)y[i] ? "" : "  <-- ошибка");
    }
    printf("  веса: w1 = %+.3f, w2 = %+.3f, b = %+.3f\n", w1, w2, b);
    printf("  верно: %d из %d (%.0f%%)\n\n", correct, NS, 100.0 * correct / NS);
}

int main(void) {
    printf("=== перцептрон: один нейрон ===\n");
    printf("нейрон: p = sigmoid(w1*x1 + w2*x2 + b), обучение 20000 эпох, lr = 0.5\n\n");

    static const double AND_Y[NS] = {0, 0, 0, 1};
    static const double OR_Y[NS]  = {0, 1, 1, 1};
    static const double XOR_Y[NS] = {0, 1, 1, 0};

    train("AND (линейно разделим)", AND_Y, 20000, 0.5, 1);
    train("OR  (линейно разделим)", OR_Y, 20000, 0.5, 1);
    train("XOR (НЕ разделим прямой)", XOR_Y, 20000, 0.5, 1);

    printf("вывод: один нейрон задаёт одну прямую. AND и OR ею разделяются, XOR — нет.\n");
    printf("       это предел перцептрона (Минский и Паперт, 1969) — нужен скрытый слой.\n");
    return 0;
}
