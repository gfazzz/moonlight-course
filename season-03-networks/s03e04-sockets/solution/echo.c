/*
 * OPERATION MOONLIGHT — s03e04 "Сокеты"
 * Эталонное решение: echo.c
 *
 * Концепт серии: TCP-сокет API — socket/bind/listen/accept (сервер) и
 *                socket/connect (клиент), обмен через send/recv (write/read).
 * Задача: сервер и клиент в одном процессе через fork, диалог по loopback
 *         (127.0.0.1). Чтобы вывод был детерминированным, печатает ТОЛЬКО родитель;
 *         клиент (ребёнок) работает молча и сообщает результат кодом выхода.
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
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  /* 127.0.0.1 */
    addr.sin_port = 0;                              /* 0 = любой свободный порт */

    if (bind(srv, (struct sockaddr *)&addr, sizeof addr) < 0) { perror("bind"); return 1; }
    socklen_t al = sizeof addr;
    getsockname(srv, (struct sockaddr *)&addr, &al);  /* узнать назначенный порт */
    if (listen(srv, 1) < 0) { perror("listen"); return 1; }

    printf("=== TCP LOOPBACK ECHO ===\n");
    printf("server: listening on 127.0.0.1\n");

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* --- КЛИЕНТ (молча) --- */
        int c = socket(AF_INET, SOCK_STREAM, 0);
        if (c < 0) _exit(2);
        if (connect(c, (struct sockaddr *)&addr, sizeof addr) < 0) _exit(2);
        write(c, "PING", 4);
        char buf[8] = {0};
        ssize_t n = read(c, buf, sizeof buf - 1);
        close(c);
        _exit((n == 4 && strcmp(buf, "PONG") == 0) ? 0 : 1);
    }

    /* --- СЕРВЕР (родитель, печатает всё) --- */
    int cli = accept(srv, NULL, NULL);
    if (cli < 0) { perror("accept"); return 1; }
    char buf[8] = {0};
    ssize_t n = read(cli, buf, sizeof buf - 1);
    buf[(n > 0) ? n : 0] = 0;
    printf("server: received \"%s\" from client\n", buf);
    write(cli, "PONG", 4);
    printf("server: sent \"PONG\"\n");
    close(cli);
    close(srv);

    int status = 0;
    waitpid(pid, &status, 0);
    int ok = (WIFEXITED(status) && WEXITSTATUS(status) == 0);
    printf("client: exchange %s\n", ok ? "ok" : "failed");
    return ok ? 0 : 1;
}
