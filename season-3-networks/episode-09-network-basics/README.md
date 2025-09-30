# Episode 09: "Network Basics" 🌐

> *"Every connection has an address. Every address tells a story."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   04:27, среда, 1 октября                       ║
║  СТАТУС:  🟢 NEW LEAD DETECTED                           ║
╚══════════════════════════════════════════════════════════╝
```

Вы дошли до финала Season 2. Утечки памяти устранены. `buggy_logger` исправлен.  
Но в 04:27 утра на ваш защищённый канал приходит новое сообщение от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TO:   Agent [REDACTED]                     │
│  TIME: 04:27:19 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  Память — это только начало.                │
│  Настоящая игра начинается в сети.          │
│                                             │
│  Я оставил файл:                            │
│  > addresses.txt                            │
│                                             │
│  Это не просто адреса.                      │
│  Это карта серверов MOONLIGHT.              │
│                                             │
│  Найди паттерн. Найди первый узел.          │
│                                             │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

Вы открываете `addresses.txt`:

```
192.168.1.100:8080
10.0.0.5:22
8.8.8.8:53
172.16.0.1:443
127.0.0.1:80
203.0.113.42:31337
...
```

**Что вы понимаете:**
- IP-адреса — это **координаты в киберпространстве**
- Порты указывают на **сервисы** (SSH, HTTP, DNS...)
- Private IP (192.168, 10.0, 172.16) — **внутренние сети**
- Public IP — **доступны из интернета**
- Паттерн в адресах может указать на **первый узел сети MOONLIGHT**

V. не зря прислал это. Пора освоить **сетевое программирование**.

### 🎯 ЗАДАЧА

Создать **`net_analyzer`** — программу для анализа IP-адресов:
1. ✅ Парсинг IP:PORT из файла
2. ✅ Классификация (private/public/loopback)
3. ✅ Определение сервисов по портам
4. ✅ Конвертация в hex формат
5. ✅ Извлечение паттерна (путь к серверу)

**Сложность:** 🟡 Medium  
**Новые навыки:** TCP/IP, сетевые структуры, byte order, сокеты (preview)

```
╔══════════════════════════════════════════════╗
║  [!] ВНИМАНИЕ:                               ║
║  Вы входите в Season 3: NETWORKS.           ║
║  Здесь каждый пакет — след.                  ║
║  Каждое соединение — зацепка.                ║
╚══════════════════════════════════════════════╝
```

---

## 📚 Теория: Основы сетей TCP/IP

### 1. Модель OSI и TCP/IP

**OSI (7 слоёв)** vs **TCP/IP (4 слоя)**:

```
OSI Model          TCP/IP Model      Примеры
-------------------------------------------------
Application  ]     
Presentation ] --> Application  --> HTTP, FTP, DNS
Session      ]     

Transport    ] --> Transport    --> TCP, UDP

Network      ] --> Internet     --> IP, ICMP

Data Link    ]
Physical     ] --> Link         --> Ethernet, WiFi
```

**В C программируем на уровне Transport и Internet.**

---

### 2. IP-адреса и структуры

#### IPv4 адрес (32 бита = 4 байта)

```c
#include <netinet/in.h>
#include <arpa/inet.h>

// Структура для IPv4
struct in_addr {
    uint32_t s_addr;  // Network byte order (big-endian)
};

// Структура для сокета (IP + порт)
struct sockaddr_in {
    sa_family_t    sin_family;  // AF_INET
    in_port_t      sin_port;    // Порт (network byte order)
    struct in_addr sin_addr;    // IP адрес
    char           sin_zero[8]; // Padding
};
```

#### Преобразование адресов

```c
// Строка → бинарный формат
struct in_addr addr;
inet_pton(AF_INET, "192.168.1.1", &addr);

// Бинарный → строка
char ip_str[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN);
printf("IP: %s\n", ip_str);  // "192.168.1.1"

