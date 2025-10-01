# Episode 10: "Socket Programming" 🔌

> *"Every connection is a handshake. Master the protocol, control the channel."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Safe House (неизвестный адрес), Москва        ║
║  ВРЕМЯ:   09:30, среда, 1 октября                       ║
║  СТАТУС:  🔴 SERVER CONNECTION CRITICAL — DEADLINE!      ║
╚══════════════════════════════════════════════════════════╝
```

После анализа `addresses.txt` вы нашли **первый узел**: `203.0.113.42:31337`.  
GPS-координаты расшифрованы: Красная площадь — точка встречи.

**Но теперь нужно ПОДКЛЮЧИТЬСЯ к серверу V.**

В 09:28 приходит срочная инструкция:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 09:28:14 UTC                         │
│  PRIORITY: 🔴 CRITICAL                      │
├─────────────────────────────────────────────┤
│                                             │
│  Адрес найден. Теперь — подключение.        │
│                                             │
│  Сервер: 203.0.113.42:31337                 │
│  Протокол: TCP                              │
│  Аутентификация: Challenge-Response         │
│                                             │
│  ⚠️ КРИТИЧНО:                               │
│  Сервер самоуничтожится через 5 часов!      │
│  Deadline: 14:30 UTC                        │
│                                             │
│  Причина: Риск компрометации.               │
│  Преследователи Z. сканируют сеть.          │
│                                             │
│  Шаги:                                      │
│  1. Подключиться через socket()             │
│  2. Отправить команду: AUTH <key>           │
│  3. Получить challenge (число)              │
│  4. Отправить response (XOR с ключом)       │
│  5. Получить координаты следующего узла     │
│                                             │
│  Ключ для AUTH: MOONLIGHT2024               │
│                                             │
│  Поторопись. Время идёт.                    │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

### ⏱️ Временная линия событий

```
09:28 → 📧 Инструкция от V.: подключиться к серверу
        ⏰ DEADLINE: 5 часов до автоудаления (14:30)
        
09:30 → 🔧 Начало разработки TCP-клиента
        Изучение socket API (socket, connect, send, recv)
        
10:15 → ⚡ ПЕРВАЯ ПОПЫТКА подключения
        $ ./moonlight_client 203.0.113.42 31337
        
10:16 → 💥 CONNECTION REFUSED!
        🚨 Обнаружено: Сервер под DDoS-атакой!
        Тысячи SYN-пакетов от 185.220.101.0/24
        (Преследователи Z. атакуют сервер!)
        
10:17 → 📞 SMS от V.:
        "Они пытаются заблокировать доступ.
         Попробуй снова через 30 минут.
         Атаку отражаю."
         
10:47 → 🔄 ВТОРАЯ ПОПЫТКА подключения
        connect() успешен!
        ✓ TCP handshake: SYN → SYN-ACK → ACK
        
10:48 → 🔐 Аутентификация начата
        → AUTH MOONLIGHT2024
        ← CHALLENGE 0x7FA3B2C1
        
        💡 Обнаружение: Challenge меняется каждые 60 сек!
        (Дополнительная защита от перехвата)
        
10:49 → 🧮 Вычисление response
        hash("MOONLIGHT2024") = 0x4F2A1B3C
        response = 0x7FA3B2C1 XOR 0x4F2A1B3C = 0x3089A9FD
        
10:50 → ✅ Отправка response
        → RESPONSE 0x3089A9FD
        
10:51 → 🎯 ACCESS GRANTED!
        ← "Следующий узел: 10.0.0.1:4433"
        ← "Протокол: Encrypted TCP"
        ← "Episode 11: Packet Analysis"
        
        ⏱️ Время до deadline: 3 часа 39 минут
        ✓ Успели вовремя!
