# Episode 11: "Packet Analysis" 📡

> *"Every packet tells a story. Learn to listen."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   18:20, среда, 1 октября                       ║
║  СТАТУС:  🔴 INTERCEPTION REQUIRED                       ║
╚══════════════════════════════════════════════════════════╝
```

После успешного подключения к серверу V., вы получили доступ. Но в 18:17 ситуация меняется.

Новое сообщение от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 18:17:42 UTC                         │
├─────────────────────────────────────────────┤
│                                             │
│  Подключиться — это полдела.                │
│  Настоящая игра — ПЕРЕХВАТ.                 │
│                                             │
│  На твоём интерфейсе идёт трафик.           │
│  Это не просто пакеты.                      │
│  Это коммуникация агентов противника.       │
│                                             │
│  ЗАДАЧА:                                    │
│  Перехватить TCP пакеты на порту 9999       │
│  Извлечь payload из каждого пакета          │
│  Найти скрытое сообщение                    │
│                                             │
│  Используй RAW SOCKETS.                     │
│  Это единственный способ видеть ВСЁ.        │
│                                             │
│  ⚠️ Требуются ROOT права (sudo)             │
│                                             │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

Вы открываете консоль и запускаете `netstat`:

```
Active Internet connections
Proto Local Address    Foreign Address    State
TCP   127.0.0.1:9999   127.0.0.1:54321   ESTABLISHED
TCP   127.0.0.1:54321  127.0.0.1:9999    ESTABLISHED
```

**Что вы понимаете:**
- На порту **9999** идёт активная коммуникация
- Обычные сокеты видят только данные **вашего приложения**
- **Raw sockets** видят **все пакеты** на сетевом интерфейсе
- Нужно **парсить IP и TCP заголовки** вручную
- Извлечь **payload** (данные после заголовков)

Это момент истины. Время стать **packet sniffer**.

### 🎯 ЗАДАЧА

Создать **`packet_sniffer`** — анализатор сетевых пакетов:
1. ✅ Создать raw socket (SOCK_RAW)
2. ✅ Перехватывать все TCP пакеты
3. ✅ Парсить IP заголовок (source, dest, protocol)
4. ✅ Парсить TCP заголовок (ports, flags, seq)
5. ✅ Извлекать payload (данные)
6. ✅ Фильтровать по порту 9999
7. ✅ Hex dump пакетов
8. ✅ Найти скрытое сообщение

**Сложность:** 🔴 Hard  
**Новые навыки:** Raw sockets, packet sniffing, header parsing, hex dump

```
╔══════════════════════════════════════════════╗
║  [!] ВНИМАНИЕ:                               ║
║  Raw sockets — мощный инструмент.            ║
║  Вы видите ВСЁ. Используйте ответственно.    ║
║  Требуется sudo (root права).                ║
╚══════════════════════════════════════════════╝
```

---

## 📚 Теория: Raw Sockets и Packet Sniffing

### 💡 Зачем это нужно в реальности?

**Packet sniffing** используется в:
- 🔍 **Network debugging**: Wireshark, tcpdump
- 🛡️ **Intrusion Detection**: Snort, Suricata
- 📊 **Traffic analysis**: мониторинг сети
- 🔐 **Security research**: анализ атак
- 🎮 **Protocol reverse engineering**: игровые протоколы

**Это основа всех сетевых анализаторов.**

---

### 1. Raw Sockets — что это?

**Обычный сокет** (SOCK_STREAM):
- Видит только данные **вашего** приложения
- Ядро обрабатывает заголовки
- Удобно, но ограниченно

**Raw сокет** (SOCK_RAW):
- Видит **все пакеты** на интерфейсе
- Получаете **полный** пакет (с заголовками!)
- Полный контроль, но нужно парсить самому

```c
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

// Создание raw socket для TCP пакетов
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

// ⚠️ Требует root!
if (sockfd < 0) {
    perror("socket (are you root?)");
    exit(1);
}
```

**Почему нужен root?** Потому что raw socket может перехватывать **любой** трафик (угроза безопасности).

---

### 2. Структура IP заголовка (IPv4)

```c
#include <netinet/ip.h>

