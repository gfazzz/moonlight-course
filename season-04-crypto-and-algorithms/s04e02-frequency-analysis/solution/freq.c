/*
 * OPERATION MOONLIGHT — s04e02 "Частотный анализ"
 * Эталонное решение: freq.c
 *
 * Концепт серии: подстановка сохраняет статистику языка. Считаем частоты букв
 *                и сравниваем с эталонными частотами английского через
 *                хи-квадрат: наименьшее значение = наиболее правдоподобный ключ.
 * Задача: вскрыть шифр БЕЗ перебора глазами — программа сама выбирает ключ.
 */
#include <stdio.h>
#include <string.h>

/* Эталонные частоты букв A..Z в английском тексте (в процентах). */
static const double ENG[26] = {
    8.17, 1.49, 2.78, 4.25,12.70, 2.23, 2.02, 6.09, 6.97, 0.15,
    0.77, 4.03, 2.41, 6.75, 7.51, 1.93, 0.10, 5.99, 6.33, 9.06,
    2.76, 0.98, 2.36, 0.15, 1.97, 0.07
};

static void caesar(const char *in, char *out, int shift) {
    size_t i;
    for (i = 0; in[i]; i++) {
        char c = in[i];
        if (c >= 'A' && c <= 'Z')      out[i] = (char)('A' + (((c - 'A' + shift) % 26) + 26) % 26);
        else if (c >= 'a' && c <= 'z') out[i] = (char)('a' + (((c - 'a' + shift) % 26) + 26) % 26);
        else                            out[i] = c;
    }
    out[i] = '\0';
}

/* Хи-квадрат: сумма (наблюдаемое - ожидаемое)^2 / ожидаемое по 26 буквам. */
static double chi_squared(const char *text) {
    int count[26] = {0};
    int total = 0;
    for (size_t i = 0; text[i]; i++) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') { count[c - 'A']++; total++; }
        else if (c >= 'a' && c <= 'z') { count[c - 'a']++; total++; }
    }
    if (total == 0) return 1e9;
    double chi = 0.0;
    for (int i = 0; i < 26; i++) {
        double expected = ENG[i] / 100.0 * total;
        double diff = count[i] - expected;
        chi += (diff * diff) / (expected > 0 ? expected : 1e-9);
    }
    return chi;
}

int main(void) {
    /* Перехват: подстановка со сдвигом, ключ неизвестен. */
    const char *cipher =
        "ESP XZZYWTRSE WPORPC TD CPLW PGPCJ "
        "ECLYDLNETZY DTRYPO HTES L DEZWPY VPJ PGL NLY ACZGP TE";

    printf("=== FREQUENCY ANALYSIS ===\n");
    printf("cipher: %s\n\n", cipher);

    /* Пробуем все сдвиги, считаем хи-квадрат, ищем минимум. */
    int best = 0;
    double best_chi = 1e18;
    double chi[26];
    for (int s = 0; s < 26; s++) {
        char t[512];
        caesar(cipher, t, -s);
        chi[s] = chi_squared(t);
        if (chi[s] < best_chi) { best_chi = chi[s]; best = s; }
    }

    /* Три лучших кандидата (по возрастанию хи-квадрат). */
    printf("--- top-3 candidates (lower chi2 = better) ---\n");
    for (int rank = 0; rank < 3; rank++) {
        int idx = -1; double cur = 1e18;
        for (int s = 0; s < 26; s++) if (chi[s] < cur) { cur = chi[s]; idx = s; }
        printf("shift %2d: chi2 = %8.2f\n", idx, cur);
        chi[idx] = 1e18;   /* исключаем найденный из следующего поиска */
    }

    char plain[512];
    caesar(cipher, plain, -best);
    printf("\nbest shift = %d\n", best);
    printf("decrypted: %s\n", plain);
    return 0;
}
