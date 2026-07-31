/*
 * OPERATION MOONLIGHT — s08e05 "Линейная регрессия и градиентный спуск"
 * Каркас. Скопируй в artifacts/regression.c и закрой TODO.
 *
 *   cp starter.c artifacts/regression.c
 *   make test
 *
 * Модель: y = w*x + b. Ошибка: MSE. Обучение: шаг против градиента.
 *   dMSE/dw = 2/N * Σ (pred - y) * x     dMSE/db = 2/N * Σ (pred - y)
 */
#include <stdio.h>

#define N 100
#define TRUE_W 2.5
#define TRUE_B 7.0

static double xs[N], ys[N];

static void make_data(void) {
    unsigned long s = 31337UL;
    for (int i = 0; i < N; i++) {
        s = s * 1103515245UL + 12345UL;
        double noise = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 2.0;
        xs[i] = i * 0.1;
        ys[i] = TRUE_W * xs[i] + TRUE_B + noise;
    }
}

/* TODO 1: mse(w, b) — среднее квадратов ошибок (pred - y). */
static double mse(double w, double b);

/* TODO 2: step(&w, &b, lr) — посчитать dw, db по формулам выше и обновить:
   *w -= lr*dw; *b -= lr*db. */
static void step(double *w, double *b, double lr);

int main(void) {
    make_data();
    printf("=== линейная регрессия: градиентный спуск ===\n");
    printf("точек: %d, истинные параметры: w = %.2f, b = %.2f\n\n", N, TRUE_W, TRUE_B);

    /* TODO 3: обучение: w = b = 0, lr = 0.02, 2001 эпоха (0..2000);
       печатать каждые 400 эпох: "%5d  %7.4f  %7.4f  %8.4f" (epoch, w, b, mse);
       заголовки:
         "--- обучение (lr = %.2f) ---"
         "эпоха      w        b       MSE" */

    /* TODO 4: точное решение (МНК):
         w_exact = (N*Σxy - Σx*Σy) / (N*Σxx - Σx*Σx);
         b_exact = (Σy - w_exact*Σx) / N;
       печать:
         "\n--- сравнение ---"
         "градиентный спуск: w = %.4f, b = %.4f, MSE = %.6f"
         "точное решение:    w = %.4f, b = %.4f, MSE = %.6f"
         "расхождение:       dw = %.5f, db = %.5f"
         "спуск сошёлся к точному решению: да|нет\n"   (dw < 0.01 && db < 0.05) */

    /* TODO 5: расходимость при lr = 1.0: 20 эпох из нуля, затем
         "--- слишком большой шаг (lr = 1.0) ---"
         "после 20 эпох MSE = огромна (расходимость)|конечна"
         "модель разошлась: да|нет"     (bad > 1e12 || bad != bad)
         "\nвывод: шаг слишком мал — учимся вечно; слишком велик — улетаем." */

    (void)mse; (void)step;
    return 0;
}

static double mse(double w, double b) { (void)w; (void)b; return 0.0; /* TODO 1 */ }
static void step(double *w, double *b, double lr) { (void)w; (void)b; (void)lr; /* TODO 2 */ }