```

**Что вы понимаете:**
- Нужно написать **TCP-клиент** на C
- Использовать **socket API**: `socket()`, `connect()`, `send()`, `recv()`
- Реализовать **challenge-response** аутентификацию
- **Время ограничено** — сервер самоуничтожится!
- **Преследователи активны** — DDoS-атака на сервер
- Challenge **динамический** — защита от replay-атак

Это **первое реальное сетевое соединение под давлением**. Пора стать мастером сокетов.

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

### 6. 💡 МЕТАФОРА: TCP = Телефонный звонок

Лучший способ понять сокеты — **представить их как телефонный звонок**:

```
╔════════════════════════════════════════════════════════════╗
║  📞 TCP СОЕДИНЕНИЕ — Телефонный звонок                     ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  socket() = КУПИТЬ ТЕЛЕФОН                                 ║
║  ┌──────────┐                                              ║
║  │   📱     │  "Теперь у меня есть устройство"            ║
║  └──────────┘  Но ещё никому не звонил!                   ║
║                                                            ║
║  connect() = ПОЗВОНИТЬ НА НОМЕР                            ║
║  📱 → 📞  "Набираю 203.0.113.42:31337..."                 ║
║                                                            ║
║  TCP 3-WAY HANDSHAKE = Установка связи:                   ║
║  ┌────────────────────────────────────────┐               ║
║  │ ВЫ → СЕРВЕР:    SYN                    │               ║
║  │ "Алло, можно Виктора?"                 │               ║
║  │                                        │               ║
║  │ СЕРВЕР → ВЫ:    SYN-ACK                │               ║
║  │ "Да, слушаю!"                          │               ║
║  │                                        │               ║
║  │ ВЫ → СЕРВЕР:    ACK                    │               ║
║  │ "Окей, начинаю разговор"               │               ║
║  └────────────────────────────────────────┘               ║
║  ✅ СОЕДИНЕНИЕ УСТАНОВЛЕНО!                                ║
║                                                            ║
║  send() = ГОВОРИТЬ В ТРУБКУ                                ║
║  📱 "AUTH MOONLIGHT2024" → 📡                              ║
║  Вы передаёте сообщение                                    ║
║                                                            ║
║  recv() = СЛУШАТЬ ОТВЕТ                                    ║
║  📡 "CHALLENGE 0x12345678" → 📱                            ║
║  Вы получаете ответ                                        ║
║                                                            ║
║  close() = ПОЛОЖИТЬ ТРУБКУ                                 ║
║  📱 ✖️  "Разговор окончен"                                 ║
║  Отправляется FIN → FIN-ACK (вежливое завершение)         ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
```

**ASCII-диаграмма TCP Handshake:**

```
     КЛИЕНТ (ВЫ)                    СЕРВЕР (V. на 31337)
         📱                                🖥️
          │                                 │
    socket()                          socket()
    bind()                            bind()
    connect() ─────────────────────→  listen()
          │                                 │
          │  ──── SYN (seq=1000) ────────> │
          │  "Привет! Хочу подключиться!"   │
          │                            accept()
          │                                 │
          │  <──── SYN-ACK ──────────────── │
          │  (seq=5000, ack=1001)           │
          │  "Окей! Я готов!"               │
          │                                 │
          │  ──── ACK (ack=5001) ────────> │
          │  "Отлично! Начинаем!"           │
          │                                 │
          │  ═══ CONNECTED ════════════════ │
          │                                 │
    send()                              recv()
          │  ──── "AUTH MOONLIGHT2024" ──> │
          │                                 │
          │                            send()
          │  <──── "CHALLENGE 0x..." ────── │
    recv()│                                 │
          │                                 │
    send()                              recv()
          │  ──── "RESPONSE 0x..." ──────> │
          │                                 │
          │                            send()
          │  <──── "ACCESS GRANTED" ──────── │
    recv()│                                 │
          │                                 │
    close()                            close()
          │  ──── FIN ─────────────────────> │
          │  <──── FIN-ACK ───────────────── │
          │  ──── ACK ─────────────────────> │
          │                                 │
          ✖️                                  ✖️
    Соединение закрыто          Соединение закрыто
```

**Типичные ошибки (аналогия с телефоном):**

```
╔════════════════════════════════════════════════════════════╗
║  ОШИБКА                   │  ТЕЛЕФОННАЯ АНАЛОГИЯ            ║
╠════════════════════════════════════════════════════════════╣
║  ECONNREFUSED             │  "Абонент недоступен"          ║
║  (сервер не запущен)      │  Телефон выключен              ║
║                           │                                ║
║  ETIMEDOUT                │  "Нет ответа"                  ║
║  (сервер не отвечает)     │  Звоните, но никто не берёт    ║
║                           │                                ║
║  ECONNRESET               │  "Сбросили звонок"             ║
║  (сервер разорвал связь)  │  Положили трубку в лицо        ║
║                           │                                ║
║  ENETUNREACH              │  "Сеть недоступна"             ║
║  (нет маршрута)           │  Нет связи вообще              ║
║                           │                                ║
║  EAGAIN / EWOULDBLOCK     │  "Линия занята"                ║
║  (неблокирующий режим)    │  Попробуйте позже              ║
╚════════════════════════════════════════════════════════════╝
```

**Практический пример в коде:**

```c
// ШАГ 1: Купить телефон
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) {
    perror("Не могу создать сокет (купить телефон)");
    return 1;
}

