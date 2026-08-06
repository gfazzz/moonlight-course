/* OPERATION MOONLIGHT — s05e02 «Деньги не бывают double»
 * Эталонное решение: money.c
 *
 * Нью-Йорк, 8 октября, 21:15.
 *
 * Отчёт управляющего не сходится с выпиской брокера на четыре цента —
 * на восьми сделках одного счёта. Управляющий объясняет это округлением
 * при конвертации. Объяснение почти верное, и здесь оно проверяется.
 *
 * Концепт серии: деньги хранятся целыми в минимальных единицах.
 *   1) почему double не годится для денег;
 *   2) фиксированная точка: цент как int64_t;
 *   3) три режима округления: усечение, half-up, банковское;
 *   4) почему односторонний сдвиг объясняется не округлением, а усечением.
 *
 * Детерминизм: вся денежная арифметика целочисленная, поэтому вывод
 * побитово одинаков на любой платформе и при любом уровне оптимизации.
 * Два числа с плавающей точкой печатаются намеренно — как экспонат.
 * Собирается без -lm.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- фиксированная точка: деньги в центах ---------- */

typedef int64_t money_t;                 /* минимальные единицы (центы) */

#define ML_TRUNC     0                   /* отбросить дробную часть        */
#define ML_HALF_UP   1                   /* ровно половина — вверх          */
#define ML_HALF_EVEN 2                   /* ровно половина — к чётному      */

/* Разбор "1234.56" в центы. Возвращает 0 при неверном формате.
   Строку читаем сами: strtod дал бы double, а мы как раз от него уходим. */
static int money_parse(const char *s, money_t *out) {
    int64_t whole = 0, frac = 0;
    int digits = 0, neg = 0;
    const char *p = s;

    if (*p == '-') { neg = 1; p++; }
    if (*p < '0' || *p > '9') return 0;
    while (*p >= '0' && *p <= '9') { whole = whole * 10 + (*p - '0'); p++; }

    if (*p == '.') {
        p++;
        if (*p < '0' || *p > '9') return 0;
        while (*p >= '0' && *p <= '9') {
            if (digits == 2) return 0;   /* больше двух знаков — это не деньги */
            frac = frac * 10 + (*p - '0');
            digits++;
            p++;
        }
    }
    if (*p != '\0') return 0;
    while (digits < 2) { frac *= 10; digits++; }

    money_t v = whole * 100 + frac;
    *out = neg ? -v : v;
    return 1;
}

/* Печать центов как "1234.56". Знак — перед целой частью, иначе -0.04
   превратилось бы в "0.-4". */
static void money_str(money_t m, char *buf, size_t n) {
    const char *sign = "";
    if (m < 0) { sign = "-"; m = -m; }
    snprintf(buf, n, "%s%lld.%02lld", sign, (long long)(m / 100), (long long)(m % 100));
}

/* Умножение суммы на долю, заданную в миллионных (ppm): 0.15% = 1500 ppm.

   Целочисленное деление в C отбрасывает дробную часть в сторону нуля —
   это и есть ML_TRUNC, и получается он по умолчанию, если не думать.
   Остальные два режима досчитывают решение по остатку. */
static money_t money_mul_ppm(money_t amount, int64_t ppm, int mode) {
    int64_t num = amount * ppm;          /* до 9.2e18 — в int64_t умещается */
    int64_t q   = num / 1000000;
    int64_t r   = num % 1000000;
    if (r == 0) return q;

    int64_t ar = r < 0 ? -r : r;
    int step = 0;                        /* 0 — оставить q, 1 — шагнуть по модулю */

    if (mode == ML_HALF_UP) {
        step = (ar * 2 >= 1000000);
    } else if (mode == ML_HALF_EVEN) {
        if      (ar * 2 > 1000000) step = 1;
        else if (ar * 2 < 1000000) step = 0;
        else                       step = (q % 2 != 0);   /* ровно половина */
    }
    if (!step) return q;                 /* ML_TRUNC приходит сюда всегда */
    return num < 0 ? q - 1 : q + 1;
}

