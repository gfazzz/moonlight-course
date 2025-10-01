# Episode 09: "Network Basics" 🌐

> *"Every connection has an address. Every address tells a story."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Safe House (неизвестный адрес), Москва        ║
║  ВРЕМЯ:   04:27, среда, 1 октября                       ║
║  СТАТУС:  🟡 NEW LEAD + POTENTIAL THREAT                 ║
╚══════════════════════════════════════════════════════════╝
```

Вы дошли до финала Season 2. Утечки памяти устранены. `buggy_logger` исправлен.  
После обнаружения GPS-координат вашей старой квартиры вы эвакуировались в safe house.

**Вы в безопасности... или нет?**

В 04:27 утра на ваш защищённый канал приходит новое сообщение от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TO:   Agent [REDACTED]                     │
│  TIME: 04:27:19 UTC                         │
│  PRIORITY: HIGH                             │
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
│  Но будь осторожен.                         │
│  Они следят за трафиком.                    │
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

### ⏱️ Временная линия событий

```
04:27 → 📧 Сообщение от V.: файл addresses.txt получен
05:30 → 🔍 Начало анализа IP-адресов и портов
06:15 → 🚨 КРИТИЧЕСКОЕ ОТКРЫТИЕ!
        IP 203.0.113.42 в hex: CB 00 71 2A
        
        Вы замечаете странность...
        CB = 203 (decimal)
        00 = 0
        71 = 113
        2A = 42
        
        Но если интерпретировать как координаты...
        203.0.113.42 → преобразование...
        
        🌍 КООРДИНАТЫ: 55.7558°N 37.6173°E
        
        Вы проверяете на карте: **КРАСНАЯ ПЛОЩАДЬ, МОСКВА!**
        
06:16 → ⚠️ SMS от номера UNKNOWN:
        "Ты слишком близко. Остановись."
        
06:17 → 💬 Сообщение от V.:
        "Игнорируй их. Продолжай анализ.
         Красная площадь — точка встречи.
         Но сначала освой сети."
         
06:45 → 🎯 Решение: продолжить анализ адресов
        Изучить основы сетей
        Создать net_analyzer
```

**Что вы понимаете:**
- IP-адреса — это **координаты в киберпространстве**
- Порты указывают на **сервисы** (SSH, HTTP, DNS...)
- Private IP (192.168, 10.0, 172.16) — **внутренние сети**
- Public IP — **доступны из интернета**
- Некоторые IP могут содержать **скрытые сообщения** (GPS-координаты!)
- **Преследователи следят** — каждое действие под угрозой обнаружения
- Красная площадь — **точка встречи с V.** (но сначала нужно освоить сети)

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

### 8. 💡 МЕТАФОРА: Интернет = Город с адресами

Чтобы лучше понять IP-адреса и порты, представь **Интернет как огромный город**:

```
╔════════════════════════════════════════════════════════════╗
║  🏙️ ГОРОД ИНТЕРНЕТ — Карта Киберпространства              ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  IP-АДРЕС = КООРДИНАТЫ ЗДАНИЯ                              ║
║  ┌────────────────────────────────────┐                   ║
║  │ 192.168.1.1                        │                   ║
║  │ = "Улица Частная, дом 192,         │                   ║
║  │    квартал 168, участок 1"         │                   ║
║  └────────────────────────────────────┘                   ║
║                                                            ║
║  ПОРТ = НОМЕР КВАРТИРЫ (или офиса) в здании                ║
║  ┌────────────────────────────────────┐                   ║
║  │ :80  → Квартира №80 (HTTP)         │                   ║
║  │ :443 → Квартира №443 (HTTPS)       │                   ║
║  │ :22  → Квартира №22 (SSH)          │                   ║
║  │ :31337 → Секретный офис V.         │                   ║
║  └────────────────────────────────────┘                   ║
║                                                            ║
║  PRIVATE IP (192.168.x.x, 10.0.x.x) = ЗАКРЫТЫЙ ПОСЁЛОК    ║
║  ┌────────────────────────────────────┐                   ║
║  │ 🚧 Ворота с охраной (NAT Router)   │                   ║
║  │                                    │                   ║
║  │  🏘️ Внутри посёлка:                │                   ║
║  │  ├─ Дом 1 (192.168.1.1)            │                   ║
║  │  ├─ Дом 100 (192.168.1.100)        │                   ║
║  │  └─ Дом 254 (192.168.1.254)        │                   ║
║  │                                    │                   ║
║  │  Снаружи не видны!                 │                   ║
║  │  Нужен пропуск через ворота.       │                   ║
║  └────────────────────────────────────┘                   ║
║                                                            ║
║  PUBLIC IP (8.8.8.8, 203.0.113.42) = ГЛАВНАЯ ПЛОЩАДЬ      ║
║  ┌────────────────────────────────────┐                   ║
║  │ 🏛️ Публичное здание                 │                   ║
║  │ Видно всем!                        │                   ║
║  │ Доступно из любой точки города.    │                   ║
║  └────────────────────────────────────┘                   ║
║                                                            ║
║  LOOPBACK (127.0.0.1) = ВАШ ДОМ                            ║
║  ┌────────────────────────────────────┐                   ║
║  │ 🏠 "Home Sweet Home"                │                   ║
║  │ Всегда указывает на ваш компьютер  │                   ║
║  │ Независимо от сети                 │                   ║
║  └────────────────────────────────────┘                   ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
```

**ASCII-арт города:**

```
           🌐 ИНТЕРНЕТ — ГОРОД СЕРВЕРОВ
    ┌─────────────────────────────────────────────┐
    │                                             │
    │  🏛️ Публичная площадь (8.8.8.8)            │
    │    ├─ Офис :53 (DNS сервис)                │
    │    ├─ Офис :80 (HTTP)                      │
    │    └─ Офис :443 (HTTPS)                    │
    │                                             │
    │  🏘️ Закрытый посёлок (192.168.0.0/16)      │
    │    🚧 NAT Router (охрана на воротах)        │
    │    ├─ 🏠 Дом 1 (192.168.1.1)               │
    │    │   ├─ Квартира :22 (SSH)               │
    │    │   └─ Квартира :80 (локальный веб)     │
    │    ├─ 🏠 Дом 100 (192.168.1.100)           │
    │    │   └─ Квартира :8080 (приложение)      │
    │    └─ 🏠 Дом 254 (192.168.1.254)           │
    │        └─ Квартира :53 (локальный DNS)     │
    │                                             │
    │  📍 Секретное убежище (203.0.113.42)       │
    │    └─ Офис :31337 (MOONLIGHT server)       │
    │       Координаты: 55.7558°N 37.6173°E      │
    │       (Красная площадь!)                   │
    │                                             │
    │  🏠 Ваш дом (127.0.0.1 - localhost)        │
    │    └─ Все ваши локальные сервисы           │
    │                                             │
    └─────────────────────────────────────────────┘
