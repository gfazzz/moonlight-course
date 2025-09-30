# Миссия: "MOONLIGHT Client"

## 🎯 Цель

Создать TCP клиент для подключения к серверу V. и обмена зашифрованными сообщениями.

## Требования

1. Подключение к серверу (IP:PORT из аргументов)
2. Отправка команд
3. Получение ответов
4. XOR шифрование (из Episode 01)

**Команды сервера**:
- `HELLO` → сервер отвечает приветствием
- `GET_KEY` → сервер отправляет ключ
- `SEND <message>` → отправить зашифрованное сообщение
- `QUIT` → завершить соединение

## Starter Code

```c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    // TODO: Создать TCP клиент
    // 1. socket()
    // 2. connect()
    // 3. send/recv в цикле
    // 4. close()
    return 0;
}
```

См. `starter.c` для полного шаблона.
