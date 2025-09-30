# Episode 10: "Socket Programming" 🔌

> *"Every connection is a handshake. Master the protocol, control the channel."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   09:30, среда, 1 октября                       ║
║  СТАТУС:  🟡 SERVER CONNECTION REQUIRED                  ║
╚══════════════════════════════════════════════════════════╝
```

После анализа `addresses.txt` вы нашли **первый узел**: `203.0.113.42:31337`.  
Теперь нужно **подключиться** к серверу V.

В 09:28 приходит инструкция:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 09:28:14 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  Адрес найден. Теперь — подключение.        │
│                                             │
│  Сервер: 203.0.113.42:31337                 │
│  Протокол: TCP                              │
│  Аутентификация: Challenge-Response         │
│                                             │
│  Шаги:                                      │
│  1. Подключиться через socket()             │
│  2. Отправить команду: AUTH <key>           │
│  3. Получить challenge (число)              │
│  4. Отправить response (XOR с ключом)       │
│  5. Получить секретное сообщение            │
│                                             │
│  Ключ для AUTH: MOONLIGHT2024               │
│                                             │
│  Удачи.                                     │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

**Что вы понимаете:**
- Нужно написать **TCP-клиент** на C
- Использовать **socket API**: `socket()`, `connect()`, `send()`, `recv()`
- Реализовать **challenge-response** аутентификацию
- Сервер проверит вас и отправит секретные данные

Это **первое реальное сетевое соединение**. Пора стать мастером сокетов.

### 🎯 ЗАДАЧА

Создать **`moonlight_client`** — TCP-клиент для связи с сервером:
1. ✅ Подключение к серверу по TCP
2. ✅ Отправка команды AUTH с ключом
3. ✅ Получение challenge от сервера
4. ✅ Вычисление response (XOR challenge с ключом)
5. ✅ Получение секретного сообщения
6. ✅ Корректное закрытие соединения

**Сложность:** 🟡 Medium  
**Новые навыки:** Socket programming, TCP client, challenge-response protocol

```
╔══════════════════════════════════════════════╗
║  [!] ВНИМАНИЕ:                               ║
║  Это ваш первый TCP-клиент.                  ║
║  Каждый вызов socket API — шаг в сеть.       ║
║  Ошибка = разрыв соединения.                 ║
╚══════════════════════════════════════════════╝
```

---

## 📚 Теория: Сокеты в C

### 💡 Зачем это нужно в реальности?

**Сокеты** — основа всех сетевых приложений:
- 🌐 **Веб**: браузеры, серверы (HTTP/HTTPS)
- 📧 **Email**: SMTP, IMAP, POP3
- 💬 **Мессенджеры**: WhatsApp, Telegram
- 🎮 **Игры**: многопользовательские онлайн-игры
- 🔒 **Безопасность**: VPN, SSH, SSL/TLS

**Без сокетов нет интернета.**

---

### 1. Что такое сокет?

**Сокет** — это:
- **Конечная точка** для отправки/получения данных
- **Файловый дескриптор** (как файл, но для сети)
- **Абстракция** над TCP/IP

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Создание сокета
int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // TCP
// AF_INET   - IPv4
// SOCK_STREAM - TCP (надежный, с установкой соединения)
// 0 - протокол по умолчанию

// Или UDP:
int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);   // UDP
// SOCK_DGRAM - UDP (ненадежный, без соединения)
```

**Аналогия**: Сокет — это "телефон". TCP — звонок (с установкой связи). UDP — SMS (отправил и забыл).

---

### 2. Структура адреса

```c
struct sockaddr_in {
    sa_family_t    sin_family;  // AF_INET (IPv4)
    in_port_t      sin_port;    // Порт (network byte order!)
    struct in_addr sin_addr;    // IP адрес
    char           sin_zero[8]; // Padding (обнулить!)
};

// Пример заполнения:
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));  // Обнулить!

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(31337);  // Host to Network Short
inet_pton(AF_INET, "203.0.113.42", &server_addr.sin_addr);
```

**Важно**: Всегда используйте `htons()` для портов! (Byte order!)

---

### 3. TCP Клиент (пошагово)

#### Шаг 1: Создать сокет

```c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) {
    perror("socket failed");
    return 1;
}
```

#### Шаг 2: Настроить адрес сервера

```c
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(31337);
inet_pton(AF_INET, "203.0.113.42", &server_addr.sin_addr);
```

#### Шаг 3: Подключиться

```c
if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect failed");
    close(sockfd);
    return 1;
}

printf("Connected to server!\n");
```

**Момент connect()**: происходит **TCP handshake** (SYN, SYN-ACK, ACK).

#### Шаг 4: Отправить данные

```c
char message[] = "AUTH MOONLIGHT2024\n";
ssize_t sent = send(sockfd, message, strlen(message), 0);
if (sent < 0) {
    perror("send failed");
}
```

**Важно**: `send()` возвращает количество отправленных байтов (может быть меньше запрошенного!).

#### Шаг 5: Получить данные