```

**Byte Order = Почтовый индекс:**

```
╔════════════════════════════════════════════════════════════╗
║  📮 BYTE ORDER — Международная почта                       ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  В России (Big-Endian, Network Order):                    ║
║  Индекс: 123456  →  пишем слева направо                   ║
║           ↑                                                ║
║      старшая цифра первой                                  ║
║                                                            ║
║  В США (Little-Endian, Host Order на Intel):               ║
║  Индекс: 654321  →  пишем наоборот                        ║
║           ↑                                                ║
║      младшая цифра первой                                  ║
║                                                            ║
║  ПРОБЛЕМА: Международное письмо!                           ║
║  Отправляем из России в США — нужна конвертация!          ║
║                                                            ║
║  В КОДЕ:                                                   ║
║  uint16_t port = 8080;  // В порядке вашей машины          ║
║  uint16_t net_port = htons(port);  // → Network order!    ║
║                                                            ║
║  htons = "Host TO Network Short"                           ║
║  ntohs = "Network TO Host Short"                           ║
║                                                            ║
║  ⚠️ ВСЕГДА конвертируй перед отправкой в сеть!             ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
```

**Практический пример:**

```c
// ❌ НЕПРАВИЛЬНО (без конвертации)
struct sockaddr_in addr;
addr.sin_port = 8080;  // Будет проблема!

// ✅ ПРАВИЛЬНО (с конвертацией)
struct sockaddr_in addr;
addr.sin_port = htons(8080);  // Network byte order!

// Проверка:
printf("Host order: %d\n", 8080);          // 8080
printf("Network order: %d\n", htons(8080)); // 36895 (на little-endian)
// В сети будет правильно интерпретировано как 8080!
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
│   ├── ip_converter.c   ← 🎯 ПРОМЕЖУТОЧНАЯ ЗАДАЧА (начните с неё!)
│   └── net_analyzer.c   ← Финальная задача
├── solution/
│   ├── ip_converter_solution.c     ← Решение промежуточной
│   └── net_analyzer_solution.c     ← Решение финальной
└── tests/
    ├── test_ip_converter.sh        ← Тест промежуточной
    └── test.sh                     ← Тест финальной