// ШАГ 2: Набрать номер (настроить адрес)
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(31337);  // Порт — как добавочный номер
inet_pton(AF_INET, "203.0.113.42", &server_addr.sin_addr);

// ШАГ 3: Позвонить (установить соединение)
printf("Звоню на 203.0.113.42:31337...\n");
if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("Не могу дозвониться (connect failed)");
    // Может быть: ECONNREFUSED, ETIMEDOUT, ENETUNREACH...
    close(sockfd);
    return 1;
}
printf("✅ Соединение установлено! (connected)\n");

// ШАГ 4: Говорить в трубку (отправить данные)
char message[] = "AUTH MOONLIGHT2024\n";
printf("Говорю: %s", message);
ssize_t sent = send(sockfd, message, strlen(message), 0);
if (sent < 0) {
    perror("Не могу говорить (send failed)");
    close(sockfd);
    return 1;
}

// ШАГ 5: Слушать ответ (получить данные)
char buffer[1024];
memset(buffer, 0, sizeof(buffer));
ssize_t received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
if (received < 0) {
    perror("Не могу слышать (recv failed)");
    close(sockfd);
    return 1;
} else if (received == 0) {
    printf("❌ Сервер положил трубку (connection closed)\n");
} else {
    printf("Слышу: %s\n", buffer);
}

// ШАГ 6: Положить трубку (закрыть соединение)
printf("Кладу трубку...\n");
close(sockfd);
printf("✅ Разговор завершён\n");
```

**Ключевые отличия TCP vs UDP:**

```
TCP (SOCK_STREAM) = Телефонный звонок
├─ Нужно установить соединение (connect)
├─ Гарантия доставки (если сказали — услышат)
├─ Порядок сохраняется (слова в правильном порядке)
└─ Медленнее (handshake, подтверждения)

UDP (SOCK_DGRAM) = Отправить открытку по почте
├─ Без установки соединения (просто отправил)
├─ Нет гарантии доставки (может потеряться)
├─ Порядок не гарантирован (могут прийти вразнобой)
└─ Быстрее (нет overhead)
```

---

### 7. Обработка ошибок

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
│   ├── simple_echo_client_solution.c  ← Промежуточная задача
│   ├── moonlight_client_solution.c
│   └── test_server.c      ← Локальный тестовый сервер
└── tests/
    ├── test_simple_echo_client.sh  ← Тесты для промежуточной задачи
    └── test.sh
```

### 💡 Как работать:

**🔸 STEP 1: Промежуточная задача `simple_echo_client.c`**

Прежде чем создавать сложный клиент с аутентификацией, освойте базовые сокеты!

1. **Создайте** файл `simple_echo_client.c` в `artifacts/`
2. **Реализуйте** простой TCP-клиент:
   - `socket()` — создание сокета
   - `connect()` — подключение к серверу
   - `send()` — отправка строки "HELLO MOONLIGHT"
   - `recv()` — получение эха обратно
   - `close()` — закрытие соединения
3. **Компиляция**:
   ```bash
   gcc -Wall -o simple_echo_client simple_echo_client.c
   ```
4. **Тестирование**:
   ```bash
   # Запустите echo-сервер в отдельном терминале:
   nc -l 7777
   
   # В другом терминале запустите клиент:
   ./simple_echo_client 127.0.0.1 7777
   ```
5. **Автотесты**:
   ```bash
   cd tests/
   ./test_simple_echo_client.sh
   ```

**Пример вывода**:
```
═══════════════════════════════════════
    SIMPLE ECHO CLIENT
═══════════════════════════════════════

Target: 127.0.0.1:7777
Message: HELLO MOONLIGHT

[1/5] Creating socket...
  ✓ Socket created (fd=3)

[2/5] Setting up server address...
  ✓ Address: 127.0.0.1:7777
  ✓ Network byte order: 0x1E61

[3/5] Connecting to server...
  ✓ Connected!
  ✓ TCP handshake complete (SYN → SYN-ACK → ACK)

[4/5] Sending message...
  ✓ Sent: "HELLO MOONLIGHT" (15 bytes)

[5/5] Receiving echo...
  ✓ Received: "HELLO MOONLIGHT" (15 bytes)

═══════════════════════════════════════
    RESULT
═══════════════════════════════════════

✅ Echo successful!
   Sent:     "HELLO MOONLIGHT"
   Received: "HELLO MOONLIGHT"
   Match: ✓

🔒 Connection closed.
```

