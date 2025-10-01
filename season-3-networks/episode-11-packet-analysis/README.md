# Episode 11: "Packet Analysis" 📡

> *"Every packet tells a story. Learn to listen."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Safe House (неизвестный адрес), Москва        ║
║  ВРЕМЯ:   18:20, среда, 1 октября                       ║
║  СТАТУС:  🔴 COUNTER-SURVEILLANCE DETECTED!              ║
╚══════════════════════════════════════════════════════════╝
```

После успешного подключения к серверу V., вы получили доступ.  
Координаты следующего узла: `10.0.0.1:4433`.

**Но в 18:17 ситуация кардинально меняется.**

Новое сообщение от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 18:17:42 UTC                         │
│  PRIORITY: 🔴 CRITICAL                      │
├─────────────────────────────────────────────┤
│                                             │
│  Подключиться — это полдела.                │
│  Настоящая игра — ПЕРЕХВАТ.                 │
│                                             │
│  На твоём интерфейсе идёт трафик.           │
│  Агенты Z. обмениваются данными на 9999.    │
│                                             │
│  ⚠️ ОПАСНОСТЬ:                              │
│  Они ТОЖЕ используют packet sniffing!       │
│  Возможно, они уже знают о safe house.      │
│                                             │
│  ЗАДАЧА:                                    │
│  Перехватить их TCP пакеты на порту 9999    │
│  Извлечь payload из каждого пакета          │
│  Найти: что они знают о тебе?               │
│                                             │
│  Используй RAW SOCKETS.                     │
│  Это единственный способ видеть ВСЁ.        │
│                                             │
│  ⚠️ Требуются ROOT права (sudo)             │
│                                             │
│  Будь осторожен. Охотник может стать        │
│  добычей.                                   │
│                                             │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

### ⏱️ Временная линия событий

```
18:17 → 📧 Сообщение от V.: перехватить трафик агентов Z.
        Порт 9999 — их канал коммуникации
        
18:20 → 🔍 Запуск netstat для анализа
        $ netstat -an | grep 9999
        
        Active Internet connections:
        TCP  127.0.0.1:9999   127.0.0.1:54321  ESTABLISHED
        TCP  127.0.0.1:54321  127.0.0.1:9999   ESTABLISHED
        
        💡 Обнаружено: Активная коммуникация!
        
18:25 → 🛠️ Разработка packet sniffer
        Изучение raw sockets, struct iphdr, struct tcphdr
        
19:45 → 🚀 Первый запуск packet_sniffer
        $ sudo ./packet_sniffer
        
        Listening on all interfaces...
        Filtering for port 9999
        
19:46 → 📡 ПЕРВЫЙ ПАКЕТ перехвачен!
        From: 127.0.0.1:54321 → 127.0.0.1:9999
        Protocol: TCP, Flags: PSH ACK
        Payload (48 bytes):
        "Target confirmed. Location: Safe house."
        
19:47 → 🚨 ВТОРОЙ ПАКЕТ — ШОКИРУЮЩЕЕ СОДЕРЖАНИЕ!
        From: 127.0.0.1:9999 → 127.0.0.1:54321
        Payload (64 bytes):
        "Coordinates: 55.7520°N 37.6175°E"
        "Status: Under surveillance"
        "Agent count: 1"
        
        💀 ЭТО КООРДИНАТЫ ВАШЕГО SAFE HOUSE!!!
        
19:48 → ⚠️ ТРЕТИЙ ПАКЕТ:
        "Order: Continue monitoring. Do not engage."
        "Report to HQ every 30 minutes."
        
20:00 → 💡 ОСОЗНАНИЕ:
        Преследователи Z. ТОЖЕ используют packet sniffing!
        Они перехватывают ВАШ трафик!
        "Охотник" стал "добычей"...
        
20:15 → 📞 SMS от V.:
        "Они следят за тобой.
         Но теперь ТЫ следишь за ними.
         Информационная война.
         Продолжай перехват."
         
20:30 → 🎯 ЧЕТВЁРТЫЙ ПАКЕТ — КЛЮЧЕВАЯ ИНФОРМАЦИЯ:
        "Next node: 10.0.0.1:4433"
        "Protocol: Encrypted TCP"
        "Objective: Intercept encrypted stream"
        
        ✅ Получены координаты следующего узла!
        (Тот же адрес, что дал V. — подтверждение!)
        
22:35 → ✅ Перехват завершён
        Извлечено: 127 пакетов
        Скрытое сообщение декодировано
        Episode 11 Complete
