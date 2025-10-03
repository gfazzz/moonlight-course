# Episode 24: "IoT Integration" 🌐
## Season 6: Embedded & IoT | Episode 24/42 | Season Finale

> *"Всё связано. Всё под контролем."*

---

## 📋 Briefing

**От:** The Architect  
**Кому:** Agent [ВАШ_ПОЗЫВНОЙ]  
**Тема:** Финальная интеграция IoT инфраструктуры  
**Приоритет:** 🔴🔴 МАКСИМАЛЬНЫЙ

**Ситуация:**

У нас есть все части головоломки:
- ✅ Arduino & GPIO (Episode 21)
- ✅ СКУД & Modbus (Episode 22)
- ✅ IP Cameras & RTSP (Episode 23)

Теперь нужно объединить всё в единую систему используя **MQTT** — стандартный протокол для IoT.

**Финальная миссия Season 6:**
1. Создать MQTT клиент для C
2. Интегрировать все IoT устройства через MQTT
3. Создать центральную систему мониторинга
4. Обойти систему безопасности дата-центра

---

## 🎯 Цели эпизода

- ✅ Работать с протоколом MQTT
- ✅ Publish/Subscribe паттерн
- ✅ Интегрировать несколько IoT систем
- ✅ Создать dashboard для мониторинга
- ✅ Завершить Season 6!

---

## 📚 Теория

### 1. Протокол MQTT (Message Queuing Telemetry Transport)

**MQTT** — легковесный протокол pub/sub для IoT устройств.

#### Архитектура MQTT:

```
[Arduino]──┐
[RFID]─────┤
[Camera]───┼──→ [MQTT Broker] ←──→ [Your Dashboard]
[Sensor]───┤       (mosquitto)
[Modbus]───┘
```

#### MQTT Topics (темы):

```
moonlight/access/door1          # RFID события
moonlight/camera/cam1/motion    # Детекция движения
moonlight/sensor/temp           # Датчики
moonlight/modbus/device1/data   # Modbus данные
```

#### Основные операции:

| Операция | Описание |
|----------|----------|
| CONNECT | Подключение к брокеру |
| PUBLISH | Публикация сообщения в topic |
| SUBSCRIBE | Подписка на topic |
| DISCONNECT | Отключение |

#### Простой MQTT клиент (используя libmosquitto):

```c
#include <mosquitto.h>
#include <stdio.h>

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
    printf("Connected to broker! Result code: %d\n", rc);
    if (rc == 0) {
        // Подписаться на все топики moonlight
        mosquitto_subscribe(mosq, NULL, "moonlight/#", 0);
    }
}

void on_message(struct mosquitto *mosq, void *obj, 
                const struct mosquitto_message *msg) {
    printf("Message received on %s: %s\n", 
           msg->topic, (char *)msg->payload);
}

int main() {
    mosquitto_lib_init();
    
    struct mosquitto *mosq = mosquitto_new("moonlight_client", true, NULL);
    
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    
    // Подключиться к брокеру
    if (mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Unable to connect\n");
        return 1;
    }
    
    // Главный цикл
    mosquitto_loop_forever(mosq, -1, 1);
    
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    
    return 0;
}
```

**Компиляция:**
```bash
gcc mqtt_client.c -o mqtt_client -lmosquitto
```

---

### 2. IoT Integration Architecture

#### Схема интеграции:

```
┌─────────────────────────────────────────────────────┐
│                 MQTT Broker                         │
│              (mosquitto @ localhost:1883)           │
└──────────────┬──────────────────────────────────────┘
               │
     ┌─────────┼─────────┬──────────┬────────────┐
     │         │         │          │            │
┌────▼───┐ ┌──▼───┐ ┌───▼────┐ ┌───▼──────┐ ┌──▼───────┐
│ RFID   │ │Camera│ │ Arduino│ │  Modbus  │ │Dashboard │
│ Reader │ │ Feed │ │Sensors │ │ Devices  │ │ (You!)   │
└────────┘ └──────┘ └────────┘ └──────────┘ └──────────┘
```

#### Topics структура:

```
moonlight/
├── access/
│   ├── door1       # {"card_id": 12345, "granted": true}
│   └── door2
├── camera/
│   ├── cam1/motion # {"detected": true, "timestamp": 123456}
│   └── cam2/status
├── sensor/
│   ├── temp        # {"value": 25.3, "unit": "C"}
│   └── humidity
└── status/
    └── system      # {"online": true, "devices": 5}
```

