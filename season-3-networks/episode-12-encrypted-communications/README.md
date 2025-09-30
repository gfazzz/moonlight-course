# Episode 12: "Encrypted Communications" 🔐

> *"The final piece of the puzzle lies in the encrypted stream."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   02:15, четверг, 2 октября                     ║
║  СТАТУС:  🔴 FINAL MISSION - SEASON 3                    ║
╚══════════════════════════════════════════════════════════╝
```

Вы прошли три этапа: **IP-адресацию**, **сокеты**, **перехват пакетов**.  
Теперь — **финал Season 3**.

В 02:12 приходит последнее сообщение V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED CHANNEL)               │
│  TIME: 02:12:58 UTC                         │
│  PRIORITY: CRITICAL                         │
├─────────────────────────────────────────────┤
│                                             │
│  Это финал Season 3.                        │
│  Всё, чему ты научился, соберётся воедино.  │
│                                             │
│  СИТУАЦИЯ:                                  │
│  Два агента обмениваются зашифрованными     │
│  данными через TCP.                         │
│                                             │
│  Адрес: 10.0.0.1:4433                       │
│  Протокол: TCP с XOR шифрованием            │
│  Ключ: Встроен в первый пакет               │
│                                             │
│  ЗАДАЧА:                                    │
│  1. Перехватить TCP-поток (Episode 11)      │
│  2. Реконструировать сессию по seq          │
│  3. Извлечь ключ из первого пакета          │
│  4. Расшифровать XOR (Episode 01)           │
│  5. Получить финальное сообщение            │
│                                             │
│  Это проверка ВСЕХ твоих навыков.           │
│                                             │
│  Удачи. Ты готов.                           │
│  — V.                                       │
│                                             │
└─────────────────────────────────────────────┘
```

**Что вы понимаете:**
- Это **интеграция** всех эпизодов Season 3
- Нужно объединить **packet sniffing** + **TCP session tracking** + **XOR decryption**
- **Sequence numbers** помогут упорядочить пакеты
- **Первый пакет** содержит ключ шифрования
- **Финальное сообщение** откроет путь к Season 4

Это **момент истины**. Всё, что вы изучили, должно сработать.

### 🎯 ЗАДАЧА

Создать **`comm_interceptor`** — систему перехвата зашифрованных коммуникаций:
1. ✅ Перехватить TCP трафик на порту 4433 (raw sockets)
2. ✅ Реконструировать TCP сессию по sequence numbers
3. ✅ Упорядочить пакеты по seq
4. ✅ Извлечь ключ из первого пакета
5. ✅ Объединить payload всех пакетов
6. ✅ Расшифровать XOR
7. ✅ Получить финальное сообщение V.

**Сложность:** 🔴 Hard (Finale)  
**Новые навыки:** TCP session reconstruction, packet ordering, stream assembly

```
╔══════════════════════════════════════════════╗
║  [!] ФИНАЛЬНАЯ МИССИЯ SEASON 3               ║
║  Все навыки соберутся воедино:               ║
║  - XOR (Episode 01)                          ║
║  - IP адресация (Episode 09)                 ║
║  - Сокеты (Episode 10)                       ║
║  - Packet sniffing (Episode 11)              ║
╚══════════════════════════════════════════════╝
```

---

## 📚 Теория: TCP Session Reconstruction

### 💡 Зачем это нужно в реальности?

**Session reconstruction** используется в:
- 🔍 **Network forensics**: восстановление данных после инцидента
- 🛡️ **IDS/IPS**: анализ атак в реальном времени
- 📊 **Traffic analysis**: понимание полного контекста коммуникации
- 🔐 **Malware analysis**: извлечение C&C команд
- 📡 **Protocol reverse engineering**: понимание неизвестных протоколов

**Wireshark делает именно это: "Follow TCP Stream".**

---

### 1. TCP Sequence Numbers

**Каждый TCP пакет** имеет:
- **SEQ** (sequence number) — номер первого байта в этом пакете
- **ACK** (acknowledgment) — подтверждение получения

```c
struct tcphdr *tcp = ...;
uint32_t seq = ntohl(tcp->seq);
uint32_t ack = ntohl(tcp->ack_seq);

printf("SEQ: %u, ACK: %u\n", seq, ack);
```

**Пример сессии**:
```
[Packet 1] SEQ=1000, len=100  → данные с байта 1000 по 1099
[Packet 2] SEQ=1100, len=50   → данные с байта 1100 по 1149
[Packet 3] SEQ=1150, len=200  → данные с байта 1150 по 1349
```

**Порядок пакетов** может нарушаться! Нужно **сортировать по SEQ**.