struct iphdr {
    uint8_t  ihl:4;        // IP Header Length (в 32-bit словах)
    uint8_t  version:4;    // Версия (4 для IPv4)
    uint8_t  tos;          // Type of Service
    uint16_t tot_len;      // Total Length (весь пакет)
    uint16_t id;           // Identification
    uint16_t frag_off;     // Fragment Offset
    uint8_t  ttl;          // Time To Live
    uint8_t  protocol;     // Protocol (6=TCP, 17=UDP, 1=ICMP)
    uint16_t check;        // Header Checksum
    uint32_t saddr;        // Source Address
    uint32_t daddr;        // Destination Address
};
```

**Важно**:
- `ihl` — длина заголовка в **32-bit словах** (обычно 5 = 20 байт)
- `protocol` — тип протокола (TCP=6, UDP=17)
- `saddr/daddr` — в **network byte order** (нужен `ntohl()`)

---

### 3. Структура TCP заголовка

```c
#include <netinet/tcp.h>

struct tcphdr {
    uint16_t source;       // Source port
    uint16_t dest;         // Destination port
    uint32_t seq;          // Sequence number
    uint32_t ack_seq;      // Acknowledgment number
    uint16_t res1:4;       // Reserved
    uint16_t doff:4;       // Data offset (длина заголовка)
    uint16_t fin:1, syn:1, rst:1, psh:1,
             ack:1, urg:1, ece:1, cwr:1;  // Flags
    uint16_t window;       // Window size
    uint16_t check;        // Checksum
    uint16_t urg_ptr;      // Urgent pointer
};
```

**TCP Flags**:
- `SYN` — начало соединения
- `ACK` — подтверждение
- `FIN` — закрытие соединения
- `RST` — сброс соединения
- `PSH` — немедленная отправка данных

---

### 4. Парсинг пакета

```c
unsigned char buffer[65536];  // Максимальный размер IP пакета

// Получить пакет
ssize_t size = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

// 1. Парсить IP заголовок
struct iphdr *ip = (struct iphdr*)buffer;

// Размер IP заголовка (в байтах)
int ip_header_len = ip->ihl * 4;

// 2. Парсить TCP заголовок (после IP)
struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_header_len);

// Размер TCP заголовка (в байтах)
int tcp_header_len = tcp->doff * 4;

// 3. Payload (данные после заголовков)
unsigned char *payload = buffer + ip_header_len + tcp_header_len;
int payload_len = ntohs(ip->tot_len) - ip_header_len - tcp_header_len;

printf("Payload: ");
for (int i = 0; i < payload_len; i++) {
    printf("%02X ", payload[i]);
}
printf("\n");
```

---

### 5. Конвертация адресов

```c
// IP в network byte order → строка
struct in_addr src_addr, dst_addr;
src_addr.s_addr = ip->saddr;
dst_addr.s_addr = ip->daddr;

char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &src_addr, src_ip, INET_ADDRSTRLEN);
inet_ntop(AF_INET, &dst_addr, dst_ip, INET_ADDRSTRLEN);

