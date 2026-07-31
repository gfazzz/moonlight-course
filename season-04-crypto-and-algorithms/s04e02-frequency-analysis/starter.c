/*
 * OPERATION MOONLIGHT — s04e02 "Частотный анализ"
 * Каркас. Скопируй в artifacts/freq.c и закрой TODO.
 *
 *   cp starter.c artifacts/freq.c
 *   make test
 *
 * Концепт: подстановка сохраняет частоты языка. Сравниваем распределение букв
 *          с эталонным (английский) через хи-квадрат: минимум = верный ключ.
 */
#include <stdio.h>
#include <string.h>

/* Эталонные частоты A..Z в английском (%). */
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

/* TODO 1: chi_squared(text):
   - посчитай count[26] по буквам (регистр не важен) и total;
   - expected = ENG[i]/100.0 * total;
   - chi += (count[i]-expected)^2 / expected;  верни chi. */

int main(void) {
    const char *cipher =
        "ESP XZZYWTRSE WPORPC TD CPLW PGPCJ "
        "ECLYDLNETZY DTRYPO HTES L DEZWPY VPJ PGL NLY ACZGP TE";

    printf("=== FREQUENCY ANALYSIS ===\n");
    printf("cipher: %s\n\n", cipher);

    /* TODO 2: для каждого сдвига 0..25 расшифруй и посчитай chi[s];
       запомни best (минимальный chi). */

    /* TODO 3: выведи три лучших кандидата (формат — см. tests/expected.txt):
         "--- top-3 candidates (lower chi2 = better) ---"
         "shift %2d: chi2 = %8.2f" */

    /* TODO 4: расшифруй лучшим сдвигом и выведи:
         "\nbest shift = %d\n"  "decrypted: %s\n" */
    return 0;
}