```

Вы открываете консоль и запускаете `netstat`:

```
Active Internet connections
Proto Local Address    Foreign Address    State
TCP   127.0.0.1:9999   127.0.0.1:54321   ESTABLISHED
TCP   127.0.0.1:54321  127.0.0.1:9999    ESTABLISHED
```

**Что вы понимаете:**
- На порту **9999** идёт активная коммуникация **агентов Z.**
- Обычные сокеты видят только данные **вашего приложения**
- **Raw sockets** видят **все пакеты** на сетевом интерфейсе
- Нужно **парсить IP и TCP заголовки** вручную
- Извлечь **payload** (данные после заголовков)
- **КРИТИЧНО**: В пакетах могут быть данные о ВАШЕМ местоположении!
- **"Охотник становится добычей"** — взаимная слежка

Это момент истины. Время стать **packet sniffer** и узнать, что враги знают о вас.

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

## 🎭 МЕТАФОРА: Packet Sniffing = Подслушивающее устройство

**Представьте:**  
Вы сидите в кафе. Вокруг много людей, и все они разговаривают. Обычно вы слышите только своего собеседника, а остальные голоса — фоновый шум.

**НО:** Если у вас есть **направленный микрофон** (подслушивающее устройство), вы можете:
- Настроиться на **любой разговор** в кафе
- Слышать **весь разговор** целиком
- **Записывать** каждое слово
- **Фильтровать** по голосу (например, только мужские голоса)

**В сети:**

```
Обычный TCP сокет (socket()):
├─ Вы → Собеседник (203.0.113.42:31337)
└─ Слышите ТОЛЬКО ваш разговор
   ❌ Другие разговоры не видны

Raw socket (SOCK_RAW):
├─ Вы → ВСЯ СЕТЬ
├─ Слышите ВСЕ разговоры на вашем сегменте сети!
│   ├─ 192.168.1.50 → 8.8.8.8:53 (DNS запрос)
│   ├─ 192.168.1.100 → 203.0.113.42:31337 (ваш трафик)
│   └─ 192.168.1.150 → 10.0.0.1:9999 (агенты Z.!)
└─ ✅ Можете фильтровать по IP/порту/протоколу
```

**ASCII-диаграмма:**

```
         КАФЕ (СЕТЬ 192.168.1.0/24)
         
  [Стол 1]        [Стол 2]        [Стол 3]
  A ↔ B           C ↔ D           ВЫ ↔ V.
  (DNS)           (HTTPS)         (TCP:31337)
    ↓               ↓               ↓
    └───────────────┴───────────────┘
                    │
            [Подслушивающее
             устройство]
             (Raw socket)
                    │
           ┌────────┴────────┐
           │  Вы слышите:    │
           │  ✓ A → B        │
           │  ✓ C → D        │
           │  ✓ ВЫ ↔ V.      │
           │  ✓ Агенты Z.!   │
           └─────────────────┘

НО! Агенты Z. тоже имеют подслушивающее устройство:
  → Они слышат ВАШ трафик!
  → Информационная война (взаимный sniffing)
```

**Почему это работает:**

1. **Ethernet = широковещательная среда:**
   - Все пакеты передаются **всем** устройствам на сегменте
   - Обычно: ваша сетевая карта **отбрасывает** чужие пакеты
   - Raw socket: вы говорите карте **НЕ отбрасывать** → видите всё!

2. **Promiscuous mode:**
   ```c
   // Обычный режим:
   NIC: "Этот пакет не для меня (dest MAC != мой MAC) → отбросить"
   
   // Promiscuous mode:
   NIC: "Этот пакет не для меня → ВСЁ РАВНО ПРИНЯТЬ и отдать приложению!"
   ```

3. **Фильтрация:**
   ```c
   // Все пакеты → фильтр → только интересующие
   if (tcp->dest == htons(9999)) {  // Агенты Z. на порту 9999
       printf("Перехвачено сообщение агентов!\n");
   }
   ```

**В миссии:**
```
18:20 → Агенты Z. общаются на порту 9999
        ВЫ (192.168.1.100) запускаете packet_sniffer
        
19:15 → Перехвачен пакет:
        Source: 192.168.1.150 (агент Z.)
        Dest:   10.0.0.1:9999
        Payload: "Coordinates: 55.7520°N 37.6175°E"
        
19:16 → ВЫ УЗНАЛИ координаты safe house!
        Агенты Z. знают, где вы!
        
НО:
        Агенты Z. ТАКЖЕ перехватывают ВАШ трафик!
        → Взаимный sniffing
        → Информационная война