printf("From: %s:%d\n", src_ip, ntohs(tcp->source));
printf("To:   %s:%d\n", dst_ip, ntohs(tcp->dest));
```

---

### 6. Hex Dump

```c
void hex_dump(unsigned char *data, int len) {
    printf("Offset   Hex                                            ASCII\n");
    printf("-------- ---------------------------------------------- ----------------\n");
    
    for (int i = 0; i < len; i += 16) {
        printf("%08X ", i);
        
        // Hex
        for (int j = 0; j < 16; j++) {
            if (i + j < len) {
                printf("%02X ", data[i + j]);
            } else {
                printf("   ");
            }
        }
        
        printf(" ");
        
        // ASCII
        for (int j = 0; j < 16 && i + j < len; j++) {
            unsigned char c = data[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        
        printf("\n");
    }
}
```

---

### 7. Фильтрация пакетов

```c
// Фильтровать только TCP (protocol = 6)
if (ip->protocol != IPPROTO_TCP) {
    continue;
}

// Фильтровать по порту
uint16_t src_port = ntohs(tcp->source);
uint16_t dst_port = ntohs(tcp->dest);

if (dst_port == 9999 || src_port == 9999) {
    printf("MOONLIGHT traffic detected!\n");
    // Анализировать payload
}
```

---

### 8. Promiscuous Mode (опционально)

**Обычный режим**: сетевая карта отбрасывает пакеты не для нас.  
**Promiscuous mode**: карта принимает **все** пакеты в сети.

```c
#include <sys/ioctl.h>
#include <net/if.h>

struct ifreq ifr;
strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);

// Получить текущие флаги
ioctl(sockfd, SIOCGIFFLAGS, &ifr);

// Установить promiscuous mode
ifr.ifr_flags |= IFF_PROMISC;
ioctl(sockfd, SIOCSIFFLAGS, &ifr);
```

**Осторожно**: В promiscuous mode вы видите **весь** трафик сегмента сети!

---

## 🎯 Миссия: "Packet Sniffer"

### Легенда

Агенты противника обмениваются сообщениями на порту **9999**. V. подозревает, что в пакетах скрыта координата следующего узла.

**Ваша задача**: перехватить TCP трафик и извлечь скрытое сообщение.

**Протокол коммуникации**:
```
Agent A → Agent B: TCP 127.0.0.1:54321 → 127.0.0.1:9999
Payload: [зашифрованные данные]

Agent B → Agent A: TCP 127.0.0.1:9999 → 127.0.0.1:54321
Payload: [ответ]
```

**Скрытое сообщение**: извлекается из **payload** каждого пакета на порт 9999.

---

### Технические требования

**1. Создать raw socket для TCP**:
```c
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
```

**2. Перехватывать пакеты**:
```c
while (1) {
    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    // Парсинг
}
```

**3. Извлечь информацию**:
- Source IP:Port
- Destination IP:Port
- TCP flags (SYN, ACK, PSH, FIN)
- Payload (данные после TCP заголовка)

**4. Фильтровать по порту 9999**

**5. Hex dump payload**

**6. Извлечь секретное сообщение** (ASCII символы из payload)

---

## 📝 Структура эпизода

```
episode-11-packet-analysis/
├── README.md              ← Вы здесь
├── mission.md
├── starter.c              ← Шаблон packet sniffer
├── artifacts/
│   └── packet_sniffer.c   ← Создайте этот файл
├── solution/
│   ├── packet_sniffer_solution.c
│   └── traffic_generator.c  ← Генератор тестового трафика
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там базовый raw socket с TODO
2. **Создайте** файл `artifacts/packet_sniffer.c`
3. **Реализуйте** packet sniffing:
   - Создание raw socket (`SOCK_RAW`, `IPPROTO_TCP`)
   - Парсинг IP заголовка (`struct iphdr`)
   - Парсинг TCP заголовка (`struct tcphdr`)
   - Извлечение payload
   - Фильтрация по порту 9999
   - Hex dump и ASCII extraction
4. **Скомпилируйте**: `gcc -Wall -o packet_sniffer artifacts/packet_sniffer.c`
5. **Запустите с sudo**: `sudo ./packet_sniffer`

> 💡 **Для тестирования**: используйте `solution/traffic_generator.c` для генерации тестовых пакетов!

---

### Подсказки

<details>
<summary>Как создать raw socket?</summary>

```c
#include <sys/socket.h>
#include <netinet/ip.h>

int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
if (sockfd < 0) {
    perror("socket (are you root?)");
    exit(1);
}
```

**Важно**: запускайте с `sudo`!
</details>

<details>
<summary>Как парсить заголовки?</summary>

```c
unsigned char buffer[65536];
ssize_t size = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

// IP header
struct iphdr *ip = (struct iphdr*)buffer;
int ip_len = ip->ihl * 4;

// TCP header (после IP)
struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_len);
int tcp_len = tcp->doff * 4;

// Payload
unsigned char *payload = buffer + ip_len + tcp_len;
int payload_len = ntohs(ip->tot_len) - ip_len - tcp_len;
```
</details>

<details>
<summary>Как фильтровать по порту?</summary>

```c
uint16_t src_port = ntohs(tcp->source);
uint16_t dst_port = ntohs(tcp->dest);

if (dst_port == 9999 || src_port == 9999) {
    printf("Target traffic detected!\n");
    // Обработать
}
```
</details>

<details>
<summary>Как извлечь ASCII из payload?</summary>

```c
printf("ASCII: ");
for (int i = 0; i < payload_len; i++) {
    if (payload[i] >= 32 && payload[i] <= 126) {
        printf("%c", payload[i]);
    } else {
        printf(".");
    }
}
printf("\n");
```
</details>

---

## ✅ Проверка

```bash
# Компиляция
gcc -Wall -o packet_sniffer artifacts/packet_sniffer.c

# В одном терминале: генератор трафика
gcc -Wall -o traffic_gen solution/traffic_generator.c
./traffic_gen &

# В другом терминале: sniffer (с sudo!)
sudo ./packet_sniffer

# Остановить генератор
pkill traffic_gen
```

**Критерии успеха**:
- Raw socket создан успешно
- Парсит IP и TCP заголовки
- Извлекает payload
- Фильтрует по порту 9999
- Выводит hex dump
- Извлекает ASCII сообщение
- Находит скрытое сообщение

---

## 🎬 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   22:35, среда, 1 октября                       ║
║  СТАТУС:  🟢 MESSAGE INTERCEPTED                         ║
╚══════════════════════════════════════════════════════════╝
```

`packet_sniffer` запущен с sudo. Перехват начат. Результат:

```
=== PACKET SNIFFER ===
Listening on all interfaces...
Filtering for port 9999

[Packet #1]
From: 127.0.0.1:54321 → 127.0.0.1:9999
Protocol: TCP
Flags: PSH ACK
Payload (24 bytes):

Offset   Hex                                  ASCII
-------- ------------------------------------ ----------------
00000000 54 68 65 20 6E 65 78 74 20 6E 6F 64 The next nod
00000010 65 3A 20 31 30 2E 30 2E 30 2E 31     e: 10.0.0.1

Decoded: "The next node: 10.0.0.1"

[Packet #2]
From: 127.0.0.1:9999 → 127.0.0.1:54321
Protocol: TCP
Flags: PSH ACK
Payload (18 bytes):

Offset   Hex                                  ASCII
-------- ------------------------------------ ----------------
00000000 50 6F 72 74 3A 20 34 34 33 33 0A     Port: 4433.

🎯 HIDDEN MESSAGE EXTRACTED:
┌─────────────────────────────────────────┐
│  Next node: 10.0.0.1:4433               │
│  Protocol: Encrypted TCP                │
│  Mission: Episode 12                    │
└─────────────────────────────────────────┘
```

**Что вы узнали:**
- **Raw sockets** дают полный контроль над пакетами
- **IP и TCP заголовки** содержат всю информацию о коммуникации
- **Payload** — это данные после заголовков
- Следующий узел: **10.0.0.1:4433** (зашифрованный канал)

В 22:33 финальное сообщение от V.:

```
┌─────────────────────────────────────────┐
│  FROM: V.                               │
├─────────────────────────────────────────┤
│                                         │
│  Ты научился перехватывать.             │
│  Теперь финал: РАСШИФРОВКА.             │
│                                         │
│  Episode 12: Encrypted Communications   │
│  Все знания соберутся воедино.          │
│                                         │
│  10.0.0.1:4433 ждёт тебя.               │
│                                         │
│  — V.                                   │
└─────────────────────────────────────────┘
```

**Видеть пакеты — это сила. Понимать их — это искусство.**

### 🎖️ Навык разблокирован:
**"Packet Master"** — Вы видите сеть на уровне байтов. Raw sockets больше не тайна.

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Отсылки и секреты</summary>

1. **Raw sockets** — основа Wireshark, tcpdump, Snort
2. **Port 9999** — часто используется в малварах (backdoor port)
3. **10.0.0.1** — private IP класса A, внутренняя сеть
4. **Promiscuous mode** — термин из радиоперехвата (принимать всё)
5. **struct iphdr** — прямой доступ к сетевому стеку Linux
6. **PSH flag** — "push", немедленная отправка (признак интерактивного траффика)
7. **V.'s timestamp 22:35** — 0x5623 hex, отсылка к CVE-2022-5623 (network vulnerability)
8. **Hex dump format** — классический формат из `hexdump -C`

**Философия эпизода**: *"Сеть прозрачна для тех, кто знает, как смотреть. Raw socket — это рентген."*

</details>

---

## 🔗 Навигация

- [← Episode 10: Socket Programming](../episode-10-socket-programming/README.md)
- [→ Episode 12: Encrypted Communications](../episode-12-encrypted-communications/README.md)
- [📋 Season 3: Networks](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"In the network, everything is visible. You just need to know where to look."* — V.