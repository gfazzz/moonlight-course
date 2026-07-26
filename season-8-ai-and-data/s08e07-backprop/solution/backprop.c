/*
 * OPERATION MOONLIGHT — s08e07 "Backprop: скрытый слой и XOR"
 * Эталонное решение: backprop.c
 *
 * Концепт серии: один нейрон не решает XOR (s08e06). Скрытый слой преобразует
 *                пространство признаков так, что задача становится линейно
 *                разделимой. Но как обучать веса, не связанные с выходом напрямую?
 *                ЦЕПНОЕ ПРАВИЛО: ошибка выхода протаскивается назад через слои,
 *                умножаясь на производные. Это и есть backpropagation.
 * Задача: сеть 2-H-1, обучить её XOR, разобрать формулы прямого и обратного хода.
 *
 * Математика (сеть 2-H-1, сигмоида везде, кросс-энтропия на выходе):
 *   прямой ход:   h_j = s(Σ_i w1[i][j]*x_i + b1_j)
 *                 out = s(Σ_j w2_j*h_j + b2)
 *   обратный ход: d_out  = out - y                      (см. s08e06)
 *                 d_h_j  = d_out * w2_j * h_j*(1 - h_j) (производная сигмоиды)
 *   градиенты:    dw2_j    = d_out * h_j        db2 = d_out
 *                 dw1[i][j] = d_h_j * x_i       db1_j = d_h_j
 */
#include <stdio.h>

#define NIN 2
#define NH  4
#define NS  4
#define EPOCHS 60000

static double my_exp(double x) {          /* без -lm, детерминированно */
    if (x < -30.0) return 0.0;
    if (x > 30.0) x = 30.0;
    int k = 0;
    while (x > 0.5 || x < -0.5) { x /= 2.0; k++; }
    double term = 1.0, sum = 1.0;
    for (int i = 1; i <= 16; i++) { term *= x / i; sum += term; }
    while (k-- > 0) sum *= sum;
    return sum;
}
static double s(double z) { return 1.0 / (1.0 + my_exp(-z)); }

static const double X[NS][NIN] = {{0,0}, {0,1}, {1,0}, {1,1}};
static const double Y[NS]      = { 0,     1,     1,     0   };   /* XOR */

/* Веса. Стартовые значения ФИКСИРОВАНЫ и несимметричны — иначе скрытые нейроны
   будут получать одинаковые градиенты и навсегда останутся одинаковыми.
   Скрытых нейронов взято 4, хотя теоретически для XOR хватает 2: с двумя спуск
   застревает в локальном минимуме примерно в 40% запусков (замер — в theory.md). */
static double w1[NIN][NH] = {{ 0.50, -0.40,  0.912, -0.733},
                             {-0.30,  0.80, -0.641,  0.257}};
static double b1[NH]      = { 0.10, -0.20,  0.334, -0.118};
static double w2[NH]      = { 0.70, -0.60,  0.425, -0.889};
static double b2          = 0.05;

static void forward(const double *x, double *h, double *out) {
    for (int j = 0; j < NH; j++) {
        double z = b1[j];
        for (int i = 0; i < NIN; i++) z += w1[i][j] * x[i];
        h[j] = s(z);
    }
    double z = b2;
    for (int j = 0; j < NH; j++) z += w2[j] * h[j];
    *out = s(z);
}

static double loss_all(void) {            /* среднеквадратичная — только для лога */
    double sum = 0.0, h[NH], out;
    for (int k = 0; k < NS; k++) {
        forward(X[k], h, &out);
        double e = out - Y[k];
        sum += e * e;
    }
    return sum / NS;
}

int main(void) {
    printf("=== backprop: сеть 2-%d-1 учит XOR ===\n", NH);
    printf("эпох: %d, lr = 0.5, старт весов фиксирован\n\n", EPOCHS);

    const double lr = 0.5;

    printf("эпоха        loss\n");
    for (int e = 0; e <= EPOCHS; e++) {
        if (e % 12000 == 0) printf("%6d   %9.6f\n", e, loss_all());
        if (e == EPOCHS) break;

        for (int k = 0; k < NS; k++) {            /* по одному примеру: SGD */
            double h[NH], out;
            forward(X[k], h, &out);

            /* --- обратный ход --- */
            double d_out = out - Y[k];            /* градиент по входу выходного нейрона */

            double d_h[NH];
            for (int j = 0; j < NH; j++)
                d_h[j] = d_out * w2[j] * h[j] * (1.0 - h[j]);   /* цепное правило */

            /* --- обновление (выходной слой) --- */
            for (int j = 0; j < NH; j++) w2[j] -= lr * d_out * h[j];
            b2 -= lr * d_out;

            /* --- обновление (скрытый слой) --- */
            for (int j = 0; j < NH; j++) {
                for (int i = 0; i < NIN; i++) w1[i][j] -= lr * d_h[j] * X[k][i];
                b1[j] -= lr * d_h[j];
            }
        }
    }

    printf("\n--- проверка на XOR ---\n");
    int correct = 0;
    for (int k = 0; k < NS; k++) {
        double h[NH], out;
        forward(X[k], h, &out);
        int pred = (out >= 0.5) ? 1 : 0;
        if (pred == (int)Y[k]) correct++;
        printf("  (%d,%d) -> out = %.4f -> %d, ожидалось %d %s\n",
               (int)X[k][0], (int)X[k][1], out, pred, (int)Y[k],
               pred == (int)Y[k] ? "" : "  <-- ошибка");
    }
    printf("  верно: %d из %d (%.0f%%)\n\n", correct, NS, 100.0 * correct / NS);

    printf("--- что выучил скрытый слой ---\n");
    for (int k = 0; k < NS; k++) {
        double h[NH], out;
        forward(X[k], h, &out);
        printf("  (%d,%d) -> h = [", (int)X[k][0], (int)X[k][1]);
        for (int j = 0; j < NH; j++) printf("%s%.3f", j ? ", " : "", h[j]);
        printf("]\n");
    }

    printf("\nвывод: скрытый слой перевёл входы в пространство, где XOR разделим прямой.\n");
    printf("       один нейрон этого не мог — s08e06 давал 50%%.\n");
    return 0;
}