---

### 2. Определение TCP сессии

**Сессия** идентифицируется по:
- Source IP
- Source Port
- Destination IP
- Destination Port

```c
typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
} SessionKey;

// Сравнение сессий
int same_session(SessionKey *a, SessionKey *b) {
    return (a->src_ip == b->src_ip &&
            a->dst_ip == b->dst_ip &&
            a->src_port == b->src_port &&
            a->dst_port == b->dst_port);
}
```

---

### 3. Хранение пакетов сессии

```c
#define MAX_PACKETS 1000

typedef struct {
    uint32_t seq;
    unsigned char data[2048];
    int data_len;
} Packet;

typedef struct {
    SessionKey key;
    Packet packets[MAX_PACKETS];
    int packet_count;
} Session;

// Добавить пакет в сессию
void add_packet(Session *session, uint32_t seq, unsigned char *data, int len) {
    int idx = session->packet_count;
    session->packets[idx].seq = seq;
    memcpy(session->packets[idx].data, data, len);
    session->packets[idx].data_len = len;
    session->packet_count++;
}
```

---

### 4. Сортировка по SEQ

```c
int compare_packets(const void *a, const void *b) {
    Packet *p1 = (Packet*)a;
    Packet *p2 = (Packet*)b;
    return (p1->seq > p2->seq) - (p1->seq < p2->seq);
}

// Сортировать пакеты
qsort(session->packets, session->packet_count, sizeof(Packet), compare_packets);
```

---

### 5. Сборка потока данных

```c
// Объединить все пакеты в один буфер
unsigned char stream[65536];
int stream_len = 0;

for (int i = 0; i < session->packet_count; i++) {
    memcpy(stream + stream_len, 
           session->packets[i].data, 
           session->packets[i].data_len);
    stream_len += session->packets[i].data_len;
}

printf("Total stream size: %d bytes\n", stream_len);
```

---

### 6. Извлечение ключа из первого пакета

**Протокол V.**:
```
[Packet 1] (с наименьшим SEQ):
Формат: "KEY:<hex_key>\n<encrypted_data>"
Пример: "KEY:0x42\nEncrypted message here"
```

```c
// Парсинг ключа
unsigned char *first_packet = session->packets[0].data;
uint8_t key = 0;

if (sscanf((char*)first_packet, "KEY:0x%02hhx\n", &key) == 1) {
    printf("Encryption key: 0x%02X\n", key);
}

// Найти начало зашифрованных данных (после "\n")
unsigned char *encrypted = (unsigned char*)strchr((char*)first_packet, '\n');
if (encrypted) {
    encrypted++;  // Пропустить '\n'
}
```

---

### 7. Расшифровка XOR (из Episode 01)

```c
void xor_decrypt(unsigned char *data, int len, uint8_t key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// Использование
xor_decrypt(encrypted, encrypted_len, key);
printf("Decrypted: %s\n", encrypted);
```

---

### 8. Обработка нескольких сессий (advanced)

```c
#define MAX_SESSIONS 10

Session sessions[MAX_SESSIONS];
int session_count = 0;

// Найти или создать сессию
Session* find_or_create_session(SessionKey *key) {
    // Искать существующую
    for (int i = 0; i < session_count; i++) {
        if (same_session(&sessions[i].key, key)) {
            return &sessions[i];
        }
    }
    
    // Создать новую
    if (session_count < MAX_SESSIONS) {
        sessions[session_count].key = *key;
        sessions[session_count].packet_count = 0;
        return &sessions[session_count++];
    }
    
    return NULL;
}
```

---

## 🎯 Миссия: "Communication Interceptor"

### Легенда

Два агента противника обмениваются критической информацией через зашифрованный TCP канал на **10.0.0.1:4433**.

**Протокол коммуникации**:
1. **Первый пакет** (наименьший SEQ): содержит ключ в формате `KEY:0x??`
2. **Остальные пакеты**: зашифрованные данные (XOR)
3. **Порядок пакетов**: случайный (нужна сортировка по SEQ)

**Ваша задача**: перехватить, реконструировать, расшифровать.

---

### Алгоритм решения

```
1. Создать raw socket (SOCK_RAW, IPPROTO_TCP)
2. Перехватывать пакеты в цикле
3. Для каждого пакета:
   - Парсить IP и TCP заголовки
   - Фильтровать по порту 4433
   - Извлечь SEQ и payload
   - Сохранить в массив
4. После завершения (FIN flag):
   - Сортировать пакеты по SEQ
   - Извлечь ключ из первого
   - Объединить payload всех пакетов
   - Расшифровать XOR
   - Вывести сообщение
```

