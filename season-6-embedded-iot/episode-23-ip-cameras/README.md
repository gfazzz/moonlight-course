# Episode 23: "IP Cameras & RTSP" 📹
## Season 6: Embedded & IoT | Episode 23/42

> *"Камеры не лгут. Но протоколы можно обмануть."*

---

## 📋 Briefing

**От:** The Architect  
**Кому:** Agent [ВАШ_ПОЗЫВНОЙ]  
**Тема:** Доступ к системе видеонаблюдения  
**Приоритет:** 🔴 КРИТИЧЕСКИЙ

**Ситуация:**

Виктор передал координаты дата-центра. Система охраняется круглосуточным видеонаблюдением. Нам нужен доступ к камерам, чтобы:
1. Изучить маршруты охраны
2. Найти слепые зоны
3. Подменить видеопоток при необходимости

Современные IP-камеры используют протокол **RTSP** (Real Time Streaming Protocol) для передачи видео. Ваша задача — разобраться с этим протоколом и получить доступ к видеопотокам.

**Ваша миссия:**
1. Реализовать RTSP-клиент для подключения к IP-камерам
2. Парсить RTSP ответы (OPTIONS, DESCRIBE, SETUP, PLAY)
3. Извлечь информацию о видеопотоках (SDP)
4. Создать сканер IP-камер в сети

---

## 🎯 Цели эпизода

После прохождения вы научитесь:
- ✅ Работать с протоколом RTSP
- ✅ Парсить SDP (Session Description Protocol)
- ✅ Использовать TCP сокеты для RTSP
- ✅ Сканировать сеть на наличие IP-камер
- ✅ Извлекать метаданные из видеопотоков

---

## 📚 Теория

### 1. Протокол RTSP (Real Time Streaming Protocol)

**RTSP** — это протокол управления медиа-потоками, похожий на HTTP.

#### Архитектура RTSP

```
[Client] ---RTSP---> [Camera/Server]
   ↓                      ↓
OPTIONS               200 OK
DESCRIBE          SDP description
SETUP            Session info
PLAY             Start streaming
   ↓                      ↓
[Receive RTP video stream]
```

#### Основные RTSP методы:

| Метод | Описание |
|-------|----------|
| OPTIONS | Узнать поддерживаемые методы |
| DESCRIBE | Получить SDP описание потока |
| SETUP | Настроить сессию |
| PLAY | Начать воспроизведение |
| PAUSE | Приостановить |
| TEARDOWN | Завершить сессию |

#### Пример RTSP запроса (OPTIONS):

```
OPTIONS rtsp://192.168.1.100:554/stream1 RTSP/1.0
CSeq: 1
User-Agent: MOONLIGHT/1.0

```

#### Ответ:

```
RTSP/1.0 200 OK
CSeq: 1
Server: Camera/2.0
Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN

```

**Метафора:** RTSP — это "пульт управления" для видеопотока, как HTTP для веб-страниц.

---

### 2. SDP (Session Description Protocol)

**SDP** описывает параметры медиа-сессии (кодеки, разрешение, битрейт).

#### Пример SDP:

```
v=0
o=- 2251938463 2251938463 IN IP4 192.168.1.100
s=RTSP Session
i=H.264 Video Stream
c=IN IP4 0.0.0.0
t=0 0
a=tool:LIVE555 Streaming Media v2018
a=type:broadcast
a=control:*
a=range:npt=0-
m=video 0 RTP/AVP 96
a=rtpmap:96 H264/90000
a=fmtp:96 packetization-mode=1;profile-level-id=42001f
a=control:track1
```

#### Основные поля SDP:

```c
v=    // Версия (всегда 0)
o=    // Origin (создатель сессии)
s=    // Session name
c=    // Connection info (IP адрес)
m=    // Media description (video/audio)
a=    // Attributes (кодек, параметры)
```

#### Код для парсинга SDP:

