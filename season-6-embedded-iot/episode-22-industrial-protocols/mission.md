# Миссия 22: Промышленные протоколы и СКУД

## 🎯 Цель

Разработать инструменты для работы с промышленными протоколами СКУД (Wiegand, Modbus RTU) для проникновения в систему контроля доступа НИИ.

## 📋 Брифинг миссии

**Дата:** 22 декабря, 09:00 MSK  
**Локация:** Novosibirsk Akademgorodok, НИИ "Квантовые системы"  
**Статус:** 🔴 КРИТИЧНО

После Episode 21 обнаружена активность врага в Lab 201. Dmitry Orlov анализировал систему СКУД всю ночь. Необходимо изучить промышленные протоколы для обхода системы контроля доступа.

**Ваша миссия:**
1. Реализовать Wiegand protocol parser (26-bit и 34-bit)
2. Создать Modbus RTU client для опроса устройств
3. Разработать RFID card emulator
4. Получить доступ к Lab 201 через клонированную карту

**Временной лимит:** До 18:00 MSK (9 часов — полный рабочий день)

---

## 📂 Предоставленные файлы

- `artifacts/wiegand_samples.txt` — Примеры Wiegand кодов с камер
- `artifacts/modbus_devices.txt` — Список Modbus устройств в сети
- `artifacts/valid_cards.txt` — База валидных RFID карт (123,45678 формат)

---

## ✅ Критерии успеха

### 1. Wiegand Parser (⭐⭐⭐☆☆)

Реализовать функции:

```c
typedef struct {
    uint8_t facility_code;  // 8 бит
    uint16_t card_id;       // 16 бит
    bool valid;             // Проверка чётности
} WiegandCard;

// Парсинг Wiegand-26
WiegandCard parse_wiegand26(uint32_t raw_data);

// Генерация Wiegand-26
uint32_t generate_wiegand26(uint8_t facility_code, uint16_t card_id);

// Проверка валидности
bool wiegand_is_valid(uint32_t raw_data);
```

**Требования:**
- Правильное извлечение FC (facility code, 8 бит) и Card ID (16 бит)
- Проверка **even parity** для первых 13 бит
- Проверка **odd parity** для последних 13 бит
- Генерация валидных кодов с корректной чётностью

**Формат Wiegand-26:**
```
26 бит = [P0][FC(8 бит)][Card ID(16 бит)][P1]

P0 = even parity первых 13 бит
P1 = odd parity последних 13 бит
```

---

### 2. Modbus Client (⭐⭐⭐⭐☆)

Реализовать Modbus RTU клиент:

```c
// CRC16 для Modbus (полином 0xA001)
uint16_t modbus_crc16(const uint8_t *data, size_t len);

// Создать запрос Read Holding Registers (function 0x03)
int modbus_read_holding_registers(uint8_t *buf, uint8_t slave_id,
                                   uint16_t addr, uint16_t count);

// Проверить CRC ответа
bool modbus_check_crc(const uint8_t *frame, size_t len);

// Парсить ответ
int modbus_parse_response(const uint8_t *frame, size_t len, 
                          uint16_t *values);
```

**Требования:**
- CRC16 по стандарту Modbus RTU (полином 0xA001)
- Корректное формирование запроса: [slave][func][addr_h][addr_l][count_h][count_l][crc_l][crc_h]
- Проверка CRC в ответе
- Обработка ошибок (timeout, invalid CRC, exception responses)

**Пример запроса Read Holding Registers:**
```
01 03 00 00 00 0A C5 CD
│  │  │     │     └─ CRC16 (low, high)
│  │  │     └─ Количество регистров (10)
│  │  └─ Стартовый адрес (0x0000)
│  └─ Function code (0x03 = Read Holding Registers)
└─ Slave ID (0x01)
```

---

### 3. RFID Emulator (⭐⭐⭐⭐⭐)

Создать эмулятор RFID-карт для НИИ:

