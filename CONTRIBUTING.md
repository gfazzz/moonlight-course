# 🤝 Contributing to OPERATION MOONLIGHT

Спасибо за интерес к развитию курса! Мы рады любому вкладу.

---

## 📋 Как можно помочь

### 1. Создание новых эпизодов

Курс планируется на 32+ эпизода. Многие ещё не созданы!

**Что нужно для эпизода:**
- `README.md` — теория + сюжет
- `mission.md` — краткое ТЗ
- `starter.c` — шаблоны кода
- `solution/*.c` — рабочее решение
- `tests/test.sh` — автотесты
- `Makefile` — сборка

**Пример:** `season-1-foundations/episode-01-strange-message/`

### 2. Улучшение существующих материалов

- 📝 Исправление опечаток
- 🎨 Улучшение объяснений
- 💡 Добавление подсказок
- 🐛 Исправление багов в коде

### 3. Перевод на другие языки

Курс сейчас на русском. Переводы приветствуются!

- English translation
- Español
- 中文
- Français

### 4. Тестирование

- Проходите эпизоды
- Сообщайте о проблемах
- Проверяйте на разных ОС

---

## 🔄 Процесс контрибуции

### 1. Fork репозитория

Нажмите **Fork** вверху страницы на GitHub.

### 2. Clone к себе

```bash
git clone git@github.com:YOUR_USERNAME/moonlight-course.git
cd moonlight-course
```

### 3. Создайте ветку

```bash
git checkout -b feature/episode-03-patterns
# или
git checkout -b fix/typo-in-readme
```

**Naming convention:**
- `feature/*` — новые эпизоды/функции
- `fix/*` — исправления
- `docs/*` — документация
- `test/*` — тесты

### 4. Внесите изменения

Следуйте стилю существующего кода.

### 5. Протестируйте

```bash
# Скомпилируйте код
cd season-X/.../episode-XX/
make

# Запустите тесты
make test

# Проверьте всё работает
./program
```

### 6. Commit

```bash
git add .
git commit -m "Add Episode 03: Patterns in Time

- Added theory on loops
- Created pattern finder exercise
- Added automated tests
- Storyline: analyzing time sequences"
```

**Commit message format:**
```
<type>: <short summary>

<detailed description>
```

**Types:**
- `feat:` — новая функция/эпизод
- `fix:` — исправление
- `docs:` — документация
- `test:` — тесты
- `style:` — форматирование

### 7. Push

```bash
git push origin feature/episode-03-patterns
```

### 8. Создайте Pull Request

1. Откройте ваш fork на GitHub
2. Нажмите **Pull Request**
3. Опишите изменения
4. Дождитесь review

---

## 📐 Стандарты кода

### C Code Style

```c
// Используйте понятные имена
int calculate_checksum(unsigned char *data, int length);

// Комментируйте сложные места
// XOR each byte with key (symmetric encryption)
for (int i = 0; i < length; i++) {
    decrypted[i] = encrypted[i] ^ key;
}

// Проверяйте ошибки
FILE *fp = fopen("data.txt", "r");
if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    return 1;
}
```

### Markdown Style

```markdown
# Заголовок H1 — для названия эпизода

## Заголовок H2 — для секций

### Заголовок H3 — для подсекций

- Списки с чёткой структурой
- Код в блоках ```c ... ```
- Эмодзи для визуального разделения 🎯
```

### Структура README эпизода

```markdown
# Episode XX: "Название"
## Season X: Тема сезона

> *"Цитата из сюжета"*

---

## 🎬 BRIEFING
[Сюжет]

## 📚 THEORY
[Объяснение концепций]

## 🎯 MISSION
[Задачи]

## 💡 ПОДСКАЗКИ
[Hints]

## 🎓 ЧТО ВЫ ИЗУЧИТЕ
[Список навыков]

## 🎬 DEBRIEFING
[Что дальше]
```

---

## ✅ Checklist для нового эпизода

Перед созданием PR убедитесь:

- [ ] README.md написан и понятен
- [ ] Теория объяснена с метафорами
- [ ] Сюжет связан с общей историей
- [ ] Стартовый код создан
- [ ] Решение работает и протестировано
- [ ] Автотесты проходят
- [ ] Makefile настроен
- [ ] Нет опечаток
- [ ] Код компилируется с `-Wall -Wextra`
- [ ] Всё закоммичено

---

## 🐛 Reporting Bugs

### Нашли баг?

Создайте **Issue** с описанием:

**Заголовок:**
```
[Bug] Episode 01: Decoder doesn't compile on Linux
```

**Описание:**
```markdown
## Описание проблемы
Decoder не компилируется на Ubuntu 22.04

## Шаги воспроизведения
1. cd season-1-foundations/episode-01-strange-message/
2. make
3. Ошибка компиляции

## Ожидаемое поведение
Код должен скомпилироваться

## Фактическое поведение
```
error: unknown type name 'uint8_t'
```

## Окружение
- OS: Ubuntu 22.04
- GCC: 11.3.0
- Make: 4.3
```

---

## 💡 Идеи для эпизодов

Нужны идеи для новых эпизодов? Вот список:

**Season 1 (осталось 3 эпизода):**
- Episode 02: Chain of Clues (условия)
- Episode 03: Patterns in Time (циклы)
- Episode 04: First Tool (функции, файлы)

**Season 2-8:** См. CURRICULUM.md

**Можете предложить свой эпизод!** Создайте Issue с тегом `enhancement`.

---

## 📞 Вопросы?

- **Создайте Issue** с тегом `question`
- **Обсуждения** на вкладке Discussions
- **Email** (если указан в профиле)

---

## 🎖️ Contributors

Все контрибьюторы будут указаны в README.md и CONTRIBUTORS.md!

Спасибо за вклад в обучение программированию на C! 🚀

---

## 📜 Code of Conduct

Будьте вежливы и уважительны:
- ✅ Конструктивная критика
- ✅ Помощь новичкам
- ✅ Уважение к разным уровням знаний
- ❌ Токсичность
- ❌ Оскорбления
- ❌ Дискриминация

---

**Удачи с вашим первым PR!** 🎯

*— The Architect*
