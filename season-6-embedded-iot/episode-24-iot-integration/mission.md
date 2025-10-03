# Миссия 24: IoT Integration & System Bypass — SEASON 6 FINALE

## 🎯 Цель

**ФИНАЛЬНАЯ ОПЕРАЦИЯ:** Интегрировать все IoT системы НИИ через MQTT для создания единого backdoor. Обойти охрану, проникнуть в серверную, собрать доказательства активности конкурентов.

## 📋 Брифинг миссии

**Дата:** 23 декабря, 00:00 MSK (полночь)  
**Локация:** Novosibirsk Akademgorodok, НИИ "Квантовые системы"  
**Статус:** ★★★ ФИНАЛЬНАЯ МИССИЯ ★★★

После анализа СКУД (Ep22) и камер (Ep23) обнаружено:
- **3 фигуры** в Lab 201 (конкуренты)
- Используют те же **blind spots**, что вы нашли
- **Ghost mode** активирован ими: CAM-006 disabled, датчики подавлены

Dmitry Orlov активировал **MQTT broker** — центр управления всеми IoT системами НИИ. Ваша миссия — перехватить контроль, создать backdoor, и закрыть Lab 201 с врагами внутри!

**Ваша миссия:**
1. Создать MQTT client для интеграции всех IoT систем
2. Реализовать IoT bridge (СКУД + камеры + датчики → MQTT)
3. Создать security dashboard для мониторинга
4. Выполнить финальную операцию: поймать конкурентов в ловушку

**Временной лимит:** До 01:00 MSK (1 час — финальный штурм!)

---

## 📂 Предоставленные файлы

- `artifacts/mqtt_broker_config.conf` — Настройки MQTT broker
- `artifacts/device_registry.json` — 47 IoT устройств НИИ
- `artifacts/event_log.csv` — Логи событий за последние 7 дней

---

## ✅ Критерии успеха

### 1. MQTT Client (⭐⭐⭐☆☆)

Реализовать MQTT клиент используя libmosquitto:

```c
#include <mosquitto.h>

typedef struct {
    struct mosquitto *mosq;
    char client_id[64];
    bool connected;
} MQTTClient;

// Callback при подключении
void on_connect(struct mosquitto *mosq, void *obj, int rc);

// Callback при получении сообщения
void on_message(struct mosquitto *mosq, void *obj, 
                const struct mosquitto_message *msg);

// Инициализация клиента
MQTTClient* mqtt_client_init(const char *broker, int port);

// Публикация сообщения
int mqtt_publish(MQTTClient *client, const char *topic, const char *payload);

// Подписка на топики
int mqtt_subscribe(MQTTClient *client, const char *topic_pattern);
```

**Требования:**
- Подключение к broker (localhost:1883 or mosquitto.local)
- QoS 0, 1, 2 support
- Persistent session
- Last Will and Testament (для обнаружения disconnects)

**MQTT Topics структура:**
```
moonlight/
├── access/
│   ├── door/lab201       # {"event": "unlock", "card_id": 12345}
│   └── door/main
├── camera/
│   ├── cam006/motion     # {"detected": true, "confidence": 0.81}
│   ├── cam006/status     # {"online": true, "fps": 25}
│   └── cam006/control    # {"action": "disable", "duration": 3600}
├── sensor/
│   ├── pir/lab201        # {"motion": true}
│   └── temp/lab201       # {"value": 24.5, "unit": "C"}
└── system/
    ├── status            # {"devices_online": 47, "alerts": 0}
    └── command           # {"action": "ghost_mode", "target": "lab201"}
```

---

### 2. IoT Bridge (⭐⭐⭐⭐☆)

Создать мосты для интеграции устройств:

```c
// RFID → MQTT bridge
void rfid_to_mqtt(MQTTClient *mqtt, uint8_t fc, uint16_t id);

// Camera motion → MQTT
void camera_motion_to_mqtt(MQTTClient *mqtt, const char *camera_id, 
                           bool detected, float confidence);

// Arduino sensors → MQTT
void sensor_to_mqtt(MQTTClient *mqtt, const char *sensor_id,
                   float value, const char *unit);

// MQTT → Relay control
void mqtt_to_relay(const char *payload, int relay_pin);
```