// Старый способ (deprecated, но часто встречается)
addr.s_addr = inet_addr("192.168.1.1");
printf("IP: %s\n", inet_ntoa(addr));
```

---

### 3. Byte Order (порядок байтов)

**Проблема**: разные процессоры хранят числа по-разному.

- **Little-endian** (Intel x86): младший байт первым  
  `0x12345678` → `78 56 34 12`

- **Big-endian** (Network): старший байт первым  
  `0x12345678` → `12 34 56 78`

**Сеть всегда использует big-endian (network byte order)!**

#### Функции преобразования

```c
#include <arpa/inet.h>

// Host to Network (Short - 16 bit)
uint16_t port = 8080;
uint16_t network_port = htons(port);

// Host to Network (Long - 32 bit)
uint32_t ip = 0xC0A80101;  // 192.168.1.1
uint32_t network_ip = htonl(ip);

// Network to Host
uint16_t host_port = ntohs(network_port);
uint32_t host_ip = ntohl(network_ip);
```

**Правило**: Всегда конвертируйте перед отправкой/после получения!

---

### 4. Порты и протоколы

**Порт** — это число от 0 до 65535, идентифицирующее приложение.

```c
// Well-known ports (0-1023) — зарезервированы
#define PORT_HTTP   80
#define PORT_HTTPS  443
#define PORT_SSH    22
#define PORT_FTP    21

// Registered ports (1024-49151)
// Dynamic/private (49152-65535)
```

**TCP vs UDP**:

| TCP                          | UDP                        |
|------------------------------|----------------------------|
| Connection-oriented          | Connectionless             |
| Надёжный (retransmit)        | Ненадёжный (fire and forget)|
| Упорядоченный                | Неупорядоченный            |
| Медленнее                    | Быстрее                    |
| HTTP, SSH, FTP               | DNS, VoIP, streaming       |

---

### 5. Структуры IP и TCP заголовков

#### IP Header (IPv4)

```c
struct iphdr {
    uint8_t  version:4;       // IP версия (4)
    uint8_t  ihl:4;           // Header length
    uint8_t  tos;             // Type of service
    uint16_t tot_len;         // Total length
    uint16_t id;              // Identification
    uint16_t frag_off;        // Fragment offset
    uint8_t  ttl;             // Time to live
    uint8_t  protocol;        // Protocol (TCP=6, UDP=17)
    uint16_t check;           // Checksum
    uint32_t saddr;           // Source address
    uint32_t daddr;           // Destination address
};
```

#### TCP Header

```c
struct tcphdr {
    uint16_t source;          // Source port
    uint16_t dest;            // Destination port
    uint32_t seq;             // Sequence number
    uint32_t ack_seq;         // Acknowledgment
    uint16_t res1:4;          // Reserved
    uint16_t doff:4;          // Data offset
    uint16_t fin:1, syn:1, rst:1, psh:1,
             ack:1, urg:1, ece:1, cwr:1;  // Flags
    uint16_t window;          // Window size
    uint16_t check;           // Checksum
    uint16_t urg_ptr;         // Urgent pointer
};
```

---

### 6. Основные сетевые функции

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Создать сокет
int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // TCP
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);   // UDP

// Заполнить структуру адреса
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

// Привязать к адресу (для сервера)
bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

// Подключиться (для клиента)
connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
```

---

### 7. Полезные утилиты

```bash
# Показать сетевые интерфейсы
ifconfig         # macOS/старый Linux
ip addr          # Linux

# Показать таблицу маршрутизации
netstat -rn      # macOS
ip route         # Linux

# Показать активные соединения
netstat -an      # Все
ss -tunap        # Linux (современный)

# Проверить доступность хоста
ping 8.8.8.8

# Трассировка маршрута
traceroute google.com
```

---

## 🎯 Миссия: "Анализатор адресов"

### Легенда

V. оставил зашифрованный файл со списком IP-адресов и портов. Это не просто адреса — в них закодирована информация:

- **IP-адреса** содержат геолокацию серверов
- **Порты** указывают на сервисы
- **Комбинации** образуют паттерн

**Ваша задача**: создать `net_analyzer` — утилиту, которая:
1. Читает файл с IP:PORT записями
2. Проверяет корректность адресов
3. Классифицирует по типам (private/public, well-known ports)
4. Извлекает скрытое сообщение

---

### Формат входных данных