---

**🔸 STEP 2: Основная задача `moonlight_client.c`**

После освоения базовых сокетов переходите к основной миссии!

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

## 📝 КОНТРОЛЬНЫЕ ВОПРОСЫ

Проверьте своё понимание Episode 10 (вопросы интегрированы в сюжет миссии):

### 🟢 Базовое понимание (4 вопроса)

**1. Почему при первой попытке подключения (10:16) вы получили CONNECTION REFUSED?**

<details>
<summary>Ответ</summary>

**Сервер V. был под DDoS-атакой от агентов Z.!**

Технически:
- Агенты Z. отправляли тысячи SYN-пакетов с 185.220.101.0/24
- Сервер не мог обработать легитимные запросы
- `connect()` вернул ошибку `ECONNREFUSED`

В коде:
```c
if (connect(sockfd, ...) < 0) {
    if (errno == ECONNREFUSED) {
        printf("Server не отвечает - возможно, под атакой!\n");
    }
}
```

**Урок:** В реальности серверы могут быть недоступны по разным причинам (атака, перегрузка, firewall).
</details>

**2. Что такое TCP 3-way handshake и как он помог установить соединение с сервером V.?**

<details>
<summary>Ответ</summary>

**TCP 3-way handshake** — это процесс установки надёжного соединения:

```
ВЫ → СЕРВЕР V.:     SYN (seq=1000)
                    "Привет! Хочу подключиться!"

СЕРВЕР V. → ВЫ:     SYN-ACK (seq=5000, ack=1001)
                    "Окей! Я готов!"

ВЫ → СЕРВЕР V.:     ACK (ack=5001)
                    "Отлично! Начинаем!"

═══ СОЕДИНЕНИЕ УСТАНОВЛЕНО ═══
```

**В миссии:**
- 10:47 — вторая попытка
- `connect()` выполнил handshake автоматически
- После ACK → можно отправлять `AUTH MOONLIGHT2024`

**Зачем 3 пакета?**
- Гарантия: оба знают, что канал работает в обе стороны
- Синхронизация sequence numbers
- Защита от old duplicate packets
</details>

**3. Зачем V. использовал challenge-response аутентификацию вместо простой отправки пароля?**

<details>
<summary>Ответ</summary>

**Безопасность!**

**Плохой вариант** (простая отправка):
```
ВЫ → СЕРВЕР: "Пароль: MOONLIGHT2024"
```
❌ Агенты Z. перехватывают пакет → узнают пароль!

**Хороший вариант** (challenge-response):
```
ВЫ → СЕРВЕР:    "AUTH MOONLIGHT2024"
СЕРВЕР → ВЫ:    "CHALLENGE 0x7FA3B2C1"  (случайное число!)
ВЫ → СЕРВЕР:    "RESPONSE 0x3089A9FD"   (challenge XOR hash(пароль))
```
✅ Каждый раз challenge **новый**!  
✅ Перехват бесполезен — в следующий раз challenge другой!

**В миссии:**
- Challenge меняется каждые 60 секунд
- Защита от replay-атак
- Агенты Z. не могут повторить вашу сессию

**Урок:** Никогда не отправляйте пароли напрямую!
</details>

**4. Какая ошибка возникла бы, если бы вы забыли вызвать `close(sockfd)` после завершения?**

<details>
<summary>Ответ</summary>

**Утечка файлового дескриптора!**

**Проблема:**
```c
int sockfd = socket(...);
connect(sockfd, ...);
send(sockfd, ...);
recv(sockfd, ...);
// Забыли close(sockfd)!  ← УТЕЧКА!
```

**Последствия:**
1. **Ресурсы не освобождаются:**
   - Дескриптор занят навсегда
   - У процесса лимит дескрипторов (обычно 1024)
   
2. **Сервер ждёт FIN:**
   - Соединение висит в ESTABLISHED
   - Занимает память на сервере V.
   
3. **При повторных запусках:**
   - Быстро исчерпаете лимит
   - Новые `socket()` вернут -1 (EMFILE)