**Пример интеграции:**
```c
// При срабатывании PIR датчика
if (pir_detected()) {
    char payload[256];
    snprintf(payload, sizeof(payload),
             "{\"motion\": true, \"sensor\": \"PIR-12\", "
             "\"timestamp\": %ld}", time(NULL));
    
    mqtt_publish(client, "moonlight/sensor/pir/lab201", payload);
}
```

---

### 3. Security Dashboard (⭐⭐⭐⭐⭐)

Создать dashboard для мониторинга всех систем:

```c
// Dashboard state
typedef struct {
    int doors_locked;
    int doors_unlocked;
    int cameras_online;
    int motion_events;
    int temperature_alerts;
    time_t last_update;
} DashboardState;

// Обработка MQTT событий
void dashboard_on_message(const struct mosquitto_message *msg,
                         DashboardState *state);

// Визуализация в терминале
void dashboard_render(DashboardState *state);

// Алерты при подозрительных событиях
void dashboard_check_alerts(DashboardState *state);
```

**Пример вывода dashboard:**
```
╔═══════════════════════════════════════════════════════════════════╗
║  🌙 MOONLIGHT SECURITY DASHBOARD                                  ║
╠═══════════════════════════════════════════════════════════════════╣
║  Time: 23 Dec 2024, 00:47:32 MSK                                  ║
║  Status: 🔴 ACTIVE THREAT — Ghost Mode Engaged                    ║
║                                                                   ║
║  📊 SYSTEM STATUS:                                                ║
║  ┌─────────────────┬───────┬─────────┬────────┐                  ║
║  │ Component       │ Total │ Online  │ Alerts │                  ║
║  ├─────────────────┼───────┼─────────┼────────┤                  ║
║  │ Access Control  │   16  │   16    │   0    │                  ║
║  │ IP Cameras      │   24  │   23    │   1 🔴 │                  ║
║  │ Motion Sensors  │   24  │   23    │   1 🔴 │                  ║
║  │ Temp Sensors    │   12  │   12    │   0    │                  ║
║  └─────────────────┴───────┴─────────┴────────┘                  ║
║                                                                   ║
║  🚨 ACTIVE ALERTS:                                                ║
║  • CAM-006 (Lab 201): OFFLINE (manually disabled)                ║
║  • PIR-12 (Lab 201):  MOTION DETECTED (3 targets)                ║
║  • Door Lab 201:      UNLOCKED (unauthorized!)                   ║
║                                                                   ║
║  📹 LIVE FEED:                                                    ║
║  [CAM-006] Lab 201 Main:      🔴 OFFLINE                          ║
║  [CAM-005] Lab 201 Corridor:  🟢 ONLINE | Motion: YES            ║
║  [CAM-007] Lab 201 Emergency: 🟢 ONLINE | Motion: NO             ║
║                                                                   ║
║  🎯 TACTICAL SITUATION:                                           ║
║  Enemies inside Lab 201. Ghost mode active.                       ║
║  Recommendation: RE-LOCK Lab 201, FORCE CAM-006 online (4K)       ║
║                                                                   ║
║  [Press 'L' to lock Lab 201 | 'C' to force cameras | 'A' alarm]  ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

### 4. FINALE: Ghost Mode & Trap (⭐⭐⭐⭐⭐)

Финальная операция — активировать ловушку:

```c
typedef enum {
    OP_LOCK_LAB201,           // Запереть Lab 201
    OP_FORCE_CAMERA_ONLINE,   // Принудительно включить CAM-006
    OP_TRIGGER_ALARM,         // Пожарная сигнализация
    OP_GHOST_MODE,            // Невидимый режим
    OP_CAPTURE_FACES          // Захватить лица в 4K
} Operation;

// Выполнить операцию через MQTT
int execute_operation(MQTTClient *client, Operation op);

