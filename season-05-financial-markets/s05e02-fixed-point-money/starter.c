/* OPERATION MOONLIGHT — s05e02 «Деньги не бывают double»
 * Каркас: скопируй в artifacts/money.c и заполни TODO.
 *
 *     cp starter.c artifacts/money.c
 *     make test
 *
 * Заполнять нужно три функции: money_parse, money_str, money_mul_ppm.
 * Всё остальное — печать и сцены — уже готово: серия не про форматирование,
 * а про то, в каком виде хранятся деньги.
 *
 * Правила:
 *   - ни одного double в денежной арифметике;
 *   - ни одного -lm;
 *   - вывод должен совпасть с tests/expected.txt посимвольно.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef int64_t money_t;                 /* минимальные единицы (центы) */

#define ML_TRUNC     0
#define ML_HALF_UP   1
#define ML_HALF_EVEN 2

/* TODO 1. Разобрать "1234.56" в центы (123456). Вернуть 1 при успехе, 0 при ошибке.
   Что должно работать:
     "1240.00" -> 124000     "990.25" -> 99025      "10" -> 1000
     "1234.5"  -> 123450     "-0.04"  -> -4
   Что должно отвергаться (вернуть 0):
     "12.345"  (три знака после точки — это уже не деньги)
     "12."     (точка без цифр)
     "abc", "1 2", "" (мусор)
   Подсказка: strtod здесь нельзя — он вернёт double, а мы как раз от него уходим.
   Подсказка: знак применяй в самом конце, к уже собранному числу.            */
static int money_parse(const char *s, money_t *out) {
    (void)s; (void)out;
    return 0;
}

/* TODO 2. Записать центы в buf как "1234.56".
   123456 -> "1234.56"    4 -> "0.04"    -4 -> "-0.04"    0 -> "0.00"
   Ловушка: если печатать знак вместе с остатком, -4 превратится в "0.-4".
   Знак нужно вынести отдельно, а дальше работать с модулем.                  */
static void money_str(money_t m, char *buf, size_t n) {
    (void)m;
    snprintf(buf, n, "?");
}

/* TODO 3. Умножить сумму на долю, заданную в миллионных (ppm): 0.15% = 1500 ppm.
   Вернуть результат в центах, округлив по режиму mode.

     money_mul_ppm(99025,  1500, ML_TRUNC)     -> 148   (точно 148.5375)
     money_mul_ppm(99025,  1500, ML_HALF_UP)   -> 149
     money_mul_ppm(2003000,1500, ML_TRUNC)     -> 3004  (точно 3004.5 — ровно половина)
     money_mul_ppm(2003000,1500, ML_HALF_UP)   -> 3005
     money_mul_ppm(2003000,1500, ML_HALF_EVEN) -> 3004  (3004 уже чётное)

   Порядок действий: сначала умножить, потом делить. Наоборот — потеряешь
   всё, что меньше миллиона.
   Целочисленное деление в C уже отбрасывает дробную часть — это ML_TRUNC,
   и он получается сам собой, если не думать. Остальные два режима решают
   по остатку: сравни удвоенный модуль остатка с делителем.
   Про отрицательные суммы не забудь: шагать нужно от нуля, а не вверх.      */
static money_t money_mul_ppm(money_t amount, int64_t ppm, int mode) {
    (void)amount; (void)ppm; (void)mode;
    return 0;
}

/* ---------- дальше править не нужно ---------- */

/* Длина строки в символах, а не в байтах: в UTF-8 кириллическая буква
   занимает два байта, и "%-16s" считает её за два знака — колонки едут.
   Продолжающие байты имеют вид 10xxxxxx и не начинают новый символ. */
static size_t utf8_len(const char *s) {
    size_t n = 0;
    for (; *s; s++) if ((*s & 0xC0) != 0x80) n++;
    return n;
}

static void pad_left(const char *s, size_t width) {
    size_t l = utf8_len(s);
    fputs(s, stdout);
    for (; l < width; l++) putchar(' ');
}

