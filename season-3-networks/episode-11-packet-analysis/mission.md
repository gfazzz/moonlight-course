# Миссия: "Packet Sniffer"

## 🎯 Цель

Создать packet sniffer для перехвата и анализа TCP/UDP трафика.

## Требования

1. Захват пакетов с raw socket
2. Парсинг IP/TCP/UDP заголовков
3. Фильтрация по портам
4. Hex dump содержимого
5. Вывод статистики

**⚠️ Требуется sudo/root!**

```bash
sudo ./packet_sniffer
```

## Функционал

- Перехват всех TCP/UDP пакетов
- Отображение IP-адресов, портов
- Hex dump первых 64 байт данных
- Счётчики пакетов

## Starter Code

```c
#include <netinet/ip.h>
#include <netinet/tcp.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket (need root!)");
        return 1;
    }
    
    // TODO: Capture and parse packets
    return 0;
}
```
