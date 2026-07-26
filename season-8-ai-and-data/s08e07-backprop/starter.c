/*
 * OPERATION MOONLIGHT — s08e07 "Backprop: скрытый слой и XOR"
 * Каркас. Скопируй в artifacts/backprop.c и закрой TODO.
 *
 *   cp starter.c artifacts/backprop.c
 *   make test
 *
 * Формулы (сеть 2-H-1, сигмоида везде):
 *   прямой ход:   h_j = s(Σ_i w1[i][j]*x_i + b1_j);  out = s(Σ_j w2_j*h_j + b2)
 *   обратный ход: d_out = out - y
 *                 d_h_j = d_out * w2_j * h_j*(1 - h_j)      <-- цепное правило
 *   обновление:   w2_j    -= lr * d_out * h_j;   b2   -= lr * d_out
 *                 w1[i][j] -= lr * d_h_j * x_i;  b1_j -= lr * d_h_j
 */
#include <stdio.h>

#define NIN 2
#define NH  4
#define NS  4
#define EPOCHS 60000

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
static double s(double z) { return 1.0 / (1.0 + my_exp(-z)); }

static const double X[NS][NIN] = {{0,0}, {0,1}, {1,0}, {1,1}};
static const double Y[NS]      = { 0,     1,     1,     0   };

/* Старт несимметричен — иначе скрытые нейроны получат одинаковые градиенты. */
static double w1[NIN][NH] = {{ 0.50, -0.40,  0.912, -0.733},
                             {-0.30,  0.80, -0.641,  0.257}};
static double b1[NH]      = { 0.10, -0.20,  0.334, -0.118};
static double w2[NH]      = { 0.70, -0.60,  0.425, -0.889};
static double b2          = 0.05;

/* TODO 1: forward(x, h, &out) — посчитать скрытый слой и выход по формулам выше. */
static void forward(const double *x, double *h, double *out);

/* TODO 2: loss_all() — средний квадрат ошибки по всем 4 примерам (для лога). */
static double loss_all(void);

int main(void) {
    printf("=== backprop: сеть 2-%d-1 учит XOR ===\n", NH);
    printf("эпох: %d, lr = 0.5, старт весов фиксирован\n\n", EPOCHS);

    const double lr = 0.5;

    printf("эпоха        loss\n");
    /* TODO 3: цикл по эпохам 0..EPOCHS:
         каждые 12000 эпох печатать "%6d   %9.6f" (эпоха, loss_all());
         на EPOCHS — выйти до обучения (чтобы напечатать финальный loss);
         иначе для каждого примера k: forward -> d_out = out - Y[k];
           d_h[j] = d_out * w2[j] * h[j] * (1 - h[j]);
           обновить w2, b2, затем w1, b1 (см. формулы в шапке). */

    /* TODO 4: проверка:
         "\n--- проверка на XOR ---"
         "  (%d,%d) -> out = %.4f -> %d, ожидалось %d %s"  ("  <-- ошибка" если неверно)
         "  верно: %d из %d (%.0f%%)\n" */

    /* TODO 5: что выучил скрытый слой:
         "--- что выучил скрытый слой ---"
         "  (%d,%d) -> h = [%.3f, %.3f, ...]"   (все NH значений через запятую)
       затем вывод (две строки, см. tests/expected.txt). */

    (void)forward; (void)loss_all; (void)lr;
    return 0;
}

static void forward(const double *x, double *h, double *out) {
    (void)x; (void)h; (void)out; /* TODO 1 */
}
static double loss_all(void) { return 0.0; /* TODO 2 */ }
