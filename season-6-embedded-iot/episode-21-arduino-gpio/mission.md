# Mission Brief: GPIO & Sensors

## 🎯 Objective

Создать систему мониторинга на базе микроконтроллера для обнаружения:
- Движения (PIR sensor)
- Температуры (для локации серверов)
- Открытия двери (магнитный контакт)

## 📋 Requirements

### Task 1: GPIO Configuration

Настройте GPIO пины:
- Digital Input: PIR sensor (pin 2)
- Digital Input: Door contact (pin 3)
- Analog Input: Temperature sensor (A0)
- Digital Output: LED indicator (pin 13)
- Digital Output: Relay (pin 8)

### Task 2: Sensor Reading

Реализуйте функции:
```c
int read_motion(void);           // Вернуть 1 если движение
int read_door(void);             // Вернуть 1 если открыто
float read_temperature(void);    // Вернуть температуру в °C
```

### Task 3: Actuator Control

Реализуйте функции:
```c
void led_on(void);
void led_off(void);
void relay_activate(void);       // Открыть замок
void relay_deactivate(void);
```

### Task 4: Serial Communication

Отправляйте данные через Serial:
```
MOTION: 1
DOOR: 0  
TEMP: 28.5°C
STATUS: NORMAL
```

### Task 5: Alert System

Если обнаружено:
- Движение → LED мигает
- Температура > 50°C → Сигнал тревоги
- Дверь открыта → Запись в лог

## 🧪 Testing

Ваш код должен:
1. Компилироваться без ошибок
2. Правильно читать сенсоры
3. Управлять выходами
4. Отправлять данные через Serial

## 📦 Deliverables

Файлы в `artifacts/`:
- `gpio_system.c` — основной код
- `sensors.h` / `sensors.c` — работа с сенсорами
- `actuators.h` / `actuators.c` — управление выходами

## ⏱️ Time Estimate

- Learning: 1-2 hours
- Coding: 2-3 hours
- Testing: 1 hour

**Total:** ~4-6 hours

---

**Good luck, Agent!** 🚀
