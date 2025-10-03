# Episode 21: Arduino & GPIO - SOLUTION

## 📖 О решении

Полноценный симулятор Arduino GPIO для обучения embedded programming.

**Версия:** v2.0 Enhanced Edition

## ✅ Реализованные функции

### GPIO Simulation
- ✅ Pin mode configuration (INPUT, OUTPUT, INPUT_PULLUP)
- ✅ Digital I/O (digitalWrite, digitalRead)
- ✅ Analog input (analogRead, 0-1023 ADC)
- ✅ PWM output (analogWrite, 0-255 duty cycle)

### Sensors
- ✅ **PIR Motion** — 3% detection probability, 5-cycle hold
- ✅ **Door Contact** — magnetic contact simulation
- ✅ **LM35 Temperature** — realistic thermal behavior with spikes

### Actuators
- ✅ **LED** — ON/OFF + PWM brightness control
- ✅ **Relay** — cooling/alarm activation
- ✅ **Buzzer** — beep with duration

### System Features
- ✅ Event logging (1000+ events capacity)
- ✅ Statistics tracking (uptime, detections, alerts)
- ✅ Alert system with thresholds
- ✅ Serial output (UART simulation)

## 🛠️ Компиляция

```bash
make
```

## 🚀 Запуск

```bash
./arduino_gpio
```

## 📊 Вывод

```
╔═══════════════════════════════════════════════════════════════════╗
║                    SURVEILLANCE STATUS                            ║
╚═══════════════════════════════════════════════════════════════════╝

📍 Sensors:
   PIR Motion:     🔴 DETECTED
   Door Contact:   🟢 Closed
   Temperature:    67.2°C 🔥 CRITICAL!

🎛️  Actuators:
   LED (Pin 13):    🔴 ON (PWM: 255/255)
   Relay (Pin 8):  🔴 ACTIVE

📊 System Status: ⚠️  ALERT
```

## 🎓 Образовательная ценность

Этот симулятор демонстрирует:
1. **GPIO Programming** — pin configuration и I/O
2. **Sensor Reading** — digital/analog input handling
3. **Actuator Control** — output devices + PWM
4. **Event-driven Systems** — alerts и logging
5. **State Machines** — sensor hold, timeouts

## 🔌 Реальное железо

Для работы с реальным Arduino:
1. Установите Arduino IDE
2. Замените simulation functions на hardware calls
3. Используйте Arduino delay() вместо usleep()
4. Подключите датчики: HC-SR501 (PIR), LM35, magnetic contact
5. Подключите актуаторы: LED, relay module, buzzer

## 🏆 Achievement

**"GPIO Master"** — You've mastered embedded GPIO programming.

---

**Operation MOONLIGHT** — Season 6, Episode 21  
v2.0 Enhanced Edition

