# Episode 10: "Socket Programming" 🔌

> *"Every connection is a handshake. Master the protocol, control the channel."*

## 📚 Теория: Сокеты в C

### Основы сокетов

**Сокет** — это конечная точка для отправки/получения данных в сети.

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Создание TCP сокета
int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // TCP
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);   // UDP

// Настройка адреса
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;  // Любой интерфейс
```

### TCP Сервер

```c
// 1. Создать сокет
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// 2. Привязать к адресу
bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

// 3. Слушать подключения
listen(server_fd, 5);  // Очередь = 5

// 4. Принять клиента
int client_fd = accept(server_fd, NULL, NULL);

// 5. Обмен данными
char buffer[1024];
recv(client_fd, buffer, sizeof(buffer), 0);
send(client_fd, "OK", 2, 0);

// 6. Закрыть
close(client_fd);
close(server_fd);
```

### TCP Клиент

```c
// 1. Создать сокет
int sockfd = socket(AF_INET, SOCK_STREAM, 0);

// 2. Подключиться
connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

// 3. Обмен данными
send(sockfd, "Hello", 5, 0);
recv(sockfd, buffer, sizeof(buffer), 0);

// 4. Закрыть
close(sockfd);
```

## 🎯 Миссия: "MOONLIGHT Client"

Создать TCP клиент для связи с секретным сервером V.

**Артефакт**: `moonlight_client` — клиент для шифрованной связи

См. [mission.md](mission.md) для деталей.

---

[← Episode 09](../episode-09-network-basics/README.md) | [→ Episode 11](../episode-11-packet-analysis/README.md)
