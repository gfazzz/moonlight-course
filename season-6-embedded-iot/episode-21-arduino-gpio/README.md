# Episode 21: "GPIO & Embedded Infiltration" 🔌
## Season 6: Embedded & IoT | Episode 21/42

> *"Между битами и физическим миром — всего один вольт разницы."*

---

## 🎬 BRIEFING — Начало Season 6

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Novosibirsk Akademgorodok, Россия            ║
║  ВРЕМЯ:   20:30 (MSK), пятница, 20 декабря             ║
║  СТАТУС:  🔴 IMMEDIATE THREAT — IoT COUNTERATTACK       ║
║  GPS:     54.8486°N, 83.0974°E (Академгородок)         ║
╚══════════════════════════════════════════════════════════╝
```

**20 декабря, 14:50 GMT (17:50 MSK). Лондон, финал Season 5.**

Crypto схема раскрыта. $10M доказательств собрано. Европол уведомлён.

Вы на такси в аэропорт Heathrow. Рейс в Москву через 3 часа.

**14:52 GMT. Телефон взрывается сообщениями:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED - PRIORITY: CRITICAL)  │
│  TIME: 14:52:17 GMT                         │
│  STATUS: 🔴 ACTIVE THREAT                   │
├─────────────────────────────────────────────┤
│                                             │
│  МЕНЯЙ МАРШРУТ. СЕЙЧАС.                     │
│                                             │
│  НЕ Москва. Новосибирск.                    │
│  Академгородок. НИИ "Квантовые системы".    │
│                                             │
│  Они ЗНАЮТ о Season 5. Контратака началась.│
│  Через IoT. Камеры, датчики, микроконтроллеры│
│                                             │
│  Билет отправлен:                           │
│  BA 874: LHR → SVO (17:30 GMT)             │
│  S7 031: SVO → OVB (00:45 MSK +1)          │
│                                             │
│  Встреча: Dmitry Orlov, 08:00 MSK           │
│  Координаты: 54.8486°N, 83.0974°E           │
│                                             │
│  Они используют embedded systems.           │
│  Ты должен научиться их языку.              │
│  Arduino. GPIO. Sensors. Actuators.         │
│                                             │
│  48 часов на подготовку. Потом — действуем. │
│                                             │
│  — V.                                       │
│                                             │
│  P.S. Dmitry — лучший в дронах и авионике.  │
│       Но сейчас нам нужны ОСНОВЫ.           │
│       Начни с GPIO. Остальное — потом.      │
└─────────────────────────────────────────────┘
```

---

**Рейс BA 874 + S7 031:** London → Moscow → Novosibirsk  
**Время в пути:** 11 часов (3ч London→Moscow + 4ч layover + 4ч Moscow→Novosibirsk)  
**Прибытие:** 21 декабря, 08:00 MSK (Tolmachevo Airport, OVB)

---

**21 декабря, 08:15 MSK. Аэропорт Толмачёво (OVB), Новосибирск.**

Сибирский холод ударяет в лицо: -28°C. Солнце едва поднимается над горизонтом.

У выхода — мужчина в пуховике. Dmitry Orlov. 34 года. Авионика, дроны, embedded systems.

> **Dmitry:** *"Добро пожаловать в Сибирь. Поехали. Много работы."*

**08:47 MSK. Akademgorodok, НИИ "Квантовые системы".**

Советская архитектура, засекреченный объект, периметр с камерами.

Dmitry ведёт в лабораторию. На столе — Arduino Uno, макетная плата, провода, датчики.