---

## 📝 Структура эпизода

```
episode-12-encrypted-communications/
├── README.md              ← Вы здесь
├── mission.md
├── starter.c              ← Шаблон interceptor
├── artifacts/
│   └── comm_interceptor.c ← Создайте этот файл
├── solution/
│   ├── comm_interceptor_solution.c
│   ├── encrypted_sender.c    ← Генератор зашифрованного трафика
│   └── encrypted_receiver.c
└── tests/
    └── test.sh
```

### 💡 Как работать:

1. **Изучите** `starter.c` — там структуры Session, Packet с TODO
2. **Создайте** файл `artifacts/comm_interceptor.c`
3. **Реализуйте** полную цепочку:
   - Raw socket для перехвата
   - Парсинг IP/TCP заголовков
   - Сохранение пакетов по сессиям
   - Сортировка по sequence numbers
   - Извлечение ключа
   - XOR расшифровка
   - Вывод финального сообщения
4. **Скомпилируйте**: `gcc -Wall -o comm_interceptor artifacts/comm_interceptor.c`
5. **Запустите с sudo**: `sudo ./comm_interceptor`

> 💡 **Для тестирования**: используйте `encrypted_sender/receiver` для генерации трафика!

---

### Подсказки

<details>
<summary>Как определить конец сессии?</summary>

```c
// Проверить FIN flag
if (tcp->fin) {
    printf("Session closing (FIN received)\n");
    // Обработать и расшифровать сессию
}
```
</details>

<details>
<summary>Как сортировать пакеты?</summary>

```c
#include <stdlib.h>

int compare_packets(const void *a, const void *b) {
    Packet *p1 = (Packet*)a;
    Packet *p2 = (Packet*)b;
    
    if (p1->seq < p2->seq) return -1;
    if (p1->seq > p2->seq) return 1;
    return 0;
}

qsort(session->packets, session->packet_count, sizeof(Packet), compare_packets);
```
</details>

<details>
<summary>Как извлечь ключ?</summary>

```c
// Первый пакет (после сортировки)
Packet *first = &session->packets[0];

uint8_t key = 0;
if (sscanf((char*)first->data, "KEY:0x%02hhx", &key) == 1) {
    printf("Found key: 0x%02X\n", key);
}

// Данные после "KEY:0x??\n"
char *encrypted_start = strchr((char*)first->data, '\n');
if (encrypted_start) {
    encrypted_start++;  // Skip '\n'
}
```
</details>

<details>
<summary>Как собрать весь поток?</summary>

```c
unsigned char stream[65536];
int stream_len = 0;

// Начать со второго пакета (первый - KEY)
for (int i = 1; i < session->packet_count; i++) {
    memcpy(stream + stream_len, 
           session->packets[i].data, 
           session->packets[i].data_len);
    stream_len += session->packets[i].data_len;
}

// Расшифровать
xor_decrypt(stream, stream_len, key);
printf("Decrypted message:\n%s\n", stream);
```
</details>

---

## ✅ Проверка

```bash
# Компиляция
gcc -Wall -o comm_interceptor artifacts/comm_interceptor.c

# В одном терминале: запустить приемник
gcc -Wall -o receiver solution/encrypted_receiver.c
./receiver &

# В другом терминале: запустить отправителя
gcc -Wall -o sender solution/encrypted_sender.c
./sender &

# В третьем терминале: перехватчик (с sudo!)
sudo ./comm_interceptor

# Остановить тестовый трафик
pkill receiver
pkill sender
```

**Критерии успеха**:
- Перехватывает TCP пакеты на порту 4433
- Корректно парсит SEQ numbers
- Сортирует пакеты по порядку
- Извлекает ключ из первого пакета
- Объединяет payload
- Расшифровывает XOR
- Выводит финальное сообщение V.

---

## 🎬 DEBRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: Квартира, район Арбат, Москва                 ║
║  ВРЕМЯ:   06:47, четверг, 2 октября                     ║
║  СТАТУС:  🟢 MISSION ACCOMPLISHED - SEASON 3 COMPLETE    ║
╚══════════════════════════════════════════════════════════╝
```

`comm_interceptor` запущен. Перехват начат. Реконструкция завершена. Результат:

```
=== COMMUNICATION INTERCEPTOR ===
Listening for encrypted traffic on port 4433...

[Session detected]
10.0.0.1:54321 ↔ 10.0.0.1:4433

Captured 8 packets, reconstructing stream...

