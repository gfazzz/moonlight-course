/*
 * OPERATION MOONLIGHT — s07e07 "mmap и разделяемая память"
 * Каркас. Скопируй в artifacts/shared.c и закрой TODO.
 *
 *   cp starter.c artifacts/shared.c
 *   make test
 *
 * MAP_SHARED — одна физическая память на всех; MAP_PRIVATE — copy-on-write.
 * Мьютекс в общей памяти обязан быть PTHREAD_PROCESS_SHARED.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define NKIDS 4
#define NINC  50000

typedef struct {
    pthread_mutex_t lock;
    long counter;
    char message[64];
} Shared;

int main(void) {
    printf("=== mmap: память на двоих ===\n\n");

    /* TODO 1: sh = mmap(NULL, sizeof(Shared), PROT_READ|PROT_WRITE,
                         MAP_SHARED|MAP_ANONYMOUS, -1, 0);
       проверить MAP_FAILED. */

    /* TODO 2: настроить межпроцессный мьютекс:
         pthread_mutexattr_init(&attr);
         pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
         pthread_mutex_init(&sh->lock, &attr);
         pthread_mutexattr_destroy(&attr);
       затем sh->counter = 0; strcpy(sh->message, "ожидание"); */

    /* TODO 3: NKIDS потомков; каждый NINC раз:
         lock; sh->counter++; unlock;
       потомок k == 0 дополнительно: strcpy(sh->message, "привет от потомка");
       затем _exit(0). Родитель: wait(NULL) столько же раз.
       печать:
         "[shared ] счётчик == %ld (ожидалось %ld): да|НЕТ"
         "[shared ] строка, записанная потомком, видна родителю: \"%s\"" */

    /* TODO 4: MAP_PRIVATE:
         priv = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,
                     MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
         *priv = 100; fork(): в потомке *priv = 999; _exit(0); родитель wait(NULL).
       печать:
         "\n[private] потомок записал 999, у родителя значение = %d"
         "[private] изменения видны другому процессу: да|нет"
         "[shared ] изменения видны другому процессу: да" */

    /* TODO 5: pthread_mutex_destroy(&sh->lock); munmap(sh, ...); munmap(priv, ...);
       печать:
         "\nвывод: MAP_SHARED — одна физическая память на всех;"
         "       MAP_PRIVATE — copy-on-write, у каждого своя копия." */

    return 0;
}