```

### 💡 Как работать:

**🎯 ШАГ 1: Промежуточная задача `ip_converter.c` (РЕКОМЕНДУЕТСЯ!)**

Перед финальной задачей освойте базовые операции с IP-адресами:

1. **Создайте** файл `artifacts/ip_converter.c`
2. **Реализуйте** функционал:
   - Ввод IP-адреса из командной строки или интерактивно
   - Преобразование строки в `struct in_addr` (`inet_pton`)
   - Конвертация в hex (network byte order)
   - Конвертация в decimal (host byte order)
   - Вывод отдельных октетов (побитовые операции)
   - Классификация IP (private/public/loopback)
3. **Скомпилируйте**: 
   ```bash
   gcc -Wall -Wextra -std=c11 -o ip_converter artifacts/ip_converter.c
   ```
4. **Запустите**:
   ```bash
   ./ip_converter 192.168.1.100
   # или интерактивный режим:
   ./ip_converter
   ```
5. **Проверьте**: 
   ```bash
   bash tests/test_ip_converter.sh
   ```

**Пример вывода:**
```
═══════════════════════════════════════
    IP ADDRESS CONVERTER
═══════════════════════════════════════

Input:
  String: 192.168.1.100
  ✓ Valid IPv4 address

Binary Format:
  Network byte order: 0x6401A8C0
  Host byte order:    0xC0A80164

Decimal:
  Unsigned 32-bit: 3232235876

Octets:
  192 . 168 . 1 . 100

Classification:
  Type: Private (192.168.0.0/16 - RFC 1918)
```

---

**🎯 ШАГ 2: Финальная задача `net_analyzer.c`**

После освоения `ip_converter`, переходите к финальной задаче:

1. **Изучите** `starter.c` — там структура `Address` и заготовки функций
2. **Создайте** файл `artifacts/net_analyzer.c`
3. **Реализуйте** анализ сетевых адресов:
   - Парсинг строк "IP:PORT" с помощью `sscanf()`
   - Валидация IP через `inet_pton()`
   - Определение типа IP (private/public/loopback)
   - Классификация портов (well-known/registered/dynamic)
   - Конвертация в hex формат
   - Статистика по файлу
4. **Скомпилируйте**: `gcc -Wall -o net_analyzer artifacts/net_analyzer.c`
5. **Запустите**: `./net_analyzer addresses.txt`
6. **Проверьте**: `bash tests/test.sh`

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
║  ЛОКАЦИЯ: Safe House (неизвестный адрес), Москва        ║
║  ВРЕМЯ:   07:15, среда, 1 октября                       ║
║  СТАТУС:  🟢 FIRST NODE IDENTIFIED + GPS DECODED         ║
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
┌─────────────────────────────────────────────────┐
│  Public IPs with port 31337:                    │
│  203.0.113.42:31337                             │
│                                                 │
│  Hex: 0xCB00712A                                │
│  Binary: 11001011 00000000 01110001 00101010    │
│           ↓        ↓        ↓        ↓          │
│         203  .   0   .  113  .  42              │
│                                                 │
│  🎯 FIRST NODE: 203.0.113.42:31337             │
│                                                 │
│  🌍 СКРЫТОЕ СООБЩЕНИЕ:                          │
│  GPS-координаты: 55.7558°N 37.6173°E            │
│  Локация: КРАСНАЯ ПЛОЩАДЬ, МОСКВА               │
│  (Спасская башня Кремля)                        │
└─────────────────────────────────────────────────┘
```

**Что вы узнали:**
- **Port 31337** (элитный хакерский порт, "leet" = 1337) — **не случайность**
- **203.0.113.0/24** — TEST-NET-3 (RFC 5737) — **маскировка под тестовую сеть**
- **Hex представление** содержит **GPS-координаты встречи**
- Это **первый узел сети MOONLIGHT** + **точка встречи с V.**
- Преследователи уже знают о вашем анализе (SMS от UNKNOWN)

В 07:12 приходит новое сообщение:

```
┌─────────────────────────────────────────┐
│  FROM: V.                               │
├─────────────────────────────────────────┤
│                                         │
│  Ты нашёл адрес. Ты нашёл место.        │
│                                         │
│  Красная площадь — точка встречи.       │
│  Но СНАЧАЛА научись сетям.              │
│                                         │
│  Episode 10: Socket Programming         │
│  Подключись к серверу 203.0.113.42      │
│  Жду тебя там.                          │
│                                         │
│  P.S. Игнорируй SMS от UNKNOWN.         │
│  Это пустые угрозы.                     │
│                                         │
│  — V.                                   │
└─────────────────────────────────────────┘
```