static uint32_t rng_state;
static uint32_t rng_next(void) {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state >> 16;
}

#define NTRADES 1000000
#define FEE_PPM 1500

static const char *STATEMENT[] = {
    "1240.00", "2875.50", "990.25", "20030.00",
    "734.80",  "3305.15", "6120.40", "812.60"
};
#define NSTMT ((int)(sizeof STATEMENT / sizeof STATEMENT[0]))

int main(void) {
    char a[32], b[32], c[32];

    printf("=== деньги в фиксированной точке ===\n\n");

    printf("--- выписка по счёту за 8 октября: %d сделок, комиссия 0.15%% ---\n", NSTMT);
    printf("           сумма    брокер    отчёт\n");

    money_t turn = 0, fee_broker = 0, fee_report = 0;
    for (int i = 0; i < NSTMT; i++) {
        money_t amount;
        if (!money_parse(STATEMENT[i], &amount)) {
            printf("FAIL: строка выписки не разобрана: %s\n", STATEMENT[i]);
            return 1;
        }
        money_t br = money_mul_ppm(amount, FEE_PPM, ML_HALF_UP);
        money_t rp = money_mul_ppm(amount, FEE_PPM, ML_TRUNC);
        turn += amount; fee_broker += br; fee_report += rp;

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

    printf("\n--- версия управляющего: «это double, он всегда врёт» ---\n");

    double d = 0.1 + 0.2;
    printf("0.1 + 0.2 == 0.3: %s\n", d == 0.3 ? "да" : "нет");
    printf("0.1 + 0.2 = %.17g\n", d);

    double  acc_d = 0.0;
    money_t acc_m = 0;
    for (int i = 0; i < 100000; i++) { acc_d += 0.01; acc_m += 1; }

    money_str(acc_m, a, sizeof a);
    printf("сто тысяч раз по 0.01:\n");
    printf("  double:      %.10f\n", acc_d);
    printf("  фикс. точка: %s\n", a);
    printf("  double ошибся: %s\n", acc_d == 1000.0 ? "нет" : "да");
    printf("  double ошибся в минус: %s\n", acc_d < 1000.0 ? "да" : "нет");

    printf("\n--- тот же механизм на дневном обороте: %d сделок ---\n", NTRADES);

    money_t sum_amount = 0;
    money_t f_trunc = 0, f_up = 0, f_even = 0;
    int64_t exact_num = 0;

    rng_state = 8102025u;
    for (int i = 0; i < NTRADES; i++) {
        money_t amount = 1000 + (money_t)(rng_next() % 999000);
        sum_amount += amount;
        exact_num  += amount * FEE_PPM;
        f_trunc    += money_mul_ppm(amount, FEE_PPM, ML_TRUNC);
        f_up       += money_mul_ppm(amount, FEE_PPM, ML_HALF_UP);
        f_even     += money_mul_ppm(amount, FEE_PPM, ML_HALF_EVEN);
    }

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
        int64_t h = dev * 100 / NTRADES;
        const char *sg = h < 0 ? "-" : "";
        if (h < 0) h = -h;
        snprintf(c, sizeof c, "%s%lld.%02lld", sg, (long long)(h / 100), (long long)(h % 100));
        printf("%12s %11s %10s\n", a, b, c);
    }

    printf("\n--- в какую сторону ошибается каждый способ ---\n");

    struct { const char *name; money_t dev; } v[] = {
        { "усечение",   f_trunc - f_exact },
        { "half-up",    f_up    - f_exact },
        { "банковское", f_even  - f_exact },
    };
    for (size_t i = 0; i < sizeof v / sizeof v[0]; i++) {
        money_t d10 = v[i].dev * 10 / NTRADES;
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

    printf("\nчестное округление ошибается в обе стороны и в среднем на ноль.\n");
    printf("односторонний сдвиг даёт только усечение.\n");
    printf("недостача не исчезает — она куда-то зачисляется.\n");

    return 0;
}