```
┌─────────────────────────────────────────────┐
│  DMITRY'S BRIEFING                          │
│  TIME: 08:50 MSK                            │
├─────────────────────────────────────────────┤
│                                             │
│  Организация использует IoT против нас.     │
│  Камеры, датчики движения, СКУД,            │
│  пожарная сигнализация — всё на             │
│  микроконтроллерах.                         │
│                                             │
│  Чтобы победить их систему, нужно понять    │
│  КАК она работает. На уровне железа.        │
│                                             │
│  Episode 21: GPIO basics.                   │
│  Научишься читать сенсоры, управлять        │
│  актуаторами, работать с embedded C.        │
│                                             │
│  У нас 48 часов. После — они усилят охрану. │
│                                             │
│  Начинаем с азов. Arduino Uno.              │
│  ATmega328P микроконтроллер. 16 MHz.        │
│  14 digital pins, 6 analog inputs.          │
│                                             │
│  Задача:                                    │
│  1. Научиться GPIO (digital/analog I/O)     │
│  2. Подключить PIR sensor (движение)        │
│  3. Подключить DHT11 (температура/влажность)│
│  4. Управлять relay (замок)                 │
│  5. Serial communication (отладка)          │
│                                             │
│  Через 8 часов — тест. Симуляция охранной   │
│  системы НИИ. Если пройдёшь — двигаемся     │
│  дальше. Если нет — враг победит.           │
│                                             │
│  Вопросы?                                   │
└─────────────────────────────────────────────┘
```

**Вы киваете. Вопросов нет. Только работа.**

**09:00 MSK. Начинаем Episode 21: GPIO & Embedded Systems.**

---

## 🎯 Цели обучения

### Теория (Embedded Systems)
- **GPIO (General Purpose Input/Output):** цифровые и аналоговые пины
- **Digital signals:** HIGH (5V) / LOW (0V), pull-up/pull-down резисторы
- **Analog signals:** ADC (Analog-to-Digital Converter), 0-1023 range
- **PWM (Pulse Width Modulation):** управление яркостью/скоростью
- **Sensors:** PIR (движение), DHT11 (температура/влажность), фоторезисторы
- **Actuators:** LED, relay (управление замками), servo motors
- **Serial UART:** debugging через COM-порт

### Практика
- Чтение цифровых входов (кнопки, датчики движения)
- Управление цифровыми выходами (LED, relay)
- Аналоговое чтение (температура, освещённость)
- PWM для управления яркостью
- Serial communication для debugging

---

## 📖 Теоретическая часть

### 1. GPIO (General Purpose Input/Output)

**GPIO** — универсальные входы/выходы микроконтроллера. Каждый pin может работать в разных режимах:

```
Arduino Uno (ATmega328P):
┌────────────────────────────────┐
│  Digital Pins: 0-13 (14 pins)  │
│  • INPUT / OUTPUT              │
│  • PWM capable: 3,5,6,9,10,11  │
│                                │
│  Analog Pins: A0-A5 (6 pins)   │
│  • ADC: 10-bit (0-1023)        │
│  • Can be used as digital too  │
└────────────────────────────────┘
```

**Voltage Levels:**
- **HIGH:** 5V (логическая единица)
- **LOW:** 0V (логический ноль)

---

### 2. Digital I/O — Цифровые входы/выходы

**Чтение кнопки (digital input):**

```c
// Настройка пина как вход
pinMode(2, INPUT);  // Pin 2 = вход

// Чтение состояния
int button_state = digitalRead(2);
if (button_state == HIGH) {
    // Кнопка нажата (если pull-up)
}
```

**Pull-up резисторы:**

```c
// Встроенный pull-up резистор (20кОм)
pinMode(2, INPUT_PULLUP);

// Теперь:
// - Кнопка не нажата: HIGH (5V через резистор)
// - Кнопка нажата: LOW (0V, замыкание на землю)
```

**Управление LED (digital output):**

```c
// Настройка пина как выход
pinMode(13, OUTPUT);  // Pin 13 = встроенный LED

// Включить LED
digitalWrite(13, HIGH);
delay(1000);  // 1 секунда

// Выключить LED
digitalWrite(13, LOW);
delay(1000);
```

**Низкоуровневое управление (AVR registers):**

```c
#include <avr/io.h>

// Настройка pin 13 (PB5) как выход
DDRB |= (1 << PB5);   // Data Direction Register B

// Установить HIGH
PORTB |= (1 << PB5);  // PORT B register

// Установить LOW
PORTB &= ~(1 << PB5);

// Чтение pin 2 (PD2)
uint8_t state = (PIND & (1 << PD2)) >> PD2;
```

**Почему это важно для охранных систем:**
- PIR sensor → digital input (HIGH = движение обнаружено)
- Door contact → digital input (LOW = дверь закрыта, HIGH = открыта)
- Relay control → digital output (HIGH = замок открыт)

---

### 3. Analog Input — Аналоговые входы (ADC)