// Финальная последовательность
void finale_sequence(MQTTClient *client) {
    // 1. Lock Lab 201 (trap them inside)
    execute_operation(client, OP_LOCK_LAB201);
    
    // 2. Force CAM-006 online in 4K (capture faces)
    execute_operation(client, OP_FORCE_CAMERA_ONLINE);
    
    // 3. Wait for face capture (5 seconds)
    sleep(5);
    
    // 4. Optional: Trigger alarm (their escape)
    // execute_operation(client, OP_TRIGGER_ALARM);
    
    printf("✅ 3 faces captured in 4K!\n");
    printf("✅ Evidence secured!\n");
    printf("✅ SEASON 6 COMPLETE!\n");
}
```

---

## 💡 Подсказки

**MQTT Publish:**
```c
mosquitto_publish(client->mosq, NULL, 
                 "moonlight/access/door/lab201",
                 strlen(payload), payload,
                 1,      // QoS 1 (at least once delivery)
                 false); // не retain
```

**MQTT Subscribe с wildcard:**
```c
// Подписаться на все топики moonlight
mosquitto_subscribe(client->mosq, NULL, "moonlight/#", 0);
```

**JSON Parsing (simple):**
```c
// Без библиотеки — простой парсинг
char *find_json_value(const char *json, const char *key) {
    static char value[256];
    char search[128];
    snprintf(search, sizeof(search), "\"%s\":", key);
    
    char *start = strstr(json, search);
    if (!start) return NULL;
    
    start += strlen(search);
    while (*start == ' ' || *start == '"') start++;
    
    char *end = strchr(start, ',');
    if (!end) end = strchr(start, '}');
    
    int len = end - start;
    if (end[-1] == '"') len--;
    
    strncpy(value, start, len);
    value[len] = '\0';
    return value;
}
```

**Event Injection:**
```c
// Подменить MQTT event
void inject_fake_event(MQTTClient *client, const char *topic) {
    char payload[] = "{\"event\": \"all_clear\", \"timestamp\": 0}";
    mqtt_publish(client, topic, payload);
}
```

---

## 🔧 Компиляция

**Установка libmosquitto:**
```bash
# Ubuntu/Debian
sudo apt install mosquitto mosquitto-clients libmosquitto-dev

# macOS
brew install mosquitto

# Запуск MQTT broker
mosquitto -v
```

**Сборка:**
```bash
cd solution
make                    # Собрать все
make mqtt_client        # Только MQTT client
make iot_bridge         # IoT bridges
make dashboard          # Security dashboard
```

**Компиляция с libmosquitto:**
```bash
gcc mqtt_client.c -o mqtt_client -lmosquitto
```

---

## 🏆 Достижение

**"IoT Expert"** — Полный контроль над IoT инфраструктурой НИИ

**SEASON 6 FINALE BONUS:**
- ✅ 3 лица конкурентов captured in 4K
- ✅ Evidence secured
- ✅ Lab 201 compromised
- ✅ MOONLIGHT Operation: SUCCESS

---

## 🎬 Финал Season 6

```
╔═══════════════════════════════════════════════════════════════════╗
║  🎉 SEASON 6 COMPLETE!                                            ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  Episode 21: GPIO & Sensors ✅                                    ║
║  Episode 22: Wiegand & Modbus & RFID ✅                           ║
║  Episode 23: RTSP & IP Cameras ✅                                 ║
║  Episode 24: MQTT IoT Battle ✅                                   ║
║                                                                   ║
║  💼 РЕЗУЛЬТАТЫ:                                                   ║
║  • Все IoT системы под контролем                                  ║
║  • 3 лица конкурентов идентифицированы                            ║
║  • Доказательства собраны                                         ║
║  • Враг знает ваше лицо → Season 7 WAR                            ║
║                                                                   ║
║  ⚠️ CLIFFHANGER:                                                  ║
║  Один из захваченных — знал, что его снимают.                     ║
║  Смотрел прямо в камеру. Это была ловушка?                        ║
║  Season 7: War in the shadows...                                  ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

**Следующий сезон:** [Season 7: System Programming](../../season-7-system-programming/)