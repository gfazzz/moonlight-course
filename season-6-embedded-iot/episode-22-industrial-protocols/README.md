# Episode 22: "Industrial Protocols" 🔐
## Season 6: Embedded & IoT | Episode 22/42

> *"Каждая дверь — это врата. Каждый протокол — это ключ."*

---

## 📋 Briefing

**От:** The Architect  
**Кому:** Agent [ВАШ_ПОЗЫВНОЙ]  
**Тема:** Проникновение через слаботочную инфраструктуру  
**Приоритет:** 🔴 ВЫСОКИЙ

**Ситуация:**

Виктор передал новую информацию: серверы организации защищены не только сетевой безопасностью. Физический доступ контролируется промышленной системой СКУД (Система Контроля и Управления Доступом).

Чтобы проникнуть в дата-центр, нам нужно понять, как работают промышленные протоколы:
- **Wiegand** — протокол для считывателей карт доступа
- **Modbus** — промышленный протокол для управления оборудованием

**Ваша задача:**
1. Реализовать парсер протокола Wiegand (26-bit и 34-bit)
2. Создать клиент Modbus RTU для опроса устройств
3. Эмулировать RFID-карту для обхода системы доступа

---

## 🎯 Цели эпизода

После прохождения вы научитесь:
- ✅ Работать с промышленными протоколами (Wiegand, Modbus)
- ✅ Парсить битовые данные с контроля доступа
- ✅ Реализовывать CRC16 для Modbus
- ✅ Работать с serial портами (RS-485/RS-232)
- ✅ Эмулировать RFID-устройства

---

## 📚 Теория

### 1. Протокол Wiegand

**Wiegand** — это стандарт передачи данных между считывателем карт и контроллером СКУД.

#### Формат Wiegand-26

```
26 бит = 1 бит чётности + 8 бит FC (Facility Code) + 16 бит ID + 1 бит чётности

Пример: 0 12345678 1234567890123456 1
         │    │              │      │
         P0  FC(8)        Card ID  P1
         
P0 = чётность первых 13 бит (even parity)
P1 = чётность последних 13 бит (odd parity)
```

#### Wiegand-34 (Extended)

```
34 бита = 1 + 16 бит FC + 16 бит ID + 1
```

**Физическая передача:**
- Два провода: Data0 и Data1
- Логический 0: импульс на Data0
- Логическая 1: импульс на Data1
- Длина импульса: 50 мкс, интервал: 2 мс

#### Код парсера Wiegand-26:

```c
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t facility_code;  // 8 бит
    uint16_t card_id;       // 16 бит
    bool valid;             // Проверка чётности
} WiegandCard;

// Парсинг Wiegand-26
WiegandCard parse_wiegand26(uint32_t raw_data) {
    WiegandCard card = {0};
    
    // Извлечь биты
    uint8_t p0 = (raw_data >> 25) & 0x01;  // Старший бит
    uint8_t fc = (raw_data >> 17) & 0xFF;  // 8 бит FC
    uint16_t id = (raw_data >> 1) & 0xFFFF; // 16 бит ID
    uint8_t p1 = raw_data & 0x01;           // Младший бит
    
    // Проверка чётности P0 (even parity для первых 13 бит)
    uint32_t first_half = (raw_data >> 13) & 0x1FFF;
    int count0 = __builtin_popcount(first_half);
    bool p0_valid = ((count0 % 2) == 0);
    
    // Проверка P1 (odd parity для последних 13 бит)
    uint32_t second_half = raw_data & 0x1FFF;
    int count1 = __builtin_popcount(second_half);
    bool p1_valid = ((count1 % 2) == 1);
    
    card.facility_code = fc;
    card.card_id = id;
    card.valid = p0_valid && p1_valid;
    
    return card;
}

// Генерация Wiegand-26
uint32_t generate_wiegand26(uint8_t fc, uint16_t card_id) {
    uint32_t data = 0;
    
    // Собрать средние биты (без чётности)
    data = ((uint32_t)fc << 17) | ((uint32_t)card_id << 1);
    
    // Вычислить P0 (even parity)
    uint32_t first_half = (data >> 13) & 0x1FFF;
    int count0 = __builtin_popcount(first_half);
    uint8_t p0 = (count0 % 2 == 0) ? 0 : 1;
    
    // Вычислить P1 (odd parity)
    uint32_t second_half = data & 0x1FFF;
    int count1 = __builtin_popcount(second_half);
    uint8_t p1 = (count1 % 2 == 1) ? 1 : 0;
    
    // Добавить биты чётности
    data |= ((uint32_t)p0 << 25);
    data |= p1;
    
    return data;
}
```