В 07:14 вы смотрите на карту:

```
         🌍 МОСКВА
           │
           v
    ┌──────────────┐
    │  🏛️ КРАСНАЯ   │
    │   ПЛОЩАДЬ    │  ← 55.7558°N 37.6173°E
    │              │
    │  📍 Точка    │
    │   встречи    │
    └──────────────┘
    
Расстояние от safe house: ~3.5 км
Время в пути: ~15 минут на метро
```

**IP-адреса — это координаты. GPS — это встреча. Теперь пора научиться подключаться.**

### 🎖️ Навык разблокирован:
**"Network Cartographer"** — Вы понимаете IP-адресацию и структуру сетей на уровне байтов.

---

## 📝 КОНТРОЛЬНЫЕ ВОПРОСЫ

Проверьте понимание Episode 09 (все вопросы основаны на сюжете расшифровки GPS из IP):

### 🟢 Базовое понимание (4 вопроса)

**1. В файле `addresses.txt` вы нашли IP `203.0.113.42`. Что это такое и как он помог найти Красную площадь?**

<details>
<summary>Ответ</summary>

**IP-адрес — уникальный идентификатор устройства в сети**, состоит из 4 байтов (32 бита).

**В миссии:**
```
IP: 203.0.113.42
Hex: 0xCB00712A
Binary: 11001011.00000000.01110001.00101010
```

**Расшифровка GPS:**
- Октеты `203.0` = широта `55.7558°N`
- Октеты `113.42` = долгота `37.6173°E`
- **Результат:** Красная площадь, Спасская башня Кремля!

**В коде (PATTERN DETECTED):**
```c
struct in_addr addr;
inet_pton(AF_INET, "203.0.113.42", &addr);
// → координаты встречи с V.!
```

**Технически:**
- IPv4 = 32 бита = 4 октета
- Каждый октет: 0-255
- Используется для маршрутизации пакетов
- В миссии: V. закодировал GPS в IP!

**Урок:** IP-адрес может нести скрытый смысл. Всё зависит от интерпретации.
</details>

**2. Почему IP `192.168.1.100` (из вашего safe house) не виден из интернета, а `203.0.113.42` (сервер V.) виден?**

<details>
<summary>Ответ</summary>

**`192.168.1.100` — private IP, `203.0.113.42` — public IP!**

**Private IP (RFC 1918):**
```
10.0.0.0/8        → 10.0.0.0 — 10.255.255.255
172.16.0.0/12     → 172.16.0.0 — 172.31.255.255
192.168.0.0/16    → 192.168.0.0 — 192.168.255.255  ← Ваш safe house!
```
- Используются **внутри локальных сетей**
- **НЕ маршрутизируются в интернете**
- Нужен NAT для доступа наружу

**Public IP:**
```
203.0.113.42 ← Сервер V.
```
- Уникален **глобально**
- Доступен из интернета
- Можно подключиться напрямую

**В миссии:**
```
Safe House Network:
├─ Router: 192.168.1.1 (private)
├─ Ваш компьютер: 192.168.1.100 (private)
└─ Выход в интернет: NAT → Public IP

Сервер V.:
└─ 203.0.113.42:31337 (public) ← Доступен из любой точки мира
```

**Проверка в ip_converter:**
```bash
$ ./ip_converter 192.168.1.100
Type: Private (192.168.0.0/16 - RFC 1918)

$ ./ip_converter 203.0.113.42
Type: Public (TEST-NET-3 203.0.113.0/24 - RFC 5737)
```

**Урок:** Private IP = ваша защита от прямого доступа из интернета.
</details>

**3. Сервер V. работает на `203.0.113.42:31337`. Что означает `:31337` и почему это важно?**

<details>
<summary>Ответ</summary>

**`:31337` — это порт, номер "двери" на сервере!**

**Аналогия из метафоры "Город Интернет":**
```
203.0.113.42 = адрес здания (Спасская башня)
31337 = номер квартиры (где живёт сервер V.)
```

**Зачем порт:**
- На одном IP может быть **много сервисов**
- Порт различает их

**Категории портов:**
```
0-1023      → Well-known (SSH=22, HTTP=80, HTTPS=443)
1024-49151  → Registered (приложения)
49152-65535 → Dynamic (временные соединения)
```

**В миссии:**
```
203.0.113.42:31337 ← Сервер V. (TCP)
203.0.113.42:80    ← Мог бы быть веб-сервер
203.0.113.42:22    ← Мог бы быть SSH
```

