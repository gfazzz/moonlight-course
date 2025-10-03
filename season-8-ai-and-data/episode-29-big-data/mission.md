# Брифинг миссии: Обработка Big Data
**Episode 29** | Operation MOONLIGHT — Season 8

---

## 🎯 Цель миссии

**ОСНОВНАЯ ЦЕЛЬ:** Обработать массивные датасеты (247.8 MB сетевых логов + данные сенсоров) для подготовки к AI обучению.

**КОНТЕКСТ:**  
6 дней после победы Season 7 мы обнаружили разведывательную активность. Враг тестировал наши системы:
- SSH brute-force атаки (1,847 попыток)
- RDP сканирование (892 попытки)
- HTTP flood атаки (654 запроса/2сек)
- Физические аномалии сенсоров (температурные скачки до 38.2°C)

**ВАША ЗАДАЧА:**  
Реализовать pipeline обработки big data на C для анализа ВСЕХ собранных данных и идентификации паттернов атак.

**РЕЗУЛЬТАТ:**  
AI-ready датасет для Episodes 30-32 (статистический анализ → нейросети → предсказание).

---

## 📋 Технические требования

### 1. Обработка Memory-Mapped файлов (mmap)

**Требование:**  
Обработать `network_traffic.bin` (247.8 MB, 523,847 записей) используя memory-mapped I/O.

**Реализация:**
```c
// Открыть binary файл
int fd = open("network_traffic.bin", O_RDONLY);

// Получить размер файла
struct stat sb;
fstat(fd, &sb);

// Memory-map файл (POSIX - работает на Linux/macOS/FreeBSD)
void *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

// Опционально: Подсказать ядру о последовательном доступе
posix_madvise(mapped, sb.st_size, POSIX_MADV_SEQUENTIAL);

// Обработать как массив NetworkLog
NetworkLog *logs = (NetworkLog *)mapped;
size_t count = sb.st_size / sizeof(NetworkLog);

// ... обработка данных ...

// Очистка
munmap(mapped, sb.st_size);
close(fd);
```

**Бинарный формат:**
```c
typedef struct {
    uint64_t timestamp;      // Unix microseconds
    uint32_t src_ip;         // Network byte order
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t protocol;       // 6=TCP, 17=UDP
    uint32_t bytes;
    uint8_t flags;
    uint8_t status;          // 0=нормально, 1=подозрительно, 2=угроза
} __attribute__((packed)) NetworkLog;
```

**Ожидаемый анализ:**
- Всего записей: 523,847
- Подозрительных записей: ~12,847 (2.45%)
- Записей угроз: ~347 (0.066%)
- Всего байт передано: ~1.85 GB

---

### 2. Stream обработка (CSV)

**Требование:**  
Обработать `sensor_data.csv` используя streaming (константное использование памяти).

**Реализация:**
```c
FILE *fp = fopen("sensor_data.csv", "r");
char line[4096];

// Пропустить заголовок
fgets(line, sizeof(line), fp);

// Stream обработка (online алгоритм)
SensorData data;
while (fgets(line, sizeof(line), fp)) {
    if (parse_csv_line(line, &data) == 0) {
        // Обновить статистику инкрементально (константная память)
        update_statistics(&stats, data.temperature);
        if (data.anomaly) anomaly_count++;
    }
}

fclose(fp);
```

**CSV формат:**
```
timestamp,sensor_id,temperature,humidity,pressure,anomaly
1640001000.123,SENSOR_A01,22.5,45.2,1013.25,0
```

**Ожидаемый анализ:**
- Всего записей: 50
- Аномалий: 11 (22%)
- Диапазон температур: 19.5°C - 38.2°C
- Критические события: SENSOR_B02 (38.2°C, 88.9% влажности)

---

### 3. Статистическая агрегация (Thread-Safe)

**Требование:**  
Реализовать потокобезопасный аккумулятор статистики для параллельной обработки.

**Функции для реализации:**
```c
typedef struct {
    uint64_t count;
    double sum;
    double sum_squared;
    double min;
    double max;
    pthread_mutex_t lock;
} Statistics;

void init_statistics(Statistics *stats);
void update_statistics(Statistics *stats, double value);
double get_mean(Statistics *stats);
double get_variance(Statistics *stats);
double get_stddev(Statistics *stats);
```

**Потокобезопасность:**
```c
void update_statistics(Statistics *stats, double value) {
    pthread_mutex_lock(&stats->lock);
    
    stats->count++;
    stats->sum += value;
    stats->sum_squared += value * value;
    if (value < stats->min) stats->min = value;
    if (value > stats->max) stats->max = value;
    
    pthread_mutex_unlock(&stats->lock);
}
```

---

### 4. Параллельная обработка (опциональное улучшение)

**Требование:**  
Использовать 4 worker потока для параллельной обработки данных.

**Реализация:**
```c
#define THREAD_COUNT 4

void *parallel_worker(void *arg) {
    ThreadArg *targ = (ThreadArg *)arg;
    
    for (size_t i = targ->start_idx; i < targ->end_idx; i++) {
        update_statistics(targ->stats, (double)targ->logs[i].bytes);
    }
    
    return NULL;
}

// Создать потоки
pthread_t threads[THREAD_COUNT];
size_t chunk_size = total_records / THREAD_COUNT;

for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, parallel_worker, &args[i]);
}

// Дождаться завершения
for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
}
```

---

## 🧪 Критерии тестирования

### Тест 1: mmap обработка
```bash
cd solution
make
./big_data --mmap ../artifacts/network_traffic.bin
```

