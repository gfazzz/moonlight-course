/*
 * OPERATION MOONLIGHT — s03e04 "Сокеты"
 * Каркас. Скопируй в artifacts/echo.c и закрой TODO.
 *
 *   cp starter.c artifacts/echo.c
 *   make test
 *
 * Концепт: TCP API — сервер: socket/bind/listen/accept; клиент: socket/connect;
 *          обмен: write/read. Сервер и клиент — через fork, диалог на 127.0.0.1.
 * Печатает ТОЛЬКО родитель (иначе вывод недетерминирован).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) { perror("socket"); return 1; }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;   /* любой свободный порт */

    /* TODO 1: bind(srv, ...); getsockname(srv, ...) чтобы узнать порт; listen(srv, 1). */

    printf("=== TCP LOOPBACK ECHO ===\n");
    printf("server: listening on 127.0.0.1\n");

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* КЛИЕНТ (молча): socket; connect(addr); write "PING"; read ответ;
           _exit(0) если пришло "PONG", иначе _exit(1). */
        /* TODO 2 */
        _exit(1);
    }

    /* СЕРВЕР (родитель, печатает всё):
       TODO 3: cli = accept(srv, NULL, NULL);
       read запрос; печать "server: received \"%s\" from client\n";
       write "PONG"; печать "server: sent \"PONG\"\n";
       waitpid(pid); печать "client: exchange ok|failed\n" по коду выхода. */
    (void)pid;
    return 0;
}
