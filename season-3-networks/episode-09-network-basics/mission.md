# Миссия: "Анализатор сетевых адресов"

## 🎯 Цель

Создать `net_analyzer` — утилиту для анализа IP-адресов и портов с классификацией и статистикой.

## 📋 Технические требования

### Входные данные

- Файл: `addresses.txt`
- Формат: одна строка = `IP:PORT`
- Пример:
  ```
  192.168.1.1:80
  10.0.0.5:22
  8.8.8.8:53
  ```

### Выходные данные

Для каждого адреса:
```
[N] IP:PORT
    Type: Private/Public/Loopback
    Network: X.X.X.X/MASK (для private)
    Port: XXXX (Service Name - Category)
    Hex: 0xXXXXXXXX
```

Итоговая статистика:
```
Statistics:
- Total addresses: N
- Private: X (XX%)
- Public: X (XX%)
- Loopback: X (XX%)
- Well-known ports: X
- Registered ports: X
- Dynamic ports: X
```

### Функционал

1. **Парсинг адресов**:
   - Разбор строки `IP:PORT`
   - Валидация IP-адреса (inet_pton)
   - Валидация порта (0-65535)

2. **Классификация IP**:
   - **Private** (RFC 1918):
     - 10.0.0.0/8
     - 172.16.0.0/12
     - 192.168.0.0/16
   - **Loopback**: 127.0.0.0/8
   - **Public**: все остальные

3. **Классификация портов**:
   - **Well-known** (0-1023): SSH, HTTP, HTTPS и т.д.
   - **Registered** (1024-49151)
   - **Dynamic/Private** (49152-65535)

4. **Дополнительная информация**:
   - Hex представление IP
   - Определение сети (для private)
   - Имя сервиса (для well-known портов)

---

## 🔧 Реализация

### Обязательные функции

```c
// Парсинг "IP:PORT"
int parse_address(const char *str, Address *addr);

// Проверка типа IP
int is_private_ip(struct in_addr addr);
int is_loopback_ip(struct in_addr addr);

// Классификация порта
const char* get_port_category(uint16_t port);
const char* get_service_name(uint16_t port);

// Вычисление сети (для private IP)
void calculate_network(struct in_addr addr, char *network_str);

// Конвертация в hex
uint32_t ip_to_hex(struct in_addr addr);
```

### Структура данных

```c
typedef struct {
    struct in_addr ip;
    uint16_t port;
} Address;

typedef struct {
    int total;
    int private_count;
    int public_count;
    int loopback_count;
    int wellknown_ports;
    int registered_ports;
    int dynamic_ports;
} Statistics;
```

---

## ✅ Критерии приёмки

- [ ] Программа компилируется без предупреждений
- [ ] Корректно парсит IP:PORT из файла
- [ ] Правильно классифицирует IP (private/public/loopback)
- [ ] Правильно классифицирует порты
- [ ] Выводит hex представление
- [ ] Вычисляет сеть для private IP
- [ ] Показывает имена сервисов для well-known портов
- [ ] Выводит полную статистику
- [ ] Обрабатывает некорректные адреса
- [ ] `make test` проходит успешно

---

## 💡 Подсказки

<details>
<summary>Как разобрать строку "IP:PORT"?</summary>

```c
char ip_str[INET_ADDRSTRLEN];
int port;

// Метод 1: sscanf
if (sscanf(line, "%15[^:]:%d", ip_str, &port) == 2) {
    // OK
}

// Метод 2: strtok
char *ip = strtok(line, ":");
char *port_str = strtok(NULL, ":");
port = atoi(port_str);
```
</details>

<details>
<summary>Как проверить loopback?</summary>

```c
int is_loopback_ip(struct in_addr addr) {
    uint32_t ip = ntohl(addr.s_addr);
    // 127.0.0.0/8
    return (ip & 0xFF000000) == 0x7F000000;
}
```
</details>

<details>
<summary>Как определить сеть для private IP?</summary>

```c
void calculate_network(struct in_addr addr, char *network_str) {
    uint32_t ip = ntohl(addr.s_addr);
    uint32_t mask, network;
    int prefix;
    
    if ((ip & 0xFF000000) == 0x0A000000) {
        mask = 0xFF000000;  // /8
        prefix = 8;
    } else if ((ip & 0xFFF00000) == 0xAC100000) {
        mask = 0xFFF00000;  // /12
        prefix = 12;
    } else if ((ip & 0xFFFF0000) == 0xC0A80000) {
        mask = 0xFFFF0000;  // /16
        prefix = 16;
    } else {
        sprintf(network_str, "N/A");
        return;
    }
    
    network = htonl(ip & mask);
    struct in_addr net_addr = { .s_addr = network };
    
    char net_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &net_addr, net_str, INET_ADDRSTRLEN);
    sprintf(network_str, "%s/%d", net_str, prefix);
}
```
</details>

---

## 🧪 Тестовые данные

`addresses.txt`:
```
192.168.1.1:80
10.0.0.5:22
172.16.0.10:443
8.8.8.8:53
127.0.0.1:8080
203.0.113.42:31337
0.0.0.0:0
255.255.255.255:65535
```

**Ожидаемый результат**:
- 3 private IP
- 2 public IP  
- 1 loopback IP
- 2 некорректных (0.0.0.0, 255.255.255.255)
- 4 well-known порта (22, 53, 80, 443)

---

**Удачи, агент!** 🕵️‍♂️
