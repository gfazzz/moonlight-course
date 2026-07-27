#include "ml_proc.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int ml_run(char *const argv[]) {
    fflush(stdout);                       /* иначе буфер продублируется в потомке */
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        execv(argv[0], argv);
        _exit(127);                       /* достижимо только при провале exec */
    }
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))   return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return -1;
}

int ml_run_shell(const char *command) {
    char *argv[] = {(char *)"/bin/sh", (char *)"-c", (char *)command, NULL};
    return ml_run(argv);
}
