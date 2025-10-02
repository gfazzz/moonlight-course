# Season 3: Season Project — network_interceptor

> *"Собери свой инструмент сетевого анализа из компонентов миссий"*

```
╔══════════════════════════════════════════════════════════╗
║            SEASON PROJECT - NETWORK INTERCEPTOR          ║
║                  OPERATION MOONLIGHT                     ║
╠══════════════════════════════════════════════════════════╣
║                                                          ║
║  СТАТУС:    🎯 FINAL MISSION                             ║
║  СЛОЖНОСТЬ: 🔴🔴🔴🔴 Very Hard                            ║
║  ВРЕМЯ:     ~3-5 часов                                   ║
║  ЦЕЛЬ:      Создать универсальный network interceptor    ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝
```

---

## 🎬 BRIEFING ОТ VIKTOR

**Дата:** Пятница, 3 октября  
**Время:** 08:05  
**Место:** Метро "Охотный ряд", после эвакуации

После успешной встречи на Красной площади и получения USB "S4: CRYPTO & ALGORITHMS", вы получаете финальное сообщение от Viktor через Signal:

```
╔═══════════════════════════════════════════════════════════╗
║  ОТ: Viktor Petrov                                        ║
║  ТЕМА: Финальное задание Season 3                        ║
║  ПРИОРИТЕТ: ⭐⭐⭐⭐⭐ КРИТИЧЕСКИЙ                          ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  "Morpheus. Ты в безопасности. Крылов отвлечён Ghost'ом. ║
║                                                           ║
║   Ты прошёл Season 3. Четыре миссии:                     ║
║   - Episode 09: IP-адресация и GPS (203.0.113.42)        ║
║   - Episode 10: TCP сокеты под DDoS-атакой               ║
║   - Episode 11: Packet sniffing (safe house раскрыт!)    ║
║   - Episode 12: Session reconstruction + встреча со мной  ║
║                                                           ║
║   Ты освоил сети. Теперь — финальный экзамен:            ║
║   Собери ВСЕ свои инструменты в ЕДИНУЮ СИСТЕМУ.          ║
║                                                           ║
║   network_interceptor — это НЕ просто учебный проект.    ║
║   Это КРИТИЧЕСКИ ВАЖНЫЙ инструмент для операций:         ║
║   - Season 5: Financial (анализ HFT трафика)             ║
║   - Season 6: Embedded/IoT (перехват команд дронов)      ║
║   - Season 7: System (мониторинг stealth операций)       ║
║   - Season 10: ФИНАЛ (satellite tracking + intercept)    ║
║                                                           ║
║   Твой код должен быть:                                  ║
║   ✅ Модульным (4 компонента = 4 пары .h/.c)             ║
║   ✅ Переиспользуемым (#include в будущих сезонах)       ║
║   ✅ Чистым (valgrind/leaks без ошибок)                  ║
║   ✅ Профессиональным (production-ready code)            ║
║                                                           ║
║   Время на выполнение: 3-5 часов.                        ║
║                                                           ║
║   Если справишься — получишь доступ к Season 4.          ║
║   USB "CRYPTO & ALGORITHMS" разблокируется.              ║
║                                                           ║
║   Удачи, агент. Команда верит в тебя.                    ║
║                                                           ║
║   Ghost (Алекс), Дмитрий, Анна — все помогали.           ║
║   Теперь твоя очередь собрать финальный инструмент.      ║
║                                                           ║
║                                              — Viktor"    ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🎯 ЗАДАНИЕ

### Цель
Создать **network_interceptor** — универсальный инструмент сетевого анализа, который:

1. ✅ **Конвертирует IP адреса** — обёртка над inet_pton/inet_ntop
2. ✅ **Управляет TCP сокетами** — обёртка над socket/connect/send/recv
3. ✅ **Перехватывает пакеты** — raw sockets + parsing (IP/TCP headers)
4. ✅ **Восстанавливает TCP сессии** — сортировка по SEQ + stream reassembly
5. ✅ **Профессионален** — модульный код, error handling, документация

### Компоненты
Ты извлекаешь код из своих решений Episodes 09-12 и создаёшь 4 модуля:

```
Episode 09: Network Basics
   ↓
ip_tools.c / ip_tools.h
   - inet_pton_safe() — конвертация IP (с проверкой ошибок)
   - inet_ntop_safe() — обратная конвертация
   - ip_to_gps() — декодирование координат (как в Episode 09!)
   - validate_ip() — проверка валидности IP

Episode 10: Socket Programming
   ↓