**Почему 31337?**
- **Leet speak:** 1337 = "LEET" (элитный)
- 31337 = хакерский порт (Easter Egg!)
- Скрытый сервис, не стандартный

**В коде (Episode 10):**
```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(31337);  // ← ВАЖНО: htons()!
inet_pton(AF_INET, "203.0.113.42", &addr.sin_addr);
connect(sockfd, ...);  // → Подключение к серверу V.
```

**Урок:** IP без порта бесполезен. Всегда нужно знать оба.
</details>

**4. В задаче `ip_converter.c` вы использовали `htons()`. Почему это критично для подключения к серверу V.?**

<details>
<summary>Ответ</summary>

**Network byte order (big-endian) ≠ Host byte order (little-endian на Intel)!**

**Проблема:**
```
Ваш компьютер (Intel x86):     Little-endian (младший байт первым)
Сеть (TCP/IP стандарт):          Big-endian (старший байт первым)
```

**Без `htons()` — катастрофа:**
```c
// НЕПРАВИЛЬНО:
addr.sin_port = 31337;  // 0x7A69 в little-endian

В памяти: [69 7A]
В сеть отправится: порт 26985 (0x697A)!
Сервер V. ждёт 31337, но вы стучите в 26985 → Connection refused!
```

**С `htons()` — успех:**
```c
// ПРАВИЛЬНО:
addr.sin_port = htons(31337);  // Конвертация!

Host (little): 31337 = 0x7A69 → [69 7A]
Network (big): htons() → [7A 69]
Сервер V. получает правильный порт 31337 ✓
```

**Функции преобразования:**
```c
htons()  // Host TO Network Short (16-bit порт)
htonl()  // Host TO Network Long (32-bit IP)
ntohs()  // Network TO Host Short
ntohl()  // Network TO Host Long
```

**В миссии (ip_converter.c):**
```c
printf("Network byte order: 0x%08X\n", addr.s_addr);      // Big-endian
printf("Host byte order:    0x%08X\n", ntohl(addr.s_addr)); // Little-endian

// Для 203.0.113.42:
// Network: 0xCB00712A
// Host:    0x2A7100CB
```

**Урок:** ВСЕГДА используйте `htons()`/`htonl()` для сетевых структур!
</details>

---

### 🟡 Практика (4 вопроса)

**5. Вы запустили `ip_converter` для тестирования byte order. Что вернёт `htons(0x1234)` на вашем компьютере (Intel)?**

<details>
<summary>Ответ</summary>

**`0x3412` — байты меняются местами!**

**Объяснение:**
```
Входное значение: 0x1234 (host byte order, little-endian на Intel)
В памяти Intel:   [34 12] (младший байт первым)
Network order:    [12 34] (старший байт первым - big-endian)
Результат:        0x3412
```

**В миссии (ip_converter.c):**
```c
uint16_t test_port = 0x1234;
printf("Host byte order:    0x%04X\n", test_port);     // 0x1234
printf("Network byte order: 0x%04X\n", htons(test_port)); // 0x3412
```

**Проверка:**
```bash
$ ./ip_converter
# Тестируйте разные значения:
# 0x1234 → 0x3412
# 31337 (0x7A69) → 0x697A (в network порядке)
```

**Почему это важно для миссии:**
- Порт 31337 сервера V. ОБЯЗАТЕЛЬНО через `htons()`!
- Иначе подключитесь к неправильному порту

**Урок:** Network byte order — это BIG-endian, всегда!
</details>

**6. В `net_analyzer` вы парсите `addresses.txt`. Как безопасно преобразовать строку "203.0.113.42" в `struct in_addr`?**

<details>
<summary>Ответ</summary>

**Используйте `inet_pton()` — современный и безопасный способ!**

**В миссии (net_analyzer.c):**
```c
#include <arpa/inet.h>

char line[256];
while (fgets(line, sizeof(line), file)) {
    // Прочитали строку из addresses.txt: "203.0.113.42"
    line[strcspn(line, "\n")] = '\0';  // Удалить \n
    
    struct in_addr addr;
    if (inet_pton(AF_INET, line, &addr) == 1) {
        printf("✓ Valid IP: %s\n", line);
        printf("  Binary: 0x%08X\n", ntohl(addr.s_addr));
        
        // Проверка на GPS-координаты:
        if (ntohl(addr.s_addr) == 0xCB00712A) {  // 203.0.113.42
            printf("🎯 PATTERN DETECTED: RED SQUARE!\n");
        }
    } else {
        printf("❌ Invalid IP: %s\n", line);
    }
}
```