```c
char buffer[1024];
memset(buffer, 0, sizeof(buffer));

ssize_t received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
if (received < 0) {
    perror("recv failed");
} else if (received == 0) {
    printf("Server closed connection\n");
} else {
    printf("Received: %s\n", buffer);
}
```

**Особенности `recv()`**:
- Возвращает 0 → сервер закрыл соединение
- Возвращает -1 → ошибка
- Возвращает N > 0 → получено N байтов

#### Шаг 6: Закрыть сокет

```c
close(sockfd);
```

**Важно**: Всегда закрывайте сокеты! (Утечка дескрипторов!)

---

### 4. TCP Сервер (для понимания)

```c
// 1. Создать сокет
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// 2. Опции сокета (переиспользование адреса)
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// 3. Привязать к адресу
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;  // Любой интерфейс

bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

// 4. Слушать
listen(server_fd, 5);  // Backlog = 5 (очередь подключений)

// 5. Принять клиента
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

// 6. Обмен данными
char buffer[1024];
recv(client_fd, buffer, sizeof(buffer), 0);
send(client_fd, "Response", 8, 0);

// 7. Закрыть
close(client_fd);
close(server_fd);
```

---

### 5. UDP (для сравнения)

```c
// Клиент UDP (без connect!)
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in server_addr;
// ... настройка адреса ...

// Отправить (указываем адрес каждый раз!)
sendto(sockfd, "Hello", 5, 0, 
       (struct sockaddr*)&server_addr, sizeof(server_addr));

// Получить
char buffer[1024];
socklen_t addr_len = sizeof(server_addr);
recvfrom(sockfd, buffer, sizeof(buffer), 0,
         (struct sockaddr*)&server_addr, &addr_len);

close(sockfd);
```

**Разница TCP vs UDP**:
- TCP: `connect()` → `send()` / `recv()`
- UDP: `sendto()` / `recvfrom()` (без connect)

---

### 6. Обработка ошибок

```c
// Всегда проверяйте возвращаемые значения!

if (socket(...) < 0) {
    perror("socket");  // Выводит системную ошибку
    exit(1);
}

if (connect(...) < 0) {
    switch (errno) {
        case ECONNREFUSED:
            printf("Connection refused (server not running?)\n");
            break;
        case ETIMEDOUT:
            printf("Connection timeout\n");
            break;
        case ENETUNREACH:
            printf("Network unreachable\n");
            break;
        default:
            perror("connect");
    }
    exit(1);
}
```

---

### 7. Неблокирующие сокеты (advanced)

```c
#include <fcntl.h>

// Сделать сокет неблокирующим
int flags = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

// Теперь recv() не блокирует!
ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
if (n < 0 && errno == EWOULDBLOCK) {
    printf("No data available yet\n");
}
```

**Зачем?** Для одновременной работы с несколькими сокетами (`select()`, `poll()`, `epoll()`).

---

## 🎯 Миссия: "MOONLIGHT Client"

### Легенда

Вы нашли адрес **203.0.113.42:31337**. Теперь нужно подключиться и пройти аутентификацию.

V. использует **challenge-response** протокол:

```
Client → Server: AUTH MOONLIGHT2024
Server → Client: CHALLENGE 0x12345678
Client → Server: RESPONSE 0xXXXXXXXX  (XOR challenge с хешем ключа)
Server → Client: ACCESS_GRANTED\nSECRET_MESSAGE
```

**Ваша задача**: написать клиент, который:
1. Подключается к серверу
2. Отправляет `AUTH <key>`
3. Получает challenge
4. Вычисляет response (XOR)
5. Получает секретное сообщение

---

### Формат протокола

**1. Аутентификация:**
```
Client → "AUTH MOONLIGHT2024\n"
Server → "CHALLENGE 305419896\n"  (число в decimal)
```

**2. Response:**
```
Client → "RESPONSE <calculated>\n"
```

**Вычисление response**:
```c
// Псевдокод:
uint32_t challenge = parse_from_server();
uint32_t key_hash = simple_hash("MOONLIGHT2024");  // Простой хеш
uint32_t response = challenge ^ key_hash;
```

**Простой хеш**:
```c
uint32_t simple_hash(const char *str) {
    uint32_t hash = 0;
    for (int i = 0; str[i]; i++) {
        hash = (hash * 31) + str[i];
    }
    return hash;
}
```

**3. Результат:**
```
Server → "ACCESS_GRANTED\n<secret_message>\n"
```

---

## 📝 Структура эпизода