**ADC (Analog-to-Digital Converter)** преобразует напряжение 0-5V в число 0-1023 (10-bit).

```c
// Чтение аналогового пина
uint16_t value = analogRead(A0);  // Вернёт 0-1023

// Преобразование в вольты
float voltage = value * (5.0 / 1023.0);

// Пример: temperature sensor (TMP36)
// Output: 10mV/°C, 750mV при 25°C
float temp_celsius = (voltage - 0.5) * 100.0;
```

**Практический пример — фоторезистор (LDR):**

```c
int light_level = analogRead(A0);

if (light_level < 300) {
    // Темно — включить свет
    digitalWrite(LED_PIN, HIGH);
} else {
    // Светло — выключить
    digitalWrite(LED_PIN, LOW);
}
```

**Почему это важно:**
- Температурные датчики (обнаружение серверов по теплу)
- Фоторезисторы (обнаружение изменения освещения)
- Потенциометры (настройка чувствительности)

---

### 4. PWM (Pulse Width Modulation)

**PWM** — быстрое переключение между HIGH/LOW для имитации "среднего" напряжения.

```
Duty Cycle 25%:  ▁▁▁▁████▁▁▁▁████▁▁▁▁  → 1.25V average
             
Duty Cycle 50%:  ▁▁████▁▁████▁▁████▁▁  → 2.5V average

Duty Cycle 75%:  ▁████▁████▁████▁████  → 3.75V average
```

**Код:**

```c
// Pin 9 поддерживает PWM
pinMode(9, OUTPUT);

// 0 = 0% (всегда LOW)
// 127 = 50% duty cycle
// 255 = 100% (всегда HIGH)

analogWrite(9, 128);  // 50% яркости LED
```

**Применение:**
- Управление яркостью LED
- Скорость вентиляторов
- Servo motors (углы поворота)
- "Мягкое" открытие замков (менее шумно)

---

### 5. Sensors — Датчики

**PIR (Passive Infrared) — датчик движения:**

```c
#define PIR_PIN 2

void setup() {
    pinMode(PIR_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    int motion = digitalRead(PIR_PIN);
    if (motion == HIGH) {
        Serial.println("MOTION DETECTED!");
        // Trigger alarm / log event
    }
    delay(100);
}
```

**DHT11 — температура и влажность:**

```c
// Требует библиотеку DHT.h
#include <DHT.h>

DHT dht(DHT_PIN, DHT11);

void setup() {
    dht.begin();
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    if (temp > 50.0) {
        // Аномально высокая температура — сервер работает
        Serial.println("Server detected!");
    }
    
    delay(2000);  // DHT11: минимум 2 сек между чтениями
}
```

---

### 6. Actuators — Исполнительные устройства

**Relay module — управление замками:**

```c
#define RELAY_PIN 8

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);  // Relay OFF (замок закрыт)
}

void unlock_door() {
    digitalWrite(RELAY_PIN, HIGH);  // Relay ON
    delay(3000);                    // 3 секунды открыто
    digitalWrite(RELAY_PIN, LOW);   // Relay OFF
}
```

**⚠️ Важно:** Relay модули бывают:
- **Active LOW:** HIGH = OFF, LOW = ON
- **Active HIGH:** HIGH = ON, LOW = OFF

Всегда проверяйте datasheet!

---

### 7. Serial Communication (UART)

**Serial** — связь с компьютером для debugging:

```c
void setup() {
    Serial.begin(9600);  // 9600 baud rate
    Serial.println("System initialized");
}

void loop() {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    
    // Чтение команд от компьютера
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'U') {
            unlock_door();
            Serial.println("Door unlocked");
        }
    }
    
    delay(1000);
}

```

**Формат вывода:**
```
Temperature: 23.5 C
Humidity: 45.2 %
PIR: MOTION DETECTED!
```

---

## 💻 Задания

### Задание 1: LED Blink — Hello World для микроконтроллеров ⭐☆☆☆☆

Классическая первая программа:

```c
// TODO: Реализовать
void setup() {
    pinMode(13, OUTPUT);
}

void loop() {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}
```

**Модификация:** Morse code SOS (... --- ...)

---

### Задание 2: Button Input с антидребезгом ⭐⭐☆☆☆

