/*
 * OPERATION MOONLIGHT — SEASON PROJECT: luna_ai
 *
 * Собирается из модулей ДВУХ сезонов:
 *   season-4/lib/ml_hash.c   — реестр признаков по имени   (s04e03)
 *   season-8/lib/ml_stats.c  — Уэлфорд, квантили, Тьюки    (s08e01, s08e02)
 *   season-8/lib/ml_nn.c     — сеть 3-6-1 и backprop       (s08e06, s08e07)
 *
 * Классификатор угроз: нормировка признаков, обучение, честные метрики.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml_hash.h"    /* Season 4 */
#include "ml_stats.h"   /* Season 8 */
#include "ml_nn.h"      /* Season 8 */

#define NALL   300
#define NTRAIN 220

static double X[NALL][ML_NN_IN];
static double Y[NALL];
static int    idx[NALL];

static unsigned long rs = 20260727UL;
static unsigned long rnd(void) { rs = rs * 6364136223846793005UL + 1442695040888963407UL; return (rs >> 33) & 0xFFFFFF; }
static double runif(void) { return (double)rnd() / 16777216.0; }

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 40; i++) g = 0.5 * (g + x / g);
    return g;
}

int main(void) {
    printf("=== luna_ai: сборка из модулей S4 + S8 ===\n\n");

    /* --- Season 4: имена признаков через хеш --- */
    static const char *feat[ML_NN_IN] = {"traffic", "err_rate", "night_act"};
    printf("--- модуль Season 4 (ml_hash): реестр признаков ---\n");
    for (int i = 0; i < ML_NN_IN; i++)
        printf("  %-10s -> слот %zu\n", feat[i], ml_hash_bucket(feat[i], ML_NN_IN * 4));

    /* --- данные: классы пересекаются --- */
    int pos = 0;
    for (int i = 0; i < NALL; i++) {
        int threat = (runif() < 0.25) ? 1 : 0;
        pos += threat;
        if (threat) { X[i][0] = 45 + runif()*85; X[i][1] = 0.12 + runif()*0.6; X[i][2] = 0.35 + runif()*0.65; }
        else        { X[i][0] = 10 + runif()*60; X[i][1] = runif()*0.28;       X[i][2] = runif()*0.6; }
        Y[i] = threat;
        idx[i] = i;
    }

    /* --- Season 8 (ml_stats): нормировка по Уэлфорду --- */
    printf("\n--- модуль Season 8 (ml_stats): нормировка признаков ---\n");
    for (int f = 0; f < ML_NN_IN; f++) {
        MlWelford w;
        ml_welford_init(&w);
        for (int i = 0; i < NALL; i++) ml_welford_push(&w, X[i][f]);
        double sd = my_sqrt(ml_welford_var(&w));
        if (sd <= 0) sd = 1.0;
        printf("  %-10s среднее %8.3f  sigma %7.3f\n", feat[f], ml_welford_mean(&w), sd);
        for (int i = 0; i < NALL; i++) X[i][f] = (X[i][f] - ml_welford_mean(&w)) / sd;
    }
    printf("  класс «угроза»: %.1f%% выборки\n", 100.0 * pos / NALL);

    /* перемешивание (Кнут) для честного разбиения */
    for (int i = NALL - 1; i > 0; i--) {
        int j = (int)(rnd() % (unsigned long)(i + 1));
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
    }

    /* --- Season 8 (ml_nn): обучение --- */
    printf("\n--- модуль Season 8 (ml_nn): обучение сети %d-%d-1 ---\n", ML_NN_IN, ML_NN_HID);
    MlNet net;
    ml_nn_init(&net, 424242UL);
    for (int epoch = 0; epoch < 2000; epoch++)
        for (int t = 0; t < NTRAIN; t++) {
            int k = idx[t];
            ml_nn_train_step(&net, X[k], Y[k], 0.5);
        }

    int tp = 0, tn = 0, fp = 0, fn = 0;
    for (int t = NTRAIN; t < NALL; t++) {
        int k = idx[t];
        int pred = (ml_nn_forward(&net, X[k], NULL) >= 0.5) ? 1 : 0;
        int truth = (int)Y[k];
        if (pred && truth) tp++; else if (!pred && !truth) tn++;
        else if (pred && !truth) fp++; else fn++;
    }
    int total = tp + tn + fp + fn;
    double acc  = (double)(tp + tn) / total;
    double prec = (tp + fp) ? (double)tp / (tp + fp) : 0.0;
    double rec  = (tp + fn) ? (double)tp / (tp + fn) : 0.0;
    double f1   = (prec + rec) ? 2 * prec * rec / (prec + rec) : 0.0;
    double baseline = (double)(tn + fp) / total;

    printf("  матрица: TP=%d FN=%d FP=%d TN=%d\n", tp, fn, fp, tn);
    printf("  accuracy %.3f, precision %.3f, recall %.3f, F1 %.3f\n", acc, prec, rec, f1);
    printf("  «всегда норма» дала бы accuracy %.3f при recall 0.000\n", baseline);

    printf("\n--- состав сборки ---\n");
    printf("  [S4] ml_hash.c  — реестр признаков\n");
    printf("  [S8] ml_stats.c — нормировка (Уэлфорд)\n");
    printf("  [S8] ml_nn.c    — сеть и backprop\n");
    printf("  [S8] main.c     — классификатор\n");
    printf("\n4 единицы трансляции из 2 сезонов слинкованы в luna_ai.\n");

    int ok = (f1 > 0.7) && (acc > baseline) && (total == NALL - NTRAIN);
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
