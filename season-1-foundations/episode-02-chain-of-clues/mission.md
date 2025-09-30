# Mission Brief: Chain of Clues

## Objective
Классифицировать и проанализировать данные из перехваченного файла.

## Given
- **Файл данных**: `data.dat` (координаты, время, ID)
- **Формат записей**: `<type> <value1> <value2> <timestamp>`
- **Типы**: C (Coordinates), T (Time), I (ID)

## Tasks

### 1. classifier.c (MAIN)
- [ ] Прочитать записи из stdin
- [ ] Определить тип каждой записи
- [ ] Валидировать данные
- [ ] Вывести статистику

**Валидация:**
- Coordinates: lat ∈ [-90, 90], lon ∈ [-180, 180]
- Time: hours ∈ [0, 23], minutes ∈ [0, 59]
- ID: положительное число

### 2. router.c
- [ ] Определить локацию координат (Москва или другое)
- [ ] Классифицировать время (день/ночь)
- [ ] Разделить ID на чётные/нечётные
- [ ] Вывести по категориям

### 3. pattern_finder.c (BONUS)
- [ ] Найти повторяющиеся координаты
- [ ] Найти дубликаты временных меток
- [ ] Найти самый частый ID

## Success Criteria
- ✅ Код компилируется без предупреждений
- ✅ Корректно обрабатывает все типы данных
- ✅ Валидация работает правильно
- ✅ Автотесты проходят

## Compile & Run
```bash
# Compile
gcc -Wall -Wextra -std=c11 -o classifier artifacts/classifier.c

# Run with test data
./classifier < data.dat

# Test
cd tests && ./test.sh
```

## Time Estimate
- Classifier: 45-60 min
- Router: 30-40 min
- Pattern finder: 30-45 min
- Total: ~2 hours

## Hints
<details>
<summary>Hint 1: Reading type</summary>

```c
char type;
scanf(" %c", &type);  // Space before %c ignores whitespace
```
</details>

<details>
<summary>Hint 2: Switch for types</summary>

```c
switch (type) {
    case 'C':
        // Handle coordinates
        break;
    case 'T':
        // Handle time
        break;
    case 'I':
        // Handle ID
        break;
    default:
        printf("Unknown type\n");
}
```
</details>

## What You'll Learn
- ✅ Conditional operators (if, else if, switch)
- ✅ Logical operators (&&, ||, !)
- ✅ Data validation
- ✅ Input parsing
- ✅ Switch statements

---

**Good luck, agent!** 🎯