```c
#define BUTTON_PIN 2
#define LED_PIN 13

bool led_state = false;
unsigned long last_debounce = 0;
unsigned long debounce_delay = 50;  // 50ms

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // TODO: Реализовать debouncing
    // При нажатии кнопки — переключить LED
}
```

---

### Задание 3: PIR Motion Detector ⭐⭐⭐☆☆

Создайте простую охранную систему:

```c
#define PIR_PIN 2
#define ALARM_PIN 13
#define BUZZER_PIN 8

// TODO: При обнаружении движения:
// 1. Включить LED
// 2. Включить buzzer
// 3. Вывести сообщение в Serial
// 4. Записать timestamp события
```

---

### Задание 4: Temperature Monitoring ⭐⭐⭐⭐☆

```c
#include <DHT.h>

#define DHT_PIN 7
#define TEMP_THRESHOLD 50.0

DHT dht(DHT_PIN, DHT11);

// TODO: 
// 1. Читать температуру каждые 2 секунды
// 2. Если temp > threshold → alarm
// 3. Вывести график в Serial Monitor (ASCII)
// 4. Сохранить последние 10 значений
```

---

### Задание 5: Security System Integration ⭐⭐⭐⭐⭐

Объединить всё вместе:

```c
// TODO: Создать полную охранную систему:
// 1. PIR sensor (движение)
// 2. DHT11 (температура/влажность)
// 3. Door contact (магнитный датчик)
// 4. Relay (управление замком)
// 5. Serial communication (команды и логи)
// 
// Команды через Serial:
// - 'S' → Status (состояние всех датчиков)
// - 'U' → Unlock door (открыть замок)
// - 'L' → Lock door (закрыть замок)
// - 'A' → Arm system (включить охрану)
// - 'D' → Disarm system (выключить охрану)
```

**Файл:** `artifacts/security_system.c`

---

## 🎬 DEBRIEFING — Финал Episode 21

**Локация:** Novosibirsk Akademgorodok, лаборатория НИИ  
**Время:** 21 декабря, 17:30 MSK  
**Статус:** 🟡 TRAINING COMPLETE — SYSTEM TEST PASSED

---

**17:15 MSK. 8 часов работы завершено.**

Dmitry проверяет вашу систему:

```
┌─────────────────────────────────────────────┐
│  SECURITY SYSTEM TEST — RESULTS             │
├─────────────────────────────────────────────┤
│                                             │
│  PIR Sensor:          ✅ PASS               │
│  • Detection time: 0.3s                     │
│  • False positives: 0                       │
│                                             │
│  DHT11 Sensor:        ✅ PASS               │
│  • Temperature: ±0.5°C accuracy             │
│  • Humidity: ±2% accuracy                   │
│                                             │
│  Relay Control:       ✅ PASS               │
│  • Lock/Unlock: 100% reliable               │
│  • Response time: 50ms                      │
│                                             │
│  Serial Commands:     ✅ PASS               │
│  • All commands functional                  │
│  • Logging operational                      │
│                                             │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│  OVERALL: PASSED (87/100 points)            │
│  STATUS: Ready for Episode 22               │
└─────────────────────────────────────────────┘
```

**17:25 MSK. Dmitry:**

> *"Хорошая работа. Основы освоены. Но это был только симулятор."*

Он показывает на монитор. На экране — **реальная охранная система НИИ**.

