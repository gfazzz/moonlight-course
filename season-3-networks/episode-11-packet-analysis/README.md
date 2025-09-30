# Episode 11: "Packet Analysis" 📡

> *"Every packet tells a story. Learn to listen."*

## 📚 Теория: Raw Sockets и Packet Sniffing

### Raw Sockets

```c
// Создание raw socket (требует root!)
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

// Чтение сырых пакетов
unsigned char buffer[65536];
ssize_t size = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

// Парсинг IP заголовка
struct iphdr *ip = (struct iphdr*)buffer;
printf("From: %s\n", inet_ntoa(*(struct in_addr*)&ip->saddr));
```

### Структуры заголовков

```c
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

struct iphdr *ip = (struct iphdr*)packet;
struct tcphdr *tcp = (struct tcphdr*)(packet + ip->ihl * 4);
```

## 🎯 Миссия: "Packet Sniffer"

Создать анализатор сетевых пакетов для перехвата трафика MOONLIGHT.

**Артефакт**: `packet_sniffer` — перехватчик TCP/UDP пакетов

См. [mission.md](mission.md) для деталей.

---

[← Episode 10](../episode-10-socket-programming/README.md) | [→ Episode 12](../episode-12-encrypted-communications/README.md)
