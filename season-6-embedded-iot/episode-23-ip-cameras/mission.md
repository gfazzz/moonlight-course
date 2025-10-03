# Миссия 23: IP Cameras & RTSP Surveillance

## 🎯 Цель

Получить доступ к системе видеонаблюдения НИИ через протокол RTSP. Найти слепые зоны и обнаружить активность конкурентов в Lab 201.

## 📋 Брифинг миссии

**Дата:** 22 декабря, 19:00 MSK  
**Локация:** Novosibirsk Akademgorodok, НИИ "Квантовые системы"  
**Статус:** 🔴🔴 КРИТИЧНО

После успешного изучения СКУД (Episode 22) обнаружено: враг использует слепые зоны камер для проникновения. Необходимо проанализировать всю систему видеонаблюдения (24 IP cameras) для:
1. Обнаружения слепых зон
2. Мониторинга движения в реальном времени
3. Доказательства активности конкурентов

**Ваша миссия:**
1. Реализовать RTSP client для подключения к камерам
2. Парсить SDP (Session Description Protocol) для извлечения метаданных
3. Создать сканер IP-камер в сети
4. Найти слепые зоны в системе охраны

**Временной лимит:** До 23:47 MSK (4 часа 47 минут)

---

## 📂 Предоставленные файлы

- `artifacts/camera_list.txt` — 24 IP камеры НИИ (IP адреса + credentials)
- `artifacts/motion_events.csv` — Логи детекции движения за 7 дней
- `artifacts/camera_coverage_map.txt` — Карта зон покрытия

---

## ✅ Критерии успеха

### 1. RTSP Client (⭐⭐⭐☆☆)

Реализовать базовый RTSP клиент:

```c
typedef struct {
    int sockfd;
    int cseq;               // Sequence number
    char session_id[64];    // Session ID из SETUP
} RTSPClient;

// Подключение к камере
int rtsp_connect(const char *host, int port);

// RTSP OPTIONS (узнать supported методы)
int rtsp_options(RTSPClient *client, const char *url);

// RTSP DESCRIBE (получить SDP description)
int rtsp_describe(RTSPClient *client, const char *url);

// Закрыть соединение
void rtsp_close(RTSPClient *client);
```

**Требования:**
- TCP подключение на порт 554 (standard RTSP port)
- Формирование RTSP запросов (похожи на HTTP)
- Парсинг RTSP ответов (status code, headers, body)
- Корректный CSeq (sequence number) в каждом запросе

**Пример RTSP запроса:**
```
OPTIONS rtsp://192.168.1.100:554/stream1 RTSP/1.0
CSeq: 1
User-Agent: MOONLIGHT/1.0

```

**Пример RTSP ответа:**
```
RTSP/1.0 200 OK
CSeq: 1
Server: Camera/2.0
Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN

```

---

### 2. SDP Parser (⭐⭐⭐⭐☆)

Парсить Session Description Protocol:

```c
typedef struct {
    char media_type[16];     // "video" или "audio"
    int port;                // RTP порт
    char codec[32];          // "H264", "MPEG4", etc
    int payload_type;        // RTP payload type (96, 97...)
    char control_uri[256];   // URI для SETUP
} SDPMedia;

// Парсинг media description (m= строка)
int parse_sdp_media(const char *line, SDPMedia *media);

// Парсинг codec (a=rtpmap строка)
int parse_sdp_rtpmap(const char *line, SDPMedia *media);

// Парсинг control URI
int parse_sdp_control(const char *line, SDPMedia *media);
```

**Пример SDP:**
```
v=0
o=- 2251938463 2251938463 IN IP4 192.168.1.100
s=RTSP Session
i=H.264 Video Stream
m=video 0 RTP/AVP 96
a=rtpmap:96 H264/90000
a=fmtp:96 packetization-mode=1
a=control:track1
```

**Извлечь:**
- Media type: `video`
- Codec: `H264`
- Payload type: `96`
- Control URI: `track1`

---

### 3. Camera Scanner (⭐⭐⭐⭐⭐)

Создать сканер IP-камер в сети:

```c
// Сканирование одного IP на RTSP порт
bool scan_rtsp_port(const char *ip, int port);

// Сканирование подсети (192.168.1.1-254)
void scan_subnet(const char *subnet);

// Попытка подключения с credentials
bool try_rtsp_auth(const char *ip, int port, 
                   const char *user, const char *pass);
```

