/*
 * OPERATION MOONLIGHT — библиотека Season 7: процессы
 * Извлечено из s07e02 (fork + exec + waitpid).
 */
#ifndef ML_PROC_H
#define ML_PROC_H

/* Запустить программу и дождаться её. Возврат — код завершения;
   128+N если убита сигналом N; 127 если запуск не удался. */
int ml_run(char *const argv[]);

/* Выполнить строку через /bin/sh -c. Возврат — как у ml_run. */
int ml_run_shell(const char *command);

#endif /* ML_PROC_H */