**Старый способ (НЕ ИСПОЛЬЗУЙТЕ):**
```c
addr.s_addr = inet_addr("203.0.113.42");  // ❌ Deprecated!
// Проблемы:
// - Не проверяет валидность
// - Возвращает INADDR_NONE для ошибок
// - INADDR_NONE = 255.255.255.255 (тоже валидный IP!)
```

**Почему `inet_pton` лучше:**
1. **Возвращает код ошибки:**
   - `1` = успех
   - `0` = невалидный IP
   - `-1` = системная ошибка

2. **Работает с IPv6** (если нужно)
3. **Thread-safe**
4. **Не путает ошибки с валидными IP**

**В миссии:**
- `addresses.txt` может содержать мусор
- `inet_pton()` отфильтрует невалидные IP
- Только валидные IP → анализ → поиск GPS

**Урок:** Всегда проверяйте ввод пользователя/файлов!
</details>

**7. Если V. сменит порт с 31337 на 443 (HTTPS), что изменится в подключении?**

<details>
<summary>Ответ</summary>

**Технически — только номер порта. Но семантически — всё!**

**Технические изменения:**
```c
// Было:
addr.sin_port = htons(31337);  // Хакерский порт

// Станет:
addr.sin_port = htons(443);    // HTTPS порт
```

**НО! Порт 443 подразумевает HTTPS (TLS/SSL):**

**Без шифрования (текущая миссия):**
```c
// Episode 10:
connect(sockfd, ...);           // ← Простой TCP
send(sockfd, "AUTH ...", ...);  // ← Plaintext!
```

**С HTTPS (порт 443):**
```c
// Нужно добавить TLS:
#include <openssl/ssl.h>

SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
SSL *ssl = SSL_new(ctx);
SSL_set_fd(ssl, sockfd);
SSL_connect(ssl);              // ← TLS handshake!

SSL_write(ssl, "AUTH ...", ...);  // ← Encrypted!
SSL_read(ssl, buffer, ...);       // ← Encrypted response
```

**Well-known ports и их протоколы:**
```
Порт 80   → HTTP   (plaintext, небезопасно)
Порт 443  → HTTPS  (TLS encrypted, безопасно)
Порт 22   → SSH    (encrypted shell)
Порт 31337→ Custom (что угодно, часто plaintext)
```

**В миссии:**
- Сейчас: `203.0.113.42:31337` (plaintext TCP)
- Агенты Z. могут перехватить (Episode 11!)
- Если бы был HTTPS: шифрование защитило бы от sniffing

**Почему V. НЕ использует 443:**
1. Не хочет, чтобы выглядело как HTTPS (маскировка)
2. 31337 = хакерская культура (leet speak)
3. Plaintext удобнее для учебных целей

**Урок:** Номер порта — это конвенция, не техническое ограничение.
</details>

**8. В Episode 11 вы узнали, что агенты Z. видят ваш IP safe house. Как можно было скрыть его с помощью NAT?**

<details>
<summary>Ответ</summary>

**NAT (Network Address Translation) скрывает ваш реальный IP за роутером!**

**БЕЗ NAT (проблема в миссии):**
```
Ваш компьютер: 192.168.1.100 (private)
→ Пакет к серверу V.:
   Source IP: 192.168.1.100  ← Но это private! Не работает!
```

**С NAT (защита):**
```
          Safe House Network
          
  Ваш компьютер              Router (NAT)         Интернет
  192.168.1.100 ──────→  [NAT] ──────→  Сервер V.
                         Public IP:     203.0.113.42
                         203.0.113.50
                         
Пакет ДО NAT:
  Source: 192.168.1.100:54321
  Dest:   203.0.113.42:31337

Пакет ПОСЛЕ NAT:
  Source: 203.0.113.50:12345  ← Ваш IP скрыт!
  Dest:   203.0.113.42:31337
```

**Как NAT защищает:**
1. **Скрывает внутренний IP:**
   - Сервер V. видит только public IP роутера
   - Не знает ваш реальный 192.168.1.100

2. **Невозможно подключиться извне:**
   - Агенты Z. не могут напрямую атаковать 192.168.1.100
   - Не маршрутизируется в интернете

3. **Один public IP для многих устройств:**
   - Роутер транслирует порты
   - Экономия IPv4 адресов

**В миссии (Episode 11):**
```
Агенты Z. перехватили пакет с координатами safe house:
"Coordinates: 55.7520°N 37.6175°E"

Как они узнали?
- Геолокация PUBLIC IP вашего NAT роутера!
- IP Geolocation API: Public IP → примерное местоположение
```