```
╔═══════════════════════════════════════════════════════════════════╗
║  НИИ "КВАНТОВЫЕ СИСТЕМЫ" — SECURITY OVERVIEW                      ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  Периметр:                                                        ║
║  • 24 PIR sensors (HC-SR501)                                      ║
║  • 16 door contacts (магнитные)                                   ║
║  • 12 temperature sensors (DHT22)                                 ║
║  • 8 relay modules (12V, управление замками)                      ║
║                                                                   ║
║  Управление:                                                      ║
║  • СКУД контроллер (Wiegand protocol)                             ║
║  • Modbus RTU (RS-485, промышленные датчики)                      ║
║  • IP камеры (RTSP streaming, 24 устройства)                      ║
║  • MQTT broker (IoT integration)                                  ║
║                                                                   ║
║  ⚠️ ПРОБЛЕМА:                                                     ║
║  Все эти системы работают НЕЗАВИСИМО.                             ║
║  Нужно изучить ПРОМЫШЛЕННЫЕ ПРОТОКОЛЫ.                            ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

**17:28 MSK. Dmitry продолжает:**

> *"Episode 21 — только начало. Ты научился GPIO. Отлично.*  
> *Но настоящая система использует ПРОМЫШЛЕННЫЕ протоколы:*  
> *- **Wiegand** — для RFID карт доступа*  
> *- **Modbus RTU** — для датчиков и контроллеров*  
> *- **RS-485** — для связи на большие расстояния*  
> *Episode 22 — завтра. Изучим СКУД. Это — ключ ко всему."*

**17:30 MSK. Неожиданно — монитор мигает:**

```
╔═══════════════════════════════════════════════════════════════════╗
║  🚨 SECURITY ALERT — PERIMETER BREACH                             ║
╠═══════════════════════════════════════════════════════════════════╣
║  Time: 17:30:14 MSK                                               ║
║  Location: Sector C, Lab 201                                      ║
║  Sensor: PIR-12 (motion detected)                                 ║
║  Temperature spike: +8°C (unusual)                                ║
║  Status: INVESTIGATING...                                         ║
╚═══════════════════════════════════════════════════════════════════╝
```

Dmitry хмурится. Набирает код на клавиатуре. Экран показывает:

**LAB 201: NO AUTHORIZED PERSONNEL. EMPTY.**

Но PIR датчик показывает движение. **Кто-то там есть.**

**17:31 MSK. Dmitry смотрит на вас:**

> *"Враг уже здесь. Они тестируют нашу охрану.*  
> *Episode 22 начинается СЕЙЧАС. Изучим их методы.*  
> *Пошли. Времени нет."*

**Достижение разблокировано:** 🏆 **"GPIO Master"** — основы embedded освоены

**Cliffhanger:** Кто-то проник в Lab 201. Датчики обнаружили. Камеры — нет. Как?..

---

**РЕЗУЛЬТАТ EPISODE 21:**
- ✅ GPIO освоен (digital/analog I/O)
- ✅ Sensors подключены (PIR, DHT11, buttons)
- ✅ Actuators управляются (LED, relay)
- ✅ Serial communication работает
- ✅ Security system integration выполнена
- ⚠️ **Обнаружена активность врага в Lab 201**
- ⚠️ **Срочно нужны промышленные протоколы (Episode 22)**

**Следующий шаг:** [Episode 22: Industrial Protocols →](../episode-22-industrial-protocols/)

---

## 🥚 Easter Eggs Episode 21

<details>
<summary>🔍 Отсылки и секреты (спойлеры!)</summary>

### Arduino History:
- **2005** — Arduino Uno создан в Ivrea, Italy (Interaction Design Institute)
- **ATmega328P** — легендарный микроконтроллер Atmel (теперь Microchip)
- **Pin 13** — встроенный LED на всех Arduino boards (традиция с 2005)
- **Blink** — первая программа для миллионов разработчиков

### Embedded Systems:
- **GPIO** — концепция с 1980s (первые микропроцессоры Intel 8051)
- **PWM** — изобретён в 1960s для управления моторами
- **UART** — Universal Asynchronous Receiver-Transmitter (1960s, teletype)
- **ADC 10-bit** — 0-1023 range = 2^10 значений

### Sensors:
- **PIR HC-SR501** — самый популярный motion sensor ($2, миллионы единиц)
- **DHT11** — бюджетный temp/humidity sensor (±2°C, ±5% точность)
- **TMP36** — аналоговый temperature sensor (Analog Devices)
- **Relay modules** — active HIGH vs active LOW (всегда читайте datasheet!)

### Новосибирск Akademgorodok:
- **Основан:** 1957 год, научный городок в Сибири
- **НИИ:** реальные институты квантовой физики, биологии, кибернетики
- **-28°C** — типичная зимняя температура декабря
- **Координаты:** 54.8486°N, 83.0974°E (реальные!)

### Числа:
- **8 часов** — типичное время обучения GPIO basics
- **87/100** — проходной балл для embedded systems курсов
- **17:30** — конец рабочего дня в НИИ
- **Lab 201** — отсылка к "Room 101" (George Orwell, "1984")

### Технические детали:
- **16 MHz** — тактовая частота ATmega328P
- **5V logic** — стандарт Arduino (vs 3.3V для ESP32)
- **20кОм pull-up** — встроенные резисторы ATmega328P
- **9600 baud** — стандартная скорость Serial (с 1980s)

**Философия Episode 21**: *"Between the bits and the real world is just one volt difference."*

</details>

---

## 🎓 Дополнительные материалы

### Arduino Resources
- 📚 [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- 📹 [Arduino Tutorial Series](https://www.youtube.com/user/sciguy14)
- 📖 [Arduino Cookbook](https://www.oreilly.com/library/view/arduino-cookbook/9781449399368/) — Michael Margolis

### AVR Programming
- 📚 [AVR Libc Documentation](https://www.nongnu.org/avr-libc/)
- 📖 **ATmega328P Datasheet** — 660 страниц технических спецификаций
- 📖 "Make: AVR Programming" — Elliot Williams

### Embedded Systems Theory
- 📖 "Making Embedded Systems" — Elecia White ⭐⭐⭐
- 📖 "The Art of Electronics" — Horowitz & Hill (библия электроники)
- 📹 [Embedded Systems Course](https://www.edx.org/course/embedded-systems) — UTAustinX

### Online Simulators (без оборудования)
- [Tinkercad Circuits](https://www.tinkercad.com/circuits) — Arduino симулятор онлайн
- [Wokwi](https://wokwi.com/) — ESP32 + Arduino симулятор
- [SimulIDE](https://www.simulide.com/) — offline симулятор

---

## 🏆 Бонусные задания

### Bonus 1: Morse Code Transmitter ⭐⭐⭐☆☆
LED мигает Morse code (SOS, ваше имя, сообщения).

### Bonus 2: Analog Music Player ⭐⭐⭐⭐☆
Генерация звуков через PWM (buzzer) — простые мелодии.

### Bonus 3: Interrupts ⭐⭐⭐⭐⭐
Использование hardware interrupts для мгновенной реакции на события.

### Bonus 4: Low Power Mode ⭐⭐⭐⭐⭐
Режим сна для экономии энергии (battery-powered devices).

---

## 📊 Что вы узнали

После Episode 21 вы умеете:

- ✅ Программировать микроконтроллеры на C
- ✅ Работать с GPIO (digital/analog I/O)
- ✅ Подключать sensors (PIR, temperature, buttons)
- ✅ Управлять actuators (LED, relays, motors)
- ✅ Debugging через Serial UART
- ✅ Создавать простые embedded системы

---

## 🔜 Следующий эпизод

В **Episode 22: Industrial Protocols** вы научитесь:
- Wiegand protocol для RFID карт доступа
- Modbus RTU для промышленных контроллеров
- RS-485 связь на большие расстояния
- СКУД (системы контроля доступа)

Проникновение в Lab 201 продолжается...

---

## 🔗 Навигация

- [← Season 5 Finale](../../season-5-financial-markets/episode-20-crypto-defi/)
- [↑ Season 6 Overview](../README.md)
- [→ Episode 22: Industrial Protocols](../episode-22-industrial-protocols/)
- [📋 Общий план курса](../../CURRICULUM.md)
- [🏠 Главная страница](../../README.md)

---

<div align="center">

**🌙 MOONLIGHT OPERATION**  
*Episode 21 of 42 • Season 6, Episode 1*

**"Hardware is the truth. Software is the poetry. Together — they are magic."** — Dmitry Orlov

```
// EPISODE 21: GPIO & EMBEDDED INFILTRATION
// STATUS: Enhanced (20% → 80%)
// DURATION: 3-4 hours
// LOCATION: Novosibirsk Akademgorodok (-28°C)
// ACHIEVEMENT: GPIO Master 🏆
```

[← Season 5](../../season-5-financial-markets/) | [Season 6](../README.md) | [Episode 22 →](../episode-22-industrial-protocols/)

---

**Episode 21 Complete!** ✅  
GPIO Basics → Sensors → Actuators → Security System Integration

**Next:** Episode 22 — Wiegand, Modbus, СКУД — промышленные протоколы взлома

</div>
