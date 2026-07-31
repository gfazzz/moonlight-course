/*
 * OPERATION MOONLIGHT — s08e05 "Линейная регрессия и градиентный спуск"
 * Эталонное решение: regression.c
 *
 * Концепт серии: первая модель, которая УЧИТСЯ. Прямая y = w*x + b, качество —
 *                среднеквадратичная ошибка (MSE). Градиентный спуск идёт против
 *                градиента ошибки маленькими шагами: w -= lr * dMSE/dw.
 *                Для прямой есть и точное решение (метод наименьших квадратов) —
 *                сравнив их, видно, что спуск сходится К НЕМУ.
 *                Скорость обучения решает всё: мала — ползём, велика — расходимся.
 * Задача: обучить модель спуском, сверить с аналитическим решением и показать,
 *         что происходит при слишком большом шаге.
 */
#include <stdio.h>

#define N 100
#define TRUE_W 2.5
#define TRUE_B 7.0

static double xs[N], ys[N];

/* Детерминированные данные: y = 2.5x + 7 + небольшой шум. */
static void make_data(void) {
    unsigned long s = 31337UL;
    for (int i = 0; i < N; i++) {
        s = s * 1103515245UL + 12345UL;
        double noise = ((double)((s >> 16) & 0x7FFF) / 32768.0 - 0.5) * 2.0;  /* [-1,1) */
        xs[i] = i * 0.1;
        ys[i] = TRUE_W * xs[i] + TRUE_B + noise;
    }
}

static double mse(double w, double b) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) { double e = (w * xs[i] + b) - ys[i]; sum += e * e; }
    return sum / N;
}

/* Один шаг спуска. Градиенты MSE: dw = 2/N * Σ e*x,  db = 2/N * Σ e. */
static void step(double *w, double *b, double lr) {
    double dw = 0.0, db = 0.0;
    for (int i = 0; i < N; i++) {
        double e = (*w * xs[i] + *b) - ys[i];
        dw += e * xs[i];
        db += e;
    }
    dw = 2.0 * dw / N;
    db = 2.0 * db / N;
    *w -= lr * dw;
    *b -= lr * db;
}

int main(void) {
    make_data();
    printf("=== линейная регрессия: градиентный спуск ===\n");
    printf("точек: %d, истинные параметры: w = %.2f, b = %.2f\n\n", N, TRUE_W, TRUE_B);

    /* --- 1. Обучение спуском --- */
    double w = 0.0, b = 0.0, lr = 0.02;
    printf("--- обучение (lr = %.2f) ---\n", lr);
    printf("эпоха      w        b       MSE\n");
    for (int epoch = 0; epoch <= 2000; epoch++) {
        if (epoch % 400 == 0)
            printf("%5d  %7.4f  %7.4f  %8.4f\n", epoch, w, b, mse(w, b));
        step(&w, &b, lr);
    }

    /* --- 2. Точное решение (метод наименьших квадратов) --- */
    double sx = 0, sy = 0, sxy = 0, sxx = 0;
    for (int i = 0; i < N; i++) { sx += xs[i]; sy += ys[i]; sxy += xs[i] * ys[i]; sxx += xs[i] * xs[i]; }
    double w_exact = (N * sxy - sx * sy) / (N * sxx - sx * sx);
    double b_exact = (sy - w_exact * sx) / N;

    printf("\n--- сравнение ---\n");
    printf("градиентный спуск: w = %.4f, b = %.4f, MSE = %.6f\n", w, b, mse(w, b));
    printf("точное решение:    w = %.4f, b = %.4f, MSE = %.6f\n",
           w_exact, b_exact, mse(w_exact, b_exact));

    double dw = w - w_exact, db = b - b_exact;
    if (dw < 0) dw = -dw;
    if (db < 0) db = -db;
    printf("расхождение:       dw = %.5f, db = %.5f\n", dw, db);
    printf("спуск сошёлся к точному решению: %s\n\n", (dw < 0.01 && db < 0.05) ? "да" : "нет");

    /* --- 3. Слишком большой шаг: расходимость --- */
    printf("--- слишком большой шаг (lr = 1.0) ---\n");
    double wd = 0.0, bd = 0.0;
    for (int epoch = 0; epoch < 20; epoch++) step(&wd, &bd, 1.0);
    double bad = mse(wd, bd);
    printf("после 20 эпох MSE = %s\n", (bad > 1e12 || bad != bad) ? "огромна (расходимость)" : "конечна");
    printf("модель разошлась: %s\n", (bad > 1e12 || bad != bad) ? "да" : "нет");

    printf("\nвывод: шаг слишком мал — учимся вечно; слишком велик — улетаем.\n");
    return 0;
}