Файл `addresses.txt`:
```
192.168.1.100:8080
10.0.0.5:22
8.8.8.8:53
172.16.0.1:443
127.0.0.1:80
203.0.113.42:31337
```

Каждая строка: `IP:PORT`

---

### Технические требования

**Входные данные**: файл `addresses.txt`

**Выходные данные**:
```
=== NETWORK ANALYZER ===
Analyzing addresses.txt...

[1] 192.168.1.100:8080
    Type: Private (RFC 1918)
    Network: 192.168.1.0/24
    Port: 8080 (HTTP Alternate)
    Hex: 0xC0A80164

[2] 10.0.0.5:22
    Type: Private (RFC 1918)
    Network: 10.0.0.0/8
    Port: 22 (SSH - Well-Known)
    Hex: 0x0A000005

[3] 8.8.8.8:53
    Type: Public
    Port: 53 (DNS - Well-Known)
    Hex: 0x08080808

...

Statistics:
- Total addresses: 6
- Private: 3 (50%)
- Public: 3 (50%)
- Well-known ports: 4
- Custom ports: 2

Hidden message: [Extracted from port patterns]
```

**Функционал**:
1. Парсинг IP:PORT из строк
2. Валидация IP-адресов
3. Определение типа (private/public/loopback)
4. Классификация портов (well-known/registered/dynamic)
5. Конвертация в hex формат
6. Вычисление сети по маске
7. Извлечение скрытого сообщения (опционально)

---

## 📝 Задание

### Starter Code

Файл `starter.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

#define MAX_LINE 256

typedef struct {
    struct in_addr ip;
    uint16_t port;
} Address;

// TODO: Implement functions

// Parse "IP:PORT" string
int parse_address(const char *str, Address *addr) {
    // Hint: use strtok or sscanf
    return 0;
}

// Check if IP is private (RFC 1918)
int is_private_ip(struct in_addr addr) {
    // 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16
    return 0;
}

// Get port type name
const char* get_port_type(uint16_t port) {
    // Well-known (0-1023), Registered (1024-49151), Dynamic (49152+)
    return "Unknown";
}

// Get service name for well-known port
const char* get_service_name(uint16_t port) {
    // 22=SSH, 80=HTTP, 443=HTTPS, etc.
    return "Unknown";
}

int main(int argc, char *argv[]) {
    // TODO: Read file, parse addresses, analyze, print stats
    return 0;
}
```

---

## 📝 Структура эпизода

```
episode-09-network-basics/
├── README.md            ← Вы здесь
├── mission.md
├── starter.c            ← Шаблон с примерами
├── artifacts/
│   └── net_analyzer.c   ← Создайте этот файл
├── solution/
│   └── net_analyzer_solution.c
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там структура `Address` и заготовки функций
2. **Создайте** файл `artifacts/net_analyzer.c`
3. **Реализуйте** анализ сетевых адресов:
   - Парсинг строк "IP:PORT" с помощью `sscanf()`
   - Валидация IP через `inet_pton()`
   - Определение типа IP (private/public/loopback)
   - Классификация портов (well-known/registered/dynamic)
   - Конвертация в hex формат
4. **Скомпилируйте**: `gcc -Wall -o net_analyzer artifacts/net_analyzer.c`
5. **Запустите**: `./net_analyzer addresses.txt`

---

### Подсказки

<details>
<summary>Как парсить "IP:PORT"?</summary>

```c
int parse_address(const char *str, Address *addr) {
    char ip_str[INET_ADDRSTRLEN];
    int port;
    
    if (sscanf(str, "%15[^:]:%d", ip_str, &port) != 2) {
        return -1;
    }
    
    if (inet_pton(AF_INET, ip_str, &addr->ip) != 1) {
        return -1;
    }
    
    addr->port = (uint16_t)port;
    return 0;
}
```
</details>

<details>
<summary>Как проверить private IP?</summary>

```c
int is_private_ip(struct in_addr addr) {
    uint32_t ip = ntohl(addr.s_addr);
    
    // 10.0.0.0/8
    if ((ip & 0xFF000000) == 0x0A000000) return 1;
    
    // 172.16.0.0/12
    if ((ip & 0xFFF00000) == 0xAC100000) return 1;
    
    // 192.168.0.0/16
    if ((ip & 0xFFFF0000) == 0xC0A80000) return 1;
    
    return 0;
}
```
</details>

<details>
<summary>Как получить имя сервиса?</summary>

```c
const char* get_service_name(uint16_t port) {
    switch (port) {
        case 20: return "FTP-DATA";
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 443: return "HTTPS";
        default: return "Unknown";
    }
}
```
</details>

---

## ✅ Проверка

```bash
make
make test
```

**Критерии успеха**:
- Корректно парсит все адреса
- Правильно определяет private/public
- Классифицирует порты
- Конвертирует в hex
- Выводит статистику

---

## 🎁 Артефакт

После выполнения миссии вы получите:
- **Навык**: работа с IP-адресами и портами в C
- **Инструмент**: `net_analyzer` — анализатор сетевых адресов
- **Фундамент**: готовность к Episode 10 (сокеты)

---

## 🎬 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   07:15, среда, 1 октября                       ║
║  СТАТУС:  🟢 FIRST NODE IDENTIFIED                       ║
╚══════════════════════════════════════════════════════════╝
```

