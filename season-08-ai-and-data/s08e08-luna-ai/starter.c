/*
 * OPERATION MOONLIGHT — s08e08 "luna_ai" (финал Season 8)
 * Каркас. Скопируй в artifacts/luna_ai.c и закрой TODO.
 *
 *   cp starter.c artifacts/luna_ai.c
 *   make test
 *
 * Полный цикл: данные -> нормировка -> шаффл Кнута -> train/test ->
 *              сеть 3-6-1 с backprop -> метрики (precision/recall/F1).
 * Финал сезона: здесь собирается всё, что ты написал в e01..e07.
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

static unsigned long rs = 20260726UL;
static unsigned long rnd(void) { rs = rs * 1103515245UL + 12345UL; return (rs >> 16) & 0x7FFFFFFF; }
static double runif(void) { return (double)rnd() / 2147483648.0; }

/* TODO 1: below_fair(n) — честный выбор из [0,n) с отбраковкой (как в s08e03). */
static int below_fair(int n);

static double Xd[NALL][NIN];
static double Yd[NALL];
static int    idx[NALL];

/* TODO 2: make_dataset() — для каждого узла:
     threat = (runif() < 0.2);
     если threat: X = [45+u*85, 0.12+u*0.6, 0.35+u*0.65]
     иначе:       X = [10+u*60, 0.0+u*0.28, 0.0+u*0.6]      (диапазоны пересекаются!)
     Yd[i] = threat; idx[i] = i; */
static void make_dataset(void);

/* TODO 3: normalize() — по каждому признаку: вычесть среднее, поделить на
   стандартное отклонение (sqrt — свой, метод Ньютона). */
static void normalize(void);

/* TODO 4: shuffle_idx() — перемешать idx алгоритмом Кнута через below_fair. */
static void shuffle_idx(void);

static double w1[NIN][NH], b1[NH], w2[NH], b2;

/* TODO 5: init_weights() — w1, w2 из (runif()-0.5)*2.0; b1, b2 из (runif()-0.5)*0.5. */
static void init_weights(void);

/* TODO 6: forward(x, h) — как в s08e07, возвращает выход сети. */
static double forward(const double *x, double *h);

int main(void) {
    printf("=== luna_ai: классификатор угроз (финал Season 8) ===\n");
    printf("узлов: %d, признаков: %d, сеть: %d-%d-1\n\n", NALL, NIN, NIN, NH);

    /* TODO 7: make_dataset(); normalize(); shuffle_idx(); init_weights();
       посчитать долю класса «угроза» и напечатать:
         "класс «угроза» составляет %.1f%% выборки — классы несбалансированы"
         "разбиение: %d обучающих / %d проверочных (перемешивание Кнута)\n" */

    /* TODO 8: обучение (lr = 0.5, EPOCHS эпох) ТОЛЬКО по idx[0..NTRAIN):
       forward -> d_out = out - Y -> d_h[j] = d_out*w2[j]*h[j]*(1-h[j]) ->
       обновить w2/b2, затем w1/b1. */

    /* TODO 9: оценка по idx[NTRAIN..NALL): посчитать tp, tn, fp, fn;
       accuracy = (tp+tn)/total; precision = tp/(tp+fp); recall = tp/(tp+fn);
       F1 = 2*p*r/(p+r) (со страховкой от деления на ноль).
       печать (формат см. tests/expected.txt):
         "--- матрица ошибок (test) ---" ... "  угроза  %8d %6d" / "  норма   %8d %6d"
         "--- метрики ---", accuracy/precision/recall/F1
         "«всегда норма» дала бы accuracy %.3f при recall 0.000"
         "поэтому accuracy сама по себе бесполезна на несбалансированных классах\n" */

    /* TODO 10: финальная сборка — "--- luna_ai online ---", восемь строк "  [OK] ..."
       и "\nLUNA работает на твоём коде. Season 8 complete." */

    (void)below_fair; (void)make_dataset; (void)normalize; (void)shuffle_idx;
    (void)init_weights; (void)forward; (void)sg; (void)runif;
    return 0;
}

static int below_fair(int n) { (void)n; return 0; /* TODO 1 */ }
static void make_dataset(void) { /* TODO 2 */ }
static void normalize(void) { /* TODO 3 */ }
static void shuffle_idx(void) { /* TODO 4 */ }
static void init_weights(void) { /* TODO 5 */ }
static double forward(const double *x, double *h) { (void)x; (void)h; return 0.0; /* TODO 6 */ }