**Метафора:** Wiegand — это "штрих-код" на вашей пропускной карточке, только передаётся электрически.

---

### 2. Протокол Modbus RTU

**Modbus** — промышленный протокол для связи с устройствами (датчики, контроллеры, actuators).

#### Структура Modbus RTU

```
[Slave ID][Function][Data][CRC16 Low][CRC16 High]
   1 byte    1 byte   N bytes  1 byte    1 byte

Пример запроса (Read Holding Registers):
01 03 00 00 00 0A C5 CD
│  │  │     │     └─ CRC16
│  │  │     └─ Count (10 registers)
│  │  └─ Start Address (0x0000)
│  └─ Function Code (0x03 = Read Holding Registers)
└─ Slave ID (0x01)
```

#### Основные функции Modbus:

| Код | Название | Описание |
|-----|----------|----------|
| 0x01 | Read Coils | Чтение дискретных выходов |
| 0x03 | Read Holding Registers | Чтение регистров хранения |
| 0x05 | Write Single Coil | Запись одного дискрета |
| 0x06 | Write Single Register | Запись одного регистра |
| 0x10 | Write Multiple Registers | Запись нескольких регистров |

#### CRC16 для Modbus:

```c
#include <stdint.h>

// Modbus CRC16 (полином 0xA001)
uint16_t modbus_crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

// Проверка CRC
bool modbus_check_crc(const uint8_t *frame, size_t len) {
    if (len < 4) return false;
    
    uint16_t received_crc = frame[len-2] | (frame[len-1] << 8);
    uint16_t calculated_crc = modbus_crc16(frame, len - 2);
    
    return received_crc == calculated_crc;
}
```

#### Чтение регистров Modbus:

```c
typedef struct {
    uint8_t slave_id;
    uint8_t function;
    uint16_t start_addr;
    uint16_t count;
} ModbusRequest;

// Создать Modbus RTU запрос
int modbus_read_holding_registers(uint8_t *buf, uint8_t slave_id, 
                                   uint16_t addr, uint16_t count) {
    buf[0] = slave_id;
    buf[1] = 0x03;  // Read Holding Registers
    buf[2] = (addr >> 8) & 0xFF;
    buf[3] = addr & 0xFF;
    buf[4] = (count >> 8) & 0xFF;
    buf[5] = count & 0xFF;
    
    // Добавить CRC
    uint16_t crc = modbus_crc16(buf, 6);
    buf[6] = crc & 0xFF;
    buf[7] = (crc >> 8) & 0xFF;
    
    return 8;  // Длина запроса
}
```

**Метафора:** Modbus — это "язык общения" с промышленными устройствами, как HTTP для веб-серверов.

---

### 3. СКУД (Системы Контроля и Управления Доступом)

**Архитектура СКУД:**

```
[RFID Card] → [Reader] --Wiegand--> [Controller] --Modbus/RS485--> [Server]
                                         ↓
                                    [Door Lock]
                                    [Alarm]
```

**Типичные сценарии атак:**
1. **Клонирование карты** — перехват и эмуляция Wiegand
2. **Перебор кодов** — генерация валидных Facility Code + Card ID
3. **Modbus injection** — отправка команд напрямую контроллеру
4. **Relay attack** — ретрансляция сигнала с валидной карты

---

## 🛠 Практика

### Задание 1: Парсер Wiegand (⭐⭐☆☆☆)

Реализуйте функции:
- `parse_wiegand26()` — парсинг 26-битного кода
- `generate_wiegand26()` — генерация валидного кода
- Проверка чётности