`net_analyzer` запущен. Анализ завершён. Результат:

```
=== NETWORK ANALYZER ===
Analyzing addresses.txt... ✓

Statistics:
- Total addresses: 42
- Private: 18 (42.9%)
- Public: 21 (50.0%)
- Loopback: 3 (7.1%)
- Well-known ports: 28

🔍 PATTERN DETECTED:
┌─────────────────────────────────────────┐
│  Public IPs with port 31337:            │
│  203.0.113.42:31337                     │
│                                         │
│  Hex: 0xCB00712A                        │
│  Binary: 11001011 00000000 01110001     │
│           ↓        ↓        ↓           │
│         203  .   0   .  113  .  42      │
│                                         │
│  🎯 FIRST NODE: 203.0.113.42:31337     │
└─────────────────────────────────────────┘
```

**Что вы узнали:**
- **Port 31337** (элитный хакерский порт, "leet" = 1337) — **не случайность**
- **203.0.113.0/24** — TEST-NET-3 (RFC 5737) — **маскировка под тестовую сеть**
- **Hex представление** содержит **закономерность** в битах
- Это **первый узел сети MOONLIGHT**

В 07:12 приходит новое сообщение:

```
┌─────────────────────────────────────────┐
│  FROM: V.                               │
├─────────────────────────────────────────┤
│                                         │
│  Ты нашёл адрес.                        │
│  Теперь научись ПОДКЛЮЧАТЬСЯ.           │
│                                         │
│  Episode 10: Socket Programming         │
│  Жду тебя на сервере.                   │
│                                         │
│  — V.                                   │
└─────────────────────────────────────────┘
```

**IP-адреса — это координаты. Теперь пора научиться путешествовать.**

### 🎖️ Навык разблокирован:
**"Network Cartographer"** — Вы понимаете IP-адресацию и структуру сетей на уровне байтов.

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Отсылки и секреты</summary>

1. **Port 31337** — классический "элитный" порт хакеров (1337 = leet speak)
2. **203.0.113.0/24** — TEST-NET-3, зарезервированная сеть для документации (RFC 5737)
3. **8.8.8.8** — Google Public DNS, одна из самых известных IP
4. **127.0.0.1** — "localhost", адрес "home sweet home" для любого компьютера
5. **Binary 11001011** — CB hex, начало адреса, первые байты часто используются для routing
6. **V.'s timestamp 04:27:19** — 0x042713 hex = отсылка к дате (April 27, 2013 — известный инцидент)

**Философия эпизода**: *"Сеть — это карта. IP — координаты. Порт — дверь. Научись читать карту."*

</details>

---

## 🔗 Навигация

- [← Episode 08: Fragmentation](../../season-2-memory-and-pointers/episode-08-fragmentation/README.md)
- [→ Episode 10: Socket Programming](../episode-10-socket-programming/README.md)
- [📋 Season 3: Networks](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"IP addresses are coordinates in cyberspace. Learn to navigate."* — V.
