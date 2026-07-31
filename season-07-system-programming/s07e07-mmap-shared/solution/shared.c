/*
 * OPERATION MOONLIGHT — s07e07 "mmap и разделяемая память"
 * Эталонное решение: shared.c
 *
 * Концепт серии: mmap() отображает страницы памяти в адресное пространство.
 *                С флагом MAP_SHARED одна и та же ФИЗИЧЕСКАЯ память видна
 *                нескольким процессам: запись одного мгновенно видна другому,
 *                без копирования через ядро (в отличие от pipe, s07e06).
 *                Цена — синхронизация снова на тебе (гонки из s07e05 вернулись).
 * Задача: разделить структуру между процессами, показать разницу MAP_SHARED
 *         и MAP_PRIVATE, синхронизировать доступ мьютексом в разделяемой памяти.
 *
 * Цепочка курса: mmap упоминался в Season 2 (память), детально разбирается здесь,
 * и будет использоваться в Season 8 (быстрый доступ к большим данным).
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define NKIDS 4
#define NINC  50000

/* Структура, которая будет жить в разделяемой памяти. */
typedef struct {
    pthread_mutex_t lock;
    long counter;
    char message[64];
} Shared;

int main(void) {
    printf("=== mmap: память на двоих ===\n\n");

    /* ---------- 1. MAP_SHARED: общая физическая память ---------- */
    Shared *sh = mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sh == MAP_FAILED) { perror("mmap"); return 1; }

    /* Мьютекс должен уметь работать МЕЖДУ ПРОЦЕССАМИ — это отдельный атрибут. */
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&sh->lock, &attr);
    pthread_mutexattr_destroy(&attr);

    sh->counter = 0;
    strcpy(sh->message, "ожидание");

    /* Потомки инкрементируют общий счётчик под мьютексом. */
    for (int k = 0; k < NKIDS; k++) {
        fflush(stdout);
        if (fork() == 0) {
            for (int i = 0; i < NINC; i++) {
                pthread_mutex_lock(&sh->lock);
                sh->counter++;
                pthread_mutex_unlock(&sh->lock);
            }
            if (k == 0) strcpy(sh->message, "привет от потомка");
            _exit(0);
        }
    }
    for (int k = 0; k < NKIDS; k++) wait(NULL);

    long expected = (long)NKIDS * NINC;
    printf("[shared ] счётчик == %ld (ожидалось %ld): %s\n",
           sh->counter, expected, sh->counter == expected ? "да" : "НЕТ");
    printf("[shared ] строка, записанная потомком, видна родителю: \"%s\"\n", sh->message);

    /* ---------- 2. MAP_PRIVATE: копия при записи ---------- */
    int *priv = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (priv == MAP_FAILED) { perror("mmap"); return 1; }
    *priv = 100;

    fflush(stdout);
    if (fork() == 0) {
        *priv = 999;              /* меняем СВОЮ копию страницы */
        _exit(0);
    }
    wait(NULL);

    printf("\n[private] потомок записал 999, у родителя значение = %d\n", *priv);
    printf("[private] изменения видны другому процессу: %s\n", *priv == 999 ? "да" : "нет");
    printf("[shared ] изменения видны другому процессу: %s\n", "да");

    /* ---------- 3. Освобождение ---------- */
    pthread_mutex_destroy(&sh->lock);
    munmap(sh, sizeof(Shared));
    munmap(priv, sizeof(int));

    printf("\nвывод: MAP_SHARED — одна физическая память на всех;\n");
    printf("       MAP_PRIVATE — copy-on-write, у каждого своя копия.\n");
    return 0;
}