```c
typedef struct {
    char media_type[16];     // "video" или "audio"
    int port;                // RTP порт
    char codec[32];          // "H264", "MPEG4", etc
    int payload_type;        // RTP payload type
    char control_uri[256];   // URI для SETUP
} SDPMedia;

// Парсинг m= строки (media description)
int parse_sdp_media(const char *line, SDPMedia *media) {
    // m=video 0 RTP/AVP 96
    sscanf(line, "m=%s %d RTP/AVP %d", 
           media->media_type, &media->port, &media->payload_type);
    return 0;
}

// Парсинг a=rtpmap
int parse_sdp_rtpmap(const char *line, SDPMedia *media) {
    // a=rtpmap:96 H264/90000
    int pt;
    char codec[32];
    if (sscanf(line, "a=rtpmap:%d %[^/]", &pt, codec) == 2) {
        if (pt == media->payload_type) {
            strncpy(media->codec, codec, sizeof(media->codec) - 1);
        }
    }
    return 0;
}

// Парсинг a=control
int parse_sdp_control(const char *line, SDPMedia *media) {
    // a=control:track1
    sscanf(line, "a=control:%s", media->control_uri);
    return 0;
}
```

---

### 3. RTSP Client Implementation

#### Создание RTSP запроса:

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct {
    int sockfd;
    int cseq;            // Sequence number
    char session_id[64]; // Session ID из SETUP
} RTSPClient;

// Создать RTSP OPTIONS запрос
int rtsp_options(RTSPClient *client, const char *url) {
    char request[1024];
    snprintf(request, sizeof(request),
             "OPTIONS %s RTSP/1.0\r\n"
             "CSeq: %d\r\n"
             "User-Agent: MOONLIGHT/1.0\r\n"
             "\r\n",
             url, client->cseq++);
    
    // Отправить
    send(client->sockfd, request, strlen(request), 0);
    
    // Получить ответ
    char response[4096];
    int len = recv(client->sockfd, response, sizeof(response) - 1, 0);
    response[len] = '\0';
    
    printf("RTSP Response:\n%s\n", response);
    
    return 0;
}

// Создать DESCRIBE запрос
int rtsp_describe(RTSPClient *client, const char *url) {
    char request[1024];
    snprintf(request, sizeof(request),
             "DESCRIBE %s RTSP/1.0\r\n"
             "CSeq: %d\r\n"
             "Accept: application/sdp\r\n"
             "User-Agent: MOONLIGHT/1.0\r\n"
             "\r\n",
             url, client->cseq++);
    
    send(client->sockfd, request, strlen(request), 0);
    
    char response[4096];
    int len = recv(client->sockfd, response, sizeof(response) - 1, 0);
    response[len] = '\0';
    
    // SDP находится после пустой строки
    char *sdp = strstr(response, "\r\n\r\n");
    if (sdp) {
        printf("SDP:\n%s\n", sdp + 4);
    }
    
    return 0;
}
```

#### Подключение к камере:

```c
int rtsp_connect(const char *host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addr.sin_addr);
    
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}
```

---

### 4. Сканирование IP-камер

Типичные порты для IP-камер:
- **554** — RTSP (стандартный)
- **8554** — RTSP (альтернативный)
- **80/8080** — HTTP web-интерфейс

Типичные RTSP URL:
```
rtsp://IP:554/stream1       # Основной поток
rtsp://IP:554/stream2       # Второй поток (обычно меньше качество)
rtsp://IP:554/live          # Generic
rtsp://IP:554/h264          # H.264 поток
```

#### Код сканера:

```c
bool scan_rtsp_port(const char *ip, int port) {
    int sockfd = rtsp_connect(ip, port);
    if (sockfd < 0) {
        return false;  // Не подключилось
    }
    
    RTSPClient client = {.sockfd = sockfd, .cseq = 1};
    char url[256];
    snprintf(url, sizeof(url), "rtsp://%s:%d/", ip, port);
    
    // Попробовать OPTIONS
    if (rtsp_options(&client, url) == 0) {
        close(sockfd);
        return true;  // RTSP сервер найден!
    }
    
    close(sockfd);
    return false;
}

