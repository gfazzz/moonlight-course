# Episode 21: Arduino & GPIO Fundamentals
> *"Hardware meets software"*

## 📖 Briefing

**From:** Viktor  
**Subject:** Embedded Infiltration  
**Priority:** HIGH

```
Агент,

Обнаружена секретная база. Обычные методы не работают - 
нужен физический доступ. Используйте Arduino для создания 
устройства, которое обойдёт систему безопасности.

Изучите GPIO, сенсоры, и актуаторы. Создайте прототип.

Время ограничено.
— V.
```

---

## 🎯 Что вы изучите

### Теория
- GPIO (General Purpose Input/Output)
- Digital vs Analog signals
- PWM (Pulse Width Modulation)
- Sensors (temperature, motion, light)
- Actuators (LEDs, motors, servos)
- Serial communication (UART)

### Практика
- Чтение с цифровых входов
- Управление выходами
- Аналоговое чтение (ADC)
- PWM для управления яркостью
- Serial debugging

---

## 📚 Theory: GPIO Basics

### Digital I/O

```c
// Arduino GPIO (для понимания концепции)
pinMode(13, OUTPUT);    // Настройка пина как выход
digitalWrite(13, HIGH); // Установить HIGH (5V)
digitalWrite(13, LOW);  // Установить LOW (0V)

int state = digitalRead(2); // Прочитать состояние пина
```

### На чистом C (embedded)

```c
// Registers для ATmega328P (Arduino Uno)
#include <avr/io.h>

// Настройка пина как выход
DDRB |= (1 << PB5);  // PB5 = pin 13 на Arduino

// Установка HIGH
PORTB |= (1 << PB5);

// Установка LOW
PORTB &= ~(1 << PB5);

// Чтение
uint8_t state = (PIND & (1 << PD2)) >> PD2;
```

### Analog Input (ADC)

```c
// Чтение аналогового значения (0-1023)
uint16_t value = analogRead(A0);

// Преобразование в вольты
float voltage = value * (5.0 / 1023.0);
```

### PWM (Pulse Width Modulation)

```c
// Управление яркостью LED или скоростью мотора
analogWrite(9, 128);  // 50% duty cycle (0-255)
```

---

## 💡 Сюжет

Вы разрабатываете устройство для:
1. Обнаружения движения (PIR sensor)
2. Мониторинга температуры (для обнаружения серверов)
3. Открытия электронного замка (relay)
4. Передачи данных (Serial)

---

## 🛠 Задачи

Смотрите [mission.md](mission.md) для деталей.

**Краткий список:**
1. ✅ Настроить GPIO (input/output)
2. ✅ Прочитать данные с сенсоров
3. ✅ Управлять актуаторами (LED, relay)
4. ✅ Реализовать Serial communication
5. ✅ Создать систему мониторинга

---

## 🏗 Структура проекта

```
episode-21-arduino-gpio/
├── README.md           # Этот файл
├── mission.md          # Техническое задание
├── starter.c           # Шаблон кода
├── Makefile           # Сборка
├── artifacts/         # Ваш код
├── tests/             # Тесты (симуляция)
└── solution/          # Решение (смотреть после!)
```

---

## 📝 Полезные ресурсы

- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [AVR Libc Documentation](https://www.nongnu.org/avr-libc/)
- ATmega328P Datasheet

---

## ⏭️ Следующий эпизод

**Episode 22: Industrial Protocols** — СКУД, Wiegand, Modbus

```bash
cd ../episode-22-industrial-protocols/
cat README.md
```