---

### 3. Практическая реализация

#### RFID → MQTT Bridge:

```c
#include <mosquitto.h>
#include "wiegand.h"  // Из Episode 22

void publish_rfid_event(struct mosquitto *mosq, uint8_t fc, uint16_t id) {
    char topic[128];
    char payload[256];
    
    snprintf(topic, sizeof(topic), "moonlight/access/door1");
    snprintf(payload, sizeof(payload), 
             "{\"fc\": %u, \"id\": %u, \"timestamp\": %ld}", 
             fc, id, time(NULL));
    
    mosquitto_publish(mosq, NULL, topic, strlen(payload), payload, 0, false);
    printf("Published RFID event: %s\n", payload);
}

// В главном цикле:
while (1) {
    // Читать RFID карты (симуляция)
    if (rfid_card_detected()) {
        WiegandCard card = read_rfid_card();
        publish_rfid_event(mosq, card.facility_code, card.card_id);
    }
    sleep(1);
}
```

#### Camera → MQTT Bridge:

```c
void publish_motion_event(struct mosquitto *mosq, const char *camera_id) {
    char topic[128];
    char payload[256];
    
    snprintf(topic, sizeof(topic), "moonlight/camera/%s/motion", camera_id);
    snprintf(payload, sizeof(payload), 
             "{\"detected\": true, \"timestamp\": %ld}", time(NULL));
    
    mosquitto_publish(mosq, NULL, topic, strlen(payload), payload, 0, false);
}
```

#### Dashboard (подписка на все события):

```c
void on_message(struct mosquitto *mosq, void *obj, 
                const struct mosquitto_message *msg) {
    printf("\n[%s]\n", msg->topic);
    printf("Data: %s\n", (char *)msg->payload);
    
    // Парсинг JSON (простой вариант)
    if (strstr(msg->topic, "access")) {
        printf("🚪 Access event detected\n");
    } else if (strstr(msg->topic, "motion")) {
        printf("📹 Motion detected\n");
    } else if (strstr(msg->topic, "sensor")) {
        printf("📊 Sensor data received\n");
    }
}
```

---

## 🛠 Практика

### Задание 1: MQTT Client (⭐⭐⭐☆☆)

Создать базовый MQTT клиент:
- Подключение к брокеру
- Publish сообщений
- Subscribe на топики

**Файл:** `artifacts/mqtt_client.c`

```bash
# Установить mosquitto
# Ubuntu/Debian:
sudo apt install mosquitto mosquitto-clients libmosquitto-dev

# macOS:
brew install mosquitto

# Запуск брокера
mosquitto -v

# Тестирование
./mqtt_client
```

---

### Задание 2: IoT Bridge (⭐⭐⭐⭐☆)

Создать мосты для устройств:
- RFID → MQTT
- Camera motion → MQTT
- Arduino sensors → MQTT

**Файл:** `artifacts/iot_bridge.c`

---

### Задание 3: Security Dashboard (⭐⭐⭐⭐⭐)

Создать dashboard для мониторинга:
- Подписка на все события
- Визуализация в терминале
- Логирование в файл
- Алерты при подозрительных событиях

**Файл:** `artifacts/dashboard.c`

---

### 🎯 Challenge задача (Season 6 Finale!)

**System Bypass:**
- Обнаружить паттерн движения охраны
- Подменить видеопоток (loop recording)
- Эмулировать валидную RFID карту
- Открыть дверь используя Modbus inject
- **Проникнуть в дата-центр!**

---

## 💡 Подсказки

<details>
<summary>Как установить libmosquitto?</summary>

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install libmosquitto-dev

# macOS
brew install mosquitto