**Ожидаемый вывод:**
```
📊 Статистика сетевого трафика:
  Количество: 523847
  Среднее: 3527.4
  StdDev: 2184.9
  ⚠️  Подозрительных: 12847 (2.45%)
  🚨 Угроз: 347 (0.066%)
```

### Тест 2: CSV Stream обработка
```bash
./big_data --stream ../artifacts/sensor_data.csv
```

**Ожидаемый вывод:**
```
📊 Статистика температуры:
  Количество: 50
  Среднее: 24.82
  StdDev: 4.15
  Min: 19.5
  Max: 38.2
  🔴 Аномалий: 11 (22.0%)
```

### Тест 3: Полный pipeline
```bash
./big_data --all
```

**Должен обработать оба датасета и вывести комплексный анализ.**

---

## 📦 Результаты миссии

### Файлы для создания:

1. **`solution/big_data.c`**  
   Полная реализация (~400-500 строк)

2. **`solution/Makefile`**  
   Кроссплатформенная сборка (Linux/macOS/FreeBSD)

3. **`starter.c`**  
   Скелет кода с TODO (~150 строк)

### Предоставленные файлы (в `artifacts/`):

1. **`network_traffic.bin`** (бинарный формат, 247.8 MB)  
   - 523,847 NetworkLog записей
   - Содержит паттерны атак (SSH, RDP, HTTP flood)

2. **`sensor_data.csv`** (текстовый формат, 3.2 KB)  
   - 50 показаний сенсоров
   - 11 аномалий включая критические события

3. **`analysis_results.json`**  
   - Ожидаемые результаты анализа
   - Данные для валидации вашей реализации

4. **`network_traffic_sample.txt`**  
   - Человекочитаемый sample бинарных логов
   - Для понимания бинарного формата

---

## 🎯 Критерии успеха

✅ **Эффективность памяти:**  
- mmap использует константную память (независимо от размера файла)
- Streaming CSV parser использует <10 KB памяти

✅ **Точность:**  
- Статистические расчёты совпадают с `analysis_results.json`
- Обнаружение аномалий: 11/50 (22%)
- Обнаружение угроз: 347/523847 (0.066%)

✅ **Производительность:**  
- Обработка 247.8 MB менее чем за 5 секунд (с mmap)
- Параллельная обработка показывает ускорение в 2-4x

✅ **Кроссплатформенность:**  
- Работает на Linux, macOS, FreeBSD
- Нет платформозависимого кода (только POSIX)

✅ **AI готовность:**  
- Качество данных: ХОРОШЕЕ (98.5% полнота)
- Датасет готов для Episode 30 (статистический анализ)

---

## 💡 Подсказки

1. **mmap vs read():**  
   - `mmap`: Константная память, ядро управляет paging
   - `read()`: Нужно выделить память для всего файла

2. **Streaming vs Loading:**  
   - Streaming: Обработка построчно (константная память)
   - Loading: Загрузка всего файла в память (не масштабируется)

3. **Online статистика:**  
   - Обновлять mean/variance инкрементально
   - Алгоритм Welford для численной стабильности

4. **Потокобезопасность:**  
   - Всегда блокировать mutex перед обновлением разделяемого состояния
   - Держать критические секции минимальными

5. **Кроссплатформенный mmap:**  
   - POSIX стандарт: работает везде
   - Не нужна платформозависимая детекция

---

## 📊 Успех миссии

**По завершении у вас будет:**
- Обработано 247.8 MB сетевых логов
- Идентифицировано 347 активных угроз
- Проанализировано 11 аномалий сенсоров
- Подготовлен датасет для AI обучения (Episodes 30-32)

**Одобрение Viktor:**
```
Отличная работа, Agent. Dataset готов.
Episode 30 — statistical analysis. Stanford AI Lab ждёт.

V.
```

---

**Удачи, Agent!** 🚀  
**Помните:** В данных мы доверяем. Пусть машины учатся.

---

**БРИФИНГ МИССИИ:**

Продолжение сюжета после Season 7 (6 дней спустя). Viktor и Prof. Chen (Stanford AI Lab) обнаружили разведывательную активность врага. Нужно обработать терабайты данных для:

1. **Немедленного анализа угроз** (Episode 29 - сегодня)
2. **Статистического доказательства атак** (Episode 30 - завтра)
3. **Обучения нейросети** (Episode 31 - через 2 дня)
4. **Развёртывания AI защиты** (Episode 32 - финал)

**Локация:** Mountain View, California (Silicon Valley)  
**Время:** December 27, 2024 — 14:00 PST  
**Партнёры:** Viktor + Prof. David Chen (Stanford)  
**Враг:** Возможно те же силы из Season 7, но с AI capabilities

**Технические детали:**
- 247.8 MB network_traffic.bin: 523,847 записей (SSH brute-force, RDP scan, HTTP flood)
- sensor_data.csv: 50 записей (11 аномалий, критическая: SENSOR_B02 38.2°C)
- Top threats:
  * 185.220.101.42: SSH brute-force (1,847 attempts)
  * 45.227.255.190: RDP scanning (892 attempts)
  * 194.26.192.45: HTTP flood (654 requests/2s)

**Артефакты созданы:**
- `sensor_data.csv` (3.2 KB, 51 строка)
- `analysis_results.json` (155 строк)
- `network_traffic_sample.txt` (26 строк с примерами логов)

**Успешный результат Episode 29:** Dataset validated, quality 98.5%, ready for Episodes 30-32.

---

**Следующий эпизод:** [Episode 30: Statistical Analysis →](../episode-30-statistical-analysis/)