```

**Защита от sniffing:**
- ✅ **Шифрование** (TLS/SSL) — даже если перехватят, не расшифруют
- ✅ **VPN** — туннель, скрывает содержимое
- ✅ **Физическая изоляция** — отдельный сегмент сети
- ❌ **Plaintext** — любой может прочитать (как в миссии!)

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

## 📝 КОНТРОЛЬНЫЕ ВОПРОСЫ

Проверьте понимание Episode 11 (все вопросы основаны на сюжете контр-слежки):

### 🟢 Базовое понимание (4 вопроса)

**1. В 19:47 вы перехватили ВТОРОЙ ПАКЕТ с координатами "55.7520°N 37.6175°E". Почему это был шок?**

<details>
<summary>Ответ</summary>

**Это координаты ВАШЕГО safe house! Агенты Z. знают, где вы находитесь!**

**Что произошло:**
```
19:47 → Второй пакет (From: 127.0.0.1:9999 → 127.0.0.1:54321)
        Payload: "Coordinates: 55.7520°N 37.6175°E"
                 "Status: Under surveillance"
                 "Agent count: 1"
```

**Анализ:**
- Координаты точно указывают на ваше убежище
- "Under surveillance" = вас активно отслеживают
- "Agent count: 1" = они знают, что вы один

**Технически:**
```c
// В packet_sniffer вы извлекли payload:
char *payload_start = (char*)(packet + ip_header_len + tcp_header_len);
int payload_len = total_len - ip_header_len - tcp_header_len;

// И увидели:
// "Coordinates: 55.7520°N 37.6175°E"
```

**Осознание:** "Охотник стал добычей" — вы следили за ними, но **ОНИ тоже следят за ВАМИ**.

**Урок:** Packet sniffing — обоюдоострый меч. Если вы можете перехватывать трафик, то и противник может.
</details>

**2. Почему для запуска packet_sniffer нужны ROOT права (sudo)?**

<details>
<summary>Ответ</summary>

**Потому что raw sockets требуют привилегированного доступа к сетевому стеку!**

**Причина безопасности:**
```c
// Создание raw socket:
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
// ^^^ Только root может делать это!

if (sockfd < 0) {
    perror("socket");  // "Operation not permitted" без sudo
    exit(1);
}
```

**Почему root?**
1. **Raw socket видит ВСЕ пакеты:**
   - Включая пакеты других пользователей
   - Включая зашифрованные соединения
   - Это нарушение конфиденциальности!

2. **Можно подделать пакеты:**
   - С любым source IP
   - С любым source port
   - Спуфинг атак, man-in-the-middle

3. **Обход firewall:**
   - Raw sockets работают на уровне IP
   - Минуют многие защиты

**В миссии:**
```bash
$ ./packet_sniffer
socket: Operation not permitted  ← Без sudo!

$ sudo ./packet_sniffer
Listening on all interfaces...  ✓
```

**Альтернативы (без root):**
- `pcap` библиотека (но нужны права на интерфейс)
- `tcpdump` с setuid (небезопасно)
- Docker с capabilities (CAP_NET_RAW)

**Урок:** Raw sockets = мощный инструмент, требует ответственности.
</details>

**3. В 20:00 вы осознали, что агенты Z. ТОЖЕ используют packet sniffing. Как они могли перехватить ваш трафик?**

<details>
<summary>Ответ</summary>

**Они на том же сетевом сегменте или имеют доступ к маршрутизатору!**

**Сценарий 1: Shared network (WiFi, общая сеть):**
```
       ВАШ КОМПЬЮТЕР              АГЕНТ Z.
           📱                         👤
            │                         │
            └────────┬────────────────┘
                     │
                 [WiFi Router]
                 
→ Агент Z. включает promiscuous mode на WiFi:
  sudo ifconfig wlan0 promisc
  sudo tcpdump -i wlan0
  
→ Видит ВСЕ пакеты в WiFi сети, включая ваши!
```

**Сценарий 2: Man-in-the-Middle (ARP spoofing):**
```c
// Агент Z. отправляет поддельные ARP пакеты:
// "Я — это маршрутизатор!"
// Весь трафик идёт через него
```

**Сценарий 3: Компрометированный роутер:**
```
Агенты Z. взломали WiFi роутер safe house
→ Настроили port mirroring
→ Весь трафик копируется им
```

**В миссии:**
- Safe house компрометирован
- Агенты Z. имеют доступ к локальной сети
- Они запустили свой packet_sniffer
- Перехватывают ваши соединения

**Защита:**
- VPN (шифрование трафика)
- HTTPS (TLS)
- Сменить safe house (физическая безопасность)

**Урок:** Безопасность сети = безопасность физическая + криптография.
</details>

**4. Что означает флаг "PSH ACK" в перехваченных TCP пакетах?**

<details>
<summary>Ответ</summary>

**PSH = "Push" (немедленно отправить), ACK = "Acknowledgment" (подтверждение)**

**Расшифровка:**
```c
struct tcphdr *tcp = (struct tcphdr*)(packet + ip_header_len);