/* ---------- выравнивание колонок ---------- */

/* Длина строки в символах, а не в байтах: в UTF-8 кириллическая буква
   занимает два байта, и "%-16s" считает её за два знака — колонки едут.
   Продолжающие байты имеют вид 10xxxxxx и не начинают новый символ.
   Ширина текста — тема s05e08; здесь достаточно счёта символов. */
static size_t utf8_len(const char *s) {
    size_t n = 0;
    for (; *s; s++) if ((*s & 0xC0) != 0x80) n++;
    return n;
}

/* Печать слева с добивкой до width СИМВОЛОВ. */
static void pad_left(const char *s, size_t width) {
    size_t l = utf8_len(s);
    fputs(s, stdout);
    for (; l < width; l++) putchar(' ');
}

/* ---------- поток сделок ---------- */

/* Линейный конгруэнтный генератор: одинаковый ряд на любой машине.
   Свой, а не rand(), — у rand() реализация зависит от библиотеки. */
static uint32_t rng_state;
static uint32_t rng_next(void) {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state >> 16;
}

#define NTRADES 1000000
#define FEE_PPM 1500                     /* комиссия 0.15% */

/* Выписка брокера по счёту за 8 октября: восемь сделок, суммы как в отчёте. */
static const char *STATEMENT[] = {
    "1240.00", "2875.50", "990.25", "20030.00",
    "734.80",  "3305.15", "6120.40", "812.60"
};
#define NSTMT ((int)(sizeof STATEMENT / sizeof STATEMENT[0]))

