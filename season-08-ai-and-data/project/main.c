/*
 * OPERATION MOONLIGHT — SEASON PROJECT: luna_ai
 *
 * Собирается из модулей ДВУХ сезонов:
 *   season-04/lib/ml_hash.c   — реестр признаков по имени   (s04e03)
 *   season-08/lib/ml_stats.c  — Уэлфорд, квантили, Тьюки    (s08e01, s08e02)
 *   season-08/lib/ml_nn.c     — сеть 3-6-1 и backprop       (s08e06, s08e07)
 *
 * Классификатор угроз: нормировка признаков, обучение, честные метрики.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml_csv.h"    /* Season 5 */
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

/* Контрольная выборка приходит файлом, а не порождается генератором:
   на своих же случайных числах модель проверять нельзя. Строки даны
   ровно в том виде, в каком выгрузка их отдаёт, — с шапкой, кавычками,
   CRLF и одной битой записью. */
static const char *CALIB =
    "traffic,err_rate,night_act,label\n"
    "118.4,0.31,0.72,1\r\n"
    "22.7,0.05,0.11,0\n"
    "96.2,0.44,0.58,1\n"
    "15.3,0.02,0.31,0\n"
    "131.9,0.52,0.81,1\n"
    "41.0,0.19,0.24,0\n"
    "104.5,0.28,0.66,1\n"
    "8.9,0.01,0.07,0\n"
    "77.2,0.13,0.49\n"                 /* битая: три поля вместо четырёх */
    "127.6,0.61,0.93,1\n"
    "33.1,0.09,0.18,0\n";

typedef struct {
    double mean[ML_NN_IN], sd[ML_NN_IN];   /* нормировка обучающей выборки */
    const MlNet *net;
    int rows, broken, correct;
} calib_t;

/* Обработчик записи контрольной выборки. */
static int calib_row(const ml_csv_row *row, void *user) {
    calib_t *c = (calib_t *)user;

    if (row->status != ML_CSV_OK || row->nfield != ML_NN_IN + 1) { c->broken++; return 1; }
    if (row->field[0][0] == 't') return 1;              /* шапка */

    double x[ML_NN_IN];
    for (int f = 0; f < ML_NN_IN; f++) {
        char *end = NULL;
        double v = strtod(row->field[f], &end);
        if (end == row->field[f] || *end != '\0') { c->broken++; return 1; }
        /* strtod здесь уместен: это измерения, а не деньги. Разница
           разобрана в s05e02 — сходиться при сверке они не обязаны. */
        x[f] = (v - c->mean[f]) / c->sd[f];
    }
    int truth = row->field[ML_NN_IN][0] - '0';
    int pred  = (ml_nn_forward(c->net, x, NULL) >= 0.5) ? 1 : 0;

    c->rows++;
    if (pred == truth) c->correct++;
    return 1;
}

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

    /* Нормировка запоминается: контрольную выборку надо привести к тем же
       единицам, иначе сравнение бессмысленно. */
    static double norm_mean[ML_NN_IN], norm_sd[ML_NN_IN];

    /* --- Season 8 (ml_stats): нормировка по Уэлфорду --- */
    printf("\n--- модуль Season 8 (ml_stats): нормировка признаков ---\n");
    for (int f = 0; f < ML_NN_IN; f++) {
        MlWelford w;
        ml_welford_init(&w);
        for (int i = 0; i < NALL; i++) ml_welford_push(&w, X[i][f]);
        double sd = my_sqrt(ml_welford_var(&w));
        if (sd <= 0) sd = 1.0;
        printf("  %-10s среднее %8.3f  sigma %7.3f\n", feat[f], ml_welford_mean(&w), sd);
        norm_mean[f] = ml_welford_mean(&w);
        norm_sd[f]   = sd;
        for (int i = 0; i < NALL; i++) X[i][f] = (X[i][f] - norm_mean[f]) / sd;
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

    /* --- Season 5 (ml_csv): контрольная выборка приходит файлом --- */
    printf("\n--- модуль Season 5 (ml_csv): проверка на внешних данных ---\n");
    calib_t c = { { 0 }, { 0 }, &net, 0, 0, 0 };
    for (int f = 0; f < ML_NN_IN; f++) { c.mean[f] = norm_mean[f]; c.sd[f] = norm_sd[f]; }

    FILE *cf = fmemopen((void *)CALIB, strlen(CALIB), "rb");
    long seen = cf ? ml_csv_parse(cf, ',', calib_row, &c) : 0;
    if (cf) fclose(cf);

    printf("  записей в файле:     %ld\n", seen);
    printf("  принято:             %d\n", c.rows);
    printf("  отброшено битых:     %d\n", c.broken);
    printf("  угадано:             %d из %d\n", c.correct, c.rows);
    printf("  битая строка отброшена, а не подставлена нулями: %s\n",
           c.broken == 1 ? "да" : "НЕТ");
    int calib_ok = (c.broken == 1) && (c.rows == 10) && (c.correct >= 8);
    printf("  внешняя проверка пройдена: %s\n", calib_ok ? "да" : "НЕТ");

    printf("\n--- состав сборки ---\n");
    printf("  [S4] ml_hash.c  — реестр признаков\n");
    printf("  [S5] ml_csv.c   — чтение контрольной выборки\n");
    printf("  [S8] ml_stats.c — нормировка (Уэлфорд)\n");
    printf("  [S8] ml_nn.c    — сеть и backprop\n");
    printf("  [S8] main.c     — классификатор\n");
    printf("\n5 единиц трансляции из 3 сезонов слинкованы в luna_ai.\n");

    int ok = (f1 > 0.7) && (acc > baseline) && (total == NALL - NTRAIN) && calib_ok;
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
