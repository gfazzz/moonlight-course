# Mission Brief: Patterns in Time

## Objective
Найти периодичность и паттерны в последовательности данных.

## Given
- **Файл данных**: `data.txt`
- **Формат**: `timestamp latitude longitude`
- **Объём**: ~100 записей

## Tasks

### 1. pattern_finder.c (MAIN)
- [ ] Прочитать временные метки
- [ ] Вычислить интервалы между метками
- [ ] Найти самый частый интервал (период)
- [ ] Вывести статистику

### 2. sequence_analyzer.c
- [ ] Прочитать координаты
- [ ] Вычислить расстояния между точками
- [ ] Найти среднее расстояние
- [ ] Найти аномалии (большие скачки)

### 3. bruteforce.c (BONUS)
- [ ] Перебрать PIN коды 0000-9999
- [ ] Вычислить контрольную сумму для каждого
- [ ] Найти совпадение с эталоном
- [ ] Вывести найденный PIN

## Success Criteria
- ✅ Компилируется без warning
- ✅ Находит правильный период
- ✅ Вычисляет расстояния корректно
- ✅ Bruteforce находит PIN
- ✅ Автотесты проходят

## Compile & Run
```bash
# Pattern finder
make pattern
./build/pattern < data.txt

# Sequence analyzer
make sequence
./build/sequence < data.txt

# Bruteforce
make bruteforce
./build/bruteforce

# All tests
make test
```

## Time Estimate
- Pattern finder: 45-60 min
- Sequence analyzer: 50-70 min
- Bruteforce: 30-40 min
- Total: ~2.5 hours

## Hints
<details>
<summary>Hint 1: Reading until EOF</summary>

```c
long ts;
float lat, lon;
int count = 0;

while (scanf("%ld %f %f", &ts, &lat, &lon) == 3) {
    // Process
    count++;
}
```
</details>

<details>
<summary>Hint 2: Finding mode (most frequent)</summary>

```c
int freq[MAX] = {0};

// Count occurrences
for (each interval) {
    freq[interval]++;
}

// Find max
int max_freq = 0, mode = 0;
for (int i = 0; i < MAX; i++) {
    if (freq[i] > max_freq) {
        max_freq = freq[i];
        mode = i;
    }
}
```
</details>

<details>
<summary>Hint 3: Distance formula</summary>

```c
#include <math.h>

float distance(float lat1, float lon1, float lat2, float lon2) {
    float dlat = lat2 - lat1;
    float dlon = lon2 - lon1;
    return sqrt(dlat*dlat + dlon*dlon) * 111.0;  // км
}

// Compile with -lm
```
</details>

## What You'll Learn
- ✅ for, while, do-while loops
- ✅ break and continue
- ✅ Nested loops
- ✅ Array processing
- ✅ Statistical analysis
- ✅ Brute force algorithms

---

**Find the pattern, agent!** 🎯