**Требования:**
- Параллельное сканирование (threading опционально)
- Timeout для каждого подключения (2 секунды)
- Обработка authentication (Basic/Digest)
- Вывод найденных камер с метаданными

**Пример вывода:**
```
╔═══════════════════════════════════════════════════════════════════╗
║  IP CAMERA SCANNER — Network Surveillance Analysis                ║
╠═══════════════════════════════════════════════════════════════════╣
║  Scanning subnet: 192.168.1.0/24                                  ║
║  RTSP port: 554                                                   ║
║                                                                   ║
║  ✓ 192.168.1.101:554 — H264, Hikvision DS-2CD2032-I              ║
║  ✓ 192.168.1.102:554 — H264, Dahua IPC-HFW1320S                  ║
║  ✓ 192.168.1.106:554 — H264, Axis M1065-L (LAB 201!)             ║
║  ...                                                              ║
║                                                                   ║
║  Total cameras found: 24                                          ║
║  Vulnerable (default credentials): 18 (75%)                       ║
║  🔴 Critical: CAM-006 (Lab 201) — motion detected!                ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

## 💡 Подсказки

**RTSP Request формирование:**
```c
snprintf(request, sizeof(request),
         "OPTIONS %s RTSP/1.0\r\n"
         "CSeq: %d\r\n"
         "User-Agent: MOONLIGHT/1.0\r\n"
         "\r\n",
         url, client->cseq++);

send(client->sockfd, request, strlen(request), 0);
```

**RTSP Response парсинг:**
```c
char response[4096];
int len = recv(client->sockfd, response, sizeof(response) - 1, 0);
response[len] = '\0';

// Первая строка: RTSP/1.0 200 OK
int status_code;
sscanf(response, "RTSP/1.0 %d", &status_code);

// SDP находится после пустой строки (\r\n\r\n)
char *sdp = strstr(response, "\r\n\r\n");
if (sdp) {
    sdp += 4;  // Пропустить \r\n\r\n
    printf("SDP:\n%s\n", sdp);
}
```

**Timeout для socket:**
```c
#include <sys/select.h>

struct timeval tv;
tv.tv_sec = 2;   // 2 секунды timeout
tv.tv_usec = 0;
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
```

**Common RTSP URLs:**
```
rtsp://IP:554/stream1       # Основной поток (высокое качество)
rtsp://IP:554/stream2       # Второй поток (низкое качество)
rtsp://IP:554/live          # Generic stream
rtsp://IP:554/h264          # H.264 stream
```

**Default Credentials (для тестирования):**
```
admin:admin
admin:12345
root:root
admin:password
(часто оставляют default!)
```

---

## 🔧 Компиляция и тестирование

**Сборка:**
```bash
cd solution
make                          # Собрать все
make rtsp_client              # Только RTSP client
make camera_scanner           # Только scanner
```

**Запуск:**
```bash
# RTSP client
./rtsp_client rtsp://192.168.1.100:554/stream1

# Camera scanner
./camera_scanner 192.168.1    # Сканирует .1-.254

# С credentials
./camera_scanner 192.168.1 --user admin --pass admin
```

**Тестирование:**
```bash
make test
```

---

## 🔍 Ожидаемый результат

После сканирования вы обнаружите:

```
╔═══════════════════════════════════════════════════════════════════╗
║  BLIND SPOT ANALYSIS                                              ║
╠═══════════════════════════════════════════════════════════════════╣
║  Total cameras: 24                                                ║
║  Coverage: 87% (хорошо, но не идеально)                           ║
║                                                                   ║
║  🔴 BLIND SPOTS DETECTED:                                         ║
║  1. Stairwell B, Floor 2 → 3 (между CAM-012 и CAM-013)           ║
║  2. Loading Dock, North entrance (CAM-018 offline)                ║
║  3. Parking Lot, Sector C (CAM-020 повёрнута не туда)            ║
║  4. Lab 201 corridor (5 метров между CAM-005 и CAM-006)          ║
║                                                                   ║
║  ⚠️ MOTION DETECTED (CAM-006, Lab 201):                           ║
║     Timestamp: 22 Dec, 23:05:47 MSK                               ║
║     Confidence: 81%                                               ║
║     Objects: 3 (human-shaped)                                     ║
║                                                                   ║
║  💡 CONCLUSION: Враг использует blind spots!                      ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

## 🏆 Достижение

**"Camera Master"** — Полное проникновение в систему видеонаблюдения

---

**Следующий эпизод:** [Episode 24: IoT Integration — FINALE](../episode-24-iot-integration/)