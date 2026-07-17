# Mission — s02e04 «Навигация по цепочке»

## Что сдать
`artifacts/navigator.c`: читает `pointers.dat` (массив записей в произвольном порядке), идёт по цепочке `next` от offset 0 и собирает спрятанное сообщение.

## Дано
- `pointers.dat` — 16 записей по 12 байт: `uint32 offset, value, next` (little-endian).
- Старт — запись с `offset == 0`; конец — `next == 0`.
- Порядок записей в файле произвольный; порядок чтения задаёт `next`.

## Критерии приёмки
- [ ] Компилируется с `-Wall -Wextra -std=c11` без предупреждений.
- [ ] Объявлен `typedef struct { uint32_t offset, value, next; } Entry;`.
- [ ] Доступ к полям записи через указатель и стрелку (`e->offset`, `e->value`, `e->next`).
- [ ] `find()` ищет запись по `offset`; цепочка обходится по `next` до `0`.
- [ ] Вывод совпадает с `tests/expected.txt` (сообщение `MOONLIGHT.ACTIVE`).
- [ ] `make test` → `PASS`.

## Как проверить
```bash
cp starter.c artifacts/navigator.c
make test
```

## Оценка времени
~40–50 минут.

## Результат в сюжете
Собрав цепочку, ты вытащил из дампа сигнал: `MOONLIGHT.ACTIVE`. Кто-то держит систему включённой прямо сейчас.