**В миссии:**
- Если бы вы забыли `close()` после каждой попытки подключения
- После ~1000 попыток → "Too many open files"
- Программа сломается

**Правильно:**
```c
close(sockfd);  // ВСЕГДА закрывайте сокеты!
```

**Аналогия:** Как потерянные ключи от квартир (из Episode 07 про утечки памяти)!
</details>

---

### 🟡 Практика (5 вопросов)

**5. В миссии вы отправляли "AUTH MOONLIGHT2024\n". Что произойдёт, если забыть `\n`?**

<details>
<summary>Ответ</summary>

**Сервер может не распознать команду!**

**Зависит от протокола сервера:**

**Вариант 1:** Сервер ждёт `\n` как признак конца команды:
```c
// Сервер:
char buffer[1024];
recv(sockfd, buffer, sizeof(buffer), 0);
// Ищет \n чтобы понять, что команда готова
char *newline = strchr(buffer, '\n');
if (!newline) {
    // Команда не завершена, ждём ещё данных
    // Timeout → разрыв соединения
}
```

**Вариант 2:** Сервер читает фиксированное количество байт:
```c
recv(sockfd, buffer, 19, 0);  // "AUTH MOONLIGHT2024" = 19 байт
// Работает БЕЗ \n
```

**В миссии V.:**
- Протокол текстовый, построчный
- `\n` = разделитель команд
- Без `\n` → сервер ждёт продолжения → timeout → ECONNRESET

**Урок:** Всегда читайте спецификацию протокола!
</details>

**6. Вычислите response для challenge 0x12345678, если ключ "MOONLIGHT2024":**

<details>
<summary>Ответ</summary>

**Шаг 1:** Вычислить hash ключа
```c
uint32_t simple_hash(const char *str) {
    uint32_t hash = 0;
    for (int i = 0; str[i]; i++) {
        hash = (hash * 31) + str[i];
    }
    return hash;
}

hash("MOONLIGHT2024") = 0x4F2A1B3C  // (результат вычисления)
```

**Шаг 2:** XOR challenge с hash
```c
uint32_t challenge = 0x12345678;
uint32_t key_hash  = 0x4F2A1B3C;
uint32_t response  = challenge ^ key_hash;

response = 0x12345678 ^ 0x4F2A1B3C
         = 0x5D1E4D44
```

**Ответ:** `0x5D1E4D44`

**В битах:**
```
Challenge: 00010010 00110100 01010110 01111000
Key hash:  01001111 00101010 00011011 00111100
           ─────────────────────────────────────
Response:  01011101 00011110 01001101 01000100
           = 0x5D1E4D44
```
</details>

**7. Почему `send()` может вернуть меньше байт, чем вы запросили?**

<details>
<summary>Ответ</summary>

**TCP работает с буферами, а сеть может быть медленной!**

**Пример:**
```c
char message[1000] = "AUTH MOONLIGHT2024...";  // 1000 байт
ssize_t sent = send(sockfd, message, 1000, 0);
printf("Отправлено: %ld байт\n", sent);  // Может быть < 1000!
```

**Причины:**
1. **Буфер отправки полон:**
   - У сокета есть send buffer (обычно 16-64 KB)
   - Если буфер почти полон → `send()` запишет сколько поместится
   
2. **Медленная сеть:**
   - Данные не успели уйти в сеть
   - Буфер переполнен → частичная отправка
   
3. **Неблокирующий режим:**
   - `send()` не ждёт → возвращает что успел

**В миссии:**
- "AUTH MOONLIGHT2024\n" = 20 байт (мало)
- Скорее всего, `send()` вернёт 20 (всё отправлено)
- Но **всегда проверяйте!**

**Правильный код:**
```c
ssize_t total_sent = 0;
while (total_sent < message_len) {
    ssize_t sent = send(sockfd, message + total_sent, 
                        message_len - total_sent, 0);
    if (sent < 0) {
        perror("send failed");
        break;
    }
    total_sent += sent;
}
```
</details>

**8. Что означает, если `recv()` вернул 0?**

<details>
<summary>Ответ</summary>

**Сервер закрыл соединение (FIN)!**

```c
ssize_t received = recv(sockfd, buffer, sizeof(buffer), 0);
if (received == 0) {
    printf("Сервер V. завершил соединение\n");
    // Это нормально, если всё получено
    close(sockfd);
}
```

**Что произошло:**
- Сервер вызвал `close(sockfd)`
- Отправлен TCP FIN пакет
- Ваш `recv()` вернул 0 = "конец потока"