tcp_client.c / tcp_client.h
   - tcp_connect() — обёртка socket() + connect()
   - tcp_send_safe() — обёртка send() с retry
   - tcp_recv_safe() — обёртка recv() с timeout
   - tcp_close() — graceful shutdown
   - challenge_response() — реализация auth протокола

Episode 11: Packet Analysis
   ↓
packet_sniffer.c / packet_sniffer.h
   - create_raw_socket() — SOCK_RAW + promiscuous mode
   - capture_packet() — recvfrom() + parsing
   - parse_ip_header() — извлечение IP данных
   - parse_tcp_header() — извлечение TCP данных
   - extract_payload() — извлечение данных после headers
   - hex_dump() — вывод в HEX + ASCII

Episode 12: Encrypted Communications
   ↓
session_reconstructor.c / session_reconstructor.h
   - packet_sorter() — сортировка по SEQ
   - detect_gaps() — обнаружение пропусков
   - reassemble_stream() — сборка потока
   - xor_decrypt() — расшифровка XOR (из Season 1!)
   - extract_key() — извлечение ключа из первого пакета
```

### Итоговый проект: network_interceptor

```c
// Финальная программа (собирает все модули):

#include "ip_tools.h"
#include "tcp_client.h"
#include "packet_sniffer.h"
#include "session_reconstructor.h"

int main(int argc, char *argv[]) {
    // 1. Конвертация IP (Episode 09)
    uint32_t ip = ip_string_to_int("203.0.113.42");
    
    // 2. Декодирование GPS (Episode 09)
    double lat, lon;
    ip_to_gps(ip, &lat, &lon);
    
    // 3. TCP подключение (Episode 10)
    int sockfd = tcp_connect("203.0.113.42", 31337);
    
    // 4. Challenge-Response auth (Episode 10)
    challenge_response(sockfd, "MOONLIGHT2024");
    
    // 5. Packet sniffing (Episode 11)
    int raw_fd = create_raw_socket(9999);  // Port агентов Z.
    
    Packet packets[100];
    int count = 0;
    
    while (count < 8) {
        capture_packet(raw_fd, &packets[count]);
        count++;
    }
    
    // 6. Session reconstruction (Episode 12)
    packet_sorter(packets, count);
    detect_gaps(packets, count);
    
    char stream[10000];
    int stream_len = reassemble_stream(packets, count, stream);
    
    // 7. XOR decryption (Episode 01 + 12!)
    uint8_t key = extract_key(stream);
    xor_decrypt(stream, stream_len, key);
    
    printf("DECRYPTED: %s\n", stream);
    
    return 0;
}
```

---

## 📋 СТРУКТУРА ПРОЕКТА

```
season-3-networks/
├── season_project_mission.md    ← Это задание
├── season_project_starter.c     ← Шаблон с TODO
├── Makefile                      ← Правила сборки (обновлён!)
│
└── season-project/               ← Студент создаёт эту папку!
    ├── ip_tools.h
    ├── ip_tools.c
    ├── tcp_client.h
    ├── tcp_client.c
    ├── packet_sniffer.h
    ├── packet_sniffer.c
    ├── session_reconstructor.h
    ├── session_reconstructor.c
    └── network_interceptor.c     ← Главная программа
```

**Принцип Оккама:** НЕ создаём избыточную структуру! `season-project/` содержит только необходимые файлы.

---

## 🔧 ПОШАГОВАЯ ИНСТРУКЦИЯ

### ШАГ 1: Создать структуру

```bash
cd season-3-networks/
mkdir season-project
cd season-project
```

### ШАГ 2: Извлечь компоненты из Episodes

**Из Episode 09 (ip_converter.c):**
```bash
# Скопировать функции конвертации IP
# Создать ip_tools.h + ip_tools.c
```

**Из Episode 10 (moonlight_client):**
```bash
# Скопировать TCP socket код
# Создать tcp_client.h + tcp_client.c
```

**Из Episode 11 (packet_sniffer):**
```bash
# Скопировать raw socket + parsing
# Создать packet_sniffer.h + packet_sniffer.c
```

**Из Episode 12 (comm_interceptor):**
```bash
# Скопировать session reconstruction
# Создать session_reconstructor.h + session_reconstructor.c
```

### ШАГ 3: Создать .h файлы (headers)

**ip_tools.h:**
```c
#ifndef IP_TOOLS_H
#define IP_TOOLS_H

#include <stdint.h>
#include <arpa/inet.h>

// Конвертация IP string → uint32_t
uint32_t ip_string_to_int(const char *ip_str);

