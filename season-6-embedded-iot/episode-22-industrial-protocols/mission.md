# 🎯 MISSION BRIEF: Industrial Protocols
## Episode 22 | CLASSIFIED

---

## 📋 ЗАДАНИЕ

**Цель:** Разработать инструменты для работы с промышленными протоколами СКУД

**Приоритет:** 🔴 ВЫСОКИЙ

---

## ✅ ЗАДАЧИ

### 1. Wiegand Parser (wiegand_parser.c)

Реализовать функции:

```c
// Парсинг Wiegand-26
WiegandCard parse_wiegand26(uint32_t raw_data);

// Генерация Wiegand-26
uint32_t generate_wiegand26(uint8_t facility_code, uint16_t card_id);

// Проверка валидности
bool wiegand_is_valid(uint32_t raw_data);
```

**Требования:**
- Правильное извлечение FC (8 бит) и Card ID (16 бит)
- Проверка even parity для первых 13 бит
- Проверка odd parity для последних 13 бит
- Генерация валидных кодов с правильной чётностью

---

### 2. Modbus Client (modbus_client.c)

Реализовать:

```c
// CRC16 для Modbus (полином 0xA001)
uint16_t modbus_crc16(const uint8_t *data, size_t len);

// Создать запрос Read Holding Registers
int modbus_read_holding_registers(uint8_t *buf, uint8_t slave_id,
                                   uint16_t addr, uint16_t count);

// Проверить CRC ответа
bool modbus_check_crc(const uint8_t *frame, size_t len);
```

**Требования:**
- CRC16 по стандарту Modbus RTU
- Корректное формирование запроса (slave, function, addr, count, CRC)
- Проверка CRC в ответе

---

### 3. RFID Emulator (rfid_emulator.c)

Создать эмулятор RFID-карт:

```c
// Загрузить список карт из файла
int load_cards(const char *filename, CardList *cards);

// Сгенерировать Wiegand-коды для всех карт
void generate_all_codes(CardList *cards);

// Вывести коды в hex
void print_codes(CardList *cards);
```

**Формат файла:**
```
123,45678
123,45679
200,12345
```

**Вывод:**
```
Card 1: FC=123, ID=45678, Wiegand=0x1B2D84E
Card 2: FC=123, ID=45679, Wiegand=0x1B2D86F
...
```

---

## 📊 КРИТЕРИИ УСПЕХА

- [ ] Wiegand parser работает корректно
- [ ] Генерация Wiegand с валидной чётностью
- [ ] Modbus CRC16 вычисляется правильно
- [ ] Modbus запрос формируется по спецификации
- [ ] RFID emulator читает файл и генерирует коды
- [ ] Все программы компилируются без ошибок
- [ ] Тесты проходят: `make test`

---

## 🔧 ИНСТРУМЕНТЫ

**Компиляция:**
```bash
make                  # Собрать все
make wiegand          # Только Wiegand parser
make modbus           # Только Modbus client
make rfid             # Только RFID emulator
```

**Тестирование:**
```bash
make test             # Автотесты
make clean            # Очистка
```

**Использование:**
```bash
./wiegand_parser 0x1A2B3C4
./wiegand_parser --gen 123 45678

./modbus_client
# (симуляция без реального устройства)

./rfid_emulator test_cards.txt
```

---

## ⚠️ ПРЕДУПРЕЖДЕНИЕ

Эти инструменты предназначены ТОЛЬКО для образовательных целей и тестирования собственных систем с разрешения владельца.

Несанкционированный доступ к системам контроля доступа является **преступлением**.

---

## 📡 ДОНЕСЕНИЕ

После выполнения задания:

1. Убедитесь, что все тесты проходят
2. Проверьте корректность Wiegand parity
3. Убедитесь, что Modbus CRC совпадает с эталонными значениями
4. Переходите к Episode 23

---

**Статус:** 🟡 IN PROGRESS

**Агент:** [ВАШ_ПОЗЫВНОЙ]

**Отчет направить:** The Architect

---

*Код — это ключ. Протокол — это замок. Взлом — это искусство.* 🔐