**В миссии:**
```
10:51 → Вы получили "ACCESS GRANTED\n..."
        Вся информация передана
        
        Сервер: close(client_fd);  ← Отправка FIN
        
        Ваш recv() вернул 0
        ✓ Соединение завершено корректно
```

**Отличие от ошибки:**
```c
if (received < 0) {
    // Ошибка! (ECONNRESET, ETIMEDOUT...)
    perror("recv failed");
} else if (received == 0) {
    // Нормальное завершение (FIN)
    printf("Connection closed gracefully\n");
}
```
</details>

**9. В сюжете сервер самоуничтожается через 5 часов. Как это реализовать в коде?**

<details>
<summary>Ответ</summary>

**Вариант 1: Таймер на сервере**
```c
#include <time.h>

time_t start_time = time(NULL);
time_t deadline = start_time + (5 * 3600);  // 5 часов

while (1) {
    // Принимаем клиентов
    int client_fd = accept(server_fd, ...);
    
    // Проверка deadline
    if (time(NULL) >= deadline) {
        printf("DEADLINE REACHED! Self-destructing...\n");
        unlink("server_data.db");  // Удалить данные
        close(server_fd);
        exit(0);  // Завершить сервер
    }
    
    // Обработать клиента...
}
```

**Вариант 2: Отдельный поток-таймер**
```c
#include <pthread.h>

void* timer_thread(void* arg) {
    sleep(5 * 3600);  // 5 часов
    printf("Self-destruct activated!\n");
    exit(0);  // Убить весь процесс
}

int main() {
    pthread_t timer;
    pthread_create(&timer, NULL, timer_thread, NULL);
    
    // Основной цикл сервера...
}
```

**Вариант 3: Внешний cron/systemd timer**
```bash
# Запланировать уничтожение через 5 часов
at now + 5 hours <<< "pkill moonlight_server && rm -f /var/data/*"
```

**В миссии V.:**
- Сервер запущен в 09:30
- Deadline: 14:30 (5 часов)
- Вы успели подключиться в 10:51 ✓
</details>

---

### 🔴 Анализ кода (3 вопроса)

**10. Найдите ошибку в этом коде подключения к серверу V.:**

```c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = 31337;  // ← ОШИБКА?
inet_pton(AF_INET, "203.0.113.42", &addr.sin_addr);
connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
```

<details>
<summary>Ответ</summary>

**Ошибка: забыли `htons()` для порта!**

**Проблема:**
```c
addr.sin_port = 31337;  // ❌ Host byte order!
```

На little-endian машине (Intel):
- `31337` = `0x7A69`
- В памяти: `69 7A` (младший байт первым)
- Сервер ждёт: `7A 69` (network byte order)
- **Порт неправильный!**

**Исправление:**
```c
addr.sin_port = htons(31337);  // ✅ Network byte order!
```

**Результат ошибки:**
- Попытка подключиться к порту 26985 (0x697A в decimal)
- `connect()` вернёт ECONNREFUSED
- "Сервер недоступен" (хотя он работает на порту 31337!)

**Урок:** **ВСЕГДА** используйте `htons()` для портов!
</details>

**11. Почему этот код может зависнуть навсегда?**

```c
char buffer[1024];
while (1) {
    ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n > 0) {
        printf("Received: %s\n", buffer);
        break;
    }
}
```

<details>
<summary>Ответ</summary>

**`recv()` блокирует навсегда, если сервер молчит!**

**Проблема:**
- `recv()` **блокирующий** вызов по умолчанию
- Ждёт данных **бесконечно**
- Если сервер завис или медленный → вы тоже зависли

**В миссии:**
- Если сервер V. под DDoS-атакой
- Или потерял соединение
- Ваш клиент повиснет в `recv()`

**Решения:**

**1. Timeout:**
```c
struct timeval tv;
tv.tv_sec = 30;   // 30 секунд timeout
tv.tv_usec = 0;
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
if (n < 0 && errno == EWOULDBLOCK) {
    printf("Timeout! Сервер не ответил за 30 секунд\n");
}
```

**2. Неблокирующий режим:**
```c
fcntl(sockfd, F_SETFL, O_NONBLOCK);
ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
if (n < 0 && errno == EAGAIN) {
    printf("Данных пока нет\n");
}
```