// Конвертация uint32_t → GPS координаты
void ip_to_gps(uint32_t ip, double *lat, double *lon);

// Safe обёртки
int inet_pton_safe(const char *ip_str, struct in_addr *addr);
int inet_ntop_safe(struct in_addr *addr, char *buf, size_t len);

#endif
```

**tcp_client.h:**
```c
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdint.h>

// TCP соединение
int tcp_connect(const char *host, uint16_t port);

// Отправка/получение
int tcp_send_safe(int sockfd, const void *buf, size_t len);
int tcp_recv_safe(int sockfd, void *buf, size_t len, int timeout_sec);

// Challenge-Response
int challenge_response(int sockfd, const char *key);

// Graceful close
void tcp_close(int sockfd);

#endif
```

**packet_sniffer.h:**
```c
#ifndef PACKET_SNIFFER_H
#define PACKET_SNIFFER_H

#include <stdint.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint16_t payload_len;
    unsigned char payload[1500];
} Packet;

// Raw socket
int create_raw_socket(uint16_t filter_port);

// Capture
int capture_packet(int raw_fd, Packet *pkt);

// Parsing
int parse_ip_header(unsigned char *buffer, struct iphdr **ip);
int parse_tcp_header(unsigned char *buffer, int ip_len, struct tcphdr **tcp);
int extract_payload(unsigned char *buffer, int ip_len, int tcp_len, 
                    unsigned char **payload, int *payload_len);

// Hex dump
void hex_dump(unsigned char *data, int len);

#endif
```

**session_reconstructor.h:**
```c
#ifndef SESSION_RECONSTRUCTOR_H
#define SESSION_RECONSTRUCTOR_H

#include "packet_sniffer.h"

// Сортировка
void packet_sorter(Packet *packets, int count);

// Gap detection
int detect_gaps(Packet *packets, int count);

// Stream reassembly
int reassemble_stream(Packet *packets, int count, char *stream);

// XOR decryption (из Season 1!)
void xor_decrypt(unsigned char *data, int len, uint8_t key);

// Извлечение ключа
uint8_t extract_key(const char *stream);

#endif
```

### ШАГ 4: Реализовать .c файлы

Студент копирует код из своих решений Episodes 09-12 и адаптирует под API модулей.

### ШАГ 5: Создать главную программу

**network_interceptor.c:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip_tools.h"
#include "tcp_client.h"
#include "packet_sniffer.h"
#include "session_reconstructor.h"

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║    NETWORK INTERCEPTOR v1.0               ║\n");
    printf("║    Operation MOONLIGHT - Season 3         ║\n");
    printf("╚═══════════════════════════════════════════╝\n\n");
    
    // TODO: Студент реализует интеграцию всех модулей
    // Пример:
    // 1. Декодировать IP → GPS
    // 2. Подключиться через TCP
    // 3. Перехватить пакеты
    // 4. Восстановить сессию
    // 5. Расшифровать XOR
    
    return 0;
}
```

### ШАГ 6: Обновить Makefile

```makefile
# В корне season-3-networks/Makefile добавить:

# Season Project
PROJECT_SRC = season-project/ip_tools.c \
              season-project/tcp_client.c \
              season-project/packet_sniffer.c \
              season-project/session_reconstructor.c \
              season-project/network_interceptor.c

season_project: $(PROJECT_SRC)
	$(CC) $(CFLAGS) -o network_interceptor $(PROJECT_SRC)
	@echo "✅ Season Project: network_interceptor built!"

# Добавить в all:
all: episodes season_project
```

### ШАГ 7: Компиляция и тестирование

```bash
cd season-3-networks/
make season_project

# Запуск:
sudo ./network_interceptor  # Нужен root для raw sockets!

# Проверка утечек:
sudo valgrind --leak-check=full ./network_interceptor
```

---

## ✅ КРИТЕРИИ УСПЕХА

### Обязательные требования:

1. **Компиляция без ошибок:**
```bash
gcc -Wall -Wextra -Werror *.c -o network_interceptor
# Без warnings!
```

2. **Valgrind без утечек:**
```bash
valgrind --leak-check=full ./network_interceptor
# "All heap blocks were freed -- no leaks are possible"
```

3. **Модульность:**
   - 4 пары .h/.c файлов
   - Каждый header с include guards
   - API чистый и понятный

4. **Функциональность:**
   - ✅ IP конвертация работает
   - ✅ TCP подключение успешно
   - ✅ Packet capture работает (с sudo)
   - ✅ Session reconstruction собирает поток
   - ✅ XOR decryption расшифровывает

