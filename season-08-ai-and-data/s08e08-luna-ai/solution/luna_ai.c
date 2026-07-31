/*
 * OPERATION MOONLIGHT — s08e08 "luna_ai" (финал Season 8)
 * Эталонное решение: luna_ai.c
 *
 * Концепт серии: собрать сезон в одну работающую систему — классификатор угроз.
 *                Полный цикл ML на чистом C, без библиотек:
 *                  данные -> нормировка -> честный шаффл (s08e03) -> train/test
 *                  -> сеть 3-6-1 с backprop (s08e07) -> МЕТРИКИ, а не только accuracy.
 *                Главный урок финала: точность обманчива при несбалансированных
 *                классах. Нужны precision, recall, F1 и матрица ошибок.
 * Задача: обучить luna_ai различать «свой узел / скомпрометированный» и честно
 *         оценить качество на данных, которых модель не видела.
 *
 * Сезон в сборе: mmap-статистика (e01), выбросы (e02), шаффл Кнута (e03),
 * сортировки (e04), градиентный спуск (e05), нейрон (e06), backprop (e07).
 */
#include <stdio.h>

#define NALL   400
#define NIN    3
#define NH     6
#define NTRAIN 300
#define EPOCHS 3000

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
static double sg(double z) { return 1.0 / (1.0 + my_exp(-z)); }

/* --- детерминированный источник (LCG) + честный выбор из [0,n) --- */
static unsigned long rs = 20260726UL;
static unsigned long rnd(void) { rs = rs * 1103515245UL + 12345UL; return (rs >> 16) & 0x7FFFFFFF; }
static double runif(void) { return (double)rnd() / 2147483648.0; }
static int below_fair(int n) {                       /* без modulo bias — s08e03 */
    unsigned long range = 0x80000000UL, limit = range - (range % (unsigned long)n), r;
    do { r = rnd(); } while (r >= limit);
    return (int)(r % (unsigned long)n);
}

static double Xd[NALL][NIN];     /* признаки: пакетов/с, доля ошибок, ночная активность */
static double Yd[NALL];
static int    idx[NALL];

/* Класс 1 (угроза) — редкий: примерно каждый пятый узел. */
static void make_dataset(void) {
    for (int i = 0; i < NALL; i++) {
        int threat = (runif() < 0.2) ? 1 : 0;
        /* Диапазоны намеренно ПЕРЕСЕКАЮТСЯ: в реальности классы не разделены
           идеально, и часть узлов неотличима по этим трём признакам. */
        if (threat) {
            Xd[i][0] = 45.0 + runif() * 85.0;      /* трафик выше, но не всегда */
            Xd[i][1] = 0.12 + runif() * 0.6;       /* больше ошибок */
            Xd[i][2] = 0.35 + runif() * 0.65;      /* чаще активен ночью */
        } else {
            Xd[i][0] = 10.0 + runif() * 60.0;
            Xd[i][1] = 0.0 + runif() * 0.28;
            Xd[i][2] = 0.0 + runif() * 0.6;
        }
        Yd[i] = threat;
        idx[i] = i;
    }
}

/* Нормировка признаков: иначе шкала «пакетов/с» задавит остальные (s08e05). */
static void normalize(void) {
    for (int f = 0; f < NIN; f++) {
        double mean = 0.0;
        for (int i = 0; i < NALL; i++) mean += Xd[i][f];
        mean /= NALL;
        double m2 = 0.0;
        for (int i = 0; i < NALL; i++) { double d = Xd[i][f] - mean; m2 += d * d; }
        double sd = m2 / (NALL - 1);
        double g = sd;                              /* свой sqrt (Ньютон) */
        if (g > 0) for (int t = 0; t < 40; t++) g = 0.5 * (g + sd / g);
        if (g <= 0) g = 1.0;
        for (int i = 0; i < NALL; i++) Xd[i][f] = (Xd[i][f] - mean) / g;
    }
}

/* Перемешивание Кнута — честное разбиение на train/test (s08e03). */
static void shuffle_idx(void) {
    for (int i = NALL - 1; i > 0; i--) {
        int j = below_fair(i + 1);
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
    }
}

static double w1[NIN][NH], b1[NH], w2[NH], b2;

static void init_weights(void) {
    for (int i = 0; i < NIN; i++)
        for (int j = 0; j < NH; j++) w1[i][j] = (runif() - 0.5) * 2.0;
    for (int j = 0; j < NH; j++) { b1[j] = (runif() - 0.5) * 0.5; w2[j] = (runif() - 0.5) * 2.0; }
    b2 = (runif() - 0.5) * 0.5;
}