if (tcp->psh) {
    printf("Флаг PSH установлен → немедленная отправка\n");
    // Данные НЕ буферизуются, отправляются сразу
}

if (tcp->ack) {
    printf("Флаг ACK установлен → подтверждение получения\n");
    printf("ACK number: %u\n", ntohl(tcp->ack_seq));
}
```

**PSH (Push):**
- Приложение хочет отправить данные НЕМЕДЛЕННО
- Не ждать заполнения TCP буфера
- Типично для интерактивного трафика (SSH, telnet, chat)

**ACK (Acknowledgment):**
- Подтверждаю получение данных с seq = ack_seq - 1
- Присутствует почти во всех TCP пакетах (кроме первого SYN)

**В миссии (Пакет #1):**
```
[Packet #1]
From: 127.0.0.1:54321 → 127.0.0.1:9999
Flags: PSH ACK
Payload: "Target confirmed. Location: Safe house."
```

**Почему PSH ACK?**
- Агент Z. отправил сообщение
- PSH = "отправь немедленно, не буферизуй" (срочное сообщение!)
- ACK = "подтверждаю получение предыдущих данных"

**Другие флаги:**
- SYN = установка соединения
- FIN = завершение соединения
- RST = аварийный сброс
- URG = срочные данные

**Урок:** Флаги раскрывают характер коммуникации (срочность, состояние сессии).
</details>

---

### 🟡 Практика (5 вопросов)

**5. В коде вы используете `IPPROTO_TCP` для фильтрации. Как перехватить ТОЛЬКО UDP пакеты?**

<details>
<summary>Ответ</summary>

**Создать raw socket с `IPPROTO_UDP`:**

```c
// Для TCP (из миссии):
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

// Для UDP:
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

// Для ВСЕХ протоколов:
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
```

**UDP packet sniffer:**
```c
#include <netinet/udp.h>  // struct udphdr

// ...receive packet...

// Парсинг IP заголовка (аналогично TCP)
struct iphdr *ip = (struct iphdr*)buffer;
int ip_header_len = ip->ihl * 4;

// Парсинг UDP заголовка
struct udphdr *udp = (struct udphdr*)(buffer + ip_header_len);

printf("UDP Source port: %u\n", ntohs(udp->source));
printf("UDP Dest port: %u\n", ntohs(udp->dest));
printf("UDP Length: %u\n", ntohs(udp->len));
printf("UDP Checksum: 0x%04X\n", ntohs(udp->check));

// Payload
int udp_header_len = sizeof(struct udphdr);  // Всегда 8 байт
char *payload = (char*)(buffer + ip_header_len + udp_header_len);
int payload_len = ntohs(udp->len) - udp_header_len;
```

**Отличия UDP vs TCP:**
```
TCP заголовок: 20+ байт (переменный размер из-за опций)
UDP заголовок: 8 байт (фиксированный)

TCP: seq, ack_seq, flags, window...
UDP: только source, dest, len, checksum
```

**В миссии:**
- Агенты Z. используют TCP (порт 9999)
- Но могли бы использовать UDP (быстрее, меньше следов)
- Для перехвата UDP нужно изменить только `IPPROTO_TCP` → `IPPROTO_UDP`

**Урок:** Raw sockets работают со всеми протоколами (TCP, UDP, ICMP, ...).
</details>

**6. Как отфильтровать пакеты ТОЛЬКО с порта 9999, а не всех портов?**

<details>
<summary>Ответ</summary>

**Проверить source/dest port в TCP заголовке:**

```c
struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_header_len);

uint16_t src_port = ntohs(tcp->source);
uint16_t dst_port = ntohs(tcp->dest);

// Фильтр: только порт 9999
#define TARGET_PORT 9999

if (src_port == TARGET_PORT || dst_port == TARGET_PORT) {
    printf("[Packet on port 9999]\n");
    printf("From: %s:%u → ", src_ip_str, src_port);
    printf("%s:%u\n", dst_ip_str, dst_port);
    
    // Обработать payload
    // ...
} else {
    // Пропустить
    continue;
}
```

**В миссии:**
```c
// Перехват ВСЕХ TCP пакетов:
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