**3. select() с таймаутом:**
```c
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sockfd, &readfds);
struct timeval tv = {.tv_sec = 30};

if (select(sockfd + 1, &readfds, NULL, NULL, &tv) > 0) {
    recv(sockfd, buffer, sizeof(buffer), 0);  // Данные есть!
} else {
    printf("Timeout!\n");
}
```
</details>

**12. В чём проблема этой обработки challenge-response?**

```c
char buffer[1024];
recv(sockfd, buffer, sizeof(buffer), 0);
uint32_t challenge;
sscanf(buffer, "CHALLENGE %u", &challenge);

uint32_t response = challenge ^ 0x4F2A1B3C;  // ← ПРОБЛЕМА?
char resp_msg[100];
sprintf(resp_msg, "RESPONSE %u\n", response);
send(sockfd, resp_msg, strlen(resp_msg), 0);
```

<details>
<summary>Ответ</summary>

**Ошибка: hardcoded hash ключа!**

**Проблемы:**

1. **Магическое число:**
   ```c
   uint32_t response = challenge ^ 0x4F2A1B3C;  // Что это?!
   ```
   - Непонятно, откуда взялось `0x4F2A1B3C`
   - Если ключ изменится ("MOONLIGHT2025") → сломается
   - Невозможно переиспользовать для другого ключа

2. **Нет проверки `recv()`:**
   ```c
   recv(sockfd, buffer, sizeof(buffer), 0);  // А если ошибка?
   ```

3. **Buffer не обнулён:**
   ```c
   char buffer[1024];  // Может содержать мусор!
   ```

**Правильный код:**
```c
// 1. Функция вычисления hash
uint32_t simple_hash(const char *key) {
    uint32_t hash = 0;
    for (int i = 0; key[i]; i++) {
        hash = (hash * 31) + key[i];
    }
    return hash;
}

// 2. Приём challenge с проверкой
char buffer[1024];
memset(buffer, 0, sizeof(buffer));

ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
if (n <= 0) {
    perror("recv failed");
    return -1;
}

// 3. Парсинг
uint32_t challenge;
if (sscanf(buffer, "CHALLENGE %u", &challenge) != 1) {
    fprintf(stderr, "Invalid challenge format\n");
    return -1;
}

// 4. Вычисление response (не hardcode!)
const char *key = "MOONLIGHT2024";
uint32_t key_hash = simple_hash(key);
uint32_t response = challenge ^ key_hash;

// 5. Отправка
char resp_msg[100];
snprintf(resp_msg, sizeof(resp_msg), "RESPONSE %u\n", response);
send(sockfd, resp_msg, strlen(resp_msg), 0);
```

**Урок:** Никогда не hardcode криптографические значения!
</details>

---

### 🎯 Сюжет (2 вопроса)

**13. В 10:16 первая попытка подключения провалилась (DDoS-атака). Как можно было обнаружить атаку до попытки подключения?**

<details>
<summary>Ответ</summary>

**Сканирование сети перед подключением!**

**Способы обнаружения:**

**1. Ping сервера:**
```bash
$ ping 203.0.113.42
PING 203.0.113.42: 56 data bytes
Request timeout for icmp_seq 0  ← Сервер не отвечает!
Request timeout for icmp_seq 1
```

**2. Проверка порта (nc/telnet):**
```bash
$ nc -zv 203.0.113.42 31337
nc: connect to 203.0.113.42 port 31337 (tcp) failed: Connection refused
```

**3. Анализ трафика (tcpdump/wireshark):**
```bash
$ sudo tcpdump -i any host 203.0.113.42
18:16:01 IP 185.220.101.5 > 203.0.113.42: Flags [S], seq 1234
18:16:01 IP 185.220.101.7 > 203.0.113.42: Flags [S], seq 5678
18:16:01 IP 185.220.101.9 > 203.0.113.42: Flags [S], seq 9101
...
# Тысячи SYN пакетов! → DDoS-атака!
```

**4. Проверка доступности (в коде):**
```c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);

// Установить timeout на connect
struct timeval tv = {.tv_sec = 5};  // 5 секунд
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

if (connect(sockfd, ...) < 0) {
    if (errno == ETIMEDOUT) {
        printf("Сервер не отвечает - возможно, под атакой\n");
        printf("Попробуем позже...\n");
        sleep(1800);  // Подождать 30 минут
    }
}
```

**В миссии:**
- V. предупредил в SMS (10:17): "Атаку отражаю"
- Вы подождали 30 минут
- 10:47 — успешное подключение ✓
</details>

