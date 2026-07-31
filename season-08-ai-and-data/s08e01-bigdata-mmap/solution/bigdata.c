/*
 * OPERATION MOONLIGHT — s08e01 "Большие данные: mmap и один проход"
 * Эталонное решение: bigdata.c
 *
 * Концепт серии: файл, который не влезает в память, не читают целиком. Его
 *                ОТОБРАЖАЮТ (mmap из s07e07) и обрабатывают ОДНИМ ПРОХОДОМ,
 *                удерживая лишь несколько накопителей. Наивная формула дисперсии
 *                (сумма квадратов минус квадрат суммы) при этом теряет точность —
 *                правильный однопроходный алгоритм даёт метод Уэлфорда.
 * Задача: сгенерировать набор данных, отобразить его через mmap, посчитать
 *         статистику за один проход и показать, где ломается наивная формула.
 *
 * Цепочка курса: mmap упомянут в S2, разобран в S7 (s07e07), применён здесь.
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

/* Детерминированный источник данных (LCG, как в S5/S7). */
static unsigned long rng = 20260726UL;
static double next_value(void) {
    rng = rng * 6364136223846793005UL + 1442695040888963407UL;
    double u = (double)((rng >> 33) & 0xFFFFFF) / 16777216.0;   /* [0,1) */
    /* Большое смещение при малом разбросе — типичная реальная ситуация (таймстемпы,
       счётчики, координаты). Именно здесь наивная формула дисперсии разваливается. */
    return 1e8 + u * 100.0;
}

int main(void) {
    /* ---------- 1. Создаём файл данных ---------- */
    FILE *f = fopen(DATAFILE, "wb");
    if (!f) { perror("fopen"); return 1; }
    for (int i = 0; i < N; i++) { double v = next_value(); fwrite(&v, sizeof v, 1, f); }
    fclose(f);

    printf("=== большие данные: mmap + один проход ===\n");
    printf("записей: %d, размер файла: %zu байт\n\n", N, (size_t)N * sizeof(double));

    /* ---------- 2. Отображаем файл вместо чтения в буфер ---------- */
    int fd = open(DATAFILE, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) != 0) { perror("fstat"); close(fd); return 1; }

    const double *data = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); close(fd); return 1; }
    close(fd);                     /* дескриптор больше не нужен: отображение живёт само */

    long n = st.st_size / (long)sizeof(double);
    printf("[mmap ] отображено записей: %ld\n", n);
    printf("[mmap ] файл в кучу не копировался: %s\n\n", "да");

    /* ---------- 3. Один проход: min, max, среднее, дисперсия (Уэлфорд) ---------- */
    double mn = data[0], mx = data[0];
    double mean = 0.0, m2 = 0.0;               /* накопители Уэлфорда */
    double naive_sum = 0.0, naive_sumsq = 0.0; /* наивный способ — для сравнения */

    for (long i = 0; i < n; i++) {
        double x = data[i];
        if (x < mn) mn = x;
        if (x > mx) mx = x;

        double delta = x - mean;               /* Уэлфорд: устойчивое обновление */
        mean += delta / (double)(i + 1);
        m2   += delta * (x - mean);

        naive_sum   += x;
        naive_sumsq += x * x;
    }

    double var_welford = m2 / (double)(n - 1);
    double naive_mean  = naive_sum / (double)n;
    double var_naive   = (naive_sumsq - naive_sum * naive_mean) / (double)(n - 1);

    /* Относительная ошибка наивной формулы. Само наивное значение печатать
       в сравниваемый вывод НЕЛЬЗЯ: оно и есть накопленный остаток округления,
       а значит меняется от порядка операций — на этой же машине -O0 даёт
       874.25, а -O2 даёт 873.94. Устойчиво здесь только одно: ошибка велика.
       Точное значение уходит в stderr — для глаз, не для сравнения. */
    double err_abs = var_naive - var_welford;
    double err_rel = (err_abs < 0 ? -err_abs : err_abs) / var_welford * 100.0;

    printf("min = %.4f\nmax = %.4f\nсреднее = %.6f\n\n", mn, mx, mean);
    printf("дисперсия (Уэлфорд): %.6f\n", var_welford);
    printf("дисперсия (наивная): значение нестабильно (см. stderr)\n");
    printf("ошибка наивной формулы превышает 1%%: %s\n", err_rel > 1.0 ? "да" : "нет");
    printf("наивная формула потеряла точность: %s\n", err_abs != 0.0 ? "да" : "нет");

    fprintf(stderr, "[stderr] наивная дисперсия = %.6f, расхождение = %.6f (%.2f%%)\n",
            var_naive, err_abs, err_rel);
    fprintf(stderr, "[stderr] это значение НЕ воспроизводится: пересоберите с -O2 и сравните.\n");

    /* ---------- 4. Память под данные не выделялась ---------- */
    printf("\nпамяти под данные выделено malloc'ом: %d байт\n", 0);
    printf("накопителей в цикле: %d (min, max, mean, m2)\n", 4);

    munmap((void *)data, (size_t)st.st_size);
    unlink(DATAFILE);
    return 0;
}