int main(void) {
    char a[32], b[32], c[32];

    printf("=== деньги в фиксированной точке ===\n\n");

    /* ---------- 1. Сверка одного счёта ---------- */
    printf("--- выписка по счёту за 8 октября: %d сделок, комиссия 0.15%% ---\n", NSTMT);
    printf("           сумма    брокер    отчёт\n");

    money_t turn = 0, fee_broker = 0, fee_report = 0, fee_bank = 0;
    for (int i = 0; i < NSTMT; i++) {
        money_t amount;
        if (!money_parse(STATEMENT[i], &amount)) {
            printf("FAIL: строка выписки не разобрана: %s\n", STATEMENT[i]);
            return 1;
        }
        money_t br = money_mul_ppm(amount, FEE_PPM, ML_HALF_UP);
        money_t rp = money_mul_ppm(amount, FEE_PPM, ML_TRUNC);
        money_t bk = money_mul_ppm(amount, FEE_PPM, ML_HALF_EVEN);
        turn += amount; fee_broker += br; fee_report += rp; fee_bank += bk;

        money_str(amount, a, sizeof a);
        money_str(br,     b, sizeof b);
        money_str(rp,     c, sizeof c);
        printf("%16s %9s %8s\n", a, b, c);
    }

    money_str(turn,       a, sizeof a);
    money_str(fee_broker, b, sizeof b);
    money_str(fee_report, c, sizeof c);
    printf("%16s %9s %8s   <- итого\n", a, b, c);

    money_str(fee_report - fee_broker, a, sizeof a);
    printf("\nрасхождение: %s\n", a);
    printf("отчёт меньше выписки: %s\n", fee_report < fee_broker ? "да" : "нет");

    /* ---------- 2. Почему не double ---------- */
    printf("\n--- версия управляющего: «это double, он всегда врёт» ---\n");

    double d = 0.1 + 0.2;
    printf("0.1 + 0.2 == 0.3: %s\n", d == 0.3 ? "да" : "нет");
    printf("0.1 + 0.2 = %.17g\n", d);

    /* Накопление ста тысяч одинаковых сумм. Порядок сложений задан
       зависимостью по данным, поэтому компилятор его не переставляет
       и результат одинаков на -O0 и на -O3. */
    double  acc_d = 0.0;
    money_t acc_m = 0;
    for (int i = 0; i < 100000; i++) { acc_d += 0.01; acc_m += 1; }

    money_str(acc_m, a, sizeof a);
    printf("сто тысяч раз по 0.01:\n");
    printf("  double:      %.10f\n", acc_d);
    printf("  фикс. точка: %s\n", a);
    printf("  double ошибся: %s\n", acc_d == 1000.0 ? "нет" : "да");
    printf("  double ошибся в минус: %s\n", acc_d < 1000.0 ? "да" : "нет");

    /* ---------- 3. Дневной оборот фонда ---------- */
    printf("\n--- тот же механизм на дневном обороте: %d сделок ---\n", NTRADES);

    money_t sum_amount = 0;
    money_t f_trunc = 0, f_up = 0, f_even = 0;
    int64_t exact_num = 0;               /* точная сумма комиссий, в миллионных цента */

    rng_state = 8102025u;
    for (int i = 0; i < NTRADES; i++) {
        money_t amount = 1000 + (money_t)(rng_next() % 999000);   /* 10.00 .. 9999.99 */
        sum_amount += amount;
        exact_num  += amount * FEE_PPM;
        f_trunc    += money_mul_ppm(amount, FEE_PPM, ML_TRUNC);
        f_up       += money_mul_ppm(amount, FEE_PPM, ML_HALF_UP);
        f_even     += money_mul_ppm(amount, FEE_PPM, ML_HALF_EVEN);
    }

    /* Точная сумма: округление ОДИН раз в конце, а не на каждой сделке. */
    money_t f_exact = exact_num / 1000000;
    if (exact_num % 1000000 * 2 >= 1000000) f_exact += 1;

    money_str(sum_amount, a, sizeof a);
    printf("оборот: %s\n\n", a);

    printf("способ                    комиссия  отклонение  на сделку\n");

    struct { const char *name; money_t v; } rows[] = {
        { "округление в конце", f_exact },
        { "усечение",           f_trunc },
        { "half-up",            f_up    },
        { "банковское",         f_even  },
    };
    for (size_t i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        money_t dev = rows[i].v - f_exact;
        money_str(rows[i].v, a, sizeof a);
        money_str(dev,       b, sizeof b);
        pad_left(rows[i].name, 20);
        /* Сдвиг на сделку — в сотых долях цента, целыми, без double.
           Знак выносим отдельно: иначе -49 сотых напечатается как "0.49",
           потому что целая часть -49/100 равна нулю и знак теряется. */
        int64_t h = dev * 100 / NTRADES;
        const char *sg = h < 0 ? "-" : "";
        if (h < 0) h = -h;
        snprintf(c, sizeof c, "%s%lld.%02lld", sg, (long long)(h / 100), (long long)(h % 100));
        printf("%12s %11s %10s\n", a, b, c);
    }

    /* ---------- 4. Односторонность ---------- */
    printf("\n--- в какую сторону ошибается каждый способ ---\n");

    /* Механический признак систематического сдвига: отклонение больше
       десятой доли цента на сделку. У честного округления сдвиг берётся
       только из ровных половин и на три порядка меньше. */
    struct { const char *name; money_t dev; } v[] = {
        { "усечение",   f_trunc - f_exact },
        { "half-up",    f_up    - f_exact },
        { "банковское", f_even  - f_exact },
    };
    for (size_t i = 0; i < sizeof v / sizeof v[0]; i++) {
        money_t d10 = v[i].dev * 10 / NTRADES;        /* десятые доли цента */
        pad_left(v[i].name, 12);
        if (d10 == 0)     printf("сдвига нет\n");
        else if (d10 < 0) printf("систематически в минус\n");
        else              printf("систематически в плюс\n");
    }

    money_t lost = f_exact - f_trunc;
    money_str(lost, a, sizeof a);
    printf("\nусечение недосчитало за день: %s\n", a);
    printf("это половина цента на сделку: %s\n",
           lost * 1000 / NTRADES >= 495 && lost * 1000 / NTRADES <= 505 ? "да" : "нет");

    /* ---------- 5. Вывод ---------- */
    printf("\nчестное округление ошибается в обе стороны и в среднем на ноль.\n");
    printf("односторонний сдвиг даёт только усечение.\n");
    printf("недостача не исчезает — она куда-то зачисляется.\n");

    return 0;
}