```c
typedef struct {
    uint8_t fc;
    uint16_t id;
    uint32_t wiegand_code;
} Card;

typedef struct {
    Card *cards;
    int count;
} CardList;

// Загрузить список карт из файла
int load_cards(const char *filename, CardList *cards);

// Сгенерировать Wiegand-коды для всех карт
void generate_all_codes(CardList *cards);

// Вывести коды в hex (готовые к инъекции)
void print_codes(CardList *cards);

// Найти карту с доступом к определённой зоне
Card *find_card_for_zone(CardList *cards, const char *zone);
```

**Формат файла `valid_cards.txt`:**
```
# FC, Card ID
123,45678
123,45679
200,12345
```

**Ожидаемый вывод:**
```
╔═══════════════════════════════════════════════════════╗
║  RFID EMULATOR — Wiegand Code Generator               ║
╠═══════════════════════════════════════════════════════╣
║  Loaded 47 valid cards from database                 ║
║                                                       ║
║  Card 001: FC=123 ID=45678 → Wiegand=0x01B2D84E      ║
║  Card 002: FC=123 ID=45679 → Wiegand=0x01B2D86F      ║
║  Card 003: FC=200 ID=12345 → Wiegand=0x03243039      ║
║  ...                                                  ║
║                                                       ║
║  🔑 LAB 201 ACCESS CARDS:                             ║
║     Card 015: FC=200 ID=17891 → Wiegand=0x0324BDE3   ║
║     Card 027: FC=200 ID=17892 → Wiegand=0x0324BDE4   ║
║                                                       ║
╚═══════════════════════════════════════════════════════╝
```

---

## 💡 Подсказки

**Wiegand Parity Calculation:**
```c
// Even parity (чётность)
int count_bits(uint32_t value) {
    return __builtin_popcount(value);
}

uint8_t even_parity(uint32_t data) {
    return (count_bits(data) % 2 == 0) ? 0 : 1;
}

// Odd parity (нечётность)
uint8_t odd_parity(uint32_t data) {
    return (count_bits(data) % 2 == 1) ? 1 : 0;
}
```

**Modbus CRC16:**
```c
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
```

**RS-485 Communication (опционально):**
```c
#include <fcntl.h>
#include <termios.h>

int fd = open("/dev/ttyUSB0", O_RDWR);
struct termios options;
tcgetattr(fd, &options);

// 9600 baud, 8N1 (8 data bits, No parity, 1 stop bit)
cfsetispeed(&options, B9600);
cfsetospeed(&options, B9600);
options.c_cflag |= (CLOCAL | CREAD);
options.c_cflag &= ~PARENB;  // No parity
options.c_cflag &= ~CSTOPB;  // 1 stop bit

tcsetattr(fd, TCSANOW, &options);
```

---

## 🔧 Компиляция и тестирование

**Сборка:**
```bash
cd solution
make                    # Собрать все
make wiegand            # Только Wiegand parser
make modbus             # Только Modbus client
make rfid               # Только RFID emulator
```

**Запуск:**
```bash
# Wiegand parser
./wiegand_parser 0x1B2D84E          # Парсинг кода
./wiegand_parser --gen 123 45678    # Генерация кода

# Modbus client (симуляция)
./modbus_client

# RFID emulator
./rfid_emulator ../artifacts/valid_cards.txt
```

**Тестирование:**
```bash
make test               # Автотесты
make clean              # Очистка
```

---

## 🏆 Достижение

**"Access Control Master"** — Успешный обход СКУД через клонирование RFID

---

## ⚠️ ПРЕДУПРЕЖДЕНИЕ

Эти инструменты предназначены **ТОЛЬКО** для:
- Образовательных целей
- Тестирования собственных систем
- Работы с письменного разрешения владельца

**Несанкционированный доступ к СКУД является преступлением** и преследуется по закону.

---

**Следующий эпизод:** [Episode 23: IP Cameras & RTSP](../episode-23-ip-cameras/)