while (1) {
    // Получить пакет
    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    
    // Парсинг
    struct iphdr *ip = (struct iphdr*)buffer;
    struct tcphdr *tcp = (struct tcphdr*)(buffer + ip->ihl * 4);
    
    // ФИЛЬТР по порту
    if (ntohs(tcp->source) == 9999 || ntohs(tcp->dest) == 9999) {
        printf("MATCHED! Port 9999 packet\n");
        // Извлечь payload агентов Z.
    }
}
```

**Альтернатива: BPF (Berkeley Packet Filter):**
```c
// С libpcap можно задать фильтр:
pcap_compile(handle, &fp, "tcp port 9999", 0, PCAP_NETMASK_UNKNOWN);
pcap_setfilter(handle, &fp);
// Kernel будет фильтровать автоматически (эффективнее!)
```

**Урок:** Фильтрация в user space vs kernel space (BPF быстрее).
</details>

**7. В 19:46 первый пакет содержал "Target confirmed". Как определить, какой пакет пришёл первым, если они перемешаны?**

<details>
<summary>Ответ</summary>

**Использовать TCP sequence numbers!**

```c
struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_header_len);

uint32_t seq = ntohl(tcp->seq);
uint32_t ack_seq = ntohl(tcp->ack_seq);

printf("SEQ: %u\n", seq);
printf("ACK: %u\n", ack_seq);
```

**Сортировка пакетов:**
```c
typedef struct {
    uint32_t seq;
    char payload[1024];
    int payload_len;
} Packet;

Packet packets[100];
int packet_count = 0;

// При получении пакета:
packets[packet_count].seq = ntohl(tcp->seq);
memcpy(packets[packet_count].payload, payload, payload_len);
packets[packet_count].payload_len = payload_len;
packet_count++;

// Сортировка по SEQ:
qsort(packets, packet_count, sizeof(Packet), compare_seq);

int compare_seq(const void *a, const void *b) {
    Packet *pa = (Packet*)a;
    Packet *pb = (Packet*)b;
    
    if (pa->seq < pb->seq) return -1;
    if (pa->seq > pb->seq) return 1;
    return 0;
}