**14. Почему координаты следующего узла (10.0.0.1:4433) — это private IP? Как к нему подключиться?**

<details>
<summary>Ответ</summary>

**10.0.0.1 — private IP (RFC 1918)!**

**Проблема:**
- `10.0.0.0/8` — не маршрутизируется в интернете
- Это **внутренний адрес** локальной сети
- Прямое подключение из интернета невозможно

**Варианты подключения:**

**1. Вы уже внутри сети:**
```bash
# Проверка интерфейсов
$ ip addr
eth0: 10.0.0.5/8  ← Вы в сети 10.0.0.0/8!

# Можно подключиться напрямую
$ ./moonlight_client 10.0.0.1 4433
✓ Connected!
```

**2. VPN / Tunnel:**
```bash
# V. предоставил VPN доступ
$ sudo openvpn moonlight.ovpn
# Теперь у вас есть tun0 с IP 10.0.0.100
$ ./moonlight_client 10.0.0.1 4433
```

**3. SSH Port Forwarding:**
```bash
# Через промежуточный сервер
$ ssh -L 4433:10.0.0.1:4433 user@203.0.113.42
# Локально:
$ ./moonlight_client 127.0.0.1 4433
# → перенаправится на 10.0.0.1:4433
```

**4. Это loopback тест:**
```bash
# Для тестирования используется 127.0.0.1
# В реальности 10.0.0.1 → 127.0.0.1 (локальный сервер)
$ ./test_server 4433 &  # Запустить на 127.0.0.1:4433
$ ./moonlight_client 127.0.0.1 4433
```

**В сюжете:**
- Episode 11-12: перехват трафика на `10.0.0.1:4433`
- Скорее всего, это **локальный тестовый сервер**
- Для обучения используется 127.0.0.1 (loopback)
- В реальной миссии был бы public IP
</details>

---

### 🚀 Продвинутые (1 вопрос)

**15. V. упомянул, что challenge меняется каждые 60 секунд. Как это защищает от replay-атаки?**

<details>
<summary>Ответ</summary>

**Replay-атака = повтор перехваченной сессии!**

**Сценарий атаки БЕЗ защиты:**

```
1. Вы → Сервер V.: "AUTH MOONLIGHT2024"
   Сервер → Вы:    "CHALLENGE 12345"
   Вы → Сервер:    "RESPONSE 67890"
   Сервер:         "ACCESS GRANTED"
   
2. Агент Z. перехватывает пакеты (Wireshark)
   
3. Позже агент Z. повторяет:
   Агент Z. → Сервер: "AUTH MOONLIGHT2024"
   Сервер → Z.:       "CHALLENGE 12345"  ← ТОТ ЖЕ!
   Агент Z. → Сервер: "RESPONSE 67890"  ← ПОВТОР!
   Сервер:            "ACCESS GRANTED"  ← Z. ВОШЁЛ! ❌
```

**Защита: динамический challenge!**

```
1. Вы (10:48):
   Сервер: challenge = random() = 0x7FA3B2C1
   Вы: response = 0x7FA3B2C1 ^ hash("MOONLIGHT2024")
   ✓ Доступ

2. Агент Z. (10:50):
   Сервер: challenge = random() = 0x1234ABCD  ← НОВЫЙ!
   Агент Z.: "RESPONSE 0x???"  ← Старый response НЕ ПОДХОДИТ!
   Сервер: "AUTH FAILED" ❌
```

**Почему challenge через 60 секунд?**
```c
// Сервер V.:
time_t current_minute = time(NULL) / 60;
uint32_t challenge = hash(current_minute);  // Меняется каждую минуту

// Через 59 секунд — тот же challenge
// Через 61 секунду — новый challenge
```

**Защита:**
- Перехваченная сессия устаревает за 60 секунд
- Агенты Z. не могут переиспользовать
- Нужно знать ключ "MOONLIGHT2024"

**Ещё лучше:** Nonce (одноразовый, не повторяется никогда):
```c
uint32_t challenge = arc4random();  // Случайное каждый раз
```

**Урок:** Challenge-response + динамический challenge = защита от replay!
</details>

---

**🎓 Оценка:**
- **13-15 правильных** — отлично! Мастер сокетов ✅
- **10-12 правильных** — хорошо, но повторите challenge-response 📖
- **< 10 правильных** — перечитайте Episode 10 заново 🔄

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