```
episode-10-socket-programming/
├── README.md              ← Вы здесь
├── mission.md
├── starter.c              ← Шаблон TCP клиента
├── artifacts/
│   └── moonlight_client.c ← Создайте этот файл
├── solution/
│   ├── moonlight_client_solution.c
│   └── test_server.c      ← Локальный тестовый сервер
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там базовый TCP клиент с TODO
2. **Создайте** файл `artifacts/moonlight_client.c`
3. **Реализуйте** challenge-response:
   - Подключение через `socket()` + `connect()`
   - Отправка AUTH с `send()`
   - Парсинг challenge из `recv()`
   - Вычисление response (XOR)
   - Получение секретного сообщения
4. **Скомпилируйте**: `gcc -Wall -o moonlight_client artifacts/moonlight_client.c`
5. **Запустите**: `./moonlight_client 203.0.113.42 31337`

> 💡 **Для тестирования**: используйте `solution/test_server.c` локально!

---

### Подсказки

<details>
<summary>Как парсить "CHALLENGE 12345"?</summary>

```c
char buffer[1024];
recv(sockfd, buffer, sizeof(buffer), 0);

uint32_t challenge;
if (sscanf(buffer, "CHALLENGE %u", &challenge) == 1) {
    printf("Got challenge: 0x%08X\n", challenge);
}
```
</details>

<details>
<summary>Как вычислить response?</summary>

```c
uint32_t simple_hash(const char *str) {
    uint32_t hash = 0;
    for (int i = 0; str[i]; i++) {
        hash = (hash * 31) + str[i];
    }
    return hash;
}

uint32_t key_hash = simple_hash("MOONLIGHT2024");
uint32_t response = challenge ^ key_hash;
```
</details>

<details>
<summary>Как отправить response?</summary>

```c
char response_msg[256];
snprintf(response_msg, sizeof(response_msg), "RESPONSE %u\n", response);
send(sockfd, response_msg, strlen(response_msg), 0);
```
</details>

---

## ✅ Проверка

```bash
# Компиляция
gcc -Wall -o moonlight_client artifacts/moonlight_client.c

# Тестирование (локальный сервер)
gcc -Wall -o test_server solution/test_server.c
./test_server &  # Запустить в фоне
./moonlight_client 127.0.0.1 31337

# Остановить сервер
pkill test_server
```

**Критерии успеха**:
- Клиент успешно подключается
- Отправляет AUTH корректно
- Парсит challenge
- Вычисляет правильный response
- Получает ACCESS_GRANTED и секретное сообщение
- Корректно закрывает соединение

---

## 🎬 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   14:42, среда, 1 октября                       ║
║  СТАТУС:  🟢 CONNECTION ESTABLISHED                      ║
╚══════════════════════════════════════════════════════════╝
```

`moonlight_client` запущен. Соединение установлено. Результат:

```
=== MOONLIGHT CLIENT ===
Connecting to 203.0.113.42:31337... ✓
Sending AUTH... ✓
Received challenge: 0x12345678
Calculating response... ✓
Sending response... ✓

Server response:
┌─────────────────────────────────────────┐
│  ACCESS_GRANTED                         │
│                                         │
│  Ты подключился к первому узлу.         │
│  Но это только начало сети.             │
│                                         │
│  Следующий шаг:                         │
│  ПЕРЕХВАТ ТРАФИКА                       │
│                                         │
│  Episode 11: Packet Analysis            │
│  Научись читать чужие пакеты.           │
│                                         │
│  — V.                                   │
└─────────────────────────────────────────┘

Connection closed gracefully.
```

**Что вы узнали:**
- **TCP handshake** прошел успешно (SYN → SYN-ACK → ACK)
- **Challenge-response** — простая, но эффективная аутентификация
- **XOR с хешем** — базовый метод защиты
- Сервер отправил **координаты следующего узла**

В 14:40 новое сообщение:

```
┌─────────────────────────────────────────┐
│  FROM: V.                               │
├─────────────────────────────────────────┤
│                                         │
│  Соединение — это только начало.        │
│  Настоящая сила — в ПЕРЕХВАТЕ.          │
│                                         │
│  Episode 11: Packet Analysis            │
│  Жду на следующем уровне.               │
│                                         │
│  — V.                                   │
└─────────────────────────────────────────┘
```

**Подключиться может каждый. Понимать трафик — единицы.**

### 🎖️ Навык разблокирован:
**"Socket Master"** — Вы владеете TCP/IP на уровне кода. Сокеты больше не тайна.

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Отсылки и секреты</summary>

1. **Challenge-Response** — классический протокол аутентификации (CHAP, HTTP Digest)
2. **Hash * 31** — классический алгоритм Java `String.hashCode()`
3. **Port 31337** — снова leet speak (уже знакомо из Episode 09)
4. **XOR для response** — упрощенный HMAC (реальные системы используют SHA-256)
5. **TCP handshake (SYN-SYN/ACK-ACK)** — фундамент всего TCP
6. **MOONLIGHT2024** — ключ содержит год, намек на timeline операции
7. **V.'s timestamp 14:40** — 0xE28 hex, отсылка к Enigma machine (E-28 variant)

**Философия эпизода**: *"Сокет — это дверь. Протокол — это ключ. Знание — это власть."*

</details>

---

## 🔗 Навигация

- [← Episode 09: Network Basics](../episode-09-network-basics/README.md)
- [→ Episode 11: Packet Analysis](../episode-11-packet-analysis/README.md)
- [📋 Season 3: Networks](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"Master the socket, master the network. The rest is just data."* — V.