**Файл:** `artifacts/wiegand_parser.c`

**Тест:**
```bash
make wiegand
./wiegand 0x1A2B3C4  # Парсит код
./wiegand --gen 123 45678  # Генерирует FC=123, ID=45678
```

---

### Задание 2: Modbus Client (⭐⭐⭐☆☆)

Реализуйте:
- `modbus_crc16()` — вычисление CRC
- `modbus_read_holding_registers()` — формирование запроса
- `modbus_parse_response()` — парсинг ответа

**Файл:** `artifacts/modbus_client.c`

**Тест:**
```bash
make modbus
./modbus_client /dev/ttyUSB0 1 0 10  # Slave 1, addr 0, count 10
```

---

### Задание 3: RFID Emulator (⭐⭐⭐⭐☆)

Создайте эмулятор RFID-карты:
- Чтение списка валидных карт из файла
- Генерация Wiegand-кодов
- Вывод в формате, готовом для инъекции

**Файл:** `artifacts/rfid_emulator.c`

**Формат файла `valid_cards.txt`:**
```
123,45678
123,45679
200,12345
```

**Запуск:**
```bash
./rfid_emulator valid_cards.txt
# Выводит все Wiegand-коды в hex
```

---

### 🎯 Challenge задача (опционально)

**Modbus Scanner:**
- Сканирование устройств на RS-485 (slave ID 1-247)
- Определение типа устройства по регистрам
- Карта промышленной сети

---

## 💡 Подсказки

<details>
<summary>Как вычислить parity bit?</summary>

```c
// Even parity: количество единиц чётное
int ones = __builtin_popcount(data);
uint8_t parity = (ones % 2 == 0) ? 0 : 1;

// Odd parity: количество единиц нечётное
uint8_t parity = (ones % 2 == 1) ? 1 : 0;
```
</details>

<details>
<summary>Как работать с serial портом в Linux?</summary>

```c
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
struct termios options;
tcgetattr(fd, &options);

// Настройка: 9600 baud, 8N1
cfsetispeed(&options, B9600);
cfsetospeed(&options, B9600);
options.c_cflag |= (CLOCAL | CREAD);
options.c_cflag &= ~PARENB;  // No parity
options.c_cflag &= ~CSTOPB;  // 1 stop bit
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;      // 8 data bits

tcsetattr(fd, TCSANOW, &options);
```
</details>

<details>
<summary>Как отладить Modbus без оборудования?</summary>

Используйте **виртуальные serial порты**:
```bash
# Linux
socat -d -d pty,raw,echo=0 pty,raw,echo=0
# Создаст /dev/pts/X и /dev/pts/Y

# Тестирование
./modbus_master /dev/pts/X &
cat /dev/pts/Y | hexdump -C
```
</details>

---

## ✅ Критерии выполнения

- [ ] Wiegand-26 парсится корректно
- [ ] Генерация Wiegand с валидной чётностью
- [ ] Modbus CRC16 работает правильно
- [ ] Modbus запрос формируется корректно
- [ ] RFID эмулятор генерирует валидные коды
- [ ] Код компилируется без ошибок
- [ ] Все тесты проходят (`make test`)

---

## 📚 Дополнительные материалы

- [Wiegand Protocol Specification](https://en.wikipedia.org/wiki/Wiegand_interface)
- [Modbus RTU Specification](https://www.modbus.org/specs.php)
- [СКУД: основы безопасности](https://www.integriti.com/access-control/)

---

## 🎬 Сюжет

После успешного анализа промышленных протоколов, вы получаете сообщение от Виктора:

```
Отличная работа. Теперь у нас есть ключи от всех дверей.
Следующий шаг — камеры наблюдения. Нам нужны глаза внутри.

Готовься к Episode 23: IP Cameras.

— V.
```

---

**Следующий эпизод:** [Episode 23: IP Cameras →](../episode-23-ip-cameras/)

**Назад:** [← Episode 21: Arduino & GPIO](../episode-21-arduino-gpio/)

---

*MOONLIGHT Protocol: Industrial infiltration in progress... 🔐*