5. **Документация:**
   - Комментарии в .h файлах
   - README.md в season-project/

### Бонусы (опционально):

- 🌟 **GPS декодирование** работает (203.0.113.42 → Красная площадь)
- 🌟 **Challenge-Response** реализован (как в Episode 10)
- 🌟 **Gap detection** обнаруживает пропуски
- 🌟 **Promiscuous mode** активируется (ioctl)
- 🌟 **Error handling** на каждом уровне (errno, perror)
- 🌟 **CLI аргументы** (--help, --host, --port, --filter)

---

## 📊 ОЦЕНКА ВРЕМЕНИ

```
Извлечение кода:        1-1.5 часа
Создание .h/.c:         1-1.5 часа
Интеграция:             0.5-1 час
Тестирование:           0.5-1 час
Debugging + доработка:  0.5-1 час
───────────────────────────────────
ИТОГО:                  3-5 часов
```

---

## 🎓 ЧТО ВЫ ИЗУЧИТЕ

### Технические навыки:
- ✅ **Модульное программирование** — разделение на компоненты
- ✅ **API design** — проектирование интерфейсов
- ✅ **Переиспользование кода** — из Episodes → в Season Project
- ✅ **Системная интеграция** — сборка из модулей
- ✅ **Professional workflow** — как в реальных проектах

### Сетевые навыки:
- ✅ **TCP/IP stack** — от IP до Application layer
- ✅ **Raw sockets** — низкоуровневый доступ к пакетам
- ✅ **Session reconstruction** — восстановление TCP потоков
- ✅ **Network forensics** — анализ перехваченного трафика

### Операционные навыки:
- ✅ **Работа с командой** — Ghost, Дмитрий, Анна помогали
- ✅ **Сборка инструментов** — как в реальных операциях
- ✅ **OpSec** — безопасность сетевых операций

---

## 💡 ПОДСКАЗКИ

### Проблема: "Не компилируется ip_tools.c"
```bash
# Убедись что include guards правильные:
#ifndef IP_TOOLS_H
#define IP_TOOLS_H
...
#endif

# Проверь что все функции объявлены в .h и реализованы в .c
```

### Проблема: "Raw socket: Permission denied"
```bash
# Нужен root!
sudo ./network_interceptor

# Или дать права программе:
sudo setcap cap_net_raw+ep ./network_interceptor
./network_interceptor  # Теперь без sudo
```

### Проблема: "Valgrind показывает утечки"
```bash
# Проверь что все malloc/calloc имеют free:
valgrind --leak-check=full --show-leak-kinds=all ./network_interceptor

# Частые утечки:
# - Забыли close(sockfd)
# - Забыли free() после malloc
# - Не освободили буферы
```

### Проблема: "Не могу извлечь код из Episodes"
```bash
# Смотри на solution/ файлы:
cat episode-09-network-basics/solution/ip_converter_solution.c
cat episode-10-socket-programming/solution/moonlight_client_solution.c
# И т.д.

# Копируй нужные функции, адаптируй под API
```

---

## 🚀 ПОСЛЕ ЗАВЕРШЕНИЯ

После успешной сборки `network_interceptor` вы получите:

1. **Доступ к Season 4** — USB "CRYPTO & ALGORITHMS" разблокирован
2. **Инструмент для будущих сезонов** — будете использовать в S5-S10
3. **Профессиональный опыт** — модульная архитектура как в production
4. **Понимание сетей** — от байтового уровня до application layer

**Сообщение от Viktor:**
```
"network_interceptor готов. 

Season 4 ждёт тебя. Криптография, хэши, blockchain, алгоритмы.

Но помни: инструменты Season 3 понадобятся в Season 5 (Financial),
Season 6 (Embedded/IoT), Season 7 (System Programming).

Храни код. Документируй. Улучшай.

Удачи в Season 4, агент.

— Viktor Petrov"
```

---

## 📞 ПОДДЕРЖКА КОМАНДЫ

```
Ghost (Алекс Соколов):
"Если застрянешь с raw sockets — пиши. 
 Я 12 лет работал с перехватом трафика в ФСБ."

Дмитрий Орлов:
"TCP — моя специализация (авионика Аэрофлот).
 Если connect() не работает — обращайся."

Анна Ковалёва:
"Forensics — моё. Если нужно восстановить сессию
 из фрагментов — помогу с алгоритмом."

Viktor:
"Я верю в тебя. Команда верит. Ты справишься."
```

---

**ЭТО ТВОЙ МОМЕНТ. ПОКАЖИ ЧТО ТЫ ГОТОВ К SEASON 4!** 🚀