**Защита от geolocation:**
```c
// 1. VPN (скрывает реальный IP)
Public IP → VPN сервер в другой стране → скрывает локацию

// 2. Tor (анонимность)
Ваш трафик → Tor network → невозможно отследить

// 3. Proxy
Подключение через прокси-сервер
```

**NAT Table (пример):**
```
Internal IP:Port    →  Public IP:Port   →  Dest IP:Port
192.168.1.100:54321 →  203.0.113.50:12345 → 203.0.113.42:31337
192.168.1.101:54322 →  203.0.113.50:12346 → 8.8.8.8:53
```

**Урок:** NAT = базовая защита, но НЕ полная анонимность!
</details>

---

### 🔴 Анализ кода (2 вопроса)

**9. Вы пытаетесь подключиться к серверу V., но получаете ошибку. В чём проблема?**

```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = 31337;  // ← ПРОБЛЕМА?
inet_pton(AF_INET, "203.0.113.42", &addr.sin_addr);
connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
// ❌ Connection refused!
```

<details>
<summary>Ответ</summary>

**Ошибка: забыли `htons()` для порта!**

**Что происходит:**
```c
addr.sin_port = 31337;  // ❌ Host byte order!

// На Intel (little-endian):
31337 decimal = 0x7A69 hex
В памяти: [69 7A]

// В сеть отправляется: 0x697A = 26985 decimal
// Пытаетесь подключиться к порту 26985 вместо 31337!
// Сервер V. на 31337 → Connection refused!
```

**Исправление:**
```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(31337);  // ✅ Network byte order!
inet_pton(AF_INET, "203.0.113.42", &addr.sin_addr);
connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
// ✓ Connected to V.'s server!
```

**В миссии:**
- Episode 10: подключение к серверу V. на 31337
- БЕЗ `htons()` = неправильный порт = failure
- С `htons()` = correct port = success!

**Проверка в ip_converter:**
```bash
$ ./ip_converter 203.0.113.42
Binary Format:
  Network byte order: 0xCB00712A
  Host byte order:    0x2A7100CB  ← Разные!
```

**Урок:** `htons()` ОБЯЗАТЕЛЕН для портов, `htonl()` для IP!
</details>

**10. В `net_analyzer` вы парсите IP из файла. Что не так?**

```c
FILE *f = fopen("addresses.txt", "r");
char line[256];
while (fgets(line, sizeof(line), f)) {
    struct in_addr addr;
    addr.s_addr = inet_addr(line);  // ← ПРОБЛЕМА?
    
    if (ntohl(addr.s_addr) == 0xCB00712A) {
        printf("Found V.'s server!\n");
    }
}
```

<details>
<summary>Ответ</summary>

**Ошибки: `inet_addr()` deprecated + не удаляется `\n` из строки!**

**Проблема 1: `\n` в конце строки**
```c
fgets(line, ...) → "203.0.113.42\n"  ← Лишний символ!
inet_addr("203.0.113.42\n") → INVALID!
```

**Проблема 2: `inet_addr()` не проверяет ошибки**
```c
// Что если в файле мусор?
"invalid.ip.here\n" → inet_addr() → INADDR_NONE (0xFFFFFFFF)
// Но 255.255.255.255 — валидный IP! Путаница!
```

**Исправление:**
```c
FILE *f = fopen("addresses.txt", "r");
char line[256];
while (fgets(line, sizeof(line), f)) {
    // 1. Удалить \n
    line[strcspn(line, "\n")] = '\0';
    
    // 2. Использовать inet_pton (с проверкой)
    struct in_addr addr;
    if (inet_pton(AF_INET, line, &addr) == 1) {
        // Валидный IP!
        if (ntohl(addr.s_addr) == 0xCB00712A) {
            printf("🎯 Found: 203.0.113.42 → RED SQUARE!\n");
        }
    } else {
        printf("⚠️  Invalid IP: %s\n", line);
        // Пропускаем мусор
    }
}
fclose(f);
```

**В миссии:**
- `addresses.txt` содержит много IP
- Может быть мусор, комментарии, пустые строки
- `inet_pton()` фильтрует невалидные → находит только 203.0.113.42

**Урок:** Всегда валидируйте ввод! `fgets()` сохраняет `\n`.
</details>

---

### 🎯 Сюжет (2 вопроса)

**11. IP `203.0.113.42` декодируется в GPS 55.7558°N 37.6173°E (Красная площадь). Где именно на площади встреча?**

<details>
<summary>Ответ</summary>

**Спасская башня Кремля** — главный вход Кремля, самая известная башня!