// Сканирование подсети
void scan_subnet(const char *subnet) {
    // Пример: 192.168.1.0/24
    for (int i = 1; i < 255; i++) {
        char ip[32];
        snprintf(ip, sizeof(ip), "%s.%d", subnet, i);
        
        printf("Scanning %s:554... ", ip);
        if (scan_rtsp_port(ip, 554)) {
            printf("✓ RTSP server found!\n");
        } else {
            printf("✗\n");
        }
    }
}
```

---

## 🛠 Практика

### Задание 1: RTSP Client (⭐⭐⭐☆☆)

Реализуйте базовый RTSP клиент:
- `rtsp_connect()` — подключение к камере
- `rtsp_options()` — запрос OPTIONS
- `rtsp_describe()` — запрос DESCRIBE и получение SDP

**Файл:** `artifacts/rtsp_client.c`

**Запуск:**
```bash
./rtsp_client rtsp://192.168.1.100:554/stream1
```

---

### Задание 2: SDP Parser (⭐⭐⭐☆☆)

Парсинг SDP:
- Извлечь media type (video/audio)
- Извлечь codec (H264, MPEG4)
- Извлечь control URI

**Файл:** `artifacts/sdp_parser.c`

---

### Задание 3: Camera Scanner (⭐⭐⭐⭐☆)

Создать сканер IP-камер:
- Сканирование подсети на порт 554
- Проверка RTSP с помощью OPTIONS
- Вывод найденных камер

**Файл:** `artifacts/camera_scanner.c`

**Запуск:**
```bash
./camera_scanner 192.168.1
# Сканирует 192.168.1.1-254
```

---

### 🎯 Challenge задача (опционально)

**RTSP Credential Bruteforce:**
- Попытка подключения с разными логинами/паролями
- Обработка 401 Unauthorized
- Digest Authentication

---

## 💡 Подсказки

<details>
<summary>Как парсить RTSP ответ?</summary>

```c
// RTSP ответ похож на HTTP
// Первая строка: RTSP/1.0 200 OK
// Заголовки: Key: Value
// Пустая строка
// Тело (SDP)

char *line = strtok(response, "\r\n");
while (line) {
    if (strncmp(line, "RTSP/", 5) == 0) {
        // Статус
        int code;
        sscanf(line, "RTSP/1.0 %d", &code);
    } else if (strstr(line, ":")) {
        // Заголовок
        char *colon = strchr(line, ':');
        *colon = '\0';
        char *key = line;
        char *value = colon + 2;  // +2 чтобы пропустить ": "
    }
    line = strtok(NULL, "\r\n");
}
```
</details>

<details>
<summary>Как обработать timeout при сканировании?</summary>

```c
#include <sys/select.h>

// Установить timeout для recv
struct timeval tv;
tv.tv_sec = 2;  // 2 секунды
tv.tv_usec = 0;
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
```
</details>

---

## ✅ Критерии выполнения

- [ ] RTSP клиент подключается к камере
- [ ] OPTIONS запрос работает корректно
- [ ] DESCRIBE запрос получает SDP
- [ ] SDP парсится и извлекается codec
- [ ] Scanner находит камеры в подсети
- [ ] Код компилируется без ошибок
- [ ] Все тесты проходят (`make test`)

---

## 📚 Дополнительные материалы

- [RTSP RFC 2326](https://tools.ietf.org/html/rfc2326)
- [SDP RFC 4566](https://tools.ietf.org/html/rfc4566)
- [IP Camera Security](https://www.shodan.io/search?query=port:554)

---

## 🎬 Сюжет

После успешного получения доступа к камерам, Виктор отправляет новое сообщение:

```
Отлично. Теперь у нас есть глаза.
Последний шаг — объединить всё: СКУД, камеры, датчики.

Готовься к финалу Season 6: IoT Integration.

— V.
```

---

**Следующий эпизод:** [Episode 24: IoT Integration →](../episode-24-iot-integration/)

**Назад:** [← Episode 22: Industrial Protocols](../episode-22-industrial-protocols/)

---

*MOONLIGHT Protocol: Eyes everywhere... 📹*