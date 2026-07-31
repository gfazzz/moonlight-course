/*
 * OPERATION MOONLIGHT — s08e01 "Большие данные: mmap и один проход"
 * Каркас. Скопируй в artifacts/bigdata.c и закрой TODO.
 *
 *   cp starter.c artifacts/bigdata.c
 *   make test
 *
 * Идея: файл не читаем в кучу, а ОТОБРАЖАЕМ (mmap из s07e07) и обрабатываем
 * одним проходом. Дисперсию считаем методом Уэлфорда, а не «сумма квадратов минус…».
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define N       200000
#define DATAFILE "/tmp/moonlight_s08e01_data.bin"

static unsigned long rng = 20260726UL;
static double next_value(void) {
    rng = rng * 6364136223846793005UL + 1442695040888963407UL;
    double u = (double)((rng >> 33) & 0xFFFFFF) / 16777216.0;
    return 1e8 + u * 100.0;
}

int main(void) {
    /* TODO 1: создать DATAFILE и записать N значений next_value() через fwrite. */

    printf("=== большие данные: mmap + один проход ===\n");
    printf("записей: %d, размер файла: %zu байт\n\n", N, (size_t)N * sizeof(double));

    /* TODO 2: open(DATAFILE, O_RDONLY); fstat -> размер;
       data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
       проверить MAP_FAILED; close(fd) — отображение живёт само.
       печать:
         "[mmap ] отображено записей: %ld"
         "[mmap ] файл в кучу не копировался: да\n" */

    /* TODO 3: ОДИН проход по data[0..n):
         min/max;
         Уэлфорд: delta = x - mean; mean += delta/(i+1); m2 += delta*(x - mean);
         наивно:  naive_sum += x; naive_sumsq += x*x;
       затем:
         var_welford = m2/(n-1);
         naive_mean  = naive_sum/n;
         var_naive   = (naive_sumsq - naive_sum*naive_mean)/(n-1);
         err_rel     = |var_naive - var_welford| / var_welford * 100

       ВАЖНО про вывод. Само наивное значение — это и есть накопленный
       остаток округления: оно меняется от порядка операций, и на одной
       и той же машине -O0 даёт 874.25, а -O2 даёт 873.94. Поэтому в stdout
       (его сверяет тест) идут только устойчивые утверждения, а точное
       наивное значение — в stderr, куда тест не смотрит.

       печать в stdout:
         "min = %.4f", "max = %.4f", "среднее = %.6f\n"
         "дисперсия (Уэлфорд): %.6f"
         "дисперсия (наивная): значение нестабильно (см. stderr)"
         "ошибка наивной формулы превышает 1%%: да|нет"
         "наивная формула потеряла точность: да|нет"

       печать в stderr (fprintf(stderr, ...)):
         точное наивное значение, расхождение и процент. */

    /* TODO 4: печать
         "\nпамяти под данные выделено malloc'ом: 0 байт"
         "накопителей в цикле: 4 (min, max, mean, m2)"
       затем munmap(...) и unlink(DATAFILE). */

    return 0;
}