**Точные координаты:**
```
Latitude:  55.7558°N
Longitude: 37.6173°E
Точность:  ±10 метров
Локация:   Спасская башня (Spasskaya Tower)
```

**Как V. закодировал:**
```
IP: 203.0.113.42

Интерпретация 1 (октеты → координаты):
203.0  → 55.75 (широта, добавить константу)
113.42 → 37.61 (долгота, добавить константу)

Интерпретация 2 (hex → GPS):
0xCB00712A → битовые манипуляции → 55.7558, 37.6173
```

**В миссии:**
```
Episode 09 (04:27) → Обнаружено PATTERN: GPS coordinates
Episode 12 (08:00) → Встреча у Спасской башни
```

**ASCII карта из DEBRIEFING:**
```
        МОСКВА (Красная площадь)
        
            Кремль
         ┌────────────┐
         │            │
   Башня │   [✪]      │  Спасская
Никольская│            │  башня ← ЗДЕСЬ!
         │            │  55.7558°N
         └────────────┘  37.6173°E
              │
         Красная площадь
              │
         Собор Василия
         Блаженного
```

**Почему Спасская башня:**
- Самая знаменитая башня Кремля
- Главный вход (для важных персон)
- Часы-куранты (символ точности времени)
- Публичное место (безопаснее для встречи)

**В Episode 12:**
```
08:00 → Вы стоите у Спасской башни
        Осенний туман. Раннее утро.
        
        Человек в чёрном пальто подходит.
        "Ты пришёл," — V.
        
        Он передаёт USB-флешку:
        "Season 4: Crypto & Algorithms"
```

**Урок:** Координаты — не просто числа. Это место встречи, место истории.
</details>

**12. В 04:47 вы получили SMS от UNKNOWN: "Ты слишком близко. Остановись." Кто это и как они узнали про ваш прогресс?**

<details>
<summary>Ответ</summary>

**UNKNOWN = агенты Z., они отслеживают вас с самого начала!**

**Как они узнали:**

**1. Перехват трафика (Episode 11):**
```c
// Агенты Z. используют packet sniffing:
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
// Видят ВСЕ ваши пакеты на порту 31337

// Вы → Сервер V. (203.0.113.42:31337)
// Агенты перехватывают → "Он нашёл адрес!"
```

**2. Мониторинг сети:**
```
04:27 → Вы декодировали 203.0.113.42
        Ваш net_analyzer подключился к серверу
        
04:27 → Агенты Z. видят:
        Ваш IP → 203.0.113.42:31337
        "Target approaching V.'s server!"
        
04:47 → SMS отправлена (через 20 минут)
        "Ты слишком близко. Остановись."
        Попытка напугать/остановить
```

**3. Компрометированный safe house (Episode 11):**
```
Safe House Network:
├─ Ваш компьютер: 192.168.1.100
├─ Router: 192.168.1.1
└─ Агент Z. в сети: 192.168.1.50  ← Внедрён!

Агент запускает:
$ sudo tcpdump -i eth0 port 31337
→ Видит ваше подключение к V.
```

**4. Geolocation (ваш публичный IP):**
```bash
# Агенты используют:
$ curl "http://ip-api.com/json/YOUR_PUBLIC_IP"
{
  "lat": 55.7520,  ← Близко к Красной площади!
  "lon": 37.6175,
  "status": "success"
}

→ "Он в Москве, недалеко от цели!"
→ Отправить предупреждение
```

**Временная линия угроз:**
```
Episode 01 → V. "погиб" (инсценировка)
Episode 02 → SMS от UNKNOWN: "48 часов" (угроза)
Episode 03 → Второй "V" предупреждает: "ловушка"
Episode 09 → SMS: "Ты слишком близко" ← ЗДЕСЬ!
Episode 10 → DDoS-атака на сервер V. (попытка заблокировать)
Episode 11 → Координаты safe house раскрыты
Episode 12 → Финал: встреча с V., побег от Z.
```

**Кто такие Z.:**
- Противники V.
- Используют те же техники (packet sniffing, crypto)
- Цель: помешать вам встретиться с V.
- Будут раскрыты в Season 4

**Урок:** В киберпространстве за вами всегда могут наблюдать. Шифрование и OpSec критичны!
</details>

---

### 🚀 Продвинутые (0 вопросов)

**Все продвинутые концепции интегрированы в вопросы выше!**

---

**🎓 Оценка:**
- **10-12 правильных** — Network Master! Готовы к Episode 10 ✅
- **7-9 правильных** — хорошо, но повторите GPS-декодирование и byte order 📖
- **< 7 правильных** — перечитайте Episode 09, запустите ip_converter 🔄

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
