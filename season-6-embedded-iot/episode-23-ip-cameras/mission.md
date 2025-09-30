# 🎯 MISSION BRIEF: IP Cameras & RTSP
## Episode 23 | CLASSIFIED

---

## 📋 ЗАДАНИЕ

**Цель:** Получить доступ к системе видеонаблюдения через RTSP

**Приоритет:** 🔴 КРИТИЧЕСКИЙ

---

## ✅ ЗАДАЧИ

### 1. RTSP Client (rtsp_client.c)

```c
// Подключение к камере
int rtsp_connect(const char *host, int port);

// RTSP OPTIONS
int rtsp_options(RTSPClient *client, const char *url);

// RTSP DESCRIBE (получить SDP)
int rtsp_describe(RTSPClient *client, const char *url);
```

### 2. SDP Parser (sdp_parser.c)

```c
// Парсинг media description (m=)
int parse_sdp_media(const char *line, SDPMedia *media);

// Парсинг codec (a=rtpmap)
int parse_sdp_rtpmap(const char *line, SDPMedia *media);

// Парсинг control URI
int parse_sdp_control(const char *line, SDPMedia *media);
```

### 3. Camera Scanner (camera_scanner.c)

```c
// Сканирование IP на RTSP порт
bool scan_rtsp_port(const char *ip, int port);

// Сканирование подсети
void scan_subnet(const char *subnet);
```

---

## 📊 КРИТЕРИИ УСПЕХА

- [ ] RTSP OPTIONS работает
- [ ] DESCRIBE получает SDP
- [ ] SDP парсится корректно
- [ ] Scanner находит камеры
- [ ] `make test` проходит

---

## 🔧 ТЕСТИРОВАНИЕ

```bash
make
./rtsp_client rtsp://camera.local:554/stream1
./camera_scanner 192.168.1
make test
```

---

*MOONLIGHT: We see everything... 📹*
