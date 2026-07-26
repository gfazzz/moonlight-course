/*
 * OPERATION MOONLIGHT — s07e01 "Процессы и fork()"
 * Эталонное решение: fork.c
 *
 * Концепт серии: fork() создаёт КОПИЮ процесса и возвращается ДВАЖДЫ:
 *                в родителе — PID потомка (>0), в потомке — 0. Дальше два процесса
 *                живут независимо: своя память, свой PID, общий код.
 * Задача: породить потомка, доказать родство (getppid), дождаться его через wait()
 *         и снять код возврата; показать ловушку буфера stdout при fork().
 *
 * ВАЖНО о детерминизме: реальные PID меняются от запуска к запуску, поэтому
 * печатаем не числа, а ПРОВЕРЯЕМЫЕ СВОЙСТВА (да/нет). Тест сравнивает их.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILD_EXIT_CODE 7

int main(void) {
    printf("=== fork(): один процесс становится двумя ===\n\n");

    pid_t parent_pid = getpid();

    /* Ловушка: всё, что осталось в буфере stdout, скопируется в потомка
       и напечатается ДВАЖДЫ. Сбрасываем буфер ДО fork(). */
    fflush(stdout);

    pid_t rc = fork();

    if (rc < 0) {
        perror("fork");
        return 1;
    }

    if (rc == 0) {
        /* --- ветка потомка --- */
        int same_parent = (getppid() == parent_pid);
        printf("[child ] fork() вернул 0:            %s\n", "да");
        printf("[child ] getppid() == PID родителя:  %s\n", same_parent ? "да" : "нет");
        printf("[child ] выхожу с кодом %d\n", CHILD_EXIT_CODE);
        fflush(stdout);
        _exit(CHILD_EXIT_CODE);      /* _exit: не трогать буферы, унаследованные от родителя */
    }

    /* --- ветка родителя --- */
    int status = 0;
    pid_t waited = waitpid(rc, &status, 0);   /* ждём именно своего потомка */

    printf("[parent] fork() вернул PID потомка > 0: %s\n", rc > 0 ? "да" : "нет");
    printf("[parent] waitpid() вернул того же:      %s\n", waited == rc ? "да" : "нет");
    printf("[parent] потомок завершился нормально:  %s\n", WIFEXITED(status) ? "да" : "нет");
    printf("[parent] код возврата потомка:          %d\n", WEXITSTATUS(status));

    /* Память НЕ общая: копия при разделении (copy-on-write). */
    int x = 100;
    fflush(stdout);
    pid_t rc2 = fork();
    if (rc2 == 0) {
        x += 1;                       /* меняем СВОЮ копию */
        _exit((x == 101) ? 0 : 1);
    }
    waitpid(rc2, &status, 0);
    printf("\n[parent] потомок менял x, у родителя x = %d (память раздельна)\n", x);
    printf("[parent] потомок подтвердил свою копию: %s\n",
           WEXITSTATUS(status) == 0 ? "да" : "нет");

    return 0;
}