[Packet 1] SEQ=1000, len=15: "KEY:0x42\n..."
[Packet 2] SEQ=1015, len=50
[Packet 3] SEQ=1065, len=100
[Packet 4] SEQ=1165, len=75
[Packet 5] SEQ=1240, len=60
[Packet 6] SEQ=1300, len=40
[Packet 7] SEQ=1340, len=25
[Packet 8] SEQ=1365, FIN flag

Sorting by sequence number... ✓
Extracting key from first packet... ✓
Key: 0x42

Assembling stream... ✓
Total encrypted data: 350 bytes

Decrypting with XOR... ✓

🎯 DECRYPTED MESSAGE:
╔═══════════════════════════════════════════════════════╗
║                                                       ║
║  Поздравляю, агент.                                   ║
║                                                       ║
║  Ты прошёл Season 3: NETWORKS.                        ║
║                                                       ║
║  Ты освоил:                                           ║
║  ✓ IP-адресацию и структуры сетей                     ║
║  ✓ Socket programming (TCP/UDP)                       ║
║  ✓ Raw sockets и packet sniffing                      ║
║  ✓ TCP session reconstruction                         ║
║  ✓ XOR шифрование и дешифровку                        ║
║                                                       ║
║  Сеть больше не тайна. Ты видишь потоки данных.       ║
║  Ты понимаешь протоколы. Ты контролируешь канал.      ║
║                                                       ║
║  ════════════════════════════════════════             ║
║                                                       ║
║  Следующий шаг: КРИПТОГРАФИЯ.                         ║
║                                                       ║
║  Season 4: Crypto & Algorithms                        ║
║  Там тебя ждут настоящие вызовы:                      ║
║  - Симметричное шифрование                            ║
║  - Хэш-функции и blockchain                           ║
║  - Сортировка и поиск                                 ║
║  - Структуры данных                                   ║
║                                                       ║
║  Готовься. Сложность возрастает.                      ║
║                                                       ║
║  До встречи в Season 4.                               ║
║                                                       ║
║  — V.                                                 ║
║                                                       ║
╚═══════════════════════════════════════════════════════╝

Session closed gracefully.
```

---

### 🏆 SEASON 3 COMPLETE!

**Вы освоили:**
- 🌐 **IP-адресацию** — координаты в киберпространстве
- 🔌 **Сокеты** — подключение к любому серверу
- 📡 **Packet sniffing** — видите всё, что происходит в сети
- 🔐 **Session reconstruction** — восстанавливаете полную картину коммуникации

**Артефакты получены:**
- `net_analyzer` — анализатор IP-адресов
- `moonlight_client` — TCP клиент
- `packet_sniffer` — перехватчик пакетов
- `comm_interceptor` — система полного перехвата

### 🎖️ Финальный навык разблокирован:
**"Network Wizard"** — Вы владеете сетями на всех уровнях: от IP до приложений.

---

## 🥚 Easter Eggs

<details>
<summary>🔍 Отсылки и секреты</summary>

1. **TCP Session Reconstruction** — ядро функции "Follow TCP Stream" в Wireshark
2. **Port 4433** — альтернативный HTTPS порт (4+4=8, 3+3=6 → 86 → HTTP Security)
3. **10.0.0.1** — классический адрес внутреннего маршрутизатора
4. **Sequence numbers** — изобретены в 1974 году для TCP (Vint Cerf, Bob Kahn)
5. **XOR шифрование** — использовалось в RC4 (один из самых быстрых stream ciphers)
6. **FIN flag** — graceful shutdown (элегантное завершение), в отличие от RST (abort)
7. **V.'s timestamp 06:47** — 0x0647 hex, отсылка к RFC 647 (early network protocols)
8. **KEY:0x42** — снова 0x42 (Answer to Everything из Hitchhiker's Guide)

**Философия Season 3**: *"Сеть — это ткань. Пакеты — нити. Протоколы — узоры. Ты научился видеть всю картину целиком."*

</details>

---

## 🎓 Что дальше?

**Season 4: Crypto & Algorithms** ждёт вас!

Там вы изучите:
- Симметричное шифрование (AES, ChaCha20)
- Хэш-функции (SHA-256, MD5)
- Blockchain и Merkle trees
- Алгоритмы сортировки (QuickSort, MergeSort)
- Структуры данных (деревья, графы, hash tables)

**Сложность возрастает. Но вы готовы.** 🚀

---

## 🔗 Навигация

- [← Episode 11: Packet Analysis](../episode-11-packet-analysis/README.md)
- [→ Season 4: Crypto & Algorithms](../../season-4-crypto-and-algorithms/README.md)
- [📋 Season 3: Networks](../README.md)
- [🏠 Главная страница](../../README.md)

---

> *"You've mastered the network. Now master the code within. Season 4 awaits."* — V.