// Теперь packets[0] — первый пакет!
```

**В миссии:**
```
[Packet #1] SEQ=1000: "Target confirmed. Location: Safe house."
[Packet #2] SEQ=1048: "Coordinates: 55.7520°N 37.6175°E"
[Packet #3] SEQ=1112: "Order: Continue monitoring."
```

**Почему SEQ важен:**
- Пакеты могут прийти в любом порядке (routing, retransmit)
- SEQ определяет истинный порядок
- Следующий пакет: SEQ = предыдущий SEQ + payload_len

**Урок:** TCP sequence numbers = timestamp данных (для упорядочивания потока).
</details>

**8. Что такое "promiscuous mode" и зачем он нужен для packet sniffing?**

<details>
<summary>Ответ</summary>

**Promiscuous mode = "принимать ВСЕ пакеты, даже не адресованные тебе"**

**Нормальный режим:**
```
Сетевая карта фильтрует пакеты:
├─ Пакеты с вашим MAC → принять
├─ Broadcast пакеты → принять
└─ Остальные → ОТБРОСИТЬ
```

**Promiscuous mode:**
```
Сетевая карта принимает ВСЁ:
├─ Пакеты с вашим MAC → принять
├─ Пакеты с чужим MAC → принять!
├─ Broadcast → принять
└─ Всё остальное → принять!
```

**Включение в коде:**
```c
#include <sys/ioctl.h>
#include <net/if.h>

int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

// Получить имя интерфейса
struct ifreq ifr;
strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);

// Включить promiscuous mode
ioctl(sockfd, SIOCGIFFLAGS, &ifr);
ifr.ifr_flags |= IFF_PROMISC;
ioctl(sockfd, SIOCSIFFLAGS, &ifr);

printf("Promiscuous mode enabled on eth0\n");
```

**В миссии:**
- Без promiscuous: видите только свои пакеты
- С promiscuous: видите пакеты агентов Z. (порт 9999)

**На WiFi:**
- Ещё сложнее (Monitor mode)
- Нужно видеть пакеты других устройств
- `airmon-ng start wlan0`

**Легальность:**
⚠️ В своей сети: OK  
⚠️ В чужой сети: НЕЗАКОННО (перехват чужого трафика)

**Урок:** Promiscuous mode = основа всех сетевых анализаторов (Wireshark, tcpdump).
</details>

**9. Как вычислить длину payload в перехваченном пакете?**

<details>
<summary>Ответ</summary>

**`total_len - ip_header_len - tcp_header_len`:**

```c
// 1. Получить пакет
char buffer[65536];
ssize_t total_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

// 2. Парсинг IP заголовка
struct iphdr *ip = (struct iphdr*)buffer;
int ip_header_len = ip->ihl * 4;  // ihl в 32-bit словах
// ihl=5 → 5*4=20 байт (минимальный IP заголовок)

// 3. Парсинг TCP заголовка
struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_header_len);
int tcp_header_len = tcp->doff * 4;  // doff в 32-bit словах
// doff=5 → 5*4=20 байт (минимальный TCP заголовок)

// 4. Вычислить payload
char *payload = (char*)(buffer + ip_header_len + tcp_header_len);
int payload_len = total_len - ip_header_len - tcp_header_len;

if (payload_len > 0) {
    printf("Payload (%d bytes):\n", payload_len);
    for (int i = 0; i < payload_len; i++) {
        printf("%c", payload[i]);
    }
    printf("\n");
}
```

**Пример (Пакет #1 из миссии):**
```
total_len = 84 байт (от recvfrom)
ip_header_len = 20 байт (стандартный)
tcp_header_len = 20 байт (без опций)

payload_len = 84 - 20 - 20 = 44 байт
payload = "Target confirmed. Location: Safe house.\n"
```

**Важно:**
- `ihl` и `doff` — в 32-bit словах, НЕ байтах!
- Минимум: IP=20 байт, TCP=20 байт
- Максимум: IP=60 байт (с опциями), TCP=60 байт
- Payload может быть 0 (ACK без данных)

**Проверка:**
```c
if (payload_len < 0) {
    fprintf(stderr, "Invalid packet (negative payload)\n");
    continue;
}
```

**Урок:** Payload = "то, ради чего всё это" (реальные данные приложения).
</details>

---

### 🔴 Анализ кода (3 вопроса)

**10. Найдите ошибку в этом коде извлечения payload:**

```c
struct iphdr *ip = (struct iphdr*)buffer;
struct tcphdr *tcp = (struct tcphdr*)(buffer + 20);  // ← ОШИБКА?
char *payload = (char*)(buffer + 40);
int payload_len = total_len - 40;
```

<details>
<summary>Ответ</summary>

**Ошибка: hardcoded размеры заголовков!**

**Проблемы:**

1. **IP заголовок не всегда 20 байт:**
   ```c
   struct tcphdr *tcp = (struct tcphdr*)(buffer + 20);  // ❌
   ```
   - Может быть 20-60 байт (IP опции!)
   - Нужно: `ip->ihl * 4`

2. **TCP заголовок не всегда 20 байт:**
   ```c
   char *payload = (char*)(buffer + 40);  // ❌ (20+20)
   ```
   - Может быть 20-60 байт (TCP опции!)
   - Нужно: `tcp->doff * 4`

3. **Payload offset неправильный:**
   ```c
   int payload_len = total_len - 40;  // ❌
   ```

**Правильный код:**
```c
struct iphdr *ip = (struct iphdr*)buffer;
int ip_header_len = ip->ihl * 4;  // ✓ Динамический!

struct tcphdr *tcp = (struct tcphdr*)(buffer + ip_header_len);
int tcp_header_len = tcp->doff * 4;  // ✓ Динамический!

char *payload = (char*)(buffer + ip_header_len + tcp_header_len);
int payload_len = total_len - ip_header_len - tcp_header_len;
```

**Что произойдёт при ошибке:**
- Пакет с IP опциями (ihl=6, 24 байта)
- `buffer + 20` → неправильный offset
- Читаем мусор вместо TCP заголовка
- Payload содержит часть TCP заголовка
- Невозможно декодировать данные агентов Z.!

**Урок:** Никогда не hardcode размеры протокольных заголовков!
</details>

**11. Почему этот код может читать мусор?**

```c
char buffer[1024];
int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
printf("Payload: %s\n", (char*)(buffer + 40));  // ← ПРОБЛЕМА?
```

<details>
<summary>Ответ</summary>

**Проблема: `%s` требует null-terminated строку, а payload может не иметь `\0`!**

**Что происходит:**
```c
printf("Payload: %s\n", (char*)(buffer + 40));
```

- `%s` читает до `\0`
- Payload агентов Z. может НЕ иметь `\0` в конце
- `printf` продолжит читать за пределы payload
- → Чтение мусора, возможно segfault!

**Пример (безопасный вывод):**
```c
char *payload = (char*)(buffer + ip_header_len + tcp_header_len);
int payload_len = total_len - ip_header_len - tcp_header_len;

// Вариант 1: посимвольно
for (int i = 0; i < payload_len; i++) {
    printf("%c", payload[i]);
}
printf("\n");

// Вариант 2: создать null-terminated копию
char safe_payload[1024];
memcpy(safe_payload, payload, payload_len);
safe_payload[payload_len] = '\0';  // Добавить \0
printf("Payload: %s\n", safe_payload);

// Вариант 3: precision в printf
printf("Payload: %.*s\n", payload_len, payload);
//                ^^^ печатать только payload_len символов
```

**В миссии:**
```
Payload (без \0): "Target confirmed. Location: Safe house."
Следующие байты в памяти: [random garbage]

printf("%s") → "Target confirmed...Safe house.[мусор][мусор]..."
```

**Урок:** Сетевые данные — это байты, НЕ C-строки!
</details>

**12. В чём проблема этого фильтра по IP?**

```c
struct iphdr *ip = (struct iphdr*)buffer;

if (ip->saddr == inet_addr("127.0.0.1")) {  // ← ОШИБКА?
    printf("From localhost\n");
}
```

<details>
<summary>Ответ</summary>

**Ошибка: сравнение network byte order с host byte order!**

**Проблема:**
```c
if (ip->saddr == inet_addr("127.0.0.1")) {  // ❌
```

- `ip->saddr` — в **network byte order** (big-endian)
- `inet_addr()` — возвращает **host byte order** на некоторых системах (НЕ portable!)
- Сравнение может провалиться!

**Правильный вариант:**
```c
// Вариант 1: Конвертировать в строку и сравнивать
char src_ip_str[INET_ADDRSTRLEN];
struct in_addr src_addr;
src_addr.s_addr = ip->saddr;  // Уже в network byte order
inet_ntop(AF_INET, &src_addr, src_ip_str, INET_ADDRSTRLEN);

if (strcmp(src_ip_str, "127.0.0.1") == 0) {
    printf("From localhost\n");
}

// Вариант 2: Использовать inet_pton
struct in_addr expected;
inet_pton(AF_INET, "127.0.0.1", &expected);  // → network byte order

if (ip->saddr == expected.s_addr) {  // ✓ Оба в network byte order
    printf("From localhost\n");
}

// Вариант 3: Прямое сравнение с учётом byte order
if (ntohl(ip->saddr) == 0x7F000001) {  // 127.0.0.1
    printf("From localhost\n");
}
```

**В миссии (корректный код):**
```c
struct in_addr src_addr;
src_addr.s_addr = ip->saddr;
char src_ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &src_addr, src_ip, INET_ADDRSTRLEN);

// Теперь можно сравнивать строки
printf("From: %s\n", src_ip);
```

**Урок:** Всегда используйте `inet_ntop`/`inet_pton` для конвертации IP!
</details>

---

### 🎯 Сюжет (2 вопроса)

**13. В 20:15 V. написал: "Они следят за тобой. Но теперь ТЫ следишь за ними." Как это возможно — взаимный перехват?**

<details>
<summary>Ответ</summary>

**Оба используют packet sniffing в одной сети = взаимная прозрачность!**

**Сценарий:**
```
         SAFE HOUSE NETWORK
         
    ВЫ                      АГЕНТЫ Z.
    📱                         👤
     │                         │
     ├──> packet_sniffer       ├──> packet_sniffer
     │    (перехват порта 9999)│    (перехват ВСЕХ портов)
     │                         │
     └────────┬────────────────┘
              │
          [WiFi Router]
```

**Что происходит:**

1. **Агенты Z. перехватывают ВАШ трафик:**
   - Видят ваши подключения к серверу V. (Episode 10)
   - Узнают ваши координаты через GPS в IP
   - Отслеживают ваши действия

2. **ВЫ перехватываете ИХ трафик:**
   - Видите их коммуникацию на порту 9999
   - Узнаёте, что они знают о вас
   - Получаете инте о следующем узле

**Технически:**
```c
// Ваш sniffer:
if (ntohs(tcp->source) == 9999 || ntohs(tcp->dest) == 9999) {
    // Пакеты агентов Z.!
}

// Их sniffer:
if (ntohs(tcp->source) == 31337 || ntohs(tcp->dest) == 31337) {
    // Пакеты к серверу V.!
}
```

**Почему это работает:**
- Shared medium (WiFi, Ethernet hub)
- Promiscuous mode видит всё
- Шифрования нет → всё в plaintext

**Защита:**
- VPN (зашифрует весь трафик)
- TLS/SSL (Episode 12)
- Физическая изоляция (другая сеть)

**Урок:** В открытой сети все видят всех. Шифрование обязательно.
</details>

**14. Вы перехватили "Next node: 10.0.0.1:4433" в пакете агентов Z. Почему это важно для Episode 12?**

<details>
<summary>Ответ</summary>

**Это ПОДТВЕРЖДЕНИЕ адреса следующего узла от V.!**

**Что произошло:**

**Episode 10:**
```
10:51 → Подключились к серверу V. (203.0.113.42:31337)
        Получили: "Следующий узел: 10.0.0.1:4433"
```

**Episode 11:**
```
20:30 → Перехватили пакет агентов Z.:
        "Next node: 10.0.0.1:4433"
        "Protocol: Encrypted TCP"
        "Objective: Intercept encrypted stream"
```

**Анализ:**
1. **V. дал адрес:** 10.0.0.1:4433
2. **Агенты Z. ТОЖЕ знают адрес:** 10.0.0.1:4433
3. **Значит:** Они тоже пытаются подключиться!

**Почему важно:**
- Подтверждение: адрес правильный
- Опасность: агенты Z. на том же пути
- Задача Episode 12: перехватить зашифрованный поток БЫСТРЕЕ них

**В Episode 12:**
```
02:12 → V.: "Перехватить TCP-поток на 10.0.0.1:4433"
        "Расшифровать XOR"
        "Узнать координаты встречи"
        
→ Вы должны успеть ДО агентов Z.!
```

**Сюжетная линия:**
```
Episode 09: Нашли IP 203.0.113.42 (GPS → Красная площадь)
Episode 10: Подключились к 203.0.113.42:31337 (получили узел 2)
Episode 11: Подтвердили узел 2 через перехват агентов Z.
Episode 12: Расшифруете узел 2 → встреча с V. на Красной площади
```

**Урок:** Packet sniffing даёт разведданные о планах противника.
</details>

---

### 🚀 Продвинутые (1 вопрос)

**15. В сюжете агенты Z. знают координаты safe house (55.7520°N 37.6175°E). Могли ли они вычислить это из перехваченных пакетов?**

<details>
<summary>Ответ</summary>

**Да! Через корреляцию трафика, timing анализ и geolocation!**

**Способы:**

**1. GPS в пакетах (если вы их отправляли):**
```c
// Ваш код (гипотетически):
char msg[100];
snprintf(msg, sizeof(msg), "Status: Active at %f,%f", lat, lon);
send(sockfd, msg, strlen(msg), 0);

// Агенты Z. перехватывают → видят координаты!
```

**2. Timing analysis (задержки пакетов):**
```
Вы → Сервер V. (203.0.113.42)
Задержка RTT: 45 ms

Агенты Z. анализируют:
- RTT 45ms → примерно 1000 км от сервера
- Сервер в Москве → вы тоже в Москве
- Сужение круга поиска
```

**3. WiFi triangulation:**
```bash
# Агенты Z. сканируют WiFi:
$ iwlist wlan0 scan

ESSID: "SafeHouse_Guest"
MAC: AA:BB:CC:DD:EE:FF
Signal: -45 dBm  ← Очень близко!

# Trilateration с нескольких точек:
→ Вычисляют местоположение роутера
→ Координаты safe house
```

**4. IP Geolocation:**
```bash
$ curl "http://ip-api.com/json/YOUR_PUBLIC_IP"

{
  "lat": 55.7520,
  "lon": 37.6175,
  "city": "Moscow",
  "district": "Arbat"
}
```

**5. Traffic correlation:**
```
19:46 → Ваш пакет к V.: отправлен
19:46 → Пакет Z.: "Target confirmed. Location: Safe house."

Корреляция времени → вы в safe house!
```

**6. MAC адрес вашего устройства:**
```c
// В Ethernet frame:
struct ether_header {
    uint8_t ether_dhost[6];  // MAC назначения
    uint8_t ether_shost[6];  // ВАШ MAC!
    uint16_t ether_type;
};

// Агенты Z. видят ваш MAC
// → База данных производителей
// → Модель устройства
// → Возможно, история перемещений (если WiFi)
```

**Защита:**

```c
// 1. VPN (скрывает реальный IP)
sudo openvpn moonlight.ovpn

// 2. Tor (анонимность)
torify ./moonlight_client

// 3. MAC randomization
sudo ifconfig wlan0 hw ether DE:AD:BE:EF:CA:FE

// 4. Шифрование (TLS/SSL)
// Координаты не видны в plaintext

// 5. Физическая смена локации
// Переехать из safe house!
```

**В миссии:**
- Вы НЕ использовали VPN
- Агенты Z. в той же сети
- Perехватили трафик → вычислили координаты
- **Результат:** Safe house скомпрометирован

**Урок:** Метаданные (timing, geolocation, MAC) могут раскрыть местоположение даже без GPS в payload.
</details>

---

**🎓 Оценка:**
- **13-15 правильных** — Packet Master! Готовы к Episode 12 ✅
- **10-12 правильных** — Хорошо, но повторите promiscuous mode 📖
- **< 10 правильных** — Перечитайте Episode 11 и попробуйте packet sniffer 🔄

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