static double forward(const double *x, double *h) {
    for (int j = 0; j < NH; j++) {
        double z = b1[j];
        for (int i = 0; i < NIN; i++) z += w1[i][j] * x[i];
        h[j] = sg(z);
    }
    double z = b2;
    for (int j = 0; j < NH; j++) z += w2[j] * h[j];
    return sg(z);
}

int main(void) {
    printf("=== luna_ai: классификатор угроз (финал Season 8) ===\n");
    printf("узлов: %d, признаков: %d, сеть: %d-%d-1\n\n", NALL, NIN, NIN, NH);

    make_dataset();
    normalize();
    shuffle_idx();
    init_weights();

    int pos = 0;
    for (int i = 0; i < NALL; i++) pos += (int)Yd[i];
    printf("класс «угроза» составляет %.1f%% выборки — классы несбалансированы\n", 100.0 * pos / NALL);
    printf("разбиение: %d обучающих / %d проверочных (перемешивание Кнута)\n\n",
           NTRAIN, NALL - NTRAIN);

    /* --- обучение --- */
    const double lr = 0.5;
    for (int e = 0; e < EPOCHS; e++) {
        for (int t = 0; t < NTRAIN; t++) {
            int k = idx[t];
            double h[NH];
            double out = forward(Xd[k], h);
            double d_out = out - Yd[k];
            double d_h[NH];
            for (int j = 0; j < NH; j++) d_h[j] = d_out * w2[j] * h[j] * (1.0 - h[j]);
            for (int j = 0; j < NH; j++) w2[j] -= lr * d_out * h[j];
            b2 -= lr * d_out;
            for (int j = 0; j < NH; j++) {
                for (int i = 0; i < NIN; i++) w1[i][j] -= lr * d_h[j] * Xd[k][i];
                b1[j] -= lr * d_h[j];
            }
        }
    }

    /* --- оценка на ОТЛОЖЕННОЙ выборке (модель её не видела) --- */
    int tp = 0, tn = 0, fp = 0, fn = 0;
    for (int t = NTRAIN; t < NALL; t++) {
        int k = idx[t];
        double h[NH];
        int pred = (forward(Xd[k], h) >= 0.5) ? 1 : 0;
        int truth = (int)Yd[k];
        if (pred == 1 && truth == 1) tp++;
        else if (pred == 0 && truth == 0) tn++;
        else if (pred == 1 && truth == 0) fp++;
        else fn++;
    }

    int total = tp + tn + fp + fn;
    double acc = (double)(tp + tn) / total;
    double prec = (tp + fp) ? (double)tp / (tp + fp) : 0.0;
    double rec  = (tp + fn) ? (double)tp / (tp + fn) : 0.0;
    double f1   = (prec + rec) ? 2 * prec * rec / (prec + rec) : 0.0;

    printf("--- матрица ошибок (test) ---\n");
    printf("                предсказано\n");
    printf("               угроза  норма\n");
    printf("  угроза  %8d %6d\n", tp, fn);
    printf("  норма   %8d %6d\n\n", fp, tn);

    printf("--- метрики ---\n");
    printf("accuracy:  %.3f\n", acc);
    printf("precision: %.3f  (из помеченных угрозами — верны)\n", prec);
    printf("recall:    %.3f  (из реальных угроз — найдены)\n", rec);
    printf("F1:        %.3f\n\n", f1);

    /* Ориентир: модель, которая всегда говорит «норма». */
    int all_norm_correct = tn + fp;
    printf("«всегда норма» дала бы accuracy %.3f при recall 0.000\n",
           (double)all_norm_correct / total);
    printf("поэтому accuracy сама по себе бесполезна на несбалансированных классах\n\n");

    printf("--- luna_ai online ---\n");
    const char *sub[] = {
        "потоковая статистика, mmap (e01)", "устойчивые метрики, выбросы (e02)",
        "честный шаффл Кнута (e03)",        "сортировки TAOCP (e04)",
        "градиентный спуск (e05)",          "нейрон и сигмоида (e06)",
        "backprop, скрытый слой (e07)",     "метрики и классификатор (e08)"
    };
    for (int i = 0; i < 8; i++) printf("  [OK] %s\n", sub[i]);
    printf("\nLUNA работает на твоём коде. Season 8 complete.\n");
    return 0;
}