# Проверка
pkg-config --libs libmosquitto
```
</details>

<details>
<summary>Как парсить JSON без библиотеки?</summary>

```c
// Простой парсинг (для основных случаев)
char *find_json_value(const char *json, const char *key) {
    static char value[256];
    char search[128];
    snprintf(search, sizeof(search), "\"%s\": ", key);
    
    char *start = strstr(json, search);
    if (!start) return NULL;
    
    start += strlen(search);
    if (*start == '"') start++;  // Строковое значение
    
    char *end = strchr(start, ',');
    if (!end) end = strchr(start, '}');
    
    int len = end - start;
    if (end[-1] == '"') len--;
    
    strncpy(value, start, len);
    value[len] = '\0';
    return value;
}
```

Или используйте **cJSON** библиотеку для полноценного парсинга.
</details>

---

## ✅ Критерии выполнения

- [ ] MQTT клиент подключается к брокеру
- [ ] Publish работает корректно
- [ ] Subscribe получает сообщения
- [ ] IoT bridge интегрирует устройства
- [ ] Dashboard отображает события
- [ ] JSON парсится корректно
- [ ] Все тесты проходят (`make test`)
- [ ] **Season 6 завершен!** 🎉

---

## 📚 Дополнительные материалы

- [MQTT.org](https://mqtt.org/)
- [Eclipse Mosquitto](https://mosquitto.org/)
- [MQTT Essentials](https://www.hivemq.com/mqtt-essentials/)

---

## 🎬 Season 6 Finale

После успешной интеграции IoT инфраструктуры, вы получаете сообщение:

## 🎬 Финал Season 6 — FINAL SHOWDOWN

**Локация:** Novosibirsk Akademgorodok  
**Время:** 23 декабря, 23:00 (MSK)  
**Статус:** 🔴 ACTIVE HOSTILE ENGAGEMENT

---

**22:47. MQTT broker подключен. Все IoT устройства под наблюдением.**

**22:58:34. ALERT — новое подключение:**
```
Client: shadow-bot | IP: 192.168.1.253 | Subscribed: iot/#
```

**Конкуренты из Episode 23. Они здесь. Сейчас.**

**22:58:40. MQTT injection detected:**
```json
Topic: iot/access/doors/lab201
Message: {"event":"door_unlock_override"}
```

**Lab201 unlocked. Они внутри.**

**23:00:00. GHOST MODE activation:**
- CAM-006 recording → DISABLED (1 hour)
- Motion sensor → DISABLED (1 hour)  
- Alarm → Silent mode (1 hour)

**Полная темнота. Как вы и планировали. Но не вы это сделали.**

**23:01:15. V.:**

```
ВИДИШЬ? Ghost Mode. Наши методы против нас.
COUNTER-ATTACK. СЕЙЧАС.

1. Lock Lab201 (MQTT override)
2. Force CAM-006 online (4K)
3. Trigger alarm

ЗАПУСКАЙ.
```

**23:01:30. EXECUTED. Lab201 RE-LOCKED. Competitors TRAPPED.**

CAM-006 (4K, forced online): **3 фигуры у сервера. Паника. Лица видны.**

**23:05:30. Fire alarm triggered (их последний шанс) → ALL DOORS UNLOCK.**

**Они escaped. Но слишком поздно — faces captured in 4K.**

**23:15:00. V. — финальное сообщение:**

```
┌─────────────────────────────────────────────┐
│  SEASON 6 COMPLETE                          │
│  ★★★ PERFECT ★★★                            │
│                                             │
│  3 faces captured. Досье собрано.           │
│  Lab data secured. Evidence complete.       │
│                                             │
│  Season 7: System Programming               │
│  Они знают твои методы.                     │
│  Пора идти глубже — в тени системы.         │
│                                             │
│  48 часов отдыха. Welcome to underground.   │
│                                             │
│  P.S. Тот, кто узнал тебя на камере —       │
│       твой враг в Season 7. Приготовься.    │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**🎉 SEASON 6 COMPLETE! 🎉**

```
╔═══════════════════════════════════════════════════╗
║  SEASON 6: EMBEDDED & IoT — SUMMARY               ║
╠═══════════════════════════════════════════════════╣
║  📅 Ep21: GPIO & Sensors 🏆                       ║
║  📅 Ep22: Wiegand & Modbus & RFID 🏆              ║
║  📅 Ep23: RTSP & Cameras (competitors detected) 🏆║
║  📅 Ep24: MQTT Battle (3 faces captured) 🏆       ║
║                                                   ║
║  ✅ IoT Systems MASTERED                          ║
║  ✅ Competitors IDENTIFIED                        ║
║  ✅ Evidence COLLECTED                            ║
║  ⚠️ Enemy knows your face → Season 7 WAR          ║
╚═══════════════════════════════════════════════════╝
```

**Достижение:** 🏆 **"IoT Expert"** — полный контроль над IoT

**Cliffhanger:** Враг знает ваше лицо. Season 7 — война в тенях.

---

**Следующий сезон:** [Season 7: System Programming →](../../season-7-system-programming/)

**Назад:** [← Episode 23: IP Cameras](../episode-23-ip-cameras/)

---

*MOONLIGHT Protocol: The real war happens in the shadows. See